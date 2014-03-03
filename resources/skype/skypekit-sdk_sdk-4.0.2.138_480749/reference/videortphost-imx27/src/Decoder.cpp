/*
 * Copyright (C) 2010 Skype Technologies S.A. Confidential and proprietary
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained
 * in, relating to, or arising from the internet telephony software of Skype
 * Limited (including its affiliates, "Skype"), including without limitation
 * this source code, Skype API and related material of such software
 * proprietary to Skype and/or its licensors ("IP Rights") are and shall
 * remain the exclusive property of Skype and/or its licensors. The recipient
 * hereby acknowledges and agrees that any unauthorized use of the IP Rights
 * is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and ownership
 * thereof.
 */

#include <cstring>

#include "Decoder.hpp"
#include "LoggingCtrl.hpp"

#define CLEANUP(ptr) if (ptr) {delete ptr; ptr = NULL;}
#define CLEANUP_ARRAY(ptr) if (ptr) {delete [] ptr; ptr = NULL;}

Decoder::Decoder(VideoParams* params, VideoRenderer* vRend) :
	VideoDecoder(params, vRend),
	pThread(false),
	m_InQueueBuffers(NULL),
	m_FrameBuffers(NULL),
	m_ClrDispFrame(0),
	m_FrameBufferCount(0),
	m_RotatorBufferOffset(0),
	m_LastTimestamp(0),
	m_Rotate(false),
	m_InitDone(false)
{
	InitDecoder();
	Start("Decoder");
}

Decoder::~Decoder() {
	m_InitDone = false;
	CLEANUP(m_pInQueue);
	Stop();
	UninitDecoder();
}

void Decoder::InitDecoder() {
	if (m_InitDone) {
		WARN("Decoder: already initialized");
		return;
	}
	int i = 0;
	DecOpenParam dOP; // Parameters used when opening a decoder instance
	RetCode ret = RETCODE_SUCCESS;

	// i.MX27 VPU only supports YUV 4:2:0 format (FOURCC=YU12/I420)
	if (m_VPar.pixFmt != V4L2_PIX_FMT_YUV420) {
		FATAL("Decoder: Invalid pixel format");
	}

	// Initialize structures used for memory information
	memset(&m_BitStreamBuffer, 0, sizeof(vpu_mem_desc));
	memset(&m_PSSaveBuffer, 0, sizeof(vpu_mem_desc));
	memset(&m_SliceSaveBuffer, 0, sizeof(vpu_mem_desc));

	// Allocate memory for decoder's bit stream buffers
	m_BitStreamBuffer.size = SIZE_BITSTREAM_BUFS;
	if (IOGetPhyMem(&m_BitStreamBuffer) < 0) {
		FATAL("Decoder: Failed to create bit stream buffer");
	}
	// Map physical memory to user space
	if (IOGetVirtMem(&m_BitStreamBuffer) < 0) {
		FATAL("Decoder: Failed to map bit stream buffer");
	}

	// Configure parameters for decoder instance
	dOP.bitstreamFormat = STD_AVC; // H.264/AVC
	dOP.bitstreamBuffer = m_BitStreamBuffer.phy_addr; // Buffer start
	dOP.bitstreamBufferSize = m_BitStreamBuffer.size; // Buffer size
	dOP.qpReport = 0; // (Not applicable to H.264 decoding)
	dOP.reorderEnable = 0; // Enable display buffer reordering
	dOP.filePlayEnable = 0; // Provide chunk, reset write pointer each frame
	dOP.dynamicAllocEnable = 0; // Dynamic buffer allocation
	dOP.streamStartByteOffset = 0; // Start byte offset of the stream buffer
	m_PSSaveBuffer.size = PS_SAVE_SIZE;
	if (IOGetPhyMem(&m_PSSaveBuffer) < 0) {
		FATAL("Decoder: Failed to create PS save buffer");
	}
	dOP.psSaveBuffer = m_PSSaveBuffer.phy_addr; // PS save buffer start
	dOP.psSaveBufferSize = m_PSSaveBuffer.size; // PS save buffer size

	// Open a decoder instance
	pMutexLock scopedLock(*(m_VPar.mutex)); // Scoped lock on VPU mutex
	ret = vpu_DecOpen(&m_Handle, &dOP);
	scopedLock.mutex.Release(); // VPU access - unlock
	if (ret != RETCODE_SUCCESS) {
		UninitDecoder();
		FATAL("Decoder: vpu_DecOpen() failed. Return code: %d", ret);
	}

	// Configure decoding parameters (used by vpu_DecStartOneFrame())
	m_Param.prescanEnable = 1; // Scan stream buffers for full pic stream
	m_Param.prescanMode = 0; // Don't decode even if full pic stream exists
	m_Param.dispReorderBuf = 0; // (Should always be 0 if reorderEnable = 0)
	m_Param.iframeSearchEnable = 0; // Skip frame decode until next I frame
	m_Param.skipframeMode = 0; // Skip until next I frame or skip all frames
	m_Param.skipframeNum = 0; // Number of frames to skip
	m_Param.picStreamBufferAddr = 0; // Start address of pic stream buffer
	m_Param.picStartByteOffset = 0; // Start offset of picture stream buffer
	m_Param.chunkSize = 0; // Byte size of a picture stream chunk

	// Create a table of buffers for bit stream buffer data
	m_InQueueBuffers = new vpu_mem_desc*[DEC_IN_QUEUE_LEN];
	for (i = 0; i < DEC_IN_QUEUE_LEN; i++) {
		m_InQueueBuffers[i] = new vpu_mem_desc;
	}

	// Create queue for bit stream buffers
	m_pInQueue = new StaticSyncQueue<vpu_mem_desc *> (m_InQueueBuffers, DEC_IN_QUEUE_LEN);

	m_InitDone = true;
}

