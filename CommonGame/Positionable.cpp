#include "Positionable.h"

Positionable::Positionable()
{
}


Positionable::~Positionable()
{
}

const Vector& Positionable::getPosition() const
{
	return position_;
}

const Vector& Positionable::getVelocity() const
{
	return velocity_;
}

const Vector& Positionable::getAcceleration() const
{
	return acceleration_;
}

Vector Positionable::getNextPosition(double time) const
{
	return (position_ + velocity_*time);
}

void Positionable::setPosition(const Vector& p)
{
	this->position_ = p;
}

void Positionable::setVelocity(const Vector& v)
{
	this->velocity_ = v;
}

void Positionable::setAcceleration(const Vector& v)
{
	this->acceleration_ = v;
}

void Positionable::applyForce(const Vector& force, double time)
{
	this->velocity_ += force * time;
}

void Positionable::updatePosition(double time)
{
	position_ += velocity_ * time;
	velocity_ += acceleration_ * time;
}

