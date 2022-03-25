#include "StatBoxPointSort.h"

#include <algorithm>
#include "AsciiUtil.h"
using namespace AsciiUtil;
#include "SubspacePlayer.h"

void StatBoxPointSort::init(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer)
{	
	cursor_.load("graphics/spectate.bm2", false);
	cursor_.setFrames(1, 11);

	textBox_.setBackgroundColor(backgroundColor_);
	textBox_.setBorderColor(borderColor_);
	textBox_.setFont(font_);
	textBox_.setPadding(1.0, 0.0, 1.0, 1.0);

	textBox_.clear();
	players_.clear();
	pointSort(textBox_, players_, playerMap, myPlayer);

	topDisplayIndex_ = 0;
	bottomDisplayIndex_ = min(statBoxSize_-1, textBox_.size()-1);	
	textBox_.setLinesDisplayed(statBoxSize_);
}


bool StatBox_playerPointSort(const SubspacePlayer& a, const SubspacePlayer& b)
{
	if(a.getPoints() > b.getPoints())
		return true;
	else
		return false;
}

void StatBoxPointSort::pointSort(SubspaceTextBox& box, PlayerList& pagePlayers, const SubspacePlayerMap& players, const SubspacePlayer* myPlayer)
{
#define isLastPlayer() (playerNum++ >= players.size()-1)

    box.clear();

	//initialize lists
	PlayerList playerList;
	getPlayerList(players, myPlayer, &playerList);

	std::sort(playerList.begin(), playerList.end(), StatBox_playerPointSort);
	
	//set up header
	box.clearHeader();
	box.writeHeader(" ", COLOR_Green);		//cursor column
	box.writeHeader(fixedWidthString(itoa((int)players.size()), 11, centered).substr(0, 10), COLOR_Green);
	box.writeHeader("Point Sort", COLOR_Green);						//banner icon column
		
	Uint playerNum = 0;

	//insert players
	PlayerList::iterator i;
	for(i=playerList.begin(); i != playerList.end(); ++i)
	{
		SubspacePlayer* p = &(*i);
		if(myPlayer && p->getTeam() == myPlayer->getTeam())
			box.write(getNameString(p, true, !isLastPlayer()), COLOR_Yellow);
		else
			box.write(getNameString(p, true, !isLastPlayer()), COLOR_Gray);

		pagePlayers.push_back(*i);
	}

	box.flush();
}