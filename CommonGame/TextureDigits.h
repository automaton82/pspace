//David Millman

#include "DataTypes.h"

#include "TextureAnimated.h"

#ifndef _TEXTUREDIGITS_H_
#define _TEXTUREDIGITS_H_

class TextureDigits : public TextureAnimated
{
public:
	TextureDigits();
	
	int getDigitWidth() const	{ return getFrameWidth(); }
	int getDigitHeight()const	{ return getFrameHeight(); }

	void displayDigit(Uint n, Uint fontSet = 0) const;		
	void displayNumber(Uint n, Uint maxDigits = 1, Uint fontSet = 0, bool ignoreFrontZeros = true) const;
	// e.g. displayNumber(5234, 3) = 234
	// e.g. displayNumber(5234, 5, false) = 05234
	// e.g. displayNumber(5234, 5, true) = 5234


private:

	int digitSetLength_;	//number of digits in a 'digit set'
};

#endif