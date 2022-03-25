#include "StatBoxPlayerSort.h"

#include <algorithm>

#include "AsciiUtil.h"
using namespace AsciiUtil;

#include "SubspacePlayer.h"

StatBoxPlayerSort::StatBoxPlayerSort() :
	bottomDisplayIndex_(0),
	topDisplayIndex_(0),

	//selectedPlayer_(0),
	selectedPlayerIndex_(0),
	statBoxSize_(defaultStatBoxSize),
	showBanners_(false)
{
}

StatBoxPlayerSort::~StatBoxPlayerSort()
{
}

void StatBoxPlayerSort::init(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer)
{	
	cursor_.load("graphics/spectate.bm2", false);
	cursor_.setFrames(1, 11);

	textBox_.setBackgroundColor(backgroundColor_);
	textBox_.setBorderColor(borderColor_);
	textBox_.setFont(font_);
	textBox_.setPadding(1.0, 0.0, 1.0, 1.0);

	textBox_.clear();
	players_.clear();
	showBanners_ = false;
	playerSort(textBox_, players_, playerMap, myPlayer, false);

	topDisplayIndex_ = 0;
	bottomDisplayIndex_ = min(statBoxSize_-1, textBox_.size()-1);
	textBox_.setLinesDisplayed(statBoxSize_);
	//textBox_.etHeader(itoa(players_.size()), COLOR_Green);
}

double StatBoxPlayerSort::getDisplayHeight() const
{
	return textBox_.getDisplayHeight();
}

double StatBoxPlayerSort::getDisplayWidth() const
{
	return textBox_.getDisplayWidth();
}

Uint StatBoxPlayerSort::getNumPlayers() const
{
	return players_.size();
}

SubspacePlayer* StatBoxPlayerSort::getSelectedPlayer() const
{
	if(selectedPlayerIndex_ >= players_.size())
		return 0;
	
	static SubspacePlayer copy;
	copy = players_[selectedPlayerIndex_];		// TODO: this should be a real reference
	return &copy;
}

Uint StatBoxPlayerSort::getSelectedTeamID() const
{
	Uint teamID = 0;
	if(getSelectedPlayer())
		teamID = getSelectedPlayer()->getTeam();

	return teamID;
}

Uint StatBoxPlayerSort::size() const
{
	return players_.size();
}

void StatBoxPlayerSort::selectPlayer(SubspacePlayer* player)
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

void StatBoxPlayerSort::decreaseSize()
{
	//Uint offset = textBox_.getLineDisplayOffset();
	Uint lines = textBox_.getLineDisplayHeight(topDisplayIndex_);
	if(lines > 0)
		--lines;

	this->textBox_.setLinesDisplayed(lines);
}

void StatBoxPlayerSort::increaseSize()
{
	//Uint offset = textBox_.getLineDisplayOffset();
	Uint lines = textBox_.getLineDisplayHeight(topDisplayIndex_);
	
	this->textBox_.setLinesDisplayed(lines);
}

void StatBoxPlayerSort::pageDown()
{
	/*selectedPlayerIndex_ = max(selectedPlayerIndex_+textBox_.getLinesDisplayed(), textBox_.getLinesDisplayed());		// correction for integer wrapping around
	if(selectedPlayerIndex_ >= textBox_.size())
		selectedPlayerIndex_ = textBox_.size() - 1;

	bottomDisplayIndex_ = selectedPlayerIndex_;

	if(bottomDisplayIndex_ >= textBox_.getLinesDisplayed())
		topDisplayIndex_ = bottomDisplayIndex_ - textBox_.getLinesDisplayed() + 1;
	else
		topDisplayIndex_ = 0;

	textBox_.setLineDisplayOffset(topDisplayIndex_);*/
	for(int i=0; i < statBoxSize_; ++i)
		scrollDown();
}

void StatBoxPlayerSort::pageUp()
{
	/*if(selectedPlayerIndex_ > textBox_.getLinesDisplayed())
		selectedPlayerIndex_ -= textBox_.getLinesDisplayed();
	else
		selectedPlayerIndex_ = 0;

	topDisplayIndex_ = selectedPlayerIndex_;

	bottomDisplayIndex_ = min(topDisplayIndex_ + textBox_.getLinesDisplayed(), textBox_.size()-1);

	textBox_.setLineDisplayOffset(topDisplayIndex_);*/

	for(int i=0; i < statBoxSize_; ++i)
		scrollUp();
}

