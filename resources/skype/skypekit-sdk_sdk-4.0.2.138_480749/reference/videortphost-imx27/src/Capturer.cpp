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
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>

#include "Capturer.hpp"
#include "LoggingCtrl.hpp"

#define CLEANUP_ARRAY(ptr) if (ptr) {delete [] ptr; ptr = NULL;}

Capturer::Capturer(VideoParams* params, VideoRenderer* previewRenderer) :
	VideoCapturer(params, previewRenderer),
	m_Fd(-1),
	m_BufferCount(0),
	m_LastTimestamp(0),
	m_CapBufs(NULL)
{
	pMutexLock scopedLock(m_Mutex);
	OpenDevice();
	ConfigureDev();
}

Capturer::~Capturer() {
	pMutexLock scopedLock(m_Mutex);
	UnConfigureDev();
	CloseDevice();
}

void Capturer::OpenDevice() {
	if (m_Fd > 0) {
		WARN("Capture: device already opened");
	}

	// Open video input device
	if ((m_Fd = open(m_VPar.devName, O_RDWR, 0)) < 0) {
		FATAL("Capture: unable to open %s (%s)", m_VPar.devName, strerror(errno));
	}
}

void Capturer::ConfigureDev() {
	int index = 0;
	v4l2_std_id std_id = 0;
	struct v4l2_capability capability;
	struct v4l2_format fmt;
	bool resize = false;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_requestbuffers reqbufs;
	int i = 0;
	struct v4l2_buffer buffer;

	// Query current video input
	if (ioctl(m_Fd, VIDIOC_G_INPUT, &index) < 0) {
		ERROR("Capture: VIDIOC_G_INPUT failed (%s)", strerror(errno));
	}

	// Query current video standard
	if (ioctl(m_Fd, VIDIOC_G_STD, &std_id) < 0) {
		std_id = 0;
		ERROR("Capture: VIDIOC_G_STD failed (%s)", strerror(errno));
	}

	// Query device capabilities
	if (ioctl(m_Fd, VIDIOC_QUERYCAP, &capability) < 0) {
		strcpy((char *) capability.driver, "unknown");
		ERROR("Capture: VIDIOC_QUERYCAP failed (%s)", strerror(errno));
	}
	if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		UnConfigureDev();
		FATAL("Capture: not a video capture device");
	}
	if (!(capability.capabilities & V4L2_CAP_STREAMING)) {
		UnConfigureDev();
		FATAL("Capture: does not support streaming i/o");
	}

	// Set image format in driver
	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = m_VPar.pixFmt;
	fmt.fmt.pix.width = m_VPar.width;
	fmt.fmt.pix.height = m_VPar.height;
	if (ioctl(m_Fd, VIDIOC_S_FMT, &fmt) < 0) {
		ERROR("Capture: VIDIOC_S_FMT failed (%s)", strerror(errno));
	}

	// Get image format from driver
	if (ioctl(m_Fd, VIDIOC_G_FMT, &fmt) < 0) {
		UnConfigureDev();
		FATAL("Capture: VIDIOC_G_FMT failed (%s)", strerror(errno));
	}

	if (fmt.fmt.pix.pixelformat != m_VPar.pixFmt) {
		UnConfigureDev();
		FATAL("Capture: Pixelformat %c%c%c%c not supported by driver, must use %c%c%c%c", (m_VPar.pixFmt & 0xFF), ((m_VPar.pixFmt >> 8) & 0xFF), ((m_VPar.pixFmt >> 16) & 0xFF), ((m_VPar.pixFmt >> 24) & 0xFF), (fmt.fmt.pix.pixelformat & 0xFF), ((fmt.fmt.pix.pixelformat >> 8) & 0xFF), ((fmt.fmt.pix.pixelformat >> 16) & 0xFF), ((fmt.fmt.pix.pixelformat >> 24) & 0xFF));
	}
	if ((fmt.fmt.pix.pixelformat != (uint32_t) m_VPar.pixFmt) || (fmt.fmt.pix.width != (uint32_t) m_VPar.width) || (fmt.fmt.pix.height != (uint32_t) m_VPar.height)) {
		WARN("Capture: format %ux%u not supported by driver, cropping to %ux%u", m_VPar.width, m_VPar.height, fmt.fmt.pix.width, fmt.fmt.pix.height);
		resize = true;
	}

	INFO("Capture: %ux%u (%c%c%c%c) on %s", fmt.fmt.pix.width, fmt.fmt.pix.height, (fmt.fmt.pix.pixelformat & 0xFF), ((fmt.fmt.pix.pixelformat >> 8) & 0xFF), ((fmt.fmt.pix.pixelformat >> 16) & 0xFF), ((fmt.fmt.pix.pixelformat >> 24) & 0xFF), capability.driver);

	// Set streaming parameters (frame period)
	ChangeFramerate(m_VPar.framerate);

	// Crop the image depending on requested image size
	if (resize) {
		// Get limits for cropping rectangle
		memset(&cropcap, 0, sizeof(struct v4l2_cropcap));
		cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (ioctl(m_Fd, VIDIOC_CROPCAP, &cropcap) < 0) {
			UnConfigureDev();
			FATAL("Capture: VIDIOC_CROPCAP failed (%s)", strerror(errno));
		}

		// Set cropping rectangle to fit requested resolution
		memset(&crop, 0, sizeof(struct v4l2_crop));
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c.top = 0;
		crop.c.left = 0;
		crop.c.width = m_VPar.width;
		crop.c.height = m_VPar.height;
		if (ioctl(m_Fd, VIDIOC_S_CROP, &crop) < 0) {
			UnConfigureDev();
			FATAL("Capture: VIDIOC_S_CROP failed (%s)", strerror(errno));
		}
	}

	// Initiate memory mapped I/O (this will allocate memory mapped buffers)
	memset(&reqbufs, 0, sizeof(struct v4l2_requestbuffers));
	reqbufs.count = CAP_OUT_QUEUE_LEN;
	reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbufs.memory = V4L2_MEMORY_MMAP; // Memory mapped buffers
	if (ioctl(m_Fd, VIDIOC_REQBUFS, &reqbufs) < 0) {
		UnConfigureDev();
		FATAL("Capture: VIDIOC_REQBUFS failed (%s)", strerror(errno));
	}
	if (reqbufs.count < CAP_OUT_QUEUE_LEN) {
		WARN("Capture: Only %u capture buffers were allocated (requested %u)", reqbufs.count, CAP_OUT_QUEUE_LEN);
	}
	m_BufferCount = reqbufs.count;

	// Create a table of buffers for frame buffer data
	m_CapBufs = new vpu_mem_desc[m_BufferCount];

	// For each frame buffer, store driver frame buffer information
	for (i = 0; i < m_BufferCount; i++) {
		// Query buffers
		memset(&buffer, 0, sizeof(struct v4l2_buffer));
		buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buffer.memory = reqbufs.memory; // Memory mapped buffers
		buffer.index = i;
		if (ioctl(m_Fd, VIDIOC_QUERYBUF, &buffer) < 0) {
			UnConfigureDev();
			FATAL("Capture: VIDIOC_QUERYBUF failed (%s)", strerror(errno));
		}

		// Initialize the buffer with driver frame buffer information
		m_CapBufs[i].phy_addr = buffer.m.offset;
		m_CapBufs[i].virt_uaddr = 0;
		m_CapBufs[i].size = buffer.length;
	}

	// Prepare output frame buffer
	m_OutBuf.phy_addr = 0;
	m_OutBuf.virt_uaddr = 0;
	m_OutBuf.size = buffer.length;
}

