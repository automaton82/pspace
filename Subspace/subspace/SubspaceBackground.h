//David Millman

#ifndef _SUBSPACEBACKGROUND_H_
#define _SUBSPACEBACKGROUND_H_

#include "Color.h"
#include "RepeatingBackground.h"
#include "TextureAnimated.h"

class SubspaceBackground :
	public RepeatingBackground	//TODO: make the background repeat - less stars needed
	//public Background
{
public:
	typedef vector<Vector> StarList;

	struct BigStar
	{
		BigStar()
		{}

		BigStar(const Vector& pos, Uchar tex) : position(pos), texture(tex)
		{}

		Vector position;
		Uchar texture;
	};

	typedef vector<BigStar> BigStarList;

public:

	SubspaceBackground();
	~SubspaceBackground();

					//TODO: make 'sectioned' background class

	void init();	//generate random stars
	void resize();

	void drawPointStar(const Color& color) const;
	void drawPointStar(const Vector& pos, const Color& color, double repeatScale) const;
	void drawTextureStar(const Vector& pos, const TextureAnimated& tex, double repeatScale) const;

	void draw() const;

private:

	void loadTextures();
	//void initTextures();

private:

	StarList stars_;
	BigStarList bigStars_, planets_;
	double maxDepth_;

	/*static const Uint numStars = 1500;		// Lots of stuff
	static const Uint numBigStars = 400;
	static const Uint numPlanets = 250;*/

	static const Uint numStars = 850;		// normal amount of stuff
	static const Uint numBigStars = 100;
	static const Uint numPlanets = 50;

	static const Uint numBackgroundTextures = 14;
	static const Uint numStarTextures = 7;

	TextureAnimated bgTextures_[numBackgroundTextures];
	TextureAnimated starTextures_[numStarTextures];
};

#endif