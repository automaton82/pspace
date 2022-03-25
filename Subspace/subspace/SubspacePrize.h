//David Millman

#ifndef _SUBSPACEPRIZE_H_
#define _SUBSPACEPRIZE_H_

#include "SubspaceGameObject.h"
#include "TextureAnimated.h"

#include "PrizeSettings.h"

class SubspacePrize :
	public SubspaceGameObject
{
public:
	SubspacePrize();
	SubspacePrize(double x, double y, PrizeType type);
	~SubspacePrize();

	//individual prize data
	void setPrizeType(PrizeType type);
	PrizeType getPrizeType() const;
	void setTimeToLive(double time);

	bool isInactive();		//if an enemy ship flies over, becomes inactive
	bool setActive(bool active);

	//update
	void update(double time);

	//draw
	void draw() const;

private:

	PrizeType type_;
	bool active_;
	double timeToLive_;

	TextureAnimated prizeTexture_;
};

#endif