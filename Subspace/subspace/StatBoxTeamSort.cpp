#include "StatBoxTeamSort.h"

#include <algorithm>

#include "AsciiUtil.h"
using namespace AsciiUtil;

#include "SubspacePlayer.h"

StatBoxTeamSort::StatBoxTeamSort() :
	bottomDisplayIndex_(0),
	topDisplayIndex_(0),

	numPlayers_(0),
	selectedPlayerIndex_(0),
	statBoxSize_(defaultStatBoxSize)
{
}

StatBoxTeamSort::~StatBoxTeamSort()
{
}

void StatBoxTeamSort::init(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer)
{	
	cursor_.load("graphics/spectate.bm2", false);
	cursor_.setFrames(1, 11);

	textBox_.setBackgroundColor(backgroundColor_);
	textBox_.setBorderColor(borderColor_);
	textBox_.setFont(font_);
	textBox_.setPadding(1.0, 0.0, 1.0, 1.0);

	textBox_.clear();
	players_.clear();
	teamSort(textBox_, players_, playerMap, myPlayer);
	numPlayers_ = playerMap.size();

	topDisplayIndex_ = 0;
	bottomDisplayIndex_ = min(statBoxSize_-1, textBox_.size()-1);
	textBox_.setLinesDisplayed(statBoxSize_);
	//textBox_.etHeader(itoa(players_.size()), COLOR_Green);
}

double StatBoxTeamSort::getDisplayHeight() const
{
	return textBox_.getDisplayHeight();
}

double StatBoxTeamSort::getDisplayWidth() const
{
	return textBox_.getDisplayWidth();
}

Uint StatBoxTeamSort::getNumPlayers() const
{
	return numPlayers_;
}

SubspacePlayer* StatBoxTeamSort::getSelectedPlayer() const
{
	/*if(selectedPlayerIndex_ >= players_.size())
		return 0;
	
	static SubspacePlayer copy;
	copy = players_[selectedPlayerIndex_];		// TODO: this should be a real reference
	return &copy;*/
	return 0;
}

Uint StatBoxTeamSort::getSelectedTeamID() const
{
	Uint teamID = 0;
	if(getSelectedPlayer())
		teamID = getSelectedPlayer()->getTeam();

	return teamID;
}

Uint StatBoxTeamSort::size() const
{
	return players_.size();
}

void StatBoxTeamSort::selectPlayer(SubspacePlayer* player)
{
	Uint j;
	PlayerList::iterator i;
	for(i = players_.begin(), j = 0; i != players_.end(); ++i, ++j)
	{
		if((*i).getId() == player->getId())
			break;
	}

	if(j >= players_.size())
		selectedPlayerIndex_ = 0;
	else
		selectedPlayerIndex_ = j;
}

void StatBoxTeamSort::decreaseSize()
{
	//Uint offset = textBox_.getLineDisplayOffset();
	Uint lines = textBox_.getLineDisplayHeight(topDisplayIndex_);
	if(lines > 0)
		--lines;

	this->textBox_.setLinesDisplayed(lines);
}

void StatBoxTeamSort::increaseSize()
{
	//Uint offset = textBox_.getLineDisplayOffset();
	Uint lines = textBox_.getLineDisplayHeight(topDisplayIndex_);
	
	this->textBox_.setLinesDisplayed(lines);
}

void StatBoxTeamSort::pageDown()
{
	selectedPlayerIndex_ = max(selectedPlayerIndex_+textBox_.getLinesDisplayed(), textBox_.getLinesDisplayed());		// correction for integer wrapping around
	if(players_[selectedPlayerIndex_].getId() == INVALID_ID)			//hit header, move to next
		--selectedPlayerIndex_;

	if(selectedPlayerIndex_ >= textBox_.size())
		selectedPlayerIndex_ = textBox_.size() - 1;

	bottomDisplayIndex_ = selectedPlayerIndex_;

	if(bottomDisplayIndex_ > textBox_.getLinesDisplayed())
		topDisplayIndex_ = bottomDisplayIndex_ - textBox_.getLinesDisplayed();
	else
		topDisplayIndex_ = 0;

	textBox_.setLineDisplayOffset(topDisplayIndex_);
}

void StatBoxTeamSort::pageUp()
{
	if(selectedPlayerIndex_ > textBox_.getLinesDisplayed())
		selectedPlayerIndex_ -= textBox_.getLinesDisplayed();
	else
		selectedPlayerIndex_ = 0;

	if(players_[selectedPlayerIndex_].getId() == INVALID_ID)			//hit header, move to next
	{
		++selectedPlayerIndex_;
		topDisplayIndex_ = selectedPlayerIndex_-1;
	}
	else
	{
		topDisplayIndex_ = selectedPlayerIndex_;
	}

	bottomDisplayIndex_ = min(topDisplayIndex_ + textBox_.getLinesDisplayed(), textBox_.size()-1);

	textBox_.setLineDisplayOffset(topDisplayIndex_);
}

