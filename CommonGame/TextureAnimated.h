//David Millman

#ifndef _TEXTUREANIMATED_H_
#define _TEXTUREANIMATED_H_

#include "DataTypes.h"

#include "Texture.h"

/**
 * Extends the texture class for animations by breaking it up into frames.
 *
 *@author	David Millman (millmd@rpi.edu)
 *@date		4/14/2003
 */
class TextureAnimated : 
	public Texture
{
public:
	TextureAnimated();
	/*TextureAnimated(const TextureAnimated& copy);

	TextureAnimated& operator =(const TextureAnimated& rhs);*/

	/*void setFrameWidth(Uint w)		{ frameWidth_ = w; }
	void setFrameHeight(Uint h)		{ frameHeight_ = h; }*/

	void initAfterLoad();
	
	Uint getFrame() const			{ return (Uint)currentFrame_; }

	Uint getFrameWidth() const		{ return frameWidth_; }
	Uint getFrameHeight() const		{ return frameHeight_; }

	Uint getColumns() const			{ return numCols_; }
	Uint getRows() const			{ return numRows_; }
	Uint getNumFrames() const		{ return numFrames_; }
	Uint getNumFramesInRange() const{ return endFrame_ - startFrame_; }

	//void setColumns(Uint columns);	//automatically adjusts frameWidth;
	//void setRows(Uint rows);		//automatically adjusts frameHeight;
	void setFrames(Uint rows, Uint columns);

	void setFrameSize(Uint frameWidth, Uint frameHeight);
	void setAnimationPeriod(double milliseconds);
	void setFrameRange(Uint start, Uint end);	//inclusive start, exclusive end
	void setFrameInRange(Uint f);

	void setFrame(Uint f);
	void nextFrame();
	void advanceFrame(double milliseconds);
	void advanceFramePercent(double timeMultiplier);

	void displayFrame(Uint f) const;
	void displayFrameInRange(Uint f) const;
	void displayAll() const;	//displays entire texture
	void display() const;
	
private:

	Uint frameWidth_, frameHeight_;
	//Uint frameMaxWidth_, frameMaxHeight_;
	Uint numFrames_;
	Uint startFrame_, endFrame_;

	Uint numCols_, numRows_;
	//double adjustWidth_, adjustHeight_;
	double xOffset_, yOffset_;

	double currentFrame_;
	double animationPeriod_;
	double timePerFrame_;
};

#endif