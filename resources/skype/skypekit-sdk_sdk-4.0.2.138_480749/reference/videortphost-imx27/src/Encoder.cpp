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

#include "Encoder.hpp"
#include "LoggingCtrl.hpp"
#include "lib/vpu/vpu_util.h"

#define CLEANUP_ARRAY(ptr) if (ptr) {delete [] ptr; ptr = NULL;}

static uint64_t getTimeUs() {
	struct timeval tv;
	uint64_t ret;
	gettimeofday(&tv, NULL);
	ret = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
	return ret;
}

Encoder::Encoder(VideoParams* params, VideoCapturer* vCap, FrameCallback* cb) :
	VideoEncoder(params, vCap, cb),
	pThread(false),
	m_FrameBufferPool(NULL),
	m_FrameBuffers(NULL),
	m_FrameBufferCount(0),
	m_InitDone(false)
{
	InitEncoder();
}

Encoder::~Encoder() {
	m_InitDone = false;
	UninitEncoder();
}

void Encoder::InitEncoder() {
	int i = 0;
	EncOpenParam eOP; // Parameters used when opening an encoder instance
	RetCode ret = RETCODE_SUCCESS;
	SearchRamParam searchRamParam; // Search-ram parameters (for ME ops)
	EncInitialInfo initialInfo;
	int stride = 0;

	if (m_InitDone) {
		WARN("Encoder: already initialized");
		return;
	}

	// i.MX27 VPU only supports YUV 4:2:0 format (FOURCC=YU12/I420)
	if (m_VPar.pixFmt != V4L2_PIX_FMT_YUV420) {
		FATAL("Encoder: Invalid pixel format");
	}

	// Allocate memory for encoder bit stream buffer
	m_OutBuf.size = SIZE_BITSTREAM_BUFS; // Memory size
	if (IOGetPhyMem(&m_OutBuf) < 0) {
		FATAL("Encoder: Failed to create bit stream buffer %d", i);
	}
	// Map physical memory to user space
	if (IOGetVirtMem(&m_OutBuf) < 0) {
		FATAL("Encoder: Failed to map bit stream buffer %d", i);
	}

	// Configure parameters for encoder instance
	memset(&eOP, 0, sizeof(EncOpenParam)); // Initialize structure
	eOP.bitstreamBuffer = m_OutBuf.phy_addr; // Buffer address
	eOP.bitstreamBufferSize = m_OutBuf.size; // Buffer size
	eOP.bitstreamFormat = STD_AVC; // H.264/AVC
	eOP.picWidth = m_VPar.width; // Picture width in pixels
	eOP.picHeight = m_VPar.height; // Picture height in pixels
	eOP.frameRateInfo = m_VPar.framerate; // Frame rate
	eOP.bitRate = m_VPar.bitrate; // Target bitrate in kbps
	eOP.initialDelay = 0; // Initial delay before filling vbv buffer
	eOP.vbvBufferSize = 0; // vbv_buffer_size (ignored if initialDelay = 0)
	eOP.enableAutoSkip = 1; // Skip frame if target bitrate is exceeded
	eOP.gopSize = 0; // GOP size (0 - Only first picture is I)
	eOP.slicemode.sliceMode = 0; // Multiple slices per picture
	eOP.slicemode.sliceSizeMode = 0; // Size mode (ignored if sliceMode = 0)
	eOP.slicemode.sliceSize = 0; // Slice size (ignored if sliceMode = 0)
	eOP.intraRefresh = 0; // Intra MB refresh
	eOP.sliceReport = 0; // Report end address + 1 for every slice
	eOP.mbReport = 0; // Report bit position for every MB in picture
	eOP.mbQpReport = 0; // (Not applicable to H.264 encoding)
	eOP.dynamicAllocEnable = 0; // Dynamic buffer allocation
	eOP.ringBufferEnable = 0; // Packet-based streaming mode with ring-buf
	eOP.rcIntraQp = -1; // I frame quantization parameter (-1 - automatic)
	// Set H.264/AVC specific parameters
	eOP.EncStdParam.avcParam.avc_constrainedIntraPredFlag = 0; // Constr. I
	eOP.EncStdParam.avcParam.avc_disableDeblk = 0; // Disable deblock filter
	eOP.EncStdParam.avcParam.avc_deblkFilterOffsetAlpha = 0; // Alpha offset
	eOP.EncStdParam.avcParam.avc_deblkFilterOffsetBeta = 0; // Beta offset
	eOP.EncStdParam.avcParam.avc_chromaQpOffset = 0; // Chroma QP offset
	eOP.EncStdParam.avcParam.avc_audEnable = 0; // Send AUD RBSP every pic
	eOP.EncStdParam.avcParam.avc_fmoEnable = 0; // Flexible MB ordering
	eOP.EncStdParam.avcParam.avc_fmoSliceNum = 0; // Slice groups/picture
	eOP.EncStdParam.avcParam.avc_fmoType = 0; // MB to slice group map type

	// Open an encoder instance
	pMutexLock scopedLock(*(m_VPar.mutex)); // Scoped lock on VPU mutex
	ret = vpu_EncOpen(&m_Handle, &eOP);
	scopedLock.mutex.Release(); // VPU access - unlock
	if (ret != RETCODE_SUCCESS) {
		UninitEncoder();
		FATAL("Encoder: vpu_EncOpen() failed. Return code: %d", ret);
	}

	// Specify search-ram parameters for encoder ME operations
	memset(&searchRamParam, 0, sizeof(SearchRamParam)); // Init structure
	searchRamParam.searchRamAddr = SEARCHRAM_ADDR; // 0xFFFF4C00
	scopedLock.mutex.Acquire(); // VPU access - lock
	ret = vpu_EncGiveCommand(m_Handle, ENC_SET_SEARCHRAM_PARAM, &searchRamParam);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		UninitEncoder();
		FATAL("Encoder: vpu_EncGiveCommand(ENC_SET_SEARCHRAM_PARAM) failed. Return code: %d", ret);
	}

	// Get minimum required buffer count (used for allocating frame buffers)
	memset(&initialInfo, 0, sizeof(EncInitialInfo)); // Initialize structure
	scopedLock.mutex.Acquire();
	ret = vpu_EncGetInitialInfo(m_Handle, &initialInfo);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		UninitEncoder();
		FATAL("Encoder: vpu_EncGetInitialInfo() failed. Return code: %d", ret);
	}

	m_FrameBufferCount = initialInfo.minFrameBufferCount;

	// Allocate memory for encoder frame buffers
	m_FrameBufferPool = new vpu_mem_desc[m_FrameBufferCount];
	m_FrameBuffers = new FrameBuffer[m_FrameBufferCount + 1];
	for (i = 0; i < m_FrameBufferCount; i++) {
		m_FrameBufferPool[i].size = m_VPar.FrameSize();
		if (IOGetPhyMem(&(m_FrameBufferPool[i])) < 0) {
			UninitEncoder();
			FATAL("Encoder: Failed to allocate memory for encoder frame buffer %d", i);
		}
		m_FrameBuffers[i].bufY = m_FrameBufferPool[i].phy_addr;
		m_FrameBuffers[i].bufCb = m_FrameBuffers[i].bufY + m_VPar.width * m_VPar.height;
		m_FrameBuffers[i].bufCr = m_FrameBuffers[i].bufCb + m_VPar.width / 2 * m_VPar.height / 2;
	}

	// Stride: number of bytes between pixels in successive rows
	stride = ((m_VPar.width + 15) & ~15);

	// Register encoder frame buffers and stride
	scopedLock.mutex.Acquire();
	ret = vpu_EncRegisterFrameBuffer(m_Handle, m_FrameBuffers, m_FrameBufferCount, stride);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		UninitEncoder();
		FATAL("Encoder: vpu_EncRegisterFrameBuffer() failed. Return code: %d", ret);
	}

	// Configure encoding parameters (used by vpu_EncStartOneFrame())
	memset(&m_Param, 0, sizeof(EncParam));
	m_Param.sourceFrame = &m_FrameBuffers[m_FrameBufferCount]; // Source image
	m_Param.forceIPicture = 0; // Send IDR pic (ignored if skipPicture = 1)
	m_Param.skipPicture = 0; // Generate skipped picture
	m_Param.quantParam = 30; // QP (Ignored if bitRate != 0)
	m_Param.picStreamBufferAddr = 0; // Start address of pic stream buffer
	m_Param.picStreamBufferSize = 0; // Byte size of a picture stream chunk
	m_Param.allInterMB = 0; // Only Inter mb_type is allowed for P pictures

	m_InitDone = true;
}

