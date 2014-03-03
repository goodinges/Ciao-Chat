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

#include "Renderer.hpp"
#include "LoggingCtrl.hpp"

#define CLEANUP_ARRAY(ptr) if (ptr) {delete [] ptr; ptr = NULL;}

Renderer::Renderer(VideoParams*params) :
	VideoRenderer(params),
	m_Fd(-1),
	m_FrameBufferCount(0),
	m_DispBufs(NULL),
	m_InitDone(false)
{
	m_VPar.width = (m_VPar.width + 15) & ~15;
	m_VPar.height = (m_VPar.height + 15) & ~15;
	pMutexLock scopedLock(m_Mutex);
	OpenDevice();
	ConfigureDev();
}

Renderer::~Renderer() {
	pMutexLock scopedLock(m_Mutex);
	UnConfigureDev();
	CloseDevice();
}

void Renderer::OpenDevice() {
	if (m_Fd > 0) {
		WARN("Render: device already opened");
	}

	// Open video display device
	if ((m_Fd = open(m_VPar.devName, O_RDWR, 0)) < 0) {
		FATAL("Render: unable to open %s (%s)", m_VPar.devName, strerror(errno));
	}
}

void Renderer::ConfigureDev() {
	int out = 0;
	struct v4l2_capability capability;
	struct v4l2_control ctrl;
	struct v4l2_framebuffer framebuffer;
	struct v4l2_cropcap cropcap;
	int tmp = 0;
	struct v4l2_format fmt;
	bool resize = false;
	struct v4l2_crop crop;

	// Select video output number '0'
	out = 0;
	if (ioctl(m_Fd, VIDIOC_S_OUTPUT, &out) < 0) {
		UnConfigureDev();
		FATAL("Render: VIDIOC_S_OUTPUT failed");
	}

	// Query device capabilities
	if (ioctl(m_Fd, VIDIOC_QUERYCAP, &capability) < 0) {
		strcpy((char *) capability.card, "unknown");
		strcpy((char *) capability.driver, "unknown");
		ERROR("Render: VIDIOC_QUERYCAP failed (%s)", strerror(errno));
	}
	if (!(capability.capabilities & V4L2_CAP_VIDEO_OUTPUT)) {
		UnConfigureDev();
		FATAL("Render: not a video display device");
	}
	if (!(capability.capabilities & V4L2_CAP_STREAMING)) {
		UnConfigureDev();
		FATAL("Render: does not support streaming i/o");
	}

	// Set custom (driver specific) control value
	ctrl.id = V4L2_CID_PRIVATE_BASE;
	ctrl.value = 0;
	if (ioctl(m_Fd, VIDIOC_S_CTRL, &ctrl) < 0) {
		UnConfigureDev();
		FATAL("Render: VIDIOC_S_CTRL failed (%s)", strerror(errno));
	}

	// Set frame buffer overlay parameters
	memset(&framebuffer, 0, sizeof(struct v4l2_framebuffer));
	framebuffer.capability = V4L2_FBUF_CAP_EXTERNOVERLAY; // Non-destructive
	framebuffer.flags = V4L2_FBUF_FLAG_PRIMARY; // Primary graphics surface
	if (ioctl(m_Fd, VIDIOC_S_FBUF, &framebuffer) < 0) {
		UnConfigureDev();
		FATAL("Render: VIDIOC_S_FBUF failed (%s)", strerror(errno));
	}

	// Get limits for cropping rectangle
	memset(&cropcap, 0, sizeof(struct v4l2_cropcap));
	cropcap.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	if (ioctl(m_Fd, VIDIOC_CROPCAP, &cropcap) < 0) {
		UnConfigureDev();
		FATAL("Render: VIDIOC_CROPCAP failed (%s)", strerror(errno));
	}

	if ((cropcap.bounds.width < cropcap.bounds.height) && (m_VPar.width > m_VPar.height)) {
		INFO("Render: Rotating image from %ux%u to %ux%u fit on display device", m_VPar.width, m_VPar.height, m_VPar.height, m_VPar.width);
		tmp = m_VPar.width;
		m_VPar.width = m_VPar.height;
		m_VPar.height = tmp;
		m_Rotate = true;
	}

	// Get image format from driver
	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	if (ioctl(m_Fd, VIDIOC_G_FMT, &fmt) < 0) {
		ERROR("Render: VIDIOC_G_FMT failed (%s)", strerror(errno));
	}

	// Set image format in driver
	fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	fmt.fmt.pix.pixelformat = m_VPar.pixFmt;
	fmt.fmt.pix.bytesperline = m_VPar.width; // Stride
	fmt.fmt.pix.width = m_VPar.width;
	fmt.fmt.pix.height = m_VPar.height;
	if (ioctl(m_Fd, VIDIOC_S_FMT, &fmt) < 0) {
		ERROR("Render: VIDIOC_S_FMT failed (%s)", strerror(errno));
	}

	// Get image format from driver
	if (ioctl(m_Fd, VIDIOC_G_FMT, &fmt) < 0) {
		UnConfigureDev();
		FATAL("Render: VIDIOC_G_FMT failed (%s)", strerror(errno));
	}

	if (fmt.fmt.pix.pixelformat != m_VPar.pixFmt) {
		FATAL("Render: Pixelformat %c%c%c%c not supported by driver, must use %c%c%c%c", (m_VPar.pixFmt & 0xFF), ((m_VPar.pixFmt >> 8) & 0xFF), ((m_VPar.pixFmt >> 16) & 0xFF), ((m_VPar.pixFmt >> 24) & 0xFF), (fmt.fmt.pix.pixelformat & 0xFF), ((fmt.fmt.pix.pixelformat >> 8) & 0xFF), ((fmt.fmt.pix.pixelformat >> 16) & 0xFF), ((fmt.fmt.pix.pixelformat >> 24) & 0xFF));
	}
	if ((cropcap.bounds.width < (int32_t) m_VPar.width) || (cropcap.bounds.height < (int32_t) m_VPar.height)) {
		INFO("Render: format %ux%u out of bounds, cropping to %ux%u", m_VPar.width, m_VPar.height, cropcap.bounds.width, cropcap.bounds.height);
		resize = true;
	}

	INFO("Render: %ux%u (%c%c%c%c) on %s", fmt.fmt.pix.width, fmt.fmt.pix.height, (fmt.fmt.pix.pixelformat & 0xFF), ((fmt.fmt.pix.pixelformat >> 8) & 0xFF), ((fmt.fmt.pix.pixelformat >> 16) & 0xFF), ((fmt.fmt.pix.pixelformat >> 24) & 0xFF), capability.driver);

	// Crop the image depending on requested image size
	memset(&crop, 0, sizeof(struct v4l2_crop));
	crop.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	crop.c.top = 0;
	crop.c.left = 0;
	crop.c.width = m_VPar.width;
	crop.c.height = m_VPar.height;
	// Set cropping rectangle to fit max resolution of display dev
	GetCropRatio(crop.c.width, crop.c.height, cropcap.bounds.width, cropcap.bounds.height);
	if (ioctl(m_Fd, VIDIOC_S_CROP, &crop) < 0) {
		UnConfigureDev();
		FATAL("Render: VIDIOC_S_CROP failed (%s)", strerror(errno));
	}
}

