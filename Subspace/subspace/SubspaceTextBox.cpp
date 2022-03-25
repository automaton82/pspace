#include "SubspaceMenu.h"

#include "AsciiUtil.h"
#include "SubspaceSettings.h"

SubspaceTextBox::SubspaceTextBox() :
	headerPadTop_(1.0),
	headerPadBottom_(1.0),

	separatorWidth_(1.0)
{
}

SubspaceTextBox::~SubspaceTextBox()
{
}

void SubspaceTextBox::clearHeader()
{
	header_.clear();
}

double SubspaceTextBox::getDisplayHeight() const
{
	Uint lines = min(linesDisplayed_, this->size());
	double size = font_.getFontHeight() * lines + padTop_+padBottom_+borderWidth_;

	if(header_.size() > 0)
		size += font_.getFontHeight() + separatorWidth_ + headerPadTop_+headerPadBottom_;

	return size;
}

double SubspaceTextBox::getDisplayWidthStart() const
{
	double size = borderWidth_/2.0 /*+ padLeft_*/;
	return size;
}

double SubspaceTextBox::getHeaderDisplayHeight() const
{
	if(header_.size() == 0)
		return 0;

	return (font_.getFontHeight() + headerPadBottom_+headerPadTop_ + separatorWidth_);
}

double SubspaceTextBox::getLineDisplayHeight(Uint line) const
{
	Uint displayLines = min(size(), linesDisplayed_);
	Uint displayLine = displayLines-1 - line + lineDisplayOffset_;
	double size = font_.getFontHeight() * displayLine + padBottom_+borderWidth_/2.0;

	return size;
}

void SubspaceTextBox::setHeader(const string& header, Uchar color)
{
	header_.clear();
	header_.write(header, color);

	header_.setFont(font_);
	header_.setColor(COLOR_Green);
}

void SubspaceTextBox::setHeaderPadding(double headerPadTop, double headerPadBottom)
{
	headerPadTop_ = headerPadTop;
	headerPadBottom_ = headerPadBottom;
}

void SubspaceTextBox::writeHeader(const string& text, Uchar color)
{
	header_.write(text, color);

	header_.setFont(font_);
	header_.setColor(COLOR_Green);
}
		
void SubspaceTextBox::draw() const
{
	glDisable(GL_TEXTURE_2D);
	drawBackground();

	glColor4d(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslated(padLeft_ + borderWidth_/2.0, padBottom_ + borderWidth_/2.0, 0);
		drawText();
	glPopMatrix();

	glPushMatrix();
		drawHeader();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	drawSeparator();
	drawBorder();
}
	
void SubspaceTextBox::drawBackground() const
{
	double drawWidth = min(lineWidth_, maxWidth_) * font_.getFrameWidth();
	double drawHeight =  getDisplayHeight();

	glColor4d(backgroundColor_.r_, backgroundColor_.g_, backgroundColor_.b_, backgroundColor_.a_);
	glBegin(GL_QUADS);			//background
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_/2.0, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 
			drawHeight,
			0);
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_/2.0, 
			drawHeight, 
			0);
	glEnd();
}

void SubspaceTextBox::drawBorder() const
{
	double drawWidth = min(lineWidth_, maxWidth_) * font_.getFrameWidth();
	double drawHeight = getDisplayHeight();

	glColor4d(borderColor_.r_, borderColor_.g_, borderColor_.b_, borderColor_.a_);
	glLineWidth((float)borderWidth_);
	glBegin(GL_LINE_LOOP);			//main border
		glVertex3d(0, 0, 0);
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_, 0, 0);
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_, 
			drawHeight, 
			0);
		glVertex3d(0, 
			drawHeight,
			0);
		glVertex3d(0, 0, 0);
	glEnd();
}

void SubspaceTextBox::drawHeader() const
{
	if(header_.size() == 0)
		return;

	//string head = AsciiUtil::fixedWidthString(header_, min(maxWidth_, lineWidth_), AsciiUtil::leftJustified);

	glTranslated(padLeft_ + borderWidth_/2.0, getDisplayHeight() - borderWidth_/2.0 - headerPadTop_ - font_.getFontHeight(), 0);
	//font_.displayString(head.c_str(), COLOR_Green);
	header_.draw();
}


void SubspaceTextBox::drawSeparator() const
{
	if(header_.size() == 0)
		return;

	double drawWidth = min(lineWidth_, maxWidth_) * font_.getFrameWidth();
	double drawHeight = getDisplayHeight() - font_.getFontHeight() - headerPadTop_-headerPadBottom_ - separatorWidth_/2.0;

	glColor4d(borderColor_.r_, borderColor_.g_, borderColor_.b_, borderColor_.a_);
	glLineWidth((float)separatorWidth_);
	glBegin(GL_LINE_LOOP);			//main border
		glVertex3d(0, 
			drawHeight, 
			0);
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_, 
			drawHeight, 
			0);
	glEnd();
}