void StatBoxTeamSort::scrollDown()
{
	assert(textBox_.size() != 0);

	if(selectedPlayerIndex_ < textBox_.size()-1)
		++selectedPlayerIndex_;

	if(players_[selectedPlayerIndex_].getId() == INVALID_ID)			//hit header, move to next
		++selectedPlayerIndex_;

	if(selectedPlayerIndex_ > bottomDisplayIndex_)
	{
		++bottomDisplayIndex_;
		++topDisplayIndex_;
	}

	textBox_.setLineDisplayOffset(topDisplayIndex_);
}

void StatBoxTeamSort::scrollUp()
{
	assert(textBox_.size() != 0);

	if(selectedPlayerIndex_ > 0)
		--selectedPlayerIndex_;

	if(players_[selectedPlayerIndex_].getId() == INVALID_ID)			//hit header, move to next
	{
		if(selectedPlayerIndex_ > 1)
			--selectedPlayerIndex_;
		else
			++selectedPlayerIndex_;
	}

	if(selectedPlayerIndex_ < topDisplayIndex_)
	{
		--bottomDisplayIndex_;
		--topDisplayIndex_;
	}

	textBox_.setLineDisplayOffset(topDisplayIndex_);
}
	

void StatBoxTeamSort::draw() const
{
	//move to upper left

	glPushMatrix();
		glTranslated(0, -getDisplayHeight(), 0);

		glPushMatrix();
			textBox_.draw();
			//cursor_.display();
		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		drawCursors();

		glColor4d(1.0, 1.0, 1.0, 1.0);
		drawBanners();
	glPopMatrix();
}


void StatBoxTeamSort::drawBanner(Uint index) const
{
	Uint hOffset = (font_.getFontHeight() - cursor_.getFrameHeight())/2.0 + 1;	//center cursor vertically in text
	Uint wOffset = font_.getFontWidth() * (1+nameWidth);
	
	double width = textBox_.getDisplayWidthStart()+wOffset;
	if(width > textBox_.getDisplayWidth())
		return;

	const SubspacePlayer* player = &players_[index];
	const SubspaceBanner* banner = player->getBanner();

	double height = getLineHeight(index) + hOffset;
	if(height < 0)
		return;

	if(banner)
	{
		glPushMatrix();
			glTranslated(width, height, 0);
			banner->display();
		glPopMatrix();
	}
}

void StatBoxTeamSort::drawBanners() const
{
	Uint index = topDisplayIndex_;
	Uint numDisplayed = 0;

	assert(textBox_.size() == players_.size());
	//printf("textbox: %d   players: %d\n", textBox_.size(), players_.size());

	for(index; numDisplayed < statBoxSize_ && index < textBox_.size(); ++index, ++numDisplayed)
	{
		if(players_[index].getId() != INVALID_ID)
            drawBanner(index);
	}
}

void StatBoxTeamSort::drawCursor(Uint index) const
{
	Uint hOffset = (font_.getFontHeight() - cursor_.getFrameHeight())/2;	//center cursor vertically in text
	double height = getLineHeight(index) + hOffset;

	const SubspacePlayer* player = &players_[index];

    Uint cursorIcon = -1;
	
	cursorIcon = getCursorIcon(player, index==selectedPlayerIndex_);
	if(cursorIcon != -1)
	{
		glPushMatrix();
			glTranslated(textBox_.getDisplayWidthStart(), height, 0);
			cursor_.displayFrame(cursorIcon);
		glPopMatrix();
	}
		
}

void StatBoxTeamSort::drawCursors() const
{
	Uint index = topDisplayIndex_;
	Uint numDisplayed = 0;

	assert(textBox_.size() == players_.size());
	//printf("textbox: %d   players: %d\n", textBox_.size(), players_.size());

	for(index; numDisplayed < statBoxSize_ && index < textBox_.size(); ++index, ++numDisplayed)
	{
		if(players_[index].getId() != INVALID_ID)
        	drawCursor(index);
	}	
}


Uint StatBoxTeamSort::getCursorIcon(const SubspacePlayer* player, bool selected) const
{
	Uint cursorIcon = -1;

	if(selected)
		cursorIcon += 1;

	if(!player)
		return cursorIcon;
	
	//figure out which texture
	if(player->hasKoth())
		cursorIcon += 6;

	if(player->isSpectator())
		cursorIcon += 2;
	else if(player->isTurret())
		cursorIcon += 4;

	return cursorIcon;
}

