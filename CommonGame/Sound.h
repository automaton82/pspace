#ifndef _SOUND_H_
#define _SOUND_H_

#include <string>
using std::string;

#include "DataTypes.h"
#include "Vector.h"

#include "SoundManager.h"

// NOTE: this is currently acting as a 1 dimensional sound object

class Sound
{
public:

	static const Uint INVALID_ID = SoundManager::INVALID_ID;

public:

	Sound(SoundManager* soundManager = 0);
	~Sound();

	static const Vector& getListenerPosition();
	static const Vector& getListenerVelocity();

	void setLoop(bool b);
	void setManager(SoundManager* manager);
	void setPosition(const Vector& position);
	void setVelocity(const Vector& velocity);
	static void setListenerPosition(const Vector& position);
	static void setListenerVelocity(const Vector& velocity);
	static void setListenerOrientation(const Vector& at, const Vector& up);
	static void setScale(double scale); 

	bool isPlaying() const;
	bool isStopped() const;

	bool load(const string& filename);
	bool pause();
	bool play(bool saveSound = false);	//option to keep sound arround (e.g. to check if its done playing)
	bool play(const Vector& position, const Vector& velocity, bool saveSound = false);	//option to keep sound arround (e.g. to check if its done playing)
	bool stop();
	
private:

	void initSource();
	//void destroySource();

private: 

	SoundManager* soundManager_;

	Vector position_;
	Vector velocity_;
	static Vector listenerPosition_;
	static Vector listenerVelocity_;
	static double scale_;

    Uint bufferID_;
	Uint sourceID_;

	bool loop_;
};

#endif