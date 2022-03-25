#include "SubspaceBackground.h"

#include <algorithm>
#include "AsciiUtil.h"
using namespace AsciiUtil;
#include "Material.h"
#include "MathUtil.h"
using namespace MathUtil;
#include "SubspaceGlobal.h"

//static const unsigned int SubspaceBackground::maxBackgroundTextures = 14;
//static const unsigned int SubspaceBackground::maxStarTextures = 7;

SubspaceBackground::SubspaceBackground()
{ 
}

SubspaceBackground::~SubspaceBackground()
{

}

bool zSort(const SubspaceBackground::BigStar& a, const SubspaceBackground::BigStar& b)
{
	return (a.position.z_ < b.position.z_);
}

void SubspaceBackground::init()
{
	//this->setRepeatSize(100, 100);

	double dim = 1024*16;
	double segments = 4;
	maxDepth_ = 3000;
	double x, y, z;
	Uint t;
	Uint i;

	for(i=0; i < numStars; ++i)
	{
		x = unitRand() * dim;
		y = unitRand() * dim;
		z = /*floor(unitRand()*4)/4.0*/unitRand() * (double)maxDepth_*3.0/segments + maxDepth_*1.0/segments;	//minimum depth of 1/3 max
		
		stars_.push_back(Vector(x, y, -z));
	}

	for(i=0; i < numBigStars; ++i)
	{
		x = unitRand() * dim;
		y = unitRand() * dim;
		z = /*floor(unitRand()*4)/4.0*/unitRand() * (double)maxDepth_*0.5/segments + maxDepth_*0.5/segments;	//minimum depth of 1/3 max
		t = (Uint)(unitRand()*numStarTextures);

		bigStars_.push_back(BigStar(Vector(x, y, -z), t));
	}
	std::sort(bigStars_.begin() ,bigStars_.end(), zSort);

	for(i=0; i < numPlanets; ++i)
	{
		x = unitRand() * dim;
		y = unitRand() * dim;
		z = /*floor(unitRand()*4)/4.0*/unitRand() * (double)maxDepth_*0.25/segments + maxDepth_*0.1/segments;	//minimum depth of 1/3 max
		t = (Uint)(unitRand()*numBackgroundTextures);

		planets_.push_back(BigStar(Vector(x, y, -z), t));
	}
	std::sort(planets_.begin() ,planets_.end(), zSort);

	this->setViewport(subspaceGlobal.getWindowWidth(), subspaceGlobal.getWindowHeight());	
	this->setOrthoMode(0, 0, subspaceGlobal.getWindowWidth(), subspaceGlobal.getWindowHeight(), 10, 20000);

	loadTextures();
}

void SubspaceBackground::resize()
{
	this->setViewport(subspaceGlobal.getWindowWidth(), subspaceGlobal.getWindowHeight());
	this->setOrthoMode(0, 0, subspaceGlobal.getWindowWidth(), subspaceGlobal.getWindowHeight(), 10, 20000);

	this->setAspectRatio((double)subspaceGlobal.getWindowWidth()/(double)subspaceGlobal.getWindowHeight());
}


void SubspaceBackground::drawPointStar(const Color& color) const
{
	Material m;
	m.setColor(Color(color.r_, color.g_, color.b_));
	m.setAlpha(color.a_);
	m.enable();

	glColor4d(color.r_, color.g_, color.b_, color.a_);

	/*glBegin(GL_POINTS);
		glVertex3f(0, 0, 0);
	glEnd();*/

	/*glBegin(GL_QUADS);
		glVertex3f(1, 1, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
	glEnd();*/

	glPointSize(1.0);
	glBegin(GL_POINTS);
		glVertex3f(0, 0, 0);		//stars are always 1 pixel
	glEnd();
}

