#include "OpenGLRenderer.hpp"
#include "SidVideoRTPInterface.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL/SDL.h>

#include "ColorConvert.hpp"
#include "RtpTeLogging.hpp"

#define MY_TARGET GL_TEXTURE_2D
#define MY_PIXEL_FMT GL_BGRA
#define MY_PIXEL_FMT_TYPE GL_UNSIGNED_BYTE

OpenGLRenderer::OpenGLRenderer(VideoParams *params, const char *title, int locX, int locY) :
	VideoRenderer(params),
	m_InitDone(false) {
	m_RgbBuf = new uint8_t[m_VPar.height * m_VPar.width * 4];
//	InitVideo(m_VPar.width, m_VPar.height);
	for (int i = 0; i < FRAME_QUEUE_LEN; i++)
		m_pFrameQueueBufs[i] = new VideoData(m_VPar.FrameSize());

	m_pFrameQueue = new StaticSyncQueue<VideoData*>(m_pFrameQueueBufs, FRAME_QUEUE_LEN);
	Start(); // start the thread
}

OpenGLRenderer::~OpenGLRenderer() {
	m_pFrameQueue->WakeWait();
	Stop();
	delete m_pFrameQueue;

	for (int i = 0; i < FRAME_QUEUE_LEN; i++)
		delete m_pFrameQueueBufs[i];

	delete[] m_RgbBuf;
}

void makeCheckImage(char * image, int width, int height) {
	int i, j, c;

	int index = 0;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (i < height - 10 && j < width - 30) {
				c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
				char * b1 = image + index++;
				char * b2 = image + index++;
				char * b3 = image + index++;
//				char * b4 = image + index++;
				*b1 = (GLubyte) c;
				*b2 = (GLubyte) c;
				*b3 = (GLubyte) c;
//				*b4 = (GLubyte) 255;
			} else {
				c = 0;
				char * b1 = image + index++;
				char * b2 = image + index++;
				char * b3 = image + index++;
//				char * b4 = image + index++;
				*b1 = (GLubyte) c;
				*b2 = (GLubyte) c;
				*b3 = (GLubyte) c;
//				*b4 = (GLubyte) 255;

			}
		}
	}
}

void OpenGLRenderer::InitVideo(int width, int height) {
	SDL_Surface *Win = NULL;
	GLuint texName;
/*
	glutInit ("", "");
	glutInitWindowSize (width, height);
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("CS248 GLUT example");
*/
	REND_DBG("InitVideo %dx%d", width, height);
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		Win = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE
					 | SDL_ANYFORMAT | SDL_OPENGL);

		if (Win) {
			printf("makeCheckImage\n");
			makeCheckImage((char *) m_RgbBuf, width, height);

			glGenTextures(1, &texName);
			glBindTexture(GL_TEXTURE_2D, texName);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, MY_PIXEL_FMT,
					MY_PIXEL_FMT_TYPE, m_RgbBuf);

			printf("glTexImage2D\n");
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glDisable(GL_CULL_FACE);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
}

void OpenGLRenderer::UninitVideo() {
	REND_DBG("uninit");
	SDL_Quit();
}

