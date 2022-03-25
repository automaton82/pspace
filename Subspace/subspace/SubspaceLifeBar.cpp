#include "SubspaceLifeBar.h"

#include "GLUtil.h"
#include "Material.h"

const double SubspaceLifeBar::topHeight = 2.0;
const double SubspaceLifeBar::bottomHeight = 6.0;
const double SubspaceLifeBar::padding = 2.0;

SubspaceLifeBar::SubspaceLifeBar(Uint width, Uint height, double lifePercent) :
	LifeBar(width, height, lifePercent), maxLifePercent_(100.0)
{
}

void SubspaceLifeBar::init()
{
	setWidth(125);

	TextureAnimated tex;
	tex.load("graphics/gradient.bm2", false);
	tex.setFrameSize(1, 1);
	tex.setFrameRange(0, 14);
	tex.setAnimationPeriod(1000);

	setTexture(tex);
}

SubspaceLifeBar::~SubspaceLifeBar()
{

}

double SubspaceLifeBar::getMaxLifePercent() const
{
	return maxLifePercent_;
}

void SubspaceLifeBar::setMaxLifePercent(double life)
{
	maxLifePercent_ = life;
}

void SubspaceLifeBar::draw() const
{
	//scale texture to correct size
	/*Material m;
	m.setColor(color_, 1, 1);
	m.enable();

	color_.enable();*/

	/*glPushMatrix();
		glScalef(10, 10, 1);
		texture_->displayAll();
	glPopMatrix();*/

	if(lifePercent_ / maxLifePercent_ < 0.25)	//25% life
	{
		texture_->setFrameRange(3*14, 3*14 + 14);
	}
	else if(lifePercent_ / maxLifePercent_ < 0.5)	//50% life
	{
		texture_->setFrameRange(4*14, 4*14 + 14);
	}
	else
	{
		texture_->setFrameRange(0, 14);
	}
	
		
	//draw top part
	glPushMatrix();
		glScaled(width_*maxLifePercent_, topHeight, 1);
		if(texture_)
			glScaled(1.0 / (double)texture_->getFrameWidth(), 1.0 / (double)texture_->getFrameHeight(), 1);

		if(texture_)	
			texture_->displayFrameInRange(0);	//draws a frame
		else
			GLUtil::drawUnitQuad();
	glPopMatrix();

	//draw bottom part
	glPushMatrix();
		glTranslated(0, -(padding + bottomHeight), 0);	//move below top part

		glScaled(width_*maxLifePercent_, bottomHeight, 1);
		glScaled(lifePercent_, 1, 1);		//adjust amount of life

		if(texture_)
		{
			texture_->display();	//TODO: draws a frame, color is fucked up from filtering/scaling during texture load
			//texture_->displayFrame(13);
		}
		else
			GLUtil::drawUnitQuad();
	glPopMatrix();

	glPushMatrix();
		glTranslated(width_*maxLifePercent_, 0, 0);
		glScaled(width_*(1.0-maxLifePercent_), topHeight, 1);
		if(texture_)
			glScaled(1.0 / (double)texture_->getFrameWidth(), 1.0 / (double)texture_->getFrameHeight(), 1);

		if(texture_)	
			texture_->displayFrameInRange(7);	//draws a frame
		else
			GLUtil::drawUnitQuad();
	glPopMatrix();

	//color_.disable();
}