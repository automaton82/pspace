//David Millman

#ifndef _LIFEBAR_H_
#define _LIFEBAR_H_

#include "DataTypes.h"
#include "GameObject.h"

#include "TextureAnimated.h"

class LifeBar : 
	public GameObject
{
public:
	LifeBar(Uint width, Uint height, double lifePercent = 100.0);
	~LifeBar();
	
	//settings
	double getWidth() const;
	double getHeight() const;

	double getLifePercent() const;

	void setWidth(double width);
	void setHeight(double height);

	void setColor(const Color& c);
	void setTexture(const TextureAnimated& texture);

	void setLifePercent(double life);	//amount of life left
	
	//display
	void draw() const;

	//update
	void update(double time);

protected:

	void clearTexture();

protected:

	double width_, height_;
	double lifePercent_;

	Color color_;
	TextureAnimated* texture_;
};

#endif