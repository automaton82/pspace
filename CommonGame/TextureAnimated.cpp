//David Millman

#include "TextureAnimated.h"

#include <assert.h>

/**
 *Constructor
 */
TextureAnimated::TextureAnimated() : frameWidth_(0), frameHeight_(0),
	/*frameMaxWidth_(0), frameMaxHeight_(0),*/ numCols_(1), numRows_(1), /*adjustWidth_(1), adjustHeight_(1),*/
	xOffset_(0), yOffset_(0),
	currentFrame_(0), startFrame_(0), endFrame_(0), numFrames_(-1),
	animationPeriod_(1000.0), timePerFrame_(1000.0)
{
}

/*TextureAnimated::TextureAnimated(const TextureAnimated& copy)
{
	*this = copy;
}

TextureAnimated& TextureAnimated::operator =(const TextureAnimated& rhs)
{
	if(this != &rhs)
	{
		Texture::operator =(rhs);

		frameWidth_ = rhs.frameWidth_;
		frameHeight_ = rhs.frameHeight_;
		frameMaxWidth_ = rhs.frameMaxWidth_;
		frameMaxHeight_ = rhs.frameMaxHeight_;

		numCols_ = rhs.numCols_;
		numRows_ = rhs.numRows_;

		adjustWidth_ = rhs.adjustWidth_;
		adjustHeight_ = rhs.adjustHeight_;

		xOffset_ = rhs.xOffset_;
		yOffset_ = rhs.yOffset_;
	}

	return *this;
}*/

void TextureAnimated::initAfterLoad()
{
	//assume there is one frame of animation

	setFrames(1, 1);	//one row and column
	endFrame_ = 0;

	setAnimationPeriod(animationPeriod_);
}

/**
 *Draws a frame of animation of the texture
 *@param f frame number (starts at 0)
 */
void TextureAnimated::displayFrame(Uint f) const
{
	glEnable(GL_TEXTURE_2D);

	/*if(numFrames_ == 0)
	{
		printf("INVALID TEXTURE: %s\n", filename_.c_str());
		return;
	}*/
	assert(numFrames_ > 0);
		
	double x = (double)(f % numCols_) * xOffset_;
	//double y = 1 - (double)(f / numCols_) * yOffset_ - yOffset_;
	double y = adjustHeight_ - (double)(f / numCols_) * yOffset_ - yOffset_;	//TODO: use fmod operation?

	this->enable();

	glBegin(GL_QUAD_STRIP);
		glTexCoord2d(x + xOffset_, y + yOffset_);	
		glVertex2d(frameWidth_, frameHeight_);

		glTexCoord2d(x, y + yOffset_);	
		glVertex2d(0, frameHeight_);

		glTexCoord2d(x + xOffset_, y);	
		glVertex2d(frameWidth_, 0);

		glTexCoord2d(x,y);
		glVertex2d(0, 0);
	glEnd();
}

void TextureAnimated::displayFrameInRange(Uint f) const
{

	assert(getNumFramesInRange() > 0);

	Uint frame = startFrame_ + (f % getNumFramesInRange());
	displayFrame(f);
}

void TextureAnimated::displayAll() const
{
	Texture::display();
}

void TextureAnimated::setFrames(Uint rows, Uint columns)
{
	Uint fWidth = (Uint)((double)getWidth() / (double)columns);
	Uint fHeight = (Uint)((double)getHeight() / (double)rows);

	setFrameSize(fWidth, fHeight);
}

/**
 *Sets up internal data for animated textures
 *@param frameWidth width of a frame on the texture
 *@param frameHeight height of a frame on the texture
 */
void TextureAnimated::setFrameSize(Uint frameWidth, Uint frameHeight)
{
	assert(getTextureData());
	//Uint textureWidth_ = this->getWidth();

	Uint textureWidth_ = this->getTextureData()->memWidth;
	//Uint textureHeight_ = this->getHeight();
	Uint textureHeight_ = this->getTextureData()->memHeight;

	assert(getWidth() > 0 && getHeight() > 0);

	Uint frameMaxWidth_ = getWidth();
	Uint frameMaxHeight_ = getHeight();

	frameWidth_ = frameWidth;
	frameHeight_ = frameHeight;

	numCols_ = frameMaxWidth_ / frameWidth_;
	numRows_ = frameMaxHeight_ / frameHeight_;

	numFrames_ = numCols_ * numRows_;

	if(!startFrame_)
		startFrame_ = 0;

	if(!endFrame_)
		endFrame_ = numFrames_;

	//adjustWidth_ = (double)frameMaxWidth_ / (double)textureWidth_;
	//adjustHeight_ = (double)frameMaxHeight_ / (double)textureHeight_;

	xOffset_ = (double)frameWidth_ / (double)textureWidth_;	//frameWidth/frameMaxWidth * frameMaxWidth/textureWidth
	//xOffset_ = (double)frameWidth_ / (double)frameMaxWidth_;
	yOffset_ = (double)frameHeight_ / (double)textureHeight_;
	//yOffset_ = (double)frameHeight_ / (double)frameMaxHeight_;
}

void TextureAnimated::setAnimationPeriod(double milliseconds)
{
	animationPeriod_ = milliseconds;
	timePerFrame_ = animationPeriod_ / getNumFramesInRange();
}

void TextureAnimated::setFrame(Uint f)
{
	currentFrame_ = static_cast<double>(f % numFrames_);
}

void TextureAnimated::setFrameInRange(Uint f)
{
	currentFrame_ = static_cast<double>(startFrame_ + (f % getNumFramesInRange()));
}

void TextureAnimated::setFrameRange(Uint start, Uint end)
{
	assert(start < end);

	startFrame_ = start;
	endFrame_ = end;

	currentFrame_ = startFrame_ + fmod(currentFrame_, getNumFramesInRange());
	//setFrameInRange(currentFrame_);

	setAnimationPeriod(animationPeriod_); //recalc animation period stuff
}

void TextureAnimated::nextFrame()
{
	setFrame(static_cast<int>(currentFrame_+1));
}

void TextureAnimated::advanceFrame(double time)
{
	if(getNumFramesInRange() == 0)
		return;

	currentFrame_ += time / timePerFrame_;

	/*while(currentFrame_ > endFrame_)
		currentFrame_ -= getNumFramesInRange();*/

	//TODO: make sure this is correct, not displaying too many or too few frames
	if(currentFrame_ >= endFrame_)
		currentFrame_ = startFrame_ + fmod(currentFrame_-endFrame_, getNumFramesInRange());

	assert(currentFrame_ < endFrame_);
}

void TextureAnimated::advanceFramePercent(double timeMultiplier)
{
	if(!animationPeriod_)
		return;

	advanceFrame(timeMultiplier * timePerFrame_);
}	

void TextureAnimated::display() const
{
	//Texture::displayAll();
	displayFrame(static_cast<int>(currentFrame_));
}