void Decoder::UninitDecoder() {
	int i = 0;
	pMutexLock scopedLock(*(m_VPar.mutex));

	if (m_Handle != NULL) {
		vpu_DecClose(m_Handle);
	}
	IOFreeVirtMem(&m_BitStreamBuffer);
	IOFreePhyMem(&m_BitStreamBuffer);
	IOFreePhyMem(&m_PSSaveBuffer);
	IOFreePhyMem(&m_SliceSaveBuffer);
	for (i = 0; i < DEC_IN_QUEUE_LEN; i++) {
		CLEANUP(m_InQueueBuffers[i]);
	}
	CLEANUP_ARRAY(m_InQueueBuffers);
	CLEANUP_ARRAY(m_FrameBuffers);
}

void Decoder::Run(void) {
	vpu_mem_desc* inBuf = NULL;
	Uint32 inBufSize = 0;
	bool initialInfo = false;
	RetCode ret = RETCODE_SUCCESS;
	DecOutputInfo outputInfo;
	int indexFrameDisplay = 0;

	while (!Shutdown && m_InitDone) {
		inBuf = m_pInQueue->GetReadPtr();
		if (NULL == inBuf) {
			continue;
		}
		inBufSize = inBuf->size;

		// Must wait for sufficient amount of bit stream to enter buffer
		if (!initialInfo) {
			ret = GetInitialInfo();
			if (ret != RETCODE_SUCCESS) {
				if (m_pInQueue) {
					m_pInQueue->ReadDone();
				}
				continue;
			}
			if (m_Rotate) {
				indexFrameDisplay = m_RotatorBufferOffset;
			}

			initialInfo = true;
		}

		pMutexLock scopedLock(*(m_VPar.mutex));
		if (m_Rotate) {
			// Set rotator output buffer address
			vpu_DecGiveCommand(m_Handle, SET_ROTATOR_OUTPUT, &(m_FrameBuffers[indexFrameDisplay]));
			vpu_DecGiveCommand(m_Handle, ENABLE_ROTATION, 0);
		}

		// Start decoding one frame
		ret = vpu_DecStartOneFrame(m_Handle, &m_Param);
		if (ret != RETCODE_SUCCESS) {
			UninitDecoder();
			ERROR("Decoder: vpu_DecStartOneFrame() failed. Return code: %d", ret);
		}

		while (vpu_IsBusy()) {
			vpu_WaitForInt(200); // Wait for decoding to complete
		}

		// Get decoding results
		ret = vpu_DecGetOutputInfo(m_Handle, &outputInfo);
		scopedLock.mutex.Release();
		if (ret != RETCODE_SUCCESS) {
			DEC_DBG("vpu_DecGetOutputInfo() failed. Return code: %d", ret);
		}
		if (m_pInQueue) {
			m_pInQueue->ReadDone();
		}

		// Check if picture stream is incomplete
		if ((1 == m_Param.prescanEnable) && (0 == outputInfo.prescanresult)) {
			continue; // Incomplete picture stream
		}

		// Check if sequence decoding has finished (no more output)
		if (-1 == outputInfo.indexFrameDisplay) {
			continue; // End of sequence decoding
		}

		// Check if output can't be displayed at beginning of seq decode
		if ((-2 == outputInfo.indexFrameDisplay) || (-3 == outputInfo.indexFrameDisplay)) {
			if (m_ClrDispFrame >= 0) {
				// Framebuffer can be used by decoder again
				scopedLock.mutex.Acquire();
				ret = vpu_DecClrDispFlag(m_Handle, m_ClrDispFrame);
				scopedLock.mutex.Release();
			}
			if (ret != RETCODE_SUCCESS) {
				DEC_DBG("vpu_DecClrDispFlag() failed. Return code: %d", ret);
			}
			m_ClrDispFrame = outputInfo.indexFrameDisplay;
			continue; // Unable to display decoded sequence yet
		}

		// The number of erroneous macroblocks while decoding a picture
		if (outputInfo.numOfErrMBs) {
			DEC_DBG("Found %d erroneous macroblocks while decoding.", outputInfo.numOfErrMBs);
		}

		// Check for PS (SPS/PPS) save buffer overflow
		if (outputInfo.notSufficientPsBuffer) {
			DEC_DBG("PS (SPS/PPS) save buffer overflow while decoding.");
		}

		// Check for slice save buffer overflow
		if (outputInfo.notSufficientSliceBuffer) {
			DEC_DBG("Slice save buffer overflow while decoding.");
		}

		if (!m_Rotate) {
			indexFrameDisplay = outputInfo.indexFrameDisplay;
		}
		if (m_pVRend) {
			VideoData frame;
			frame.SetBuf((uint8_t *) &(indexFrameDisplay), 0);
			m_pVRend->RenderFrame(frame);
		}

		if (m_ClrDispFrame >= 0) {
			scopedLock.mutex.Acquire();
			ret = vpu_DecClrDispFlag(m_Handle, m_ClrDispFrame);
			scopedLock.mutex.Release();
		}
		if (ret != RETCODE_SUCCESS) {
			DEC_DBG("vpu_DecClrDispFlag() failed. Return code: %d", ret);
		}
		m_ClrDispFrame = outputInfo.indexFrameDisplay;

		if (m_Rotate) {
			indexFrameDisplay++;
			if (indexFrameDisplay >= m_FrameBufferCount) {
				indexFrameDisplay = m_RotatorBufferOffset;
			}
		}

		m_Timing.Update();
		m_Bitrate.Update(inBufSize);
		DEC_DBG("%s", m_Timing.GetStatStr());
		DEC_DBG("%s", m_Bitrate.GetStatStr());
	}
}

