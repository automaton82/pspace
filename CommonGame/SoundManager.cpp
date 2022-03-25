#include "SoundManager.h"

#include <al/al.h>
#include <al/alc.h>
#include <al/alu.h>
#include <al/alut.h>

#include <assert.h>

ALCcontext* SoundManager::context_ = 0;
ALCdevice* SoundManager::device_ = 0;
bool SoundManager::isInit_ = false;

SoundManager::SoundManager()
{
	if(isInit_)
		initListener();
}

SoundManager::~SoundManager()
{
}


bool SoundManager::init()
{
	device_ = alcOpenDevice((ALubyte*)"DirectSound3D");
	if(!device_)
		return false;

	context_ = alcCreateContext(device_, 0);
	if(!context_)
		return false;

	alcMakeContextCurrent(context_);
	ALenum error = alGetError();

	//alutInit(0, 0);

	isInit_ = true;

	return true;
}

bool SoundManager::destroy()
{
	if(context_)
		alcDestroyContext(context_);

	if(device_)
		alcCloseDevice(device_);

	alutExit();

	return true;
}

void SoundManager::addSource(Uint sourceID)
{
	deletePlayedSources();

	assert(sourceID != INVALID_ID);

	usedSources_.push_back(sourceID);
}

Uint SoundManager::getFreeSource()
{
	deletePlayedSources();

	Uint sourceID = INVALID_ID;
	//if(freeSources_.size() > 0)		//use an already created source
	if(0)
	{
		sourceID = freeSources_.front();
		freeSources_.pop_front();

		alSourceRewind(sourceID);

		//printf("Reusing sound source [%d] - Total (%d)\n", sourceID, freeSources_.size(), usedSources_.size());
	}
	else							//try to create a new source
	{
		alGenSources(1, &sourceID);

		ALenum error = alGetError();
		if(error != AL_NO_ERROR)
			return INVALID_ID;

		//printf("Created new sound source [%d]. - Total (%d)\n", sourceID, freeSources_.size());
	}

	return sourceID;
}

Uint SoundManager::load(const string& filename)
{
	Uint id = getBuffer(filename);
	
	if(id == INVALID_ID)
		id = loadBuffer(filename);

	return id;
}

void SoundManager::deletePlayedSources()
{
	ALint value;
	SourceList::iterator i;
	for(i = usedSources_.begin(); i != usedSources_.end(); ++i)
	{
		alGetSourcei(*i, AL_SOURCE_STATE, &value);
		if(value != AL_PLAYING)
		{
			alDeleteSources(1, &(*i));
			//freeSources_.push_back(*i);

			//printf("Adding free source [%d] - Total (%d)\n", *i, freeSources_.size());

			i = usedSources_.erase(i);
			if(i == usedSources_.end())
				break;
		}
	}

	//printf("Used sound sources: %d\n", usedSources_.size());
}

Uint SoundManager::getBuffer(const string& filename) const
{
	SoundMap::const_iterator i = soundFiles_.find(filename);
	
	if(i == soundFiles_.end())
		return INVALID_ID;
	else
		return (*i).second;
}

Uint SoundManager::loadBuffer(const string& filename)
{
	assert(soundFiles_[filename] == 0);		//make sure file does not exist

	ALuint buffer = -1;	
	ALenum format;
    ALsizei size;
    ALvoid* data = 0;
    ALsizei freq;
    ALboolean loop;

	alutLoadWAVFile((ALbyte*)filename.c_str(), &format, &data, &size, &freq, &loop);
	if(!data)
		return INVALID_ID;

	alGenBuffers(1, &buffer);

	ALenum error = alGetError();
    if(error != AL_NO_ERROR)		//error generating buffer
	{
        return INVALID_ID;
	}

	soundFiles_[filename] = buffer;
	printf("Sound data loaded [%d]: %s\n", soundFiles_.size()-1, filename.c_str());

    alBufferData(buffer, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);

	return buffer;
}

void SoundManager::initListener()
{
	ALfloat listenerPos[3] = { (ALfloat)listenerPosition_.x_, (ALfloat)listenerPosition_.y_, (ALfloat)listenerPosition_.z_ };
	ALfloat listenerVel[3] = { (ALfloat)listenerVelocity_.x_, (ALfloat)listenerVelocity_.y_, (ALfloat)listenerVelocity_.z_ };
	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ALfloat listenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);
}