//David Millman

#include "TextureFont.h"

TextureFont::TextureFont()
{
	fontSetLength_ = (int)'~'+2 - (int)' ';
}

/*TextureFont::TextureFont(const TextureFont& copy)
{
	*this = copy;
}

TextureFont& TextureFont::operator =(const TextureFont& rhs)
{
	if(this != &rhs)
	{
		TextureAnimated::operator =(rhs);

		fontSetLength_ = rhs.fontSetLength_;
	}

	return *this;
}*/

void TextureFont::displayChar(char c, Uint fontSet) const
{
	if(c < ' ' || c > '~')	//non-displayable chars (typically)
	{
		//displayFrame(0 + fontSetLength_*fontSet);
		this->displayFrameInRange(0 + fontSetLength_*fontSet);
	}
	else
	{
		//displayFrame((int)(c - ' ') + fontSetLength_*fontSet);
		this->displayFrameInRange((int)(c - ' ') + fontSetLength_*fontSet);
	}
}

void TextureFont::displayString(const char* s, Uint fontSet) const
{
	glPushMatrix();

	for(int i=0; s[i] != '\0'; i++)
	{
		displayChar(s[i], fontSet);
		glTranslated(getFrameWidth(), 0, 0);
	}

	glPopMatrix();
}