RetCode Decoder::GetInitialInfo() {
	RetCode ret = RETCODE_SUCCESS;
	DecInitialInfo initialInfo;
	int i = 0;
	vpu_mem_desc* frameBufferPool = NULL;
	DecBufInfo bufInfo;
	int angle = 0;
	int stride = 0;

	// Enable forced escape to prevent VPU from hanging during SEQ_INIT
	pMutexLock scopedLock(*(m_VPar.mutex));
	vpu_DecSetEscSeqInit(m_Handle, 1);
	// Get some crucial parameters before starting picture decoder operation
	ret = vpu_DecGetInitialInfo(m_Handle, &initialInfo);
	// Disable forced escape from SEQ_INIT
	vpu_DecSetEscSeqInit(m_Handle, 0);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		DEC_DBG("vpu_DecGetInitialInfo() failed. Return code: %d", ret);
		return ret;
	}

	// Can add more buffers to minFrameBufferCount for improved performance
	m_FrameBufferCount = initialInfo.minFrameBufferCount;

	// Allocate decoder/render frame buffers via V4L2 display device driver
	// and retreive V4L2 display device frame buffers from renderer
	if (!m_pVRend) {
		FATAL("Decoder: No render device");
	}
	frameBufferPool = (vpu_mem_desc *) m_pVRend->InitFrameBuffers((uint32_t *) &m_FrameBufferCount);

	if (m_FrameBufferCount > initialInfo.minFrameBufferCount) {
		m_RotatorBufferOffset = initialInfo.minFrameBufferCount;
		m_Rotate = true;
	}

	// Create a table of buffers for frame buffer data
	m_FrameBuffers = new FrameBuffer[m_FrameBufferCount];

	// Configure decoder to use the display buffers for its frame buffers
	for (i = 0; i < m_FrameBufferCount; ++i) {
		m_FrameBuffers[i].bufY = (frameBufferPool[i]).phy_addr;
		m_FrameBuffers[i].bufCb = m_FrameBuffers[i].bufY + (initialInfo.picWidth * initialInfo.picHeight);
		m_FrameBuffers[i].bufCr = m_FrameBuffers[i].bufCb + (initialInfo.picWidth * initialInfo.picHeight >> 2);
	}

	// Allocate memory for decoder slice save buffer (to store slice RBSP)
	m_SliceSaveBuffer.size = SLICE_SAVE_SIZE;
	if (IOGetPhyMem(&m_SliceSaveBuffer) < 0) {
		FATAL("Decoder: Failed to create slice save buffer");
	}
	memset(&bufInfo, 0, sizeof(DecBufInfo));
	bufInfo.avcSliceBufInfo.sliceSaveBuffer = m_SliceSaveBuffer.phy_addr;
	bufInfo.avcSliceBufInfo.sliceSaveBufferSize = m_SliceSaveBuffer.size;

	// Register decoder frame buffers and slice save buffer
	scopedLock.mutex.Acquire();
	ret = vpu_DecRegisterFrameBuffer(m_Handle, m_FrameBuffers, initialInfo.minFrameBufferCount, initialInfo.picWidth, &bufInfo);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		UninitDecoder();
		FATAL("Decoder: vpu_DecRegisterFrameBuffer() failed. Return code: %d", ret);
	}

	if (m_Rotate) {
		// Set angle for rotation
		angle = 90;
		scopedLock.mutex.Acquire();
		ret = vpu_DecGiveCommand(m_Handle, SET_ROTATION_ANGLE, &angle);
		scopedLock.mutex.Release();
		if (ret != RETCODE_SUCCESS) {
			ERROR("Decoder: vpu_DecGiveCommand(SET_ROTATION_ANGLE) failed. Return code: %d", ret);
		}
		// Set stride size of the frame buffer containing the rotated output
		stride = initialInfo.picHeight;
		scopedLock.mutex.Acquire();
		ret = vpu_DecGiveCommand(m_Handle, SET_ROTATOR_STRIDE, &stride);
		scopedLock.mutex.Release();
		if (ret != RETCODE_SUCCESS) {
			ERROR("Decoder: vpu_DecGiveCommand(SET_ROTATOR_STRIDE) failed. Return code: %d", ret);
		}
	}

	return ret;
}

