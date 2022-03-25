#include "SubspaceStatBoxModes.h"

#include <algorithm>

#include "AsciiUtil.h"
using namespace AsciiUtil;

#include "SubspacePlayer.h"

SubspaceStatBoxMode::SubspaceStatBoxMode() : 
	backgroundColor_(0.05, 0.1, 0.05, 0.9),
	borderColor_(0.5, 0.5, 0.5, 1.0),

	statBoxSize_(15)
{
}

SubspaceStatBoxMode::~SubspaceStatBoxMode()
{
}

void SubspaceStatBoxMode::init(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer)
{	
	/*if(cursor_.getNumFrames() <= 0)
	{
		cursor_.load("graphics/spectate.bm2", false);
		cursor_.setFrames(1, 11);
	}*/

	/*textBox_.setBackgroundColor(backgroundColor_);
	textBox_.setBorderColor(borderColor_);
	textBox_.setFont(font_);
	textBox_.setPadding(1.0, 0.0, 1.0, 1.0);		*/
}

void SubspaceStatBoxMode::setBoxSize(Uint size)
{
	statBoxSize_ = size;
}

void SubspaceStatBoxMode::setFont(const TextureFont& font)
{
	font_ = font;
}