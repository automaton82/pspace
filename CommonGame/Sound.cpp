#include "Sound.h"

#include <al/al.h>
#include <al/alc.h>
#include <al/alu.h>
#include <al/alut.h>

#include <assert.h>

#include "SoundManager.h"

Vector Sound::listenerPosition_;
Vector Sound::listenerVelocity_;
double Sound::scale_ = 1.0;

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

const Vector& Sound::getListenerPosition()
{
	return listenerPosition_;
}

const Vector& Sound::getListenerVelocity()
{
	return listenerVelocity_;
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

	if(sourceID_ != INVALID_ID)
	{
		//ALfloat pos[3] = { (ALfloat)position_.x_, (ALfloat)position_.y_, (ALfloat)position_.z_ };
		ALfloat pos[3] = { (ALfloat)position_.x_, (ALfloat)0, (ALfloat)0 };
		//alSourcefv(sourceID_, AL_POSITION, pos);
	}
}

void Sound::setVelocity(const Vector& velocity)
{
	velocity_ = velocity;
    
	if(sourceID_ != INVALID_ID)
	{
		ALfloat vel[3] = { (ALfloat)velocity_.x_, (ALfloat)velocity_.y_, (ALfloat)velocity_.z_ };		
		//alSourcefv(sourceID_, AL_VELOCITY, vel);
	}
}


void Sound::setListenerPosition(const Vector& position)
{
	listenerPosition_ = position;

	//ALfloat listenerPos[3] = { (ALfloat)position.x_, (ALfloat)position.y_, (ALfloat)position.z_ };
	ALfloat listenerPos[3] = { (ALfloat)0, (ALfloat)0, (ALfloat)0 };
	alListenerfv(AL_POSITION, listenerPos);
}

void Sound::setListenerVelocity(const Vector& velocity)
{
	listenerVelocity_ = velocity;

	//ALfloat listenerVel[3] = { (ALfloat)listenerVelocity_.x_, (ALfloat)listenerVelocity_.y_, (ALfloat)listenerVelocity_.z_ };
	//ALfloat listenerVel[3] = { (ALfloat)0, (ALfloat)0, (ALfloat)0 };
	//alListenerfv(AL_VELOCITY, listenerVel);
}

void Sound::setListenerOrientation(const Vector& at, const Vector& up)
{
	ALfloat listenerOri[6] = { (ALfloat)at.x_,(ALfloat)at.y_,(ALfloat)at.z_, (ALfloat)up.x_,(ALfloat)up.y_,(ALfloat)up.z_ };

	alListenerfv(AL_ORIENTATION, listenerOri);
}

void Sound::setScale(double scale)
{
	scale_ = scale;
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

bool Sound::play(bool saveSound)
{
	// unspecified playing positions should occur at the listener
	return play(listenerPosition_, listenerVelocity_, saveSound);
	//return play(listenerPosition_, Vector(0,0,0), saveSound);
}

bool Sound::play(const Vector& position, const Vector& velocity, bool saveSound)
{
	assert(soundManager_);

	setListenerOrientation(Vector(1,0,0),Vector(0,0,1));

	if(bufferID_ == INVALID_ID)
		return false;

	if(isPlaying())
		return true;
	else if(sourceID_ != INVALID_ID)
	{
		soundManager_->addSource(sourceID_);
		sourceID_ = INVALID_ID;
	}

	/*if(!loop_ && sourceID_ != INVALID_ID)		//put back unused sound
		soundManager_->addSource(sourceID_);*/

	initSource();		
	
	if(sourceID_ == INVALID_ID)
		return false;

	//ALfloat sourcePos[3] = { (ALfloat)position.x_, (ALfloat)position.y_, (ALfloat)position.z_ };
	Vector diff = position - listenerPosition_;
	double d = diff.magnitude();
	//double v = - (listenerVelocity_).dot(diff);
	double v = 0;
	

	printf("SOUND POSITION, VELOCITY: %f, %f\n", d, v);
	//printf("vel %f, %f, %f\n", listenerVelocity_.x_, listenerVelocity_.y_, listenerVelocity_.z_);
	ALfloat sourcePos[3] = { (ALfloat)d, (ALfloat)0, (ALfloat)0 };
	//ALfloat sourceVel[3] = { (ALfloat)velocity.x_, (ALfloat)velocity.y_, (ALfloat)velocity.z_ };		
	ALfloat sourceVel[3] = { (ALfloat)v, (ALfloat)0, (ALfloat)0 };
	
	
    alSourcefv(sourceID_, AL_POSITION, sourcePos);
    alSourcefv(sourceID_, AL_VELOCITY, sourceVel);
    alSourcei(sourceID_, AL_LOOPING, loop_);	

	alSourceRewind(sourceID_);

    alSourcePlay(sourceID_);
	ALenum error = alGetError();

	if(!loop_ && !saveSound)
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

	if(isStopped())
		return true;

	alSourceStop(sourceID_);
	ALenum error = alGetError();

	if(!loop_)
		soundManager_->addSource(sourceID_);

	return (error == AL_NO_ERROR);
}

void Sound::initSource()
{
	if(sourceID_ != INVALID_ID)
		return;

	ALuint source = soundManager_->getFreeSource();

	if(source == INVALID_ID)
		return;
    
	/*ALuint source = 0;
	/alGenSources(1, &source);

	ALenum error = alGetError();
    if(error != AL_NO_ERROR)
		return;*/

	sourceID_ = source;

	//Uint buffers[2] = {bufferID_, bufferID_};
	//alSourceQueueBuffers(sourceID_, 1, &bufferID_);

	alSourcei(sourceID_, AL_BUFFER, bufferID_);
	alSourcef(sourceID_, AL_PITCH, 1.0f);
    alSourcef(sourceID_, AL_GAIN, 1.0f);
	alSourcef(sourceID_, AL_ROLLOFF_FACTOR, (ALfloat) 1.0);
	alSourcef(sourceID_, AL_REFERENCE_DISTANCE, scale_);
}

/*void Sound::destroySource()
{
	if(sourceID_ != INVALID_ID)
		alDeleteSources(1, &sourceID_);
}*/