void Capturer::UnConfigureDev() {
	CLEANUP_ARRAY(m_CapBufs);
	m_BufferCount = 0;
}

void Capturer::CloseDevice() {
	if (close(m_Fd) < 0) {
		ERROR("Capture: unable to close %s (%s)", m_VPar.devName, strerror(errno));
	}
	m_Fd = -1;
}

void Capturer::StartCapturing() {
	pMutexLock scopedLock(m_Mutex);
	int i = 0;
	struct v4l2_buffer buffer;
	enum v4l2_buf_type type;

	// Enqueue empty capturing buffers in the driver's incoming queue
	for (i = 0; i < m_BufferCount; i++) {
		memset(&buffer, 0, sizeof(struct v4l2_buffer));
		buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buffer.memory = V4L2_MEMORY_MMAP;
		buffer.index = i;
		if (ioctl(m_Fd, VIDIOC_QBUF, &buffer) < 0) {
			ERROR("Capture: VIDIOC_QBUF failed (%s)", strerror(errno));
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(m_Fd, VIDIOC_STREAMON, &type) < 0) {
		UnConfigureDev();
		FATAL("Capture: VIDIOC_STREAMON failed (%s)", strerror(errno));
	}
}

void Capturer::StopCapturing() {
	pMutexLock scopedLock(m_Mutex);
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(m_Fd, VIDIOC_STREAMOFF, &type) < 0) {
		ERROR("Capture: VIDIOC_STREAMOFF failed (%s)", strerror(errno));
	}
}

void Capturer::CaptureFrame(VideoData & frame) {
	pMutexLock scopedLock(m_Mutex);
	struct v4l2_buffer buffer;
	int timestamp = 0;

	// V4L2 buffer used to exchange data between app and capture driver
	// Contains pointers and meta-information, the data itself is not copied
	memset(&buffer, 0, sizeof(struct v4l2_buffer));

	// Dequeue source image from V4L2 capture buffer
	buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buffer.memory = V4L2_MEMORY_MMAP;
	if (ioctl(m_Fd, VIDIOC_DQBUF, &buffer) < 0) {
		ERROR("Capture: VIDIOC_DQBUF failed (%s)", strerror(errno));
	}

	m_OutBuf.phy_addr = buffer.m.offset;
	timestamp = buffer.timestamp.tv_sec * 1000 + buffer.timestamp.tv_usec / 1000;
	frame.SetTimestamp(timestamp);

	// Pass Buffer_Handle to encoder
	frame.SetBuf((uint8_t *) m_OutBuf.phy_addr, m_OutBuf.size);

	// Enqueue new source image to V4L2 capture buffer
	if (ioctl(m_Fd, VIDIOC_QBUF, &buffer) < 0) {
		ERROR("Capture: VIDIOC_QBUF failed (%s)", strerror(errno));
	}

	m_Timing.Update();
	CAP_DBG("%s", m_Timing.GetStatStr());
	CAP_DBG("Timestamp %d, diff: %d", frame.GetTimestamp(), frame.GetTimestamp() - m_LastTimestamp);
	m_LastTimestamp = frame.GetTimestamp();
}

void Capturer::ChangeFramerate(uint32_t framerate) {
	pMutexLock scopedLock(m_Mutex);
	struct v4l2_streamparm parm;
	m_VPar.framerate = framerate;

	memset(&parm, 0, sizeof(struct v4l2_streamparm));
	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	parm.parm.capture.timeperframe.numerator = 1;
	parm.parm.capture.timeperframe.denominator = m_VPar.framerate;
	parm.parm.capture.capturemode = 0; // Disable high quality imaging mode
	if (ioctl(m_Fd, VIDIOC_S_PARM, &parm) < 0) {
		ERROR("Capture: VIDIOC_S_PARM failed (%s)", strerror(errno));
	}
}
