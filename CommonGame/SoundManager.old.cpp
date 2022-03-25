#include "SoundManager.h"

/*SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}*/

bool SoundManager::initialize(HWND hWnd)
{
	if(!DAudio_.Initialize(hWnd)) return false;
	else return true;
}

void SoundManager::cleanup()
{
	clearSegments();
}

bool SoundManager::playSegment(SegmentID id)
{
	Segment s = getSegment(id);
	if(s == NULL) return false;

	if(!DAudio_.PlaySegment(s))
	{
		printf("Play Failed!\n");
		return false;
	}
	else return true;
}

bool SoundManager::playMusicSegment(SegmentID id)
{
	Segment s = getSegment(id);
	if(s == NULL) return false;

	DAudio_.SetSegmentToRepeat(s);
	if(!DAudio_.PlaySegment(s))
	{
		printf("Play Failed!\n");
		return false;
	}
	else return true;
}

bool SoundManager::stopSegment(SegmentID id)
{
	Segment s = getSegment(id);
	if(s == NULL) return false;

    if(!DAudio_.StopSegment(s))
	{
		printf("Stop Failed!\n");
		return false;
	}
	else return true;
}

SegmentID SoundManager::loadSegment(std::string filepath)
{	
	SegmentPair s = files_[filepath];
	if(s.second == NULL)
	{
		Segment newSegment;
		DAudio_.CreateSegment(newSegment, filepath.c_str());
		if(newSegment == NULL)
		{
			return -1;
		}
		else
		{
			segments_.push_back(newSegment);
			files_[filepath] = SegmentPair((SegmentID)segments_.size()-1, newSegment); 

			return (SegmentID)segments_.size()-1;
		}
	}
	else
	{
		return s.first;
	}
}

Segment SoundManager::getSegment(SegmentID segmentID)
{
	if(segmentID < 0) return NULL;
	if(segmentID >= (int)segments_.size()) return NULL;
	else return segments_[segmentID];
}

Segment SoundManager::getSegment(std::string filepath)
{
	Segment s = files_[filepath].second;

	if(!s) return NULL;
	else return s;
}
	
void SoundManager::clearSegments()
{
	SegmentList::iterator i;
	for(i = segments_.begin(); i != segments_.end(); i++)
	{
		Segment s = *i;
		if(s != NULL)
		{
			DAudio_.DeleteSegment(s);
		}
	}

	files_.clear();
}