void Encoder::UninitEncoder() {
	int i = 0;
	pMutexLock scopedLock(*(m_VPar.mutex));

	if (m_Handle != NULL) {
		vpu_EncClose(m_Handle);
	}
	for (i = 0; i < m_FrameBufferCount; i++) {
		IOFreePhyMem(&(m_FrameBufferPool[i]));
	}
	m_FrameBufferCount = 0;
	CLEANUP_ARRAY(m_FrameBufferPool);
	CLEANUP_ARRAY(m_FrameBuffers);
	IOFreeVirtMem(&(m_OutBuf));
	IOFreePhyMem(&(m_OutBuf));
}

void Encoder::Run(void) {
	vpu_mem_desc* yuvBuf = NULL;
	bool initialKeyFrame = false;

	while (!Shutdown && m_InitDone) {
		VideoData frame;
		frame.SetBuf((uint8_t *) yuvBuf, 0);

		if (m_pVCap) {
			m_pVCap->CaptureFrame(frame);
		}

		// Must fist send SPS header and PPS header
		if (!initialKeyFrame) {
			GenerateKeyFrame();
			initialKeyFrame = true;
		}

		EncodeFrame(frame);
	}
}

void Encoder::EncodeFrame(VideoData& inFrame) {
	RetCode ret = RETCODE_SUCCESS;
	EncOutputInfo outputInfo;
	uint8_t* pBuf = NULL;

	if (m_Param.forceIPicture) {
		// Generate and insert an SPS header
		GenerateHeader(SPS_RBSP);

		// Generate and insert a PPS header
		GenerateHeader(PPS_RBSP);
	}

	// Set encoder frame buffer to point to the input buffer
	m_FrameBuffers[m_FrameBufferCount].bufY = (PhysicalAddress) inFrame.Data();
	m_FrameBuffers[m_FrameBufferCount].bufCb = m_FrameBuffers[m_FrameBufferCount].bufY + m_VPar.width * m_VPar.height;
	m_FrameBuffers[m_FrameBufferCount].bufCr = m_FrameBuffers[m_FrameBufferCount].bufCb + m_VPar.width / 2 * m_VPar.height / 2;

	m_Param.sourceFrame = &m_FrameBuffers[m_FrameBufferCount];

	// Start encoding one frame
	pMutexLock scopedLock(*(m_VPar.mutex));
	ret = vpu_EncStartOneFrame(m_Handle, &m_Param); // Initiate encoding
	if (ret != RETCODE_SUCCESS) {
		ERROR("Encoder: vpu_EncStartOneFrame() failed. Return code: %d", ret);
	}

	while (vpu_IsBusy()) {
		vpu_WaitForInt(200); // Wait for encoding to complete
	}

	// Get encoding results (including address and size of bit stream)
	ret = vpu_EncGetOutputInfo(m_Handle, &outputInfo);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		UninitEncoder();
		ERROR("Encoder: vpu_EncGetOutputInfo() failed. Return code: %d", ret);
	}

	pBuf = (uint8_t *) (m_OutBuf.virt_uaddr + (outputInfo.bitstreamBuffer - m_OutBuf.phy_addr));
	VideoData outFrame;
	outFrame.SetBuf(pBuf, outputInfo.bitstreamSize);
	outFrame.SetTimestamp(inFrame.GetTimestamp());
	m_pCb->TransmitFrame(outFrame);

	if (0 == outputInfo.picType) {
		m_Param.forceIPicture = 0;
	}

	m_Timing.Update();
	m_Bitrate.Update(outputInfo.bitstreamSize);
	ENC_DBG("%s", m_Timing.GetStatStr());
	if (m_Bitrate.IsValid()) {
		ENC_DBG("%s", m_Bitrate.GetStatStr());
	}
}

