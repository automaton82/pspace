#include "StatBoxPlayerSortPoints.h"

#include "SubspacePlayer.h"

void StatBoxPlayerSortPoints::init(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer)
{	
	cursor_.load("graphics/spectate.bm2", false);
	cursor_.setFrames(1, 11);

	textBox_.setBackgroundColor(backgroundColor_);
	textBox_.setBorderColor(borderColor_);
	textBox_.setFont(font_);
	textBox_.setPadding(1.0, 0.0, 1.0, 1.0);

	textBox_.clear();
	players_.clear();
	showBanners_ = true;
	playerSort(textBox_, players_, playerMap, myPlayer, true);

	topDisplayIndex_ = 0;
	bottomDisplayIndex_ = min(statBoxSize_-1, textBox_.size()-1);	
	textBox_.setLinesDisplayed(statBoxSize_);
}
