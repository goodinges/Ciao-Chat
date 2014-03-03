

#include <VideoTransportClient.hpp>

#ifndef VIDEO_TRANSPORT_SYSV
#ifndef VIDEO_TRANSPORT_POSIX
#error "Video transport undefined"
#endif
#endif
#ifdef VIDEO_TRANSPORT_SYSV
#include <SysVShm.hpp>
#endif
#ifdef VIDEO_TRANSPORT_POSIX
#include <PosixShm.hpp>
#endif

#ifdef __APPLE__
#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLContext.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/freeglut.h>
#endif

class glWindow
{
	public:
		glWindow( const char *wname );
		~glWindow();

		int getKey();

		static void run_events();

		static void displayFunc( void );
		static void idleFunc( void );

		static bool glutinitialized;

		void displayFrame();
		void queryFrame();

	private:
#ifdef VIDEO_TRANSPORT_SYSV
		VideoTransportClient <SysVShm> ipc;
#endif
#ifdef VIDEO_TRANSPORT_POSIX
		VideoTransportClient <PosixShm> ipc;
#endif

		static glWindow *first;
		glWindow *next;

		GLuint tex_id;
		int win_id;

		int textureWidth, textureHeight;

};
