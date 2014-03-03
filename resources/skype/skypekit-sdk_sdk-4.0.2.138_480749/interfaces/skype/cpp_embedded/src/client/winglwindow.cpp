#ifdef GL_WINDOW
#ifdef _WIN32

#include "winglwindow.h"

glWindow *glWindow::first = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			return 0;
		case WM_CLOSE:
			PostQuitMessage( 0 );
			return 0; 
		case WM_DESTROY:
			return 0;
		case WM_KEYDOWN:
			switch ( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					return 0;
			}
			return 0;

        case WM_SIZE:
			glViewport(0,0,LOWORD(lParam),HIWORD(lParam));
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			return 0;

		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

}

void glWindow::WinCreateWindow(char *title)
{
	WNDCLASS wc;

	// register window class
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "GLSample";
	RegisterClass( &wc );

	// create main window
	hWnd = CreateWindow(
			"GLSample", title,
			WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE | WS_MAXIMIZEBOX | WS_SIZEBOX,
			0, 0, 640, 480,
			NULL, NULL, NULL, NULL );

	PIXELFORMATDESCRIPTOR pfd;
	int format;

	hDC = GetDC( hWnd );

	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( hDC, &pfd );
	SetPixelFormat( hDC, format, &pfd );

	hRC = wglCreateContext( hDC );
	wglMakeCurrent( hDC, hRC );
}

glWindow::glWindow(char *title) :
	next( NULL ),
	textureWidth( 0 ),
	textureHeight( 0 ),
	hWnd( NULL ),
	hDC( NULL ),
	hRC( NULL),
	tex_id( 0 )	
{
	WinCreateWindow(title);
	
	glGenTextures(1, &tex_id);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		
	unsigned int fc[] = { 0x34324942 };
	ipc.setPreferences( 1, fc );
	
	next = first;
	first = this;
}

glWindow::~glWindow()
{
	DestroyWindow( hWnd );
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


void glWindow::displayFrame()
{
	if ( !ipc.isNewFrameAvailable() )
		return;
	VideoTransportBase::bufferstruct *b = ipc.getNewFrame();
	if ( !b || !b->clientpointer )
		return;
	void *data = ipc.bufferData( b );
	int w = b->width;
	int h = b->height;

	wglMakeCurrent( hDC, hRC );

	// need to make texture size of window, not size of bufferData
	RECT r;
	LPRECT lpRect = &r;
	HWND hWnd = WindowFromDC( hDC );
	if (!hWnd)
		MessageBox(NULL,"could not get window handle from DC","",MB_OK);
	GetWindowRect(hWnd,lpRect);
	textureWidth = lpRect->right - lpRect->left;
	textureHeight = lpRect->bottom - lpRect->top;

	glBindTexture( GL_TEXTURE_2D, tex_id );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data );
								
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, data );

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, tex_id );
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-1, 1);
	glTexCoord2f(1, 0);
	glVertex2f(1, 1);
	glTexCoord2f(1, 1);
	glVertex2f(1, -1);
	glTexCoord2f(0, 1);
	glVertex2f(-1, -1);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	SwapBuffers( hDC );
}

int glWindow::getKey()
{
	return ipc.key();
}

void glWindow::run_events()
{
	for( glWindow *w = glWindow::first; w; w = w->next )
		w->displayFrame();
}

void window_run_events()
{
	MSG msg;
	if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	glWindow::run_events();
}
#endif
#endif
