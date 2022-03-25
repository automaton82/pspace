
#include "GameDisplayManager.h"

#include "GLObject.h"

GameDisplayManager::GameDisplayManager() /*:
	endID_(0)*/
{
}

GameDisplayManager::~GameDisplayManager()
{
}

const Camera& GameDisplayManager::getCamera() const
{
	return camera_;
}

void GameDisplayManager::setCamera(const Camera& c)
{
	camera_ = c;
}

bool GameDisplayManager::isObjectInView(const GameObject& object) const
{
	return true;
}

bool GameDisplayManager::addObject(GameObject* object)
{
	assert(object);

	objects_.push_back(object);

	return true;
}

bool GameDisplayManager::removeObject(GameObject* object)
{
	objects_.remove(object);

	return true;
}

void GameDisplayManager::lookAtCamera()
{
	gluLookAt(
		camera_.getPosition().x_, camera_.getPosition().y_, camera_.getPosition().z_, 
		camera_.getViewPoint().x_, camera_.getViewPoint().y_, camera_.getViewPoint().z_, 
		0, 1, 0
	);
}

void GameDisplayManager::preDisplay() const
{
	//lookAtCamera();
}

void GameDisplayManager::display() const
{
	glPushMatrix();

		preDisplay();
		
		//glColor3f(1,1,1);

		/*glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);*/
		//glDisable(GL_TEXTURE_2D);

		/*Light l(GL_LIGHT0);
		l.setAmbient(0.9, 0.9, 0.9);
		l.setDiffuse(0.5, 0.5, 0.5);
		l.setSpecular(0.2, 0.2, 0.2);
		l.setPosition(100, 100, -100);
		l.enable();*/

		/*Material m;
		m.setAmbient(1, 1, 1);
		m.setDiffuse(0.5, 0.5, 0.5);
		m.setSpecular(0.2, 0.2, 0.2);*/

		ObjectList::const_iterator i;
		for(i = objects_.begin(); i != objects_.end(); ++i)
		{
			(*i)->display();
		}
	
	glPopMatrix();
}