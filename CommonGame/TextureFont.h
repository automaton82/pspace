//David Millman

#include "DataTypes.h"

#include "TextureAnimated.h"

#ifndef _TEXTUREFONT_H_
#define _TEXTUREFONT_H_

/**
 *Defines a specific implementation of an animated texture for use with fonts.
 *Each frame corresponds to a different character
 *
 *@author	David Millman (millmd@rpi.edu)
 *@date		4/14/2003
 */
class TextureFont : public TextureAnimated
{
public:
	TextureFont();
	/*TextureFont(const TextureFont& copy);

	TextureFont& operator=(const TextureFont& rhs);*/

	int getFontWidth() const	{ return getFrameWidth(); }
	int getFontHeight()	const	{ return getFrameHeight(); }

	void displayChar(char c, Uint fontSet = 0) const;
	void displayString(const char* s, Uint fontSet = 0) const;

private:

	int fontSetLength_;
};

#endif