void StatBoxPlayerSort::scrollDown()
{
	assert(textBox_.size() != 0);

	if(selectedPlayerIndex_ < textBox_.size()-1)
		++selectedPlayerIndex_;

	if(selectedPlayerIndex_ > bottomDisplayIndex_)
	{
		++bottomDisplayIndex_;
		++topDisplayIndex_;
	}

	textBox_.setLineDisplayOffset(topDisplayIndex_);
}

void StatBoxPlayerSort::scrollUp()
{
	assert(textBox_.size() != 0);

	if(selectedPlayerIndex_ > 0)
		--selectedPlayerIndex_;

	if(selectedPlayerIndex_ < topDisplayIndex_)
	{
		--bottomDisplayIndex_;
		--topDisplayIndex_;
	}

	textBox_.setLineDisplayOffset(topDisplayIndex_);
}
	

void StatBoxPlayerSort::draw() const
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

		if(showBanners_)
			drawBanners();
	glPopMatrix();
}


void StatBoxPlayerSort::drawBanner(Uint index) const
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

void StatBoxPlayerSort::drawBanners() const
{
	Uint index = topDisplayIndex_;
	Uint numDisplayed = 0;

	for(index; numDisplayed < statBoxSize_ && index < textBox_.size(); ++index, ++numDisplayed)
	{
		drawBanner(index);
	}
}

void StatBoxPlayerSort::drawCursor(Uint index) const
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

void StatBoxPlayerSort::drawCursors() const
{
	Uint index = topDisplayIndex_;
	Uint numDisplayed = 0;

	for(index; numDisplayed < statBoxSize_ && index < textBox_.size(); ++index, ++numDisplayed)
	{
		drawCursor(index);
	}	
}


Uint StatBoxPlayerSort::getCursorIcon(const SubspacePlayer* player, bool selected) const
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

double StatBoxPlayerSort::getLineHeight(Uint index) const
{
	Uint lineOffset = 0;
	double height = textBox_.getLineDisplayHeight(index+lineOffset);// - lineOffset*font_.getFontHeight();
	
	return height;
}

string StatBoxPlayerSort::getNameString(const SubspacePlayer* p, bool showPoints, bool newline)
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


void StatBoxPlayerSort::getPlayerList(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer, PlayerList* players, bool excludeMyPlayer)
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

void StatBoxPlayerSort::getTeamList(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer, Uint teamNum, PlayerList* team, PlayerList* others, bool excludeMyPlayer)
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

bool StatBox_playerNameSort(const SubspacePlayer& a, const SubspacePlayer& b)
{
	return (AsciiUtil::tolower(a.getName()) < AsciiUtil::tolower(b.getName()));
}

void StatBoxPlayerSort::playerSort(SubspaceTextBox& box, PlayerList& players, const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer, bool showPoints)
{
	//assert(myPlayer);

#define isLastPlayer() (playerNum++ >= playerMap.size()-1)

	//initialize lists
	PlayerList team, others;
	int myTeam = -1;
	if(myPlayer)
		myTeam = myPlayer->getTeam();
	getTeamList(playerMap, myPlayer, myTeam, &team, &others, true);

	std::sort(team.begin(), team.end(), StatBox_playerNameSort);		//sort team
	std::sort(others.begin(), others.end(), StatBox_playerNameSort);	//sort other players

	//set up header
	box.clearHeader();
	box.writeHeader(" ", COLOR_Green);		//cursor column
	box.writeHeader(fixedWidthString(itoa((int)playerMap.size()), nameWidth, centered), COLOR_Green);
	if(showPoints)
	{
		box.writeHeader("  ");						//banner icon column
		box.writeHeader("Points", COLOR_Green);	//points column
	}

	Uint playerNum = 0;

	//insert myPlayer
	if(myPlayer)
	{
		box.write(getNameString(myPlayer, showPoints, !isLastPlayer()), COLOR_Yellow);
		players.push_back(*myPlayer);
	}
	
	//insert team members
	PlayerList::const_iterator i;
	for(i=team.begin(); i != team.end(); ++i)
	{
		box.write(getNameString(&(*i), showPoints, !isLastPlayer()), COLOR_Yellow);
		players.push_back(*i);
	}

	//insert others
	for(i=others.begin(); i != others.end(); ++i)
	{
		box.write(getNameString(&(*i), showPoints, !isLastPlayer()), COLOR_Gray);
		players.push_back(*i);
	}
	
	box.flush();
}