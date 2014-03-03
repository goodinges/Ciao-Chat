/*
 * Copyright (C) 2007-2009 Skype Technologies S.A. Confidential and proprietary
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
#include "V4l2Capture.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <assert.h>

#undef LOG_PREFIX
#define LOG_PREFIX "V4l2Capture:"

#include "debugging.hpp"

#define CAP_DBG DBG

struct FrameBuf {
	void *start;
	size_t length;
};

V4l2Capture::V4l2Capture(const char *devName) :
	m_pDevName(devName),
	m_IsConfigured(0),
	m_pFrameBuf(NULL),
	m_NBuffers(0)
{
	OpenDevice();
}

V4l2Capture::~V4l2Capture()
{
	if (m_IsConfigured)
		UnConfigureDev();

	CloseDevice();
}

#define CLEAR(x) memset (&(x), 0, sizeof (x))

static int xioctl(int fd, int request, void * arg)
{
	int r;

	do {
		r = ioctl(fd, request, arg);
	} while (-1 == r && EINTR == errno);
	return r;
}

void V4l2Capture::OpenDevice()
{
	struct stat st;

	if (-1 == stat(m_pDevName, &st)) {
		FATAL("Cannot identify '%s': %d, %s\n", m_pDevName, errno,
				strerror(errno));
	}

	if (!S_ISCHR(st.st_mode)) {
		FATAL("%s is no device\n", m_pDevName);
	}

	m_Fd = open(m_pDevName, O_RDWR /* required */| O_NONBLOCK, 0);

	if (-1 == m_Fd) {
		FATAL("Cannot open '%s': %d, %s\n", m_pDevName, errno,
				strerror(errno));
	}
}

void V4l2Capture::CloseDevice()
{
	if (-1 == close(m_Fd))
		FATAL("close");

	m_Fd = -1;
}

bool V4l2Capture::SetFramerate(int fps)
{/*
	struct v4l2_streamparm parm;
	memset(&parm, 0, sizeof(parm));
	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	// first get current values
	if (xioctl(m_Fd, VIDIOC_G_PARM, &parm) == 0) {

		DBG("current framerate params: numerator %d denominator, %d", parm.parm.capture.timeperframe.numerator, 
				parm.parm.capture.timeperframe.denominator);
		
		DBG("SetFramerate VIDIOC_G_PARM: %d", fps);

		// change rate
		// time per frame is numerator / denominator in seconds
		parm.parm.capture.timeperframe.numerator = 1;
		parm.parm.capture.timeperframe.denominator = fps;
		
		// set new values
		if(-1 == xioctl(m_Fd, VIDIOC_S_PARM, &parm)) {
			ERROR("V4l2Capture: Failed to set streamparm: %s\n", strerror(errno));
			return false;
		}
	// now comes the special hack for pwc driver...
	// http://osdir.com/ml/drivers.pwc/2005-11/msg00001.html
	} else {
		DBG("V4l2Capture: Failed to get streamparm: %s, trying pwc driver hack\n", strerror(errno));
	
		struct v4l2_format fmt;
		CLEAR(fmt);
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
		if (-1 == xioctl(m_Fd, VIDIOC_G_FMT, &fmt)) {
			ERROR("V4l2Capture: VIDIOC_G_FMT failed %s\n", strerror(errno));
			return false;
		}
		
#define PWC_FPS_SHIFT           16
		fmt.fmt.pix.priv = (fps << PWC_FPS_SHIFT);
		if (-1 == xioctl(m_Fd, VIDIOC_S_FMT, &fmt)) {
			ERROR("V4l2Capture: VIDIOC_S_FMT failed %s\n", strerror(errno));
			return false;
		}
	}*/
	return true;
}

bool V4l2Capture::SetFormat(uint32_t width, uint32_t height, uint32_t pixFmt) {
	struct v4l2_format fmt;

	CLEAR(fmt);

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = width;
	fmt.fmt.pix.height = height;
	fmt.fmt.pix.pixelformat = pixFmt;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl(m_Fd, VIDIOC_S_FMT, &fmt))
		return false;

	CAP_DBG("width: %d, height: %d, size: %d, bpl: %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.sizeimage, fmt.fmt.pix.bytesperline);

	m_FrameSize = fmt.fmt.pix.sizeimage;

	/* Note VIDIOC_S_FMT may change width and height. */
	if (fmt.fmt.pix.width != width || fmt.fmt.pix.height != height) {
		ERROR("VIDIOC_S_FMT changed width or height: wanted %d %d, got %d %d", width, 
				height, fmt.fmt.pix.width, fmt.fmt.pix.height);
		return false;
	}
	
	return true;
}

