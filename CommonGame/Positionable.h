#ifndef _POSITIONABLE_H_
#define _POSITIONABLE_H_

#include "Vector.h"

class Positionable
{
public:

	Positionable();
	virtual ~Positionable();

	virtual const Vector& getPosition() const;
	virtual const Vector& getVelocity() const;			
	virtual const Vector& getAcceleration() const;

	virtual Vector getNextPosition(double time) const;
	
	virtual void setPosition(const Vector& position);	
	virtual void setVelocity(const Vector& velocity);
	virtual void setAcceleration(const Vector& acceleration);

	virtual void applyForce(const Vector& force, double time);

	virtual void updatePosition(double time);
	
protected:
	Vector position_;
	Vector velocity_;
	Vector acceleration_;
};

#endif