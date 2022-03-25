//David Millman

#include <stdio.h>

#include "Vector.h"

#ifndef _Particle_
#define _Particle_

const double PI = 3.1415926f;
const double degToRad = PI / 180.0f;
const double radToDeg = 180.0f / PI;
const double infinite = -99;
const double defaultTime = 0.1f;

class Particle
{
public:
	Particle();
	Particle(const Particle& copy);
	virtual ~Particle();

	Particle& operator=(const Particle& rhs);
	
	void setPosition(const Vector& p)			{ position_ = p; }
	void setVelocity(const Vector& v)			{ velocity_ = v; }
	void setAcceleration(const Vector& a)		{ acceleration_ = a; }
	void setRotationVelocity(const Vector& v)	{ rotationVelocity_ = v; }
	void setDirection(const Vector& d)			{ direction_ = d; }
		
	void setMass(double m)					{ mass_ = m; }
	void setTimeToLive(double t)			{ timeToLive_ = t; }
	void setMaxTimeToLive(double t)		{ maxTimeToLive_ = t; }
	void setSize(double s)					{ size_ = s; }
	
	Vector getPosition() const			{ return position_; }
	Vector getVelocity() const			{ return velocity_; }
	Vector getAcceleration() const		{ return acceleration_; }
	Vector getRotationVelocity() const	{ return rotationVelocity_; }
	Vector getDirection()				{ return direction_; }

	double getMass() const					{ return mass_; }
	double getTimeToLive() const			{ return timeToLive_; }
	double getMaxTimeToLive() const		{ return maxTimeToLive_; }
	double getSize() const					{ return size_; }

	Vector getLastPosition() const		{ return lastPosition_; }
	Vector getNextPosition() const		{ return position_+velocity_; }

	virtual void applyForce(Vector f, double time = defaultTime);
	
	virtual void update(double time = defaultTime);
	
	void move(double time = defaultTime);
	void rotate(double time = defaultTime);

	void rotateX(double angle);
	void rotateY(double angle);
	void rotateZ(double angle);

protected:
	Vector position_;
	Vector velocity_;
	Vector acceleration_;

	Vector lastPosition_;

	Vector direction_;
	Vector rotation_;
	Vector rotationVelocity_;
    double mass_;
    
	double timeToLive_;
	double maxTimeToLive_;

	double size_;
};

#endif