
#define USE_POSIX_TRANSPORT	1
//#define USE_SYSV_TRANSPORT	1

#include <VideoTransportClient.hpp>
#ifdef USE_SYSV_TRANSPORT
#include <SysVShm.hpp>
#endif
#ifdef USE_POSIX_TRANSPORT
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
#ifdef USE_SYSV_TRANSPORT
		VideoTransportClient <SysVShm> ipc;
#endif
#ifdef USE_POSIX_TRANSPORT
		VideoTransportClient <PosixShm> ipc;
#endif

		static glWindow *first;
		glWindow *next;

		GLuint tex_id;
		int win_id;

		int textureWidth, textureHeight;

};
