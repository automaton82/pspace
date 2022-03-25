#include "RepeatingBackground.h"

#include "GLObject.h"

RepeatingBackground::RepeatingBackground()
{
}

RepeatingBackground::~RepeatingBackground()
{
}

void RepeatingBackground::setRepeatSize(double x, double y)
{
	assert(x > 0 && y > 0);

	repeatX_ = x;
	repeatY_ = y;
}

void RepeatingBackground::draw() const
{
	Vector oldPos, newPos;

	GameObjectList::const_iterator i;
	for(i = objects_.begin(); i != objects_.end(); ++i)
	{
		GameObject* obj = (*i);

		oldPos = obj->getPosition();
		newPos = transformPosition(oldPos);
		newPos.x_ = fmod(newPos.x_, viewport_.x_);
		newPos.y_ = fmod(newPos.y_, viewport_.y_);
		//z position always 0
		
		if(newPos.x_ >= 0 && newPos.x_ <= viewport_.x_ &&
			newPos.y_ >= 0 && newPos.y_ <= viewport_.y_)
		{
			obj->setPosition(newPos);	//transform object position

			obj->display();										
			
			obj->setPosition(oldPos);							//reset object position
		}
	}
}


/*void RepeatingBackground::display() const
{
	double viewX = fmod(camera_.x_, repeatX_);
	double viewY = fmod(camera_.y_, repeatY_);

	assert(camera_.z_ >= 0);

	enterOrthoMode();

	draw();

	exitOrthoMode();
}*/
