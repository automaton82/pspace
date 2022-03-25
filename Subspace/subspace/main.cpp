#include "main.h"

#include <math.h>
#include <time.h>

//#include "ExtendedTextBuffer.h"
#include "GLObject.h"
#include "MathUtil.h"
#include "SoundManager.h"

#include "WindowsConsole.h"

#include "SubspaceGlobal.h"
#include "SubspaceSound.h"

//#include "TimeStream.h"
#include "TimeProfiler.h"
#include "TimeStream.h"


HWND g_hWnd;
HINSTANCE g_hInstance;

static bool active = true;		// Window Active Flag Set To TRUE By Default
//HDC			hDC=NULL;		// Private GDI Device Context
//HGLRC		hRC=NULL;		// Permanent Rendering Context

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	srand(time(NULL));

	subspaceGlobal.setWindowWidth(800);
	subspaceGlobal.setWindowHeight(600);
	
	TextureData::setDefaultColorKey(Color(0, 0, 0));
	/*double test1 = ceil(1.0);
	double test2 = ceil(1.0/1.0);
	Uint test3 = 5 / 5 + 1;

	ExtendedTextBuffer bufferTest;
	bufferTest.setLineWidth(11);
	bufferTest.write("the quick\nbrown \nfox reallybigwordhere jumped over          \nthe lazy dog");*/

	// Create Our OpenGL Window
	if (!CreateGLWindow("Subspace 0.190",subspaceGlobal.getWindowWidth(),subspaceGlobal.getWindowHeight(),16))
	{
		return 0;									// Quit If Window Was Not Created
	}

	if(!CreateConsole())
	{
		return 0;
	}

	/*TimeProfiler* tp = new TimeProfiler("timetest.txt");
	tp->enter("1a");
	tp->enter("2a");
	tp->exit("2a");
	tp->enter("2b");
	tp->enter("3");
	tp->enter("4");
	tp->exit("3");
	tp->exit();
	tp->exit();
	tp->exit("1a");
	tp->enter("1b");
	delete tp;*/

	/*TimeProfiler* tp = new TimeProfiler("timetest.txt");
	tp->enter("input");
	tp->exit("input");
	tp->enter("display");
		tp->enter("hud");
		tp->exit("hud");
		tp->enter("zone");
			tp->enter("background");
			tp->exit("background");
			tp->enter("map");
			tp->exit("map");
			tp->enter("objects");
			tp->exit("objects");
		tp->exit("zone");
	tp->exit("display");
	tp->enter("game update");
		tp->enter("chat");
		tp->exit("chat");
		tp->enter("console");
		tp->exit("console");
		tp->enter("hud");
		tp->exit("hud");
		tp->enter("network");
		tp->exit("network");
		tp->enter("statBox");
		tp->exit("statBox");
		tp->enter("zone");
	tp->exit("game update");
	delete tp;*/

	g_hInstance = hInstance;

	GameInit();

	while(1)
	{
		if(PeekMessage(&msg, g_hWnd, 0, 0, PM_NOREMOVE))
		{	
			if(!GetMessage(&msg, g_hWnd, 0, 0))
			{
				return (int)msg.wParam;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			active = true;
			if(active)
			{
				GameLoop();
			}
			else
			{
				WaitMessage();
			}
		}
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDc;
	PAINTSTRUCT ps;
	
	switch(message)
	{
	case WM_ACTIVATE:
		if(wParam)
			active = true;
		else
			active = false;
		
		//active = (bool)wParam;

		/*if(!active)
		{
			gameTime.stop();
		}
		else
		{
			gameTime.go();
		}*/
	
		return 0;

	case WM_COMMAND:
		return 0;

	case WM_CREATE:
		return 0;

	case WM_PAINT:
		hDc = BeginPaint(hWindow,&ps);
		EndPaint(hWindow,&ps);

		return 0;

	case WM_SIZE:

		ReSizeGLScene((int)lParam & 0xFFFF, (int)lParam >> 16 & 0xFFFF);		// Resize And Initialize The GL Window
		return 0;

	case WM_CLOSE:
	
		GameDestroy();

		SendMessage(hWindow, WM_DESTROY, NULL, NULL);
		return 0;

	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWindow, message, wParam, lParam);
}


bool InitGL()
{
	//glObject.getRenderState();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glDepthFunc(GL_LESS);
	//glDepthMask(false);
	
	glDisable(GL_LIGHTING);
	//glEnable(GL_NORMALIZE);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_TEXTURE_2D);

	//glObject.releaseRenderState();

	return true;
}

void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	
		GameDisplay();
	
	glPopMatrix();
		
	SwapBuffers(glObject.getDeviceContext());
}

bool CreateGLWindow(char* title, int width, int height, int bits)
{
	//GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	g_hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= g_hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(g_hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								g_hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!glObject.init(32, g_hWnd))
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Initialize OpenGL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	glObject.getRenderState();

	ShowWindow(g_hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(g_hWnd);						// Slightly Higher Priority
	SetFocus(g_hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	return true;									// Success
}


void KillGLWindow()								// Properly Kill The Window
{
	if(!glObject.cleanup())
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}

	if (g_hWnd && !DestroyWindow(g_hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		g_hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", g_hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		g_hInstance=NULL;									// Set hInstance To NULL
	}
}

void ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	//glObject.getRenderState();

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double ratio = (double)width / (double)height;				//TODO: still a little off, because of projection view - eventually make it all orthographic? - no orthographic, add projection features (3d particles, etc.)
	double fov = atan(height / 600.0) * 180.0/MathUtil::PI;		//field of view decreases with height
	gluPerspective(fov, ratio, 10.0, 20000.0);
	//gluPerspective(45.0 / ratio, (double)1.0/(double)1.0, 10.0, 20000.0);

	glDepthRange(10, 20000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glScalef(1, -1, 1);

	//glObject.releaseRenderState();

	GameResize(width, height);
}
