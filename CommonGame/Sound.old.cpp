#include "Sound.h"

#include <al/al.h>
#include <al/alc.h>
#include <al/alu.h>
#include <al/alut.h>

#include <assert.h>

#include "SoundManager.h"

Sound::Sound(SoundManager* manager) : 
	soundManager_(manager),

	bufferID_(INVALID_ID),
	sourceID_(INVALID_ID),

	loop_(false)
{
	//assert(manager);
}

Sound::~Sound()
{
	//destroySource();
}

Vector Sound::getPosition() const
{
	return position_;
}

Vector Sound::getVelocity() const
{
	return velocity_;
}

void Sound::setLoop(bool b)
{
	loop_ = b;
}

void Sound::setManager(SoundManager* manager)
{
	soundManager_ = manager;
}

void Sound::setPosition(const Vector& position)
{
	position_ = position;
}

void Sound::setVelocity(const Vector& velocity)
{
	velocity_ = velocity;
}

bool Sound::isPlaying() const
{
	if(sourceID_ == INVALID_ID)
		return false;

	ALint value;
	alGetSourcei(sourceID_, AL_SOURCE_STATE, &value);

	return (value == AL_PLAYING);
}

bool Sound::isStopped() const
{
	if(sourceID_ == INVALID_ID)
		return true;

	ALint value;
	alGetSourcei(sourceID_, AL_SOURCE_STATE, &value);

	return (value == AL_STOPPED);
}

bool Sound::load(const string& filename)
{
	assert(soundManager_);

	bufferID_ = soundManager_->load(filename);

	if(bufferID_ == INVALID_ID)
		return false;

	//initSource();

	return (bufferID_ != INVALID_ID);
}

bool Sound::play(bool once)
{
	assert(soundManager_);

	if(bufferID_ == INVALID_ID)
		return false;

	if(isPlaying())
		return true;

	initSource();		
	
	//assert(sourceID_ != INVALID_ID);
	if(sourceID_ == INVALID_ID)
		return false;

	ALfloat sourcePos[3] = { (ALfloat)position_.x_, (ALfloat)position_.y_, (ALfloat)position_.z_ };
	ALfloat sourceVel[3] = { (ALfloat)velocity_.x_, (ALfloat)velocity_.y_, (ALfloat)velocity_.z_ };

    alSourcefv(sourceID_, AL_POSITION, sourcePos);
    alSourcefv(sourceID_, AL_VELOCITY, sourceVel);
    alSourcei(sourceID_, AL_LOOPING, loop_);

    alSourcePlay(sourceID_);
	ALenum error = alGetError();

	if(!loop_ && once)
	{
		soundManager_->addSource(sourceID_);
		sourceID_ = INVALID_ID;
	}
	
	return (error == AL_NO_ERROR);
}

bool Sound::pause()
{
	alSourcePause(sourceID_);

	return (alGetError() == AL_NO_ERROR);
}

bool Sound::stop()
{
	if(sourceID_ == INVALID_ID)
		return true;

	alSourceStop(sourceID_);
	ALenum error = alGetError();

	destroySource();
	//soundManager_->addSource(sourceID_);

	return (error == AL_NO_ERROR);
}

void Sound::initSource()
{
	if(sourceID_ != INVALID_ID)
		return;

	ALuint source = 0;
	alGenSources(1, &source);

	ALenum error = alGetError();
    if(error != AL_NO_ERROR)
	{
		/*switch(error)
		{
			case AL_NO_ERROR: break;
			case AL_INVALID_NAME: break;
			case AL_INVALID_ENUM: break;
			case AL_INVALID_VALUE: break;
			case AL_INVALID_OPERATION: break;
			case AL_OUT_OF_MEMORY: break;
		};*/
        return;
	}

	sourceID_ = source;

	//Uint buffers[2] = {bufferID_, bufferID_};
	//alSourceQueueBuffers(sourceID_, 1, &bufferID_);

	alSourcei(sourceID_, AL_BUFFER, bufferID_);
	alSourcef(sourceID_, AL_PITCH, 1.0f);
    alSourcef(sourceID_, AL_GAIN, 1.0f);
}

void Sound::destroySource()
{
	if(sourceID_ != INVALID_ID)
		alDeleteSources(1, &sourceID_);
}

