#ifndef _SOUND_H_
#define _SOUND_H_

#include <string>
using std::string;

#include "DataTypes.h"
#include "Vector.h"

#include "SoundManager.h"

class Sound
{
public:

	static const Uint INVALID_ID = SoundManager::INVALID_ID;

public:

	Sound(SoundManager* soundManager = 0);
	~Sound();

	Vector getPosition() const;
	Vector getVelocity() const;
	void setLoop(bool b);
	void setManager(SoundManager* manager);
	void setPosition(const Vector& position);
	void setVelocity(const Vector& velocity);

	bool isPlaying() const;
	bool isStopped() const;

	bool load(const string& filename);
	bool pause();
	bool play(bool playOnce = true);
	bool stop();
	
private:

	void initSource();
	void destroySource();

private: 

	SoundManager* soundManager_;


    Uint bufferID_;
	Uint sourceID_;

	bool loop_;
	Vector position_;
	Vector velocity_;
};

#endif