void Encoder::GenerateHeader(AvcHeaderType headerType) {
	EncHeaderParam headerParam;
	RetCode ret = RETCODE_SUCCESS;
	uint8_t* pBuf = NULL;

	// Generate and insert header
	memset(&headerParam, 0, sizeof(EncHeaderParam));
	headerParam.headerType = headerType;
	pMutexLock scopedLock(*(m_VPar.mutex));
 	ret = vpu_EncGiveCommand(m_Handle, ENC_PUT_AVC_HEADER, &headerParam);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		UninitEncoder();
		ERROR("Encoder: vpu_EncGiveCommand(ENC_PUT_AVC_HEADER) failed. Return code: %d", ret);
		return;
	}

	pBuf = (uint8_t *) (m_OutBuf.virt_uaddr + (headerParam.buf - m_OutBuf.phy_addr));
	VideoData frame;
	frame.SetBuf(pBuf, headerParam.size);
	frame.SetTimestamp((getTimeUs() - m_StartTime)/1000);
	m_pCb->TransmitFrame(frame);
}

void Encoder::StartEncoding() {
	m_StartTime = getTimeUs();
	Start("Encoder");
}

void Encoder::StopEncoding() {
	Stop();
}

void Encoder::GenerateKeyFrame() {
	ENC_DBG("GenerateKeyFrame()");

	// Force the frame to be encoded as an Intra coded frame
	m_Param.forceIPicture = 1; // Force IDR picture
}

void Encoder::ChangeBitrate(uint32_t bitrate) {
	ENC_DBG("ChangeBitrate(%u)", bitrate);
	RetCode ret = RETCODE_SUCCESS;
	m_VPar.bitrate = bitrate;

	// Target bit rate in kbits per second
	pMutexLock scopedLock(*(m_VPar.mutex));
	ret = vpu_EncGiveCommand(m_Handle, ENC_SET_BITRATE, &m_VPar.bitrate);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		UninitEncoder();
		ERROR("Encoder: vpu_EncGiveCommand(ENC_SET_BITRATE) failed. Return code: %d", ret);
	}
}

void Encoder::ChangeFramerate(uint32_t framerate) {
	ENC_DBG("ChangeFramerate(%u)", framerate);
	RetCode ret = RETCODE_SUCCESS;
	m_VPar.framerate = framerate;

	// Target frame rate in fps
	pMutexLock scopedLock(*(m_VPar.mutex));
	ret = SetFramerate(m_Handle, (Uint32 *) &m_VPar.framerate);
	scopedLock.mutex.Release();
	if (ret != RETCODE_SUCCESS) {
		UninitEncoder();
		ERROR("Encoder: SetFramerate(%u) failed. Return code: %d", m_VPar.framerate, ret);
	}
}
