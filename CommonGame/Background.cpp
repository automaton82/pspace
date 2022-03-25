#include "Background.h"

#include "GLObject.h"

#include "MathUtil.h"

Background::Background() :
	viewport_(800, 600, 0)
{
	setFieldOfView(45);
	setAspectRatio(1.0);
	//TODO: initialize viewport using actual window data
}

Background::~Background()
{
	this->clear();
}

void Background::setCamera(const Vector& position)
{
	camera_ = position;
}

void Background::setFieldOfView(double fov)
{
	fieldOfView_ = fov;
	tanThetaY_ = tan(MathUtil::degreesToRadians(fieldOfView_));
	tanThetaX_ = tan(MathUtil::degreesToRadians(fieldOfView_ * aspectRatio_));
	//atanTheta_ = atan(45.0);
}


void Background::setAspectRatio(double aspect)
{
	aspectRatio_ = aspect;
	tanThetaX_ = tan(MathUtil::degreesToRadians(fieldOfView_ * aspectRatio_));
}

/*void Background::setScale(bool b)
{
	doScale_ = b;
}*/

void Background::setViewport(double width, double height, double depth)
{
	viewport_ = Vector(width, height, depth);
}

void Background::addObject(GameObject* object)
{
	assert(object);

	objects_.push_back(object);
}

void Background::clear()
{
	GameObjectList::iterator i;
	for(i = objects_.begin(); i != objects_.end(); ++i)
	{
		delete (*i);
	}
	objects_.clear();
}

Vector Background::transformPosition(const Vector& pos) const
{
	//transform based on camera

	Vector viewPosition;
	viewPosition.x_ = MathUtil::projectPoint<double>(pos.x_ - camera_.x_, viewport_.x_, camera_.z_ - pos.z_, tanThetaX_); 
	viewPosition.y_ = MathUtil::projectPoint<double>(pos.y_ - camera_.y_, viewport_.y_, camera_.z_ - pos.z_, tanThetaY_); 
	//MathUtil::projectPoint(pos.x_ - camera_.x_, viewport_.x_, pos.z_, atanTheta_, viewPosition.x_); 
	//MathUtil::projectPoint(pos.y_ - camera_.y_, viewport_.y_, pos.z_, atanTheta_, viewPosition.y_); 
	viewPosition.z_ = pos.z_;

	//center the camera in the viewport
	viewPosition.x_ += viewport_.x_/2.0;
	viewPosition.y_ += viewport_.y_/2.0;

	return viewPosition;
}

Vector Background::untransformPosition(const Vector& pos) const
{
	//transform based on camera

	Vector viewPosition;
	viewPosition.x_ = MathUtil::unprojectPoint<double>(pos.x_ - camera_.x_, viewport_.x_, camera_.z_ - pos.z_, tanThetaX_); 
	viewPosition.y_ = MathUtil::unprojectPoint<double>(pos.y_ - camera_.y_, viewport_.y_, camera_.z_ - pos.z_, tanThetaY_); 
	viewPosition.z_ = pos.z_;

	return viewPosition;
}

void Background::update(double time)
{
	GameObjectList::iterator i;
	for(i = objects_.begin(); i != objects_.end(); ++i)
	{
		GameObject* obj = (*i);

		obj->update(time);
	}
}

void Background::draw() const
{
	Vector oldPos, newPos;

	GameObjectList::const_iterator i;
	for(i = objects_.begin(); i != objects_.end(); ++i)
	{
		GameObject* obj = (*i);

		oldPos = obj->getPosition();
		newPos = transformPosition(oldPos);
		
		if(newPos.x_ >= 0 && newPos.x_ <= viewport_.x_ &&
			newPos.y_ >= 0 && newPos.y_ <= viewport_.y_)
		{
			obj->setPosition(newPos);	//transform object position

			obj->display();										
			
			obj->setPosition(oldPos);							//reset object position
		}
	}
}

void Background::display() const
{
	double viewX = camera_.x_;
	double viewY = camera_.y_;

	enterOrthoMode();

	//glPushMatrix();
	/*gluLookAt(
		viewX, viewY, camera_.z_, 
        viewX, viewY, 0, 
        0, 1, 0);*/

	//always look at 0,0 - transform is done on objects
	gluLookAt(
		0, 0, 1,
		0, 0, 0,
		0, 1, 0);

	//glTranslated(viewport_.x_/2.0, viewport_.y_/2.0, 0);

	draw();

	exitOrthoMode();
	//glPopMatrix();
}
