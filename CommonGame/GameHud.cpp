//David Millman

#include "GameHud.h"

#include "GameObject.h"

#include <windows.h>
#include <gl/gl.h>

GameHud::~GameHud()
{
}

int GameHud::add(GameObject* object)
{
	return objects.addItem(object);
}

void GameHud::update(double timeMultiplier)
{
	for(int i=0; i < objects.size(); ++i)
	{
		GameObject* g = objects.getItem(i);
		if(g)
		{
			g->update(timeMultiplier);
		}
	}
}

void GameHud::draw() const
{
	for(int i=0; i < objects.size(); ++i)
	{
		const GameObject* g = objects.getItem(i);
		if(g)
		{
			g->display();
		}
	}
}

void GameHud::display() const
{
	OrthographicObject::display();

	/*glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

		//VideoData& video = VideoData::getInstance();
		//glOrtho(0, video.windowWidth, 0, video.windowHeight, -1, 1);
		glOrtho(0, 800, 0, 600, -1, 1);		//TODO: use actual window size

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			draw();		
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);*/
}