#include "main.h"

#include "Timer.h"
#include "GLObject.h"

#include "InputEvent.h"
#include "DInputInterface.h"
#include "DInputKeyboard.h"

#include "SubspaceGameManager.h"
#include "SubspaceGlobal.h"
#include "TimeProfiler.h"

static TimeProfiler& timeProfiler = *TimeProfiler::getInstance();

static SubspaceGameManager gameManager;
static DInputInterface directInput;
static DInputKeyboard keyboard(&directInput);

extern HWND g_hWnd;		// Holds Our Window Handle
extern HINSTANCE g_hInstance;		// Holds The Instance Of The Application

Timer displayTimer;
Timer gameTimer;

void GameInit()
{
	directInput.Initialize(g_hWnd, g_hInstance);
	keyboard.setCallback(GameInput);

	gameManager.init();

	gameTimer.start();
	displayTimer.start();

	directInput.Reacquire();	
}

void GameDestroy()
{
	gameManager.destroy();
}

void GameInput(InputEvent input)
{
	gameManager.handleInput(input, -1, -1);
}

void GameLoop()
{
	//Input Handling
	DIMOUSESTATE* mouse = 0;
	static const DIMOUSESTATE* prevMouse = 0;
	static mouseX = 0;
	static mouseY = 0;

	//TODO: add input toggle buttons - cntrl, alt, shift

	timeProfiler.enter("input");

	//reacquire input devices if need be
	if(directInput.Reacquire())			//TODO: make this only reacquire when necessary
	{
		keyboard.refresh();
		//timeProfiler.print();

		mouse = directInput.GetMouseState();
		if(!prevMouse)
			prevMouse = mouse;

		mouseX += mouse->lX;
		mouseY += mouse->lY;
		for(int i=0; i < 4; ++i)
		{
			if(mouse->rgbButtons[i] && !prevMouse->rgbButtons[i])
			{
				gameManager.handleInput(InputEvent((InputEventType)(i+MouseBegin), INPUT_STATE_DOWN, INPUT_STATE_UP), mouse->lX, mouse->lY);
			}
		}
	}


	//timeProfiler.exit("input");
	timeProfiler.exit();
	timeProfiler.enter("game update");
			
	//Update Game State
	static double updateAccum = 0;

	double timestep = gameTimer.getElapsedTime();	//how many milliseconds
	updateAccum += timestep;

	/*double updateRate = 10.0;
	while(updateAccum > updateRate)
	{
		gameManager.update(updateRate);		// TODO: turn trigger gets reset every update, need a persistent trigger
		updateAccum -= updateRate;				
	}*/
    gameManager.update(timestep);
	gameTimer.start();

	timeProfiler.exit("game update");
	timeProfiler.enter("display");
	
	//DrawGLScene();
	//Display
	static double displayAccum = 0;

	double time = displayTimer.getElapsedTime();
	displayAccum += time;	
	displayTimer.start();

	double fpsRate = 1000.0/85.0;
	if(displayAccum > fpsRate)		
	{
		DrawGLScene();					//TODO: this should be multithreaded to better assess framerate

		//printf("fps: %f\n", 1000.0/displayTimer.getElapsedTime());		
		displayAccum = fmod(displayAccum, fpsRate);
	}
	DrawGLScene();

	timeProfiler.exit("display");
}

void GameResize(int width, int height)
{
	subspaceGlobal.setWindowWidth(width);
	subspaceGlobal.setWindowHeight(height);

	gameManager.resize();
}

/*void GameTick(double value)
{	
	gameManager.update(value);
}*/

void GameDisplay()
{
	gameManager.display();
}