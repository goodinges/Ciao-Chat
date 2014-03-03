#ifdef GL_WINDOW
#ifndef _WIN32

#include "glwindow.h"

#include <stdio.h>

bool glWindow::glutinitialized = false;
glWindow *glWindow::first = NULL;

glWindow::glWindow( const char *wname )
{
	textureWidth = 0; textureHeight = 0;
	unsigned int fc[] = { 0x34324942 };
	ipc.setPreferences(1, fc );

	if ( !glutinitialized ) {
		int dummy = 1;
		const char *argv[] = {"dummy"};
		glutInit( &dummy, (char **)argv );
		glutInitDisplayMode(GLUT_DOUBLE);
		glutinitialized = true;
	}
	glutInitWindowSize( 640, 480 );
	win_id = glutCreateWindow(wname);
	glutDisplayFunc(displayFunc);
	glutIdleFunc(idleFunc);

	glGenTextures(1,&tex_id);

	glBindTexture(GL_TEXTURE_2D, tex_id );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	next = first;
	first = this;
}

glWindow::~glWindow()
{
	glutDestroyWindow( win_id );
	if ( first == this )
		first = this->next;
	else {
		glWindow *w=first;
		while (w) {
			if ( w->next == this ) {
				w->next = this->next;
				break;
			}
		}
	}
}

void glWindow::idleFunc( void )
{
	for( glWindow *w = glWindow::first;w;w=w->next ) {
		w->queryFrame();
	}
}

void glWindow::displayFunc( void )
{
	/* Send displayFrame for each window. The window will decided
		if it's he who needs to be repainted */
	for( glWindow *w = glWindow::first;w;w=w->next ) {
		w->displayFrame();
	}
}

int glWindow::getKey()
{
	return ipc.key();
}

void glWindow::run_events()
{
	if ( glWindow::glutinitialized ) {
#ifdef __linux__
		glutMainLoopEvent();
		idleFunc();
#endif
#ifdef __APPLE__
		glutCheckLoop();
#endif
	}
}

void glWindow::queryFrame()
{
	if ( !ipc.isNewFrameAvailable() )
		return;
	/* We have a new frame. Let glut now that this window needs repainting.
		*/
	if ( win_id != glutGetWindow() )
		glutSetWindow( win_id );
	glutPostRedisplay();
}

void glWindow::displayFrame()
{
	/* This function is called from glut's displayFunc. displayFunc will
		set current window to the window that needs displaying, so
		we check if there are we */
	if ( win_id != glutGetWindow() )
		return;			 

	/* We well receive NULL at the beginning when there are no frames available.
		In that case we could show a message stating that video is starting.
	 */
	VideoTransportBase::bufferstruct *b = ipc.getFrame();
	
	//check shared memory pointer as well 
	if ( !b || !b->clientpointer)
		return;
	
	// cache the width, height immediately after getFrame to prevent any change from runtime during rendering prepare
	int width = b->width;
	int height = b->height;
	void* data = ipc.bufferData(b);

	GLenum texturetarget = GL_TEXTURE_2D;
	glEnable(texturetarget);
	glBindTexture( texturetarget, tex_id );
	
	if ( textureWidth != width || textureHeight != height ) {
		textureWidth = width; textureHeight = height;
		glTexImage2D(texturetarget, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data );
		glutReshapeWindow(textureWidth, textureHeight);
	} else {
		glTexSubImage2D(texturetarget, 0, 0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data );
	}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex2f(-1,1);
	glTexCoord2f(1,0);
	glVertex2f(1,1);
	glTexCoord2f(1,1);
	glVertex2f(1,-1);
	glTexCoord2f(0,1);
	glVertex2f(-1,-1);
	glEnd();
	glBindTexture(texturetarget,0);
	glDisable(texturetarget);
	glutSwapBuffers();
}

void window_run_events()
{
	glWindow::run_events();
}

#endif
#endif
