#include "DAudioInterface.h"

DAudioInterface::DAudioInterface() : hWnd_(NULL), performance_(NULL), loader_(NULL)
{
}

DAudioInterface::~DAudioInterface()
{
	this->Cleanup();
}

bool DAudioInterface::Initialize(HWND hWnd)
{
	hWnd_ = hWnd;

	if(!InitAudio())
	{
		printf("Init Audio Failed!\n");
		return false;
	}

	return true;
}

void DAudioInterface::Cleanup()
{
	if(performance_)
	{
		performance_->CloseDown();
		performance_->Release();
	}

	if(loader_)
	{
		loader_->Release();
	}
    
	CoUninitialize();
}

bool DAudioInterface::InitAudio()
{
	HRESULT hr;

	hr = CoInitialize(NULL);
	if(hr != S_OK)
	{
		printf("CoInitialize Failed (InitAudio)!\n");
		return false;
	}
	
	hr = CoCreateInstance(
		CLSID_DirectMusicPerformance, 
		NULL, 
		CLSCTX_INPROC, 
		IID_IDirectMusicPerformance8, (void**) &performance_
		);
	if(hr != S_OK)
	{
		printf("CoCreateInstance Failed (InitAudio)!\n");
		return false;
	}

	hr = performance_->InitAudio(
		NULL,
		NULL,
		hWnd_,
		DMUS_APATH_SHARED_STEREOPLUSREVERB,
		32,
		DMUS_AUDIOF_ALL,
		NULL
		);
	if(hr != S_OK)
	{
		printf("InitAudio Failed (InitAudio)!\n");
		return false;
	}

	hr = CoCreateInstance(
		CLSID_DirectMusicLoader, 
		NULL, 
		CLSCTX_INPROC, 
        IID_IDirectMusicLoader8, 
		(void**) &loader_
		);
	if(hr != S_OK)
	{
		printf("CoCreateInstance on Loader Failed (InitAudio)!\n");
		return false;
	}

	char searchPath[255];
	WCHAR wSearchPath[255];

	if(GetCurrentDirectory(255, searchPath) == 0)
	{
		printf("GetCurrentDirectory failed (InitAudio)!\n");
		return false;
	}
	MultiByteToWideChar(CP_ACP, 0, searchPath, -1, wSearchPath, MAX_PATH);

	loader_->SetSearchDirectory(GUID_DirectMusicAllTypes, wSearchPath, FALSE);

	return true;
}

bool DAudioInterface::CreateSegment(IDirectMusicSegment8*& segment, const char* filename)
{
	HRESULT hr;
	
	hr = CoCreateInstance(
		CLSID_DirectMusicSegment, 
		NULL, 
		CLSCTX_INPROC, 
		IID_IDirectMusicSegment8, 
		(void**) &segment
		);
	if(hr != S_OK)
	{
		printf("CoCreateInstance Failed (CreateSegment)!\n");
		return false;
	}

	int length = (int)strlen(filename)+1;
	WCHAR* wFilename = new WCHAR[length+1];

	MultiByteToWideChar(CP_ACP, 0, filename, -1, wFilename, length);
	hr = loader_->LoadObjectFromFile(
        CLSID_DirectMusicSegment,
        IID_IDirectMusicSegment8,
        wFilename,
        (void**) &segment
		);
	if(hr != S_OK)
	{
		printf("LoadObjectFromFile: %s, Failed (CreateSegment)!\n");
		return false;
	}

	hr = segment->Download(performance_);
	if(hr != S_OK)
	{
		printf("Download Band Failed (CreateSegment)!\n");
		return false;
	}

	return true;
}

bool DAudioInterface::DeleteSegment(IDirectMusicSegment8* segment)
{
	if(segment)
	{
		segment->Release();
		segment = NULL;
		return true;
	}
	
	return false;
}

bool DAudioInterface::PlaySegment(IDirectMusicSegment8* segment)
{
	HRESULT hr;

	hr = performance_->PlaySegmentEx(segment, NULL, NULL, 0, 0, NULL, NULL, NULL);
	if(hr != S_OK)
	{
		printf("PlaySegmentEx Failed (PlaySegment)!\n");
		return false;
	}

	return true;
}

bool DAudioInterface::StopSegment(IDirectMusicSegment8* segment)
{
	HRESULT hr;
	hr = performance_->StopEx(segment, 0, 0);
	if(hr != S_OK)
	{
		printf("StopSegmentEx Failed (StopSegment)!\n");
		return false;
	}
	
	return true;
}

bool DAudioInterface::SetSegmentToRepeat(IDirectMusicSegment8* segment)
{
	HRESULT hr;
	hr = segment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
	if(hr != S_OK)
	{
		printf("SetRepeats Failed (SetSegmentToRepeat)!\n");
		return false;
	}

	return true;
}


