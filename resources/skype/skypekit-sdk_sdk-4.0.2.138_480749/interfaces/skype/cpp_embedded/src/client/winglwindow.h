#ifdef _WIN32

#include <VideoTransportClient.hpp>
#include <WinShm.hpp>

#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>

class glWindow {
	public:
		glWindow(char *title);
		~glWindow();

		int getKey();

		void WinCreateWindow(char *title);

		static void run_events();

		void displayFrame();

	private:		
		VideoTransportClient <WinShm> ipc;

		glWindow *next;
		static glWindow *first;

		GLuint tex_id;

		int textureWidth, textureHeight;
		
		HDC hDC;
		HWND hWnd;
		HGLRC hRC;
};

#endif
