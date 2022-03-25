//David Millman


#include "Particle.h"

Particle::Particle() : mass_(1.0), timeToLive_(infinite), maxTimeToLive_(infinite), 
	position_(), velocity_(), acceleration_(), rotation_(), rotationVelocity_(), size_(1.0), lastPosition_()
{

}

Particle::Particle(const Particle& copy) /*: position_(copy.position_), velocity_(copy.velocity_), 
	acceleration_(copy.acceleration_), mass_(copy.mass_), timeToLive_(copy.timeToLive_)*/
{
	*this = copy;
}

Particle::~Particle()
{
}

Particle& Particle::operator =(const Particle& rhs)
{
	if(this != &rhs)
	{
		position_ = rhs.position_;
		velocity_ = rhs.velocity_;
		acceleration_ = rhs.acceleration_;

		lastPosition_ = rhs.lastPosition_;
		direction_ = rhs.direction_;
		rotation_ = rhs.rotation_;
		rotationVelocity_ = rhs.rotationVelocity_;
		
		mass_ = rhs.mass_;

		timeToLive_ = rhs.timeToLive_;
		maxTimeToLive_ = rhs.timeToLive_;

		size_ = rhs.size_;
	}
	return *this;
}

void Particle::applyForce(Vector f, double time)
{
	if(mass_ == infinite) return;

	velocity_ += f * time / mass_;
}

void Particle::update(double time)
{
	move(time);
	rotate(time);

	if(timeToLive_ != infinite)
	{
		timeToLive_ -= time;
	}
}

void Particle::move(double time)
{
	lastPosition_ = position_;

	position_ += velocity_ * time;
	velocity_ += acceleration_ * time;
}

void Particle::rotate(double time)
{
	rotation_ += rotationVelocity_ * time;

	rotateX(rotationVelocity_.x_ * time);
	rotateY(rotationVelocity_.y_ * time);
	rotateZ(rotationVelocity_.z_ * time);
}

void Particle::rotateX(double angle)
{
	double x = direction_.x_;
	double y = direction_.y_;
	double z = direction_.z_;
	
	angle *= degToRad;

	direction_.x_ = x;
	direction_.y_ = y*cos(angle) - z*sin(angle);
	direction_.z_ = y*sin(angle) - z*cos(angle);
}

void Particle::rotateY(double angle)
{
	double x = direction_.x_;
	double y = direction_.y_;
	double z = direction_.z_;

	angle *= degToRad;

	direction_.x_ = x*cos(angle) - z*sin(angle);
	direction_.y_ = y;
	direction_.z_ = -x*sin(angle) + z*cos(angle);
}

void Particle::rotateZ(double angle)
{
	double x = direction_.x_;
	double y = direction_.y_;
	double z = direction_.z_;

	angle *= degToRad;
	
	direction_.x_ = x*cos(angle) - y*sin(angle);
	direction_.y_ = x*sin(angle) + y*cos(angle);
	direction_.z_ = z;
}