void Renderer::UnConfigureDev() {
	enum v4l2_buf_type type;
	int i = 0;

	// Stop streaming output (removes all buffers from queues)
	type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	if (ioctl(m_Fd, VIDIOC_STREAMOFF, &type) < 0) {
		ERROR("Render: VIDIOC_STREAMOFF failed (%s)", strerror(errno));
	}

	for (i = 0; i < m_FrameBufferCount; i++) {
		// Unmap memory from user space
		if (munmap((void *) m_DispBufs[i].virt_uaddr, m_DispBufs[i].size) < 0) {
			ERROR("Render: Failed to unmap display buffer %d (%s)", i, strerror(errno));
		}
		m_FrameBufferCount--;
	}
	CLEANUP_ARRAY(m_DispBufs);

	m_InitDone = false;
}

void Renderer::CloseDevice() {
	if (close(m_Fd) < 0) {
		ERROR("Render: unable to close %s (%s)", m_VPar.devName, strerror(errno));
	}
	m_Fd = -1;
}

void Renderer::GetCropRatio(int& width, int& height, int maxwidth, int maxheight) {
	// Scale decoded image size to maximum display device resolution
	while ((width < maxwidth) || (height < maxheight)) {
		width *= 2;
		height *= 2;
	}
	// Shrink decoded image size to fit on display device
	while ((width > maxwidth) || (height > maxheight)) {
		width /= 2;
		height /= 2;
	}
}

