#ifndef _SoundManager_h_
#define _SoundManager_h_

#include <vector>
#include <string>
#include <map>

#include "DAudioInterface.h"

typedef int SegmentID;
typedef IDirectMusicSegment8* Segment;
typedef std::vector<Segment> SegmentList;
typedef std::pair<SegmentID, Segment> SegmentPair;
typedef std::map<std::string, SegmentPair> SegmentMap;

class SoundManager
{
public:
	
	bool initialize(HWND hWnd);
	void cleanup();
	
	SegmentID loadSegment(std::string filepath);
	Segment getSegment(SegmentID segmentID);
	Segment getSegment(std::string filepath);
    void clearSegments();

	bool playSegment(SegmentID id);
	bool playMusicSegment(SegmentID id);
	bool stopSegment(SegmentID id);

private:
	DAudioInterface DAudio_;

	SegmentList segments_;
	SegmentMap files_;

//make this class a singleton
public:
	static SoundManager& getInstance()
	{
		static SoundManager instance;
		return instance;
	}

private:
	SoundManager() {}
	~SoundManager() {}
  
	SoundManager(const SoundManager&);
	SoundManager& operator=(SoundManager);

};

#endif
