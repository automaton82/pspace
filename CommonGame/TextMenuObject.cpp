#include "TextMenuObject.h"

#include "GLUtil.h"
#include "MathUtil.h"

TextMenuObject::TextMenuObject(double width, double height) : 
	MenuObject(width, height),
	fadeTime_(0),
	currentFade_(0),
	selectTint_(1.0, 1.0, 1.0, 1.0)
{
}

TextMenuObject::~TextMenuObject()
{
}

void TextMenuObject::setFadeTime(double time)
{
	fadeTime_ = time;
}

void TextMenuObject::setFont(const TextureFont& font)
{
	font_ = font;
}

void TextMenuObject::setSelectTint(const Color& c)
{
	selectTint_ = c;
}

void TextMenuObject::setText(const string& text)
{
	text_ = text;
}

void TextMenuObject::onHover()
{
	currentFade_ = fadeTime_;
}

void TextMenuObject::draw() const
{
	this->drawBackground();

	double life = currentFade_ / fadeTime_;

	if(!fadeTime_)
		life = 1.0;

	//get selection color
	Color base(1.0, 1.0, 1.0, 1.0);
	Color color(
		(1.0-life)*base.r + (life)*selectTint_.r,
		(1.0-life)*base.g + (life)*selectTint_.g,
		(1.0-life)*base.b + (life)*selectTint_.b,
		(1.0-life)*base.a + (life)*selectTint_.a
		);
	color.enable();
	
	glPushMatrix();
		//center text
		double textWidth = text_.size() * font_.getFontWidth();
		double textHeight = font_.getFontHeight();
		glTranslated(getWidth()/2.0 - textWidth/2.0, getHeight()/2.0 - textHeight/2.0, 0);
		font_.displayString(text_.c_str(), 0);
	glPopMatrix();

	this->drawBorder();
}

void TextMenuObject::update(double time)
{
	currentFade_ -= time;
	if(currentFade_ < 0)
		currentFade_ = 0;
}