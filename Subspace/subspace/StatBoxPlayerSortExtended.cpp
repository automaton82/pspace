#include "StatBoxPlayerSortExtended.h"

#include <algorithm>
#include "AsciiUtil.h"
using namespace AsciiUtil;
#include "SubspacePlayer.h"

void StatBoxPlayerSortExtended::init(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer)
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
	playerSortExtended(textBox_, players_, playerMap, myPlayer);

	topDisplayIndex_ = 0;
	bottomDisplayIndex_ = min(statBoxSize_-1, textBox_.size()-1);	
	textBox_.setLinesDisplayed(statBoxSize_);
}


string StatBoxPlayerSortExtended::getStatsString(const SubspacePlayer* p, bool newline)
{
	assert(p);

	string retval;
	
	retval += " ";				//cursor column
	retval += fixedWidthString(p->getName(), nameWidth, leftJustified);
	retval += "  ";				//banner column
	retval += fixedWidthString(p->getSquad(), squadWidth, leftJustified);
	retval += fixedWidthString(itoa(p->getWins()), 4, rightJustified);
	retval += fixedWidthString(itoa(p->getLosses()), 4, rightJustified);
	retval += fixedWidthString(itoa(p->getRating()), 4, rightJustified);
	retval += fixedWidthString(ftoa(p->getAveKill(), 1), 6, rightJustified);
	
	if(newline)
		retval += "\n";

	return retval;
}

void StatBoxPlayerSortExtended::playerSortExtended(SubspaceTextBox& box, PlayerList& pagePlayers, const SubspacePlayerMap& players, const SubspacePlayer* myPlayer)
{
	//assert(myPlayer);

#define isLastPlayer() (playerNum++ >= players.size()-1)
	box.clear();

	//initialize lists
	PlayerList team, others;
	
	Uint teamNum = -1;
	if(myPlayer)
		teamNum = myPlayer->getTeam();

	getTeamList(players, myPlayer, teamNum, &team, &others, true);

	std::sort(team.begin(), team.end(), StatBox_playerNameSort);		//sort team
	std::sort(others.begin(), others.end(), StatBox_playerNameSort);	//sort other players

	//set up header
	box.clearHeader();
	box.writeHeader(" ", COLOR_Green);		//cursor column
	box.writeHeader(fixedWidthString(itoa((int)players.size()), nameWidth, centered), COLOR_Green);
	box.writeHeader("  ");						//banner icon column
	box.writeHeader(fixedWidthString("Squad", squadWidth, leftJustified), COLOR_Green);
	box.writeHeader("  W ", COLOR_Green);
	box.writeHeader("  L ", COLOR_Green);
	box.writeHeader("  R ", COLOR_Green);
	box.writeHeader("   Ave", COLOR_Green);
	
	Uint playerNum = 0;
	//insert myPlayer
	if(myPlayer)
	{
		box.write(getStatsString(myPlayer, !isLastPlayer()), COLOR_Yellow);
		pagePlayers.push_back(*myPlayer);
	}
	
	//insert team members
	PlayerList::iterator i;
	for(i=team.begin(); i != team.end(); ++i)
	{
		box.write(getStatsString(&(*i), !isLastPlayer()), COLOR_Yellow);
		pagePlayers.push_back(*i);
	}
	
	//insert others
	for(i=others.begin(); i != others.end(); ++i)
	{
		box.write(getStatsString(&(*i), !isLastPlayer()), COLOR_Gray);
		pagePlayers.push_back(*i);
	}

	box.flush();
}