void SubspaceBackground::drawPointStar(const Vector& pos, const Color& color, double repeatScale) const
{
	Vector newPos, newSize;

	newPos = transformPosition(pos);
													//TODO: base off viewport or tile position?
	newPos.x_ = fmod(newPos.x_, viewport_.x_*repeatScale);	
	newPos.y_ = fmod(newPos.y_, viewport_.y_*repeatScale);

	if(newPos.x_ < 0) newPos.x_ += viewport_.x_*repeatScale;
	if(newPos.y_ < 0) newPos.y_ += viewport_.y_*repeatScale;
	
	if(newPos.x_ >= 0 && newPos.x_ <= viewport_.x_ &&
		newPos.y_ >= 0 && newPos.y_ <= viewport_.y_)
	{
		glPushMatrix();
			glTranslated(newPos.x_, newPos.y_, -pos.z_);	//convert negative z coordinates to positive, into screen
			drawPointStar(color);
		glPopMatrix();
	}
	
}

void SubspaceBackground::drawTextureStar(const Vector& pos, const TextureAnimated& tex, double repeatScale) const
{
	Vector newPos;

	newPos = transformPosition(pos);
													
	if(newPos.x_+tex.getFrameWidth() < 0) newPos.x_ = fmod(newPos.x_, viewport_.x_*repeatScale);	
	if(newPos.y_+tex.getFrameHeight() < 0) newPos.y_ = fmod(newPos.y_, viewport_.y_*repeatScale);

	if(newPos.x_+tex.getFrameWidth() < 0) newPos.x_ += viewport_.x_*repeatScale;
	if(newPos.y_+tex.getFrameHeight() < 0) newPos.y_ += viewport_.y_*repeatScale;
	
	Rect objRect(tex.getFrameHeight() + newPos.y_, newPos.x_, newPos.y_, tex.getFrameWidth() + newPos.x_);
	Rect viewRect(viewport_.y_, 0, 0, viewport_.x_);

	if(viewRect.intersects(objRect))
	{
		glPushMatrix();
			glTranslated(newPos.x_, newPos.y_, -pos.z_);	//convert negative z coordinates to positive, into screen
			tex.display();
		glPopMatrix();
	}
}


void SubspaceBackground::draw() const
{
	Material m;
	m.setColor(Color(1, 1, 1));
	m.enable();

	/*glPushMatrix();
	glTranslatef(0, 0, -100);
	glutWireSphere(100, 50, 50);
	glPopMatrix();*/

	//glTranslatef(400, 300, 0);

	Vector oldPos, newPos;

	glDisable(GL_TEXTURE_2D);

	//TODO: sort everything by z-order so drawing overlap is correct
	StarList::const_iterator i;
	for(i = stars_.begin(); i != stars_.end(); ++i)
	{
		const Vector& star = (*i);
		double color = (maxDepth_+star.z_)/(maxDepth_/2.0) *0.50 + 0.05;
		//double color = 1;

		drawPointStar(star, Color(color, color, color), 1.5);
	}

	glColor4f(1, 1, 1, 1);

	BigStarList::const_iterator j;
	for(j = bigStars_.begin(); j != bigStars_.end(); ++j)
	{
		const BigStar& bs = (*j);
		drawTextureStar(bs.position, starTextures_[bs.texture], 2);
	}

	for(j = planets_.begin(); j != planets_.end(); ++j)
	{
		const BigStar& bs = (*j);
		drawTextureStar(bs.position, bgTextures_[bs.texture], 2.5);
	}
}

void SubspaceBackground::loadTextures()
{
	string filename;
	Uint i;

	//load star textures
	for(i = 0; i < numStarTextures; ++i)
	{
		filename = "graphics/star" + itoaFixed(i+1, 2) + ".bm2";
		starTextures_[i].load(filename, false);
	}

	//TextureData::setUseDefaultColorKey(false);
	//load planet textures
	for(i = 0; i < numBackgroundTextures; ++i)
	{
		filename = "graphics/bg" + itoaFixed(i+1, 2) + ".bm2";
		bgTextures_[i].load(filename, false);
	}

	//TextureData::setUseDefaultColorKey(true);
}


/*void SubspaceBackground::initTextures()
{	
	// TODO: set background and start textures to point to the global textures
}*/