double StatBoxTeamSort::getLineHeight(Uint index) const
{
	Uint lineOffset = 0;
	double height = textBox_.getLineDisplayHeight(index+lineOffset);// - lineOffset*font_.getFontHeight();
	
	return height;
}

string StatBoxTeamSort::getNameString(const SubspacePlayer* p, bool showPoints, bool newline)
{
	string retval;

	retval += " ";				//cursor column
	retval += fixedWidthString(p->getName(), nameWidth, leftJustified);
				
	if(showPoints)
	{
		retval += "  ";		//banner column
		retval += fixedWidthString(itoa(p->getPoints()), pointsWidth, rightJustified);	//write points
	}
	
	if(newline)
		retval += "\n";

	return retval;
}


void StatBoxTeamSort::getPlayerList(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer, PlayerList* players, bool excludeMyPlayer)
{
	players->clear();

	SubspacePlayerMap::const_iterator i;
	for(i = playerMap.begin(); i != playerMap.end(); ++i)
	{
		SubspacePlayer* p = (*i).second;

		if(p != myPlayer || !excludeMyPlayer)
			players->push_back(*p);
	}
}

void StatBoxTeamSort::getTeamList(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer, Uint teamNum, PlayerList* team, PlayerList* others, bool excludeMyPlayer)
{
	SubspacePlayerMap::const_iterator i;
	for(i = playerMap.begin(); i != playerMap.end(); ++i)
	{
		SubspacePlayer* p = (*i).second;
		
		if(p->getTeam() == teamNum)
		{
			if(p != myPlayer || !excludeMyPlayer)
				team->push_back(*p);
		}
		else
		{
			others->push_back(*p);
		}
	}
}

bool StatBox_playerNameSort2(const SubspacePlayer& a, const SubspacePlayer& b)
{
	return (a.getName() < b.getName());
}

void StatBoxTeamSort::teamSort(SubspaceTextBox& box, PlayerList& boxPlayers, const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer)
{
#define isLastPlayer() (playerNum++ >= playerMap.size()-1)
	box.clear();

	typedef map<Uint, PlayerList> TeamMap;
	//initialize lists
	TeamMap teams;
	PlayerList playerList;
	
	getPlayerList(playerMap, myPlayer, &playerList, true);

	//sort players
	std::sort(playerList.begin(), playerList.end(), StatBox_playerNameSort2);
	
	//arrange players in teams
		
	if(myPlayer)
		teams[myPlayer->getTeam()].push_back(*myPlayer);

	SubspacePlayerMap::const_iterator a;
	for(a = playerMap.begin(); a != playerMap.end(); ++a)
	{
		const SubspacePlayer* p = (*a).second;
		assert(p);
			
		if(p != myPlayer)
			teams[p->getTeam()].push_back(*p);		
	}

	//set up header
	box.clearHeader();
	box.writeHeader(" ", COLOR_Green);		//cursor column
	box.writeHeader(fixedWidthString(itoa((int)playerMap.size()), 11, centered), COLOR_Green);
	box.writeHeader("Team Sort", COLOR_Green);						//banner icon column

	Uint playerNum = 0;

	SubspacePlayer blank;
	PlayerData data;
	data.playerID = INVALID_ID;
	blank.setPlayerData(data, true);

	//insert my team first
    if(myPlayer)
	{
		Uint teamNum = myPlayer->getTeam();
		string teamHeader = itoaFixed(teamNum, 4) + string("-------------") + fixedWidthString(itoa((int)teams[teamNum].size()), 4, centered);
		box.write(teamHeader + "\n", COLOR_Red);
		boxPlayers.push_back(blank);
	
		PlayerList::const_iterator i;
		for(i = teams[teamNum].begin(); i != teams[teamNum].end(); ++i)
		{
			boxPlayers.push_back(*i);
			box.write(getNameString(&(*i), true, !isLastPlayer()), COLOR_Yellow);
		}
	
		teams.erase(teamNum);		// remove myTeam from entries
	}

	//insert other teams
	TeamMap::iterator t;
	for(t = teams.begin(); t != teams.end(); ++t)
	{
		const PlayerList& team = (*t).second;
		Uint teamNum = (*t).first;

		PlayerList::const_iterator k;

		//write team header
		string teamHeader = itoaFixed(teamNum, 4) + string("-------------") + fixedWidthString(itoa((int)team.size()), 4, centered);			
		box.write(teamHeader + "\n", COLOR_Red);

		boxPlayers.push_back(blank);		// blank players in place of team headers

		//wrtie team players
		for(k = team.begin(); k != team.end(); ++k)
		{
			box.write(getNameString(&(*k), true, !isLastPlayer()), COLOR_Gray);
			boxPlayers.push_back(*k);
		}
	}

	box.flush();
}