bool V4l2Capture::ConfigureDev(uint32_t width, uint32_t height, uint32_t pixFmt, uint32_t framerate)
{
	struct v4l2_capability cap;

	if (-1 == xioctl(m_Fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL== errno) {
			FATAL("%s is no V4L2 device\n", m_pDevName);
		} else {
			FATAL("VIDIOC_QUERYCAP failed");
		}
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		FATAL("%s is no video capture device\n", m_pDevName);
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		FATAL("%s does not support streaming i/o\n", m_pDevName);
	}

	if (!SetFormat(width, height, pixFmt)) {
		ERROR("SetFormat failed");
		return false;
	}

	if (!SetFramerate(framerate)) {
		ERROR("SetFramerate failed");
		return false;
	}
	
	struct v4l2_requestbuffers req;

	CLEAR (req);

	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl (m_Fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			FATAL("%s does not support "
					"memory mapping\n", m_pDevName);
		} else {
			FATAL("VIDIOC_REQBUFS failed");
		}
	}

	CAP_DBG("req.count: %d", req.count);
	if (req.count < 2) {
		FATAL("Insufficient buffer memory on %s\n",
				m_pDevName);
	}

	m_pFrameBuf = (FrameBuf *) calloc (req.count, sizeof (*m_pFrameBuf));

	if (!m_pFrameBuf) {
		FATAL("Out of memory\n");
	}

	for (m_NBuffers = 0; m_NBuffers < req.count; ++m_NBuffers) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = m_NBuffers;

		if (-1 == xioctl (m_Fd, VIDIOC_QUERYBUF, &buf))
			FATAL("VIDIOC_QUERYBUF failed");

		m_pFrameBuf[m_NBuffers].length = buf.length;
		m_pFrameBuf[m_NBuffers].start =
		mmap (NULL /* start anywhere */,
				buf.length,
				PROT_READ | PROT_WRITE /* required */,
				MAP_SHARED /* recommended */,
				m_Fd, buf.m.offset);

		if (MAP_FAILED == m_pFrameBuf[m_NBuffers].start)
			FATAL("mmap");
	}

	StartCapturing();
	m_IsConfigured = 1;
	return true;
}

void V4l2Capture::UnConfigureDev()
{
	uint32_t i;

	m_IsConfigured = 0;
	StopCapturing();

	for (i = 0; i < m_NBuffers; ++i)
		if (-1 == munmap(m_pFrameBuf[i].start, m_pFrameBuf[i].length))
			FATAL("munmap");
	free(m_pFrameBuf);
}

void V4l2Capture::StartCapturing()
{
	uint32_t i;
	enum v4l2_buf_type type;

	for (i = 0; i < m_NBuffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == xioctl(m_Fd, VIDIOC_QBUF, &buf))
			FATAL("VIDIOC_QBUF failed");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(m_Fd, VIDIOC_STREAMON, &type))
		FATAL("VIDIOC_STREAMON failed");
}

void V4l2Capture::StopCapturing()
{
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(m_Fd, VIDIOC_STREAMOFF, &type))
		FATAL("VIDIOC_STREAMOFF failed");
}

void V4l2Capture::ReadFrame(struct v4l2_buffer *buf)
{
	if (!m_IsConfigured) {
		ERROR("Not configured");
		return;
	}

	for (;;) {
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO (&fds);
		FD_SET (m_Fd, &fds);

		/* Timeout. */
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		r = select(m_Fd + 1, &fds, NULL, NULL, &tv);
		if (-1 == r) {
			if (EINTR== errno)
				continue;

			FATAL("select failed");
		}

		if (0 == r) {
			FATAL("select timeout\n");
		}

		CLEAR (*buf);

		buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf->memory = V4L2_MEMORY_MMAP;

		r = xioctl (m_Fd, VIDIOC_DQBUF, buf);
		if (-1 == r) {
			switch (errno) {
			case EAGAIN:
				continue;
			case EIO:
				WARN("VIDIOC_DQBUF returned EIO");
				return;
			default:
				FATAL("VIDIOC_DQBUF failed");
			}
		} else {
			assert (buf->index < m_NBuffers);
			return;
		}
	}
}

void V4l2Capture::CaptureFrame(uint8_t *frameBuf, uint32_t frameBufLen, int &timestamp)
{
	struct v4l2_buffer buf;

	ReadFrame(&buf);
	
	assert(m_FrameSize == frameBufLen);
	
	memcpy(frameBuf, m_pFrameBuf[buf.index].start, m_FrameSize);

	timestamp = buf.timestamp.tv_sec * 1000 + buf.timestamp.tv_usec / 1000;
	if (-1 == xioctl(m_Fd, VIDIOC_QBUF, &buf))
		FATAL("VIDIOC_QBUF failed");
}
