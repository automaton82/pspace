//David Millman

#ifndef _SUBSPACECHATBOX_H_
#define _SUBSPACECHATBOX_H_

#include "SubspaceChatDatabase.h"
#include "Sprite.h"
#include "TextureFont.h"

class SubspaceChatBox :
	public Sprite
{
public:
	SubspaceChatBox(const SubspaceChatDatabase* db = 0);

	void setChatDatabase(const SubspaceChatDatabase* db);
	
	void draw() const;

public:
	string makeChatMessage(const string& sender, const string& text, int width) const;
	
	const SubspaceChatDatabase* chatDatabase;

	int fontWidth, fontHeight;
	int numCols, numRows;
};

#endif