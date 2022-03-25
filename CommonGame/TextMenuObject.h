//David Millman

#ifndef _TEXTMENUOBJECT_H_
#define _TEXTMENUOBJECT_H_

#include <string>
using std::string;

#include "Color.h"
#include "MenuObject.h"
#include "TextureFont.h"

class TextMenuObject : 
	public MenuObject
{
public:
	TextMenuObject(double width=0, double height=0);
	virtual ~TextMenuObject();

	void setFadeTime(double time);	
	void setFont(const TextureFont& font);
	void setSelectTint(const Color& c);
	void setText(const string& text);

	virtual void onHover();		//mouse is hovering over menu object
		
	virtual void draw() const;	
	virtual void update(double time);	//for animated windows and such
	
protected:

	string text_;

	TextureFont font_;
	Color selectTint_;

	double fadeTime_;
	double currentFade_;
	
};

#endif