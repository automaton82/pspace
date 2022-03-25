#include <dmusici.h>
#include <dsound.h>
#include <stdio.h>

#ifndef _DAudioInterface_h_
#define _DAudioInterface_h_

class DAudioInterface
{
public:
	DAudioInterface();
	~DAudioInterface();
	
	bool Initialize(HWND hWnd);	
	void Cleanup();			

	bool CreateSegment(IDirectMusicSegment8*& segment, const char* filename);
	bool DeleteSegment(IDirectMusicSegment8* segment);

	bool PlaySegment(IDirectMusicSegment8* segment);
	bool StopSegment(IDirectMusicSegment8* segment);

	bool SetSegmentToRepeat(IDirectMusicSegment8* segment);

private:
	bool InitAudio();
	

	HWND hWnd_;								//window handler

	//IDirectMusicPerformance8* performance_;
	IDirectMusicPerformance8* performance_;

	IDirectMusicLoader8* loader_;

};


#endif