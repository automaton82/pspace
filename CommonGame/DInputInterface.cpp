#include "DInputInterface.h"

bool DInputInterface::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	hWnd_ = hWnd;
	hInstance_ = hInstance;

	if(!InitDirectInput())
	{
		printf("InitDirectInput Failed!\n");
		return false;
	}

	if(!InitKeyboard())
	{
		printf("InitKeyboard Failed!\n");
		return false;
	}

	if(!InitMouse())
	{
		printf("InitMouse Failed!\n");
		return false;
	}

	return true;
}

bool DInputInterface::InitDirectInput()
{
	HRESULT hr;
    
	hr = DirectInput8Create(hInstance_, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpDI_ ,NULL);
	if(FAILED(hr))
	{
		printf("DirectInputCreate Failed!");
		return false;
	}
	return true;
}

bool DInputInterface::InitKeyboard()
{
	HRESULT hr;

	hr = lpDI_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	if(FAILED(hr))
	{
		printf("InitKeyboard Failed!");
		return false;
	}

	//hr = keyboard_->SetCommonDataFormat(DIFORMAT_KEYBOARD);
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		printf("SetDataFormat Failed!");
		return false;
	}
  
	hr = keyboard_->SetCooperativeLevel(hWnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hr))
	{
		printf("SetCooperativeLevel Failed!");
		return false;
	}
	
	hr = keyboard_->Acquire();
	if(FAILED(hr))
	{
		printf("Keyboard acquire failed.\n");
		return true;		//return false
	}

	return true;
}

bool DInputInterface::InitMouse()
{
	HRESULT hr;

	hr = lpDI_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	if(FAILED(hr))
	{
		printf("CreateDevice Failed!");
		return false;
	}
	
	hr = mouse_->SetCooperativeLevel(hWnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hr))
	{
		printf("SetCooperativeLevel Failed!");
		return false;
	}

	hr = mouse_->SetDataFormat(&c_dfDIMouse);
	if(FAILED(hr))
	{
		printf("SetDataFormat Failed!");
		return false;
	}

	hr = mouse_->Acquire();
	if(FAILED(hr))
	{
		return true;
		/*
		if(hr == DIERR_ACQUIRED) printf("Acquire Failed: Already Acquired");
		else if(hr == DIERR_INVALIDPARAM) printf("Acquire Failed: Invalid Param");
		else if(hr == DIERR_NOTINITIALIZED) printf("Acquire Failed: Not Initialized");
		else printf("Acquire Failed!");
		return false;*/
	}

	return true;
}

void DInputInterface::Cleanup()
{
	if(keyboard_)
	{
		keyboard_->Unacquire();
		keyboard_->Release();
	}
	if(mouse_)
	{
		mouse_->Unacquire();
		mouse_->Release();
	}
	if(lpDI_) lpDI_->Release();
}

bool DInputInterface::Reacquire()
{
	HRESULT hr;

	hr = mouse_->Acquire();
	if(FAILED(hr))
	{
		/*if(hr == DIERR_ACQUIRED) printf("Acquire Failed: Already Acquired");
		else if(hr == DIERR_INVALIDPARAM) printf("Acquire Failed: Invalid Param");
		else if(hr == DIERR_NOTINITIALIZED) printf("Acquire Failed: Not Initialized");
		else printf("Acquire Failed!");*/
		return false;
	}
	
	hr = keyboard_->Acquire();
	if(FAILED(hr))
	{
		/*if(hr == DIERR_ACQUIRED) printf("Acquire Failed: Already Acquired");
		else if(hr == DIERR_INVALIDPARAM) printf("Acquire Failed: Invalid Param");
		else if(hr == DIERR_NOTINITIALIZED) printf("Acquire Failed: Not Initialized");
		else printf("Acquire Failed!");*/
		return false;
	}

	return true;
}

const char* DInputInterface::GetKeyStates()
{
	HRESULT hr;

	hr = keyboard_->GetDeviceState(sizeof(char) * 256, (void**) &keyState_);
    if(FAILED(hr))
	{
		printf("GetDeviceState (KEYBOARD) Failed!");
		return NULL;
	}
	/*hr = GetKeyboardState((PBYTE)&keyState_);
    if(FAILED(hr))
	{
		printf("GetDeviceState (KEYBOARD) Failed!");
		return NULL;
	}*/

	return (const char*) &keyState_;
}

void DInputInterface::GetKeyStates(char* keyState)
{
	this->GetKeyStates();
	memcpy(keyState, keyState_, sizeof(char) * 256);
}

DIMOUSESTATE* DInputInterface::GetMouseState()
{
	HRESULT hr;

	hr = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), (void**) &mouseState_);
    if(FAILED(hr))
	{
		printf("GetDeviceState (MOUSE) Failed!");
		return NULL;
	}

	return &mouseState_;
}

char DInputInterface::GetAsciiFromScanCode(int n)
{
	unsigned short result;
	BYTE state[256];

	GetKeyboardState(state);

	unsigned int virtualKey = MapVirtualKey(n, 3);
	int chars = ToAscii(virtualKey, n, state, &result, 0);

	return (char)result & 0xFF;
}