void OpenGLRenderer::RenderFrameInt(VideoData & frame) {
	if (!m_InitDone) {
		m_InitDone = true;
//		InitVideo(m_VPar.width, m_VPar.height);
	}
	REND_DBG("RenderFrame! %dx%d\n", m_VPar.width, m_VPar.height);
	ColorConvert::ConvertI420toBGRA(m_VPar.width, m_VPar.height, (uint8_t *)frame.Data(), m_RgbBuf);
	glClear(GL_COLOR_BUFFER_BIT);

	glTexSubImage2D(MY_TARGET, 0, 0, 0, m_VPar.width, m_VPar.height, MY_PIXEL_FMT,
			MY_PIXEL_FMT_TYPE, m_RgbBuf);

	glEnable(GL_TEXTURE_2D);


	switch(frame.m_Rotation) {
		case SkypeVideoRTPInterface::ROTATION_ANGLE_0:
			glBegin(GL_POLYGON);
			glTexCoord2f( 0, 0 ); glVertex2f( -1.0, 1.0 );
			glTexCoord2f( 1.0, 0 ); glVertex2f( 1.0, 1.0 );
			glTexCoord2f( 1.0, 1.0 ); glVertex2f( 1.0, -1.0 );
			glTexCoord2f( 0.0, 1.0 ); glVertex2f( -1.0, -1.0 );
			glEnd();
			break;
		case SkypeVideoRTPInterface::ROTATION_ANGLE_90:
			glBegin(GL_POLYGON);
			glTexCoord2f( 0, 0 ); glVertex2f( 1.0, 1.0 );
			glTexCoord2f( 1.0, 0 ); glVertex2f( 1.0, -1.0 );
			glTexCoord2f( 1.0, 1.0 ); glVertex2f( -1.0, -1.0 );
			glTexCoord2f( 0.0, 1.0 ); glVertex2f( -1.0, 1.0 );
			glEnd();
			break;
		case SkypeVideoRTPInterface::ROTATION_ANGLE_180:
			glBegin(GL_POLYGON);
			glTexCoord2f( 0, 0 ); glVertex2f( 1.0, -1.0 );
			glTexCoord2f( 1.0, 0 ); glVertex2f( -1.0, -1.0 );
			glTexCoord2f( 1.0, 1.0 ); glVertex2f( -1.0, 1.0 );
			glTexCoord2f( 0.0, 1.0 ); glVertex2f( 1.0, 1.0 );
			glEnd();
			break;
		case SkypeVideoRTPInterface::ROTATION_ANGLE_270:
			glBegin(GL_POLYGON);
			glTexCoord2f( 0, 0 ); glVertex2f( -1.0, -1.0 );
			glTexCoord2f( 1.0, 0 ); glVertex2f( -1.0, 1.0 );
			glTexCoord2f( 1.0, 1.0 ); glVertex2f( 1.0, 1.0 );
			glTexCoord2f( 0.0, 1.0 ); glVertex2f( 1.0, -1.0 );
			glEnd();
			break;
		default:
			ERROR("frame.m_Rotation ??? degrees %d\n", frame.m_Rotation);
	}

	glDisable(GL_TEXTURE_2D);

	SDL_GL_SwapBuffers();
}

void OpenGLRenderer::Run(void) {
	VideoData *frameBuf;
	int lastTimestampMs = 0;
	InitVideo(m_VPar.width, m_VPar.height);
	while (!Shutdown) {
		uint64_t renderStartUs = getTimeUs();
		frameBuf = m_pFrameQueue->GetReadPtr();
		if (!frameBuf) {
			break;
		}
		RenderFrameInt(*frameBuf);

		m_pFrameQueue->ReadDone();
		if (lastTimestampMs) {
			// consider sleep between frames only if queue not getting full
			if (m_pFrameQueue->GetCnt() < FRAME_QUEUE_LEN / 2) {
				int elapsedUs = (int)(getTimeUs() - renderStartUs);
				int timestampDiffUs = (frameBuf->GetTimestamp() - lastTimestampMs) * 1000;
				int sleepTimeUs = timestampDiffUs - elapsedUs;
				if (sleepTimeUs > 0 && sleepTimeUs < 200000) {
					//REND_DBG("X11Renderer::Run usleep: %d, elapsed: %d, timestampdiff: %d", sleepTimeUs, elapsedUs, timestampDiffUs);
					usleep(sleepTimeUs);
				} else {
					REND_DBG("OpenGLRenderer::Run DONT usleep: %d, elapsed: %d, timestampdiff: %d", timestampDiffUs, elapsedUs, timestampDiffUs);
				}
			} else {
				REND_DBG("OpenGLRenderer::Run DONT usleep: queue %d", m_pFrameQueue->GetCnt());
			}
		}
		lastTimestampMs = frameBuf->GetTimestamp();
	}
	UninitVideo();
	REND_DBG("Exit OpenGLRenderer::Run (%s)", m_pTitle)
}

void OpenGLRenderer::RenderFrame(VideoData & frame) {
	VideoData *frameBuf;
//	RenderFrameInt(yuvBuf);
	// Renderer can be too slow when software rendering is used. So
	// we just drop some frames so the slow renrerer can't slow  everything down
	if (m_pFrameQueue->GetCnt() == FRAME_QUEUE_LEN) {
		REND_DBG("%s: Dropping frame", m_pTitle);
	} else {
		frameBuf = m_pFrameQueue->GetWritePtr();
		memcpy((uint8_t *)frameBuf->Data(), frame.Data(), m_VPar.FrameSize());
		frameBuf->SetTimestamp(frame.GetTimestamp());
		frameBuf->SetRotation(frame.GetRotation());
		m_pFrameQueue->WriteDone();
	}
}
