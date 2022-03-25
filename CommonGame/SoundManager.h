#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include <map>
using std::map;

#include <string>
using std::string;

#include <list>
using std::list;

#include <al/alc.h>

#include "DataTypes.h"
#include "Vector.h"

class SoundManager
{
public:

	static const Uint INVALID_ID = -1;

	typedef map<string, Uint> SoundMap;
	typedef list<Uint> SourceList;

public:

	SoundManager();
	~SoundManager();

	static bool init();
	static bool destroy();
	
	Uint load(const string& filename);
	bool unload(const string& filename);

	void addSource(Uint sourceID);
	Uint getFreeSource();

private: 

	void deletePlayedSources();
	Uint getBuffer(const string& filename) const;

	Uint loadBuffer(const string& filename);
	void initListener();

private:
	static ALCcontext* context_;
	static ALCdevice* device_;
	static bool isInit_;
	
	SoundMap soundFiles_;		//filename:bufferID
	SourceList freeSources_;
	SourceList usedSources_;

	Vector listenerPosition_;
	Vector listenerVelocity_;
};

#endif