void Decoder::DecodeFrame(VideoData& frame) {
	RetCode ret = RETCODE_SUCCESS;
	PhysicalAddress dummy = 0, phyNowAddr = 0;
	Uint32 maxSize = 0;
	unsigned int wrapSize = 0;
	unsigned char *currentBuf = NULL;
	vpu_mem_desc* outBuf = NULL;

	if (Shutdown || !m_InitDone) {
		WARN("Decoder: not started");
		return;
	}

	if (0 == frame.Size()) {
		WARN("Decoder: frame.Size() == 0");
		return;
	}

	// Get bitstream buffer physical address and buffer size before decoding
	pMutexLock scopedLock(*(m_VPar.mutex));
	ret = vpu_DecGetBitstreamBuffer(m_Handle, &dummy, &phyNowAddr, &maxSize);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		DEC_DBG("vpu_DecGetBitstreamBuffer() failed. Return code: %d", ret);
		return; // Bit stream not available yet
	}

	// Check if bit stream will fit in decoder buffer
	if (frame.Size() > (int) maxSize) {
		WARN("Decoder: decoder buffer overflow, truncating %d to %d bytes", frame.Size(), maxSize);
		frame.SetBuf(frame.Data(), maxSize); // Adjust size
	}

	// Current virtual address for decoder bit stream buffer
	currentBuf = (unsigned char *) (m_BitStreamBuffer.virt_uaddr + (phyNowAddr - m_BitStreamBuffer.phy_addr));
	// Remaining bytes until decoder bit stream buffer will wrap around
	wrapSize = (unsigned int) ((m_BitStreamBuffer.phy_addr + SIZE_BITSTREAM_BUFS) - phyNowAddr);

	// Copy bit stream to decoder's buffer
	if(frame.Size() > (int) wrapSize) {
		// Decoder's buffer will wrap around
		memcpy(currentBuf, frame.Data(), wrapSize);
		memcpy((unsigned char*) m_BitStreamBuffer.virt_uaddr, (unsigned char*) (frame.Data() + wrapSize), frame.Size() - wrapSize);
	}
	else {
		// No wrap-around - normal copy
		memcpy(currentBuf, frame.Data(), frame.Size());
	}

	// Let decoder know how much bitstream has been transferred
	scopedLock.mutex.Acquire();
	ret = vpu_DecUpdateBitstreamBuffer(m_Handle, frame.Size());
	scopedLock.mutex.Release();
	if(ret != RETCODE_SUCCESS) {
		ERROR("Decoder: vpu_DecUpdateBitstreamBuffer() failed. Return code: %d", ret);
	}

	outBuf = m_pInQueue->GetWritePtr();
	if (NULL == outBuf) {
		ERROR("Decoder: failed to write bit stream buffer to input queue element");
		return;
	}
	outBuf->phy_addr = phyNowAddr;
	outBuf->virt_uaddr = (long unsigned int) currentBuf;
	outBuf->size = frame.Size();
	m_pInQueue->WriteDone();

	DEC_DBG("Timestamp %d, diff: %d", frame.GetTimestamp(), frame.GetTimestamp() - m_LastTimestamp);
	m_LastTimestamp = frame.GetTimestamp();
}
