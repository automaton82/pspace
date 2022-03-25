//David Millman

#include "GameObject.h"

#include <GL/glaux.h>
#include <GL/glut.h>

GameObject::GameObject() :
	garbage(false)
{
}

GameObject::~GameObject()
{
}

bool GameObject::isGarbage() const
{
	return garbage;
}

void GameObject::setGarbage(bool b)
{
	garbage = b;
}

void GameObject::update(double timeMultiplier)
{
	this->updatePosition(timeMultiplier);
}

void GameObject::display() const
{
	glPushMatrix();
		glTranslated(getPosition().x_, getPosition().y_, getPosition().z_);
		
		//TODO use quaternions or something for rotation
		double angle = atan2f(this->getDirection().y_, this->getDirection().x_)*RADIANS_TO_DEGREES;	//direction should be a unit vector
        glRotated(angle, 0, 0, 1);

		this->draw();
	glPopMatrix();
}