void Renderer::RenderFrame(VideoData& frame) {
	pMutexLock scopedLock(m_Mutex);
	struct v4l2_buffer buffer;
	enum v4l2_buf_type type;

	if (m_Fd < 0) {
		WARN("Render: device not opened");
		return;
	}

	// V4L2 buffer used to exchange data between app and display driver
	// Contains pointers and meta-information, the data itself is not copied
	memset(&buffer, 0, sizeof(struct v4l2_buffer));

	// Check status of buffer
	buffer.index = *(frame.Data());
	buffer.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	buffer.memory = V4L2_MEMORY_MMAP;
	if (ioctl(m_Fd, VIDIOC_QUERYBUF, &buffer) < 0) {
		ERROR("Render: VIDIOC_QUERYBUF failed (%s)", strerror(errno));
		return;
	}

	// Enqueue new source image to V4L2 display buffer
	if (ioctl(m_Fd, VIDIOC_QBUF, &buffer) < 0) {
		ERROR("Render: VIDIOC_QBUF failed (%s)", strerror(errno));
	}

	// Start Streaming on display device
	if (!m_InitDone) {
		if (*(frame.Data()) < (m_FrameBufferCount - 1)) {
			return;
		}
		type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		if (ioctl(m_Fd, VIDIOC_STREAMON, &type) < 0) {
			UnConfigureDev();
			FATAL("Render: VIDIOC_STREAMON failed (%s)", strerror(errno));
		}
		m_InitDone = true;
	}

	// Dequeue source image from V4L2 display buffer
	if (ioctl(m_Fd, VIDIOC_DQBUF, &buffer) < 0) {
		ERROR("Render: VIDIOC_DQBUF failed (%s)", strerror(errno));
	}

	m_Timing.Update();
	REND_DBG("%s", m_Timing.GetStatStr());
}

void* Renderer::InitFrameBuffers(uint32_t* bufCount) {
	pMutexLock scopedLock(m_Mutex);
	struct v4l2_requestbuffers reqbufs;
	int i = 0;
	struct v4l2_buffer buffer;
	void* pFrameBuf = NULL;

	// Add two more buffers for rotation
	if (m_Rotate) {
		*bufCount += 2;
	}

	// Initiate memory mapped I/O (this will allocate memory mapped buffers)
	memset(&reqbufs, 0, sizeof(struct v4l2_requestbuffers));
	reqbufs.count = *bufCount;
	reqbufs.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	reqbufs.memory = V4L2_MEMORY_MMAP; // Memory mapped buffers
	if (ioctl(m_Fd, VIDIOC_REQBUFS, &reqbufs) < 0) {
		FATAL("Render: VIDIOC_REQBUFS failed (%s)", strerror(errno));
	}
	if (reqbufs.count < *bufCount) {
		WARN("Render: Only %u display buffers were allocated (requested %u)", reqbufs.count, *bufCount);
	}

	// Create a table of buffers for frame buffer data
	m_DispBufs = new vpu_mem_desc[reqbufs.count];

	// For each frame buffer, map memory to user space
	for (i = 0; i < (int) reqbufs.count; i++) {
		// Query buffers
		memset(&buffer, 0, sizeof(struct v4l2_buffer));
		buffer.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		buffer.memory = reqbufs.memory; // Memory mapped buffers
		buffer.index = i;
		if (ioctl(m_Fd, VIDIOC_QUERYBUF, &buffer) < 0) {
			UnConfigureDev();
			FATAL("Render: VIDIOC_QUERYBUF failed (%s)", strerror(errno));
		}

		// Map memory to user space
		pFrameBuf = mmap(NULL, buffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, m_Fd, buffer.m.offset);
		if (MAP_FAILED == pFrameBuf) {
			UnConfigureDev();
			FATAL("Render: Failed to map display buffer %d (%s)", i, strerror(errno));
		}

		// Initialize the Buffer with driver frame buffer information
		m_DispBufs[i].phy_addr = buffer.m.offset;
		m_DispBufs[i].virt_uaddr = (unsigned long) pFrameBuf;
		m_DispBufs[i].size = buffer.length;
		m_FrameBufferCount++;
	}

	*bufCount = m_FrameBufferCount;
	// Send V4L2 display device frame buffers to decoder
	return m_DispBufs;
}
