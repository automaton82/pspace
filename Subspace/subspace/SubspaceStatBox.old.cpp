#include "SubspaceStatBox.h"

#include <algorithm>

#include "AsciiUtil.h"
using namespace AsciiUtil;

#include "SubspacePlayer.h"

TextureFont font_;
	Color backgroundColor_;
	Color borderColor_;
	
    static const Uint numPages = 6;

	static const Uint bannerWidth = 2;
	static const Uint nameWidth = 12;
	static const Uint pointsWidth = 6;
	static const Uint squadWidth = 10;

	static const Uchar teamColor = COLOR_Yellow;
	static const Uchar statColor = COLOR_Gray;
	
	Uint mode_;
	//SubspacePlayer* selectedPlayer_;
	Uint selectedIndex_;
	Uint statBoxSize_;

SubspaceStatBox::SubspaceStatBox() : 
	backgroundColor_(0.05, 0.1, 0.05, 0.9),
	borderColor_(0.5, 0.5, 0.5, 1.0),

	lastSelectedPlayer_(0),
	mode_(0),
	selectedIndex_(0),
	statBoxSize_(15),
	topPlayerIndex_(0)
{
}

SubspaceStatBox::~SubspaceStatBox()
{
}

void SubspaceStatBox::init(const SubspacePlayerMap& players, SubspacePlayer* myPlayer)
{	
	if(cursor_.getNumFrames() == 0)
	{
		cursor_.load("graphics/spectate.bm2", false);
		cursor_.setFrames(1, 11);
	}

	for(Uint i=0; i < STATBOX_NumPages; ++i)
	{
		pages_[i].clear();

		pages_[i].setBackgroundColor(backgroundColor_);
		pages_[i].setBorderColor(borderColor_);
		pages_[i].setFont(font_);
		pages_[i].setPadding(1.0, 0.0, 1.0, 1.0);

		pagePlayers_[i].clear();		
	}

	infoBoxPlayerSort(pages_[STATBOX_PlayerSort], pagePlayers_[STATBOX_PlayerSort], players, myPlayer);
	infoBoxPlayerPoints(pages_[STATBOX_PlayerSortPoints], pagePlayers_[STATBOX_PlayerSortPoints], players, myPlayer);
	infoBoxPointSort(pages_[STATBOX_PointSort], pagePlayers_[STATBOX_PointSort], players, myPlayer);
	infoBoxTeamSort(pages_[STATBOX_TeamSort], pagePlayers_[STATBOX_TeamSort], players, myPlayer);
	infoBoxPlayerExtendedSort(pages_[STATBOX_PlayerSortExtended], pagePlayers_[STATBOX_PlayerSortExtended], players, myPlayer);
	infoBoxTeamStatistics(pages_[STATBOX_TeamStats], pagePlayers_[STATBOX_TeamStats], players, myPlayer);
}

double SubspaceStatBox::getDisplayHeight() const
{
	return pages_[mode_].getDisplayHeight();
}

double SubspaceStatBox::getDisplayWidth() const
{
	return pages_[mode_].getDisplayWidth();
}

Uint SubspaceStatBox::getPlayerIndex(Uint page, SubspacePlayer* player) const
{
	assert(page < STATBOX_NumPages);

	const PlayerList& players = pagePlayers_[page];

	Uint index = 0;
	PlayerList::const_iterator i;

	for(i = players.begin(); i != players.end(); ++i, ++index)
	{
		if((*i) == player)
			break;
	}
	if(i == players.end())
		index = 0;

	return index;
}

Uint SubspaceStatBox::getPlayerTeamIndex(Uint page, SubspacePlayer* player) const
{
	Uint index = 0;

	//TeamMap::const_iterator i;
	TeamList::const_iterator i;
	for(i = teams_.begin(); i != teams_.end(); ++i)
	{
		SubspacePlayer* p = (*i).front();//((*i).second).front();
		if(p->getTeam() == player->getTeam())
			break;

		++index;
	}	
	if(i == teams_.end())
		index = 0;

	return index;
}

Uint SubspaceStatBox::getSelectedIndex() const
{
	return selectedIndex_;
}

SubspacePlayer* SubspaceStatBox::getSelectedPlayer() const
{
	if(pagePlayers_[mode_].size() == 0)
		return 0;

	return pagePlayers_[mode_][selectedIndex_];
}

void SubspaceStatBox::selectPlayerIndex(Uint index)
{
	//Uint currentIndex = pages_[mode_].getLineDisplayOffset();
	Uint currentIndex = topPlayerIndex_;
	
	if(pages_[mode_].size() == 0)
		index = 0;
	else
		index = index % pagePlayers_[mode_].size();	//safety check

	Uint teamOffset=0, topOffset=0, offset=0, teamDiff=0;
	if(mode_ == STATBOX_TeamSort && index < pagePlayers_[mode_].size())
	{
		teamOffset = getPlayerTeamIndex(mode_, pagePlayers_[mode_][index])+1;
		topOffset = getPlayerTeamIndex(mode_, pagePlayers_[mode_][topPlayerIndex_])+1;
		teamDiff = teamOffset - topOffset;

		if(index < currentIndex)	//scroll up
		{
			pages_[mode_].setLinesDisplayed(index+teamOffset, statBoxSize_);	
			topPlayerIndex_ = index;
		}
		else if(index+teamOffset > currentIndex+statBoxSize_)	//scroll down
		{
			int tempIndex = (int)index-(int)statBoxSize_+1;
			tempIndex = max(0, tempIndex);
			pages_[mode_].setLinesDisplayed(tempIndex+teamOffset, statBoxSize_);	
			topPlayerIndex_ = max(0, (int)index - (int)statBoxSize_);
		}
		else
		{
			if(topPlayerIndex_ <= 0)
				teamOffset = 0;
			
			if(index >= pagePlayers_[mode_].size()-1)		//hit bottom
			{
				index = pagePlayers_[mode_].size()-1;

				teamOffset = getPlayerTeamIndex(mode_, pagePlayers_[mode_][index])+1;
				int startLine = max(0, (int)index+(int)teamOffset-(int)min(statBoxSize_, pages_[mode_].size())+1);
				pages_[mode_].setLinesDisplayed(startLine, statBoxSize_);	
				topPlayerIndex_ = max(0, (int)index - (int)statBoxSize_);
			}
			/*else if(statBoxSize_ != 1)
				pages_[mode_].setLinesDisplayed(topPlayerIndex_+teamOffset, statBoxSize_);	*/
		}

		selectedIndex_ = index+teamOffset;
	}
	else
	{

		if(index < currentIndex)	//scroll up
		{
			pages_[mode_].setLinesDisplayed(index, statBoxSize_);
			topPlayerIndex_ = index;
		}
		else if(index >= currentIndex+statBoxSize_)	//scroll down
		{
			Uint newIndex = index - statBoxSize_+1;
			if(size() - newIndex < statBoxSize_)
				newIndex = size()-statBoxSize_;
			pages_[mode_].setLinesDisplayed(newIndex, statBoxSize_);	//set the current offset at index-minus-boxSize		
			topPlayerIndex_ = newIndex;
		}
		else	//no scroll, maybe resize
		{
			if(statBoxSize_ > topPlayerIndex_)			//check topPlayerIndex_
				topPlayerIndex_ = 0;
			else
				topPlayerIndex_ = index - statBoxSize_ + 1;

			pages_[mode_].setLinesDisplayed(topPlayerIndex_, statBoxSize_);	
		}

		selectedIndex_ = index;
	}

	if(mode_ != STATBOX_TeamStats)
	{
		if(selectedIndex_ < pagePlayers_[mode_].size())
			lastSelectedPlayer_ = pagePlayers_[mode_][selectedIndex_];
	}
}

void SubspaceStatBox::setBoxSize(Uint size)
{
	if(size == statBoxSize_)
		return;

	SubspaceTextBox& page = pages_[mode_];

	statBoxSize_ = size;

	selectPlayerIndex(getPlayerIndex(mode_, lastSelectedPlayer_));
	//page.setLinesDisplayed(page.getLineDisplayOffset(), statBoxSize_);
}

void SubspaceStatBox::setFont(const TextureFont& font)
{
	font_ = font;
}

void SubspaceStatBox::setMode(Uint mode)
{
	Uint newMode = mode % STATBOX_NumPages;

	if(newMode == STATBOX_TeamStats)
	{
		//lastSelectedPlayer_ = getSelectedPlayer();
		mode_ = newMode;
		selectPlayerIndex(getPlayerTeamIndex(mode_, lastSelectedPlayer_));	//TODO: find team
	}
	else
	{
		/*SubspacePlayer* selectedPlayer;
		if(!lastSelectedPlayer_)
		{
			selectedPlayer = getSelectedPlayer();
			lastSelectedPlayer_ = selectedPlayer;
		}
		else
			selectedPlayer = lastSelectedPlayer_;*/
		if(!lastSelectedPlayer_)
			lastSelectedPlayer_ = getSelectedPlayer();
		
		/*if(pagePlayers_[mode_].size() == 0)
			selectedPlayer = 0;
		else
			selectedPlayer = pagePlayers_[mode_][selectedIndex_];*/

		mode_ = newMode;		//set new mode

		//selectedIndex_ = getPlayerIndex(mode_, selectedPlayer);	//position in new page
		selectPlayerIndex(getPlayerIndex(mode_, lastSelectedPlayer_));
	}
}


Uint SubspaceStatBox::size() const
{
	return (Uint)pagePlayers_[mode_].size();
}

void SubspaceStatBox::draw() const
{
	//move to upper left

	glPushMatrix();
		glTranslated(0, -getDisplayHeight(), 0);

		glPushMatrix();
			pages_[mode_].draw();
		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		drawCursors();

		glColor4d(1.0, 1.0, 1.0, 1.0);
		drawBanners();
	glPopMatrix();
}

void SubspaceStatBox::drawBanner(Uint index) const
{
	const SubspaceTextBox& page = pages_[mode_];

	Uint hOffset = (font_.getFontHeight() - cursor_.getFrameHeight())/2.0 + 1;	//center cursor vertically in text
	Uint wOffset = font_.getFontWidth() * (1+nameWidth);
	
	double width = page.getDisplayWidthStart()+wOffset;
	if(width > page.getDisplayWidth())
		return;

	SubspacePlayer* player = pagePlayers_[mode_][index];;
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

void SubspaceStatBox::drawBanners() const
{
	if(mode_ == STATBOX_PlayerSort ||
		mode_ == STATBOX_TeamStats)
		return;

	const SubspaceTextBox& page = pages_[mode_];
	Uint index = topPlayerIndex_;//page.getLineDisplayOffset();
	Uint numDisplayed = 0;

	for(index; numDisplayed < statBoxSize_ && index < pagePlayers_[mode_].size(); ++index, ++numDisplayed)
	{
		drawBanner(index);
	}
}

void SubspaceStatBox::drawCursor(Uint index) const
{
	const SubspaceTextBox& page = pages_[mode_];
	
	Uint hOffset = (font_.getFontHeight() - cursor_.getFrameHeight())/2;	//center cursor vertically in text
	double height = getLineHeight(index) + hOffset;
	/*if(height < 0 || height >= pages_[mode_].getDisplayHeight()-pages_[mode_].getHeaderDisplayHeight())
		return;*/

	SubspacePlayer* player = pagePlayers_[mode_][index];

    Uint cursorIcon = -1;
	
	if(mode_ == STATBOX_TeamStats)
	{
		if(index == selectedIndex_)
			cursorIcon = getCursorIcon(0, true);
	}
	else
		cursorIcon = getCursorIcon(player, index==selectedIndex_);
	
	if(cursorIcon != -1)
	{
		glPushMatrix();
			glTranslated(page.getDisplayWidthStart(), height, 0);
			cursor_.displayFrame(cursorIcon);
		glPopMatrix();
	}
		
}

void SubspaceStatBox::drawCursors() const
{
	const SubspaceTextBox& page = pages_[mode_];
	//Uint index = page.getLineDisplayOffset();
	Uint index = topPlayerIndex_;
	/*Uint index = getSelectedIndex();
	if(statBoxSize_ < index)
		index -= statBoxSize_;
	else
		index = 0;*/
	Uint numDisplayed = 0;

	for(index; numDisplayed < statBoxSize_ && index < pagePlayers_[mode_].size(); ++index, ++numDisplayed)
	{
		drawCursor(index);
	}	
}

Uint SubspaceStatBox::getCursorIcon(SubspacePlayer* player, bool selected) const
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

double SubspaceStatBox::getLineHeight(Uint index) const
{
	const SubspaceTextBox& page = pages_[mode_];

	Uint lineOffset = 0;

	if(mode_ == STATBOX_TeamSort)
	{
		lineOffset = getPlayerTeamIndex(mode_, pagePlayers_[mode_][index])+1;
	}

	double height = page.getLineDisplayHeight(index+lineOffset);// - lineOffset*font_.getFontHeight();
	
	return height;
}

void SubspaceStatBox::getPlayerList(const SubspacePlayerMap& playerMap, SubspacePlayer* myPlayer, PlayerList* players, bool excludeMyPlayer)
{
	players->clear();

	SubspacePlayerMap::const_iterator i;
	for(i = playerMap.begin(); i != playerMap.end(); ++i)
	{
		SubspacePlayer* p = (*i).second;

		if(p != myPlayer || !excludeMyPlayer)
			players->push_back(p);
	}
}

void SubspaceStatBox::getTeamList(const SubspacePlayerMap& playerMap, SubspacePlayer* myPlayer, Uint teamNum, PlayerList* team, PlayerList* others, bool excludeMyPlayer)
{
	SubspacePlayerMap::const_iterator i;
	for(i = playerMap.begin(); i != playerMap.end(); ++i)
	{
		SubspacePlayer* p = (*i).second;
		
		if(p->getTeam() == teamNum)
		{
			if(p != myPlayer || !excludeMyPlayer)
				team->push_back(p);
		}
		else
		{
			others->push_back(p);
		}
	}
}

string SubspaceStatBox::getNameString(SubspacePlayer* p, bool showPoints, bool newline)
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

string SubspaceStatBox::getStatsString(SubspacePlayer* p, bool newline)
{
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

bool StatBox_playerNameSort(SubspacePlayer* a, SubspacePlayer* b)
{
	return (a->getName() < b->getName());
}

#define isLastPlayer() (playerNum++ >= players.size()-1)

void SubspaceStatBox::infoBoxPlayerSort(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer, bool showPoints)
{
	page.clear();

	//initialize lists
	PlayerList team, others;
	
	getTeamList(players, myPlayer, myPlayer->getTeam(), &team, &others, true);

	std::sort(team.begin(), team.end(), StatBox_playerNameSort);		//sort team
	std::sort(others.begin(), others.end(), StatBox_playerNameSort);	//sort other players

	//set up header
	page.clearHeader();
	page.writeHeader(" ", COLOR_Green);		//cursor column
	page.writeHeader(fixedWidthString(itoa((int)players.size()), nameWidth, centered), COLOR_Green);
	if(showPoints)
	{
		page.writeHeader("  ");						//banner icon column
		page.writeHeader("Points", COLOR_Green);	//points column
	}

	Uint playerNum = 0;
	//insert myPlayer
	page.write(getNameString(myPlayer, showPoints, !isLastPlayer()), COLOR_Yellow);
	pagePlayers.push_back(myPlayer);
	
	//insert team members
	PlayerList::iterator i;
	for(i=team.begin(); i != team.end(); ++i)
	{
		page.write(getNameString(*i, showPoints, !isLastPlayer()), COLOR_Yellow);
		pagePlayers.push_back(*i);
	}

	//insert others
	for(i=others.begin(); i != others.end(); ++i)
	{
		page.write(getNameString(*i, showPoints, !isLastPlayer()), COLOR_Gray);
		pagePlayers.push_back(*i);
	}
	
	page.flush();
}

void SubspaceStatBox::infoBoxPlayerPoints(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer)
{
	infoBoxPlayerSort(page, pagePlayers, players, myPlayer, true);
}

bool StatBox_playerPointSort(SubspacePlayer* a, SubspacePlayer* b)
{
	if(a->getPoints() > b->getPoints())
		return true;
	else
		return false;
}

void SubspaceStatBox::infoBoxPointSort(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer)
{
    page.clear();

	//initialize lists
	PlayerList playerList;
	getPlayerList(players, myPlayer, &playerList);

	std::sort(playerList.begin(), playerList.end(), StatBox_playerPointSort);
	
	//set up header
	page.clearHeader();
	page.writeHeader(" ", COLOR_Green);		//cursor column
	page.writeHeader(fixedWidthString(itoa((int)players.size()), 11, centered).substr(0, 10), COLOR_Green);
	page.writeHeader("Point Sort", COLOR_Green);						//banner icon column
		
	Uint playerNum = 0;

	//insert players
	PlayerList::iterator i;
	for(i=playerList.begin(); i != playerList.end(); ++i)
	{
		SubspacePlayer* p = (*i);
		if(p->getTeam() == myPlayer->getTeam())
			page.write(getNameString(p, true, !isLastPlayer()), COLOR_Yellow);
		else
			page.write(getNameString(p, true, !isLastPlayer()), COLOR_Gray);

		pagePlayers.push_back(*i);
	}

	page.flush();
}



void SubspaceStatBox::infoBoxTeamSort(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer)
{
	page.clear();

	//initialize lists
	TeamMap teams;

	teams_.clear();

	//sort players
	SubspacePlayer* p;
	SubspacePlayerMap::const_iterator i;
	for(i = players.begin(); i != players.end(); ++i)
	{
		p = (*i).second;
		assert(p);
			
		teams[p->getTeam()].push_back(p);
		//++numPlayers;
	}

	//set up header
	page.clearHeader();
	page.writeHeader(" ", COLOR_Green);		//cursor column
	page.writeHeader(fixedWidthString(itoa((int)players.size()), 11, centered), COLOR_Green);
	page.writeHeader("Team Sort", COLOR_Green);						//banner icon column

	Uint playerNum = 0;

	//move my team to first
	teams_.push_back(teams[myPlayer->getTeam()]);
	
	//add other teams
	TeamMap::const_iterator t;
	for(t = teams.begin(); t != teams.end(); ++t)
	{
		if( (*t).first != myPlayer->getTeam())
			teams_.push_back( (*t).second);
	}

	//insert teams
	TeamList::iterator j;
	for(j = teams_.begin(); j != teams_.end(); ++j)
	{
		const PlayerList& team = (*j);//.second;
		PlayerList::const_iterator k;

		//write team header
		Uint teamNum = team.front()->getTeam();//(*j).first;
		string teamHeader = itoaFixed(teamNum, 4) + string("-------------") + fixedWidthString(itoa((int)team.size()), 4, centered);
			
		page.write(teamHeader + "\n", COLOR_Red);

		//wrtie team players
		for(k = team.begin(); k != team.end(); ++k)
		{
			if(teamNum == myPlayer->getTeam())
				page.write(getNameString(*k, true, !isLastPlayer()), COLOR_Yellow);
			else
				page.write(getNameString(*k, true, !isLastPlayer()), COLOR_Gray);

			pagePlayers.push_back(*k);
		}
	}

	page.flush();
}


void SubspaceStatBox::infoBoxPlayerExtendedSort(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer)
{
	page.clear();

	//initialize lists
	PlayerList team, others;
	
	getTeamList(players, myPlayer, myPlayer->getTeam(), &team, &others, true);

	std::sort(team.begin(), team.end(), StatBox_playerNameSort);		//sort team
	std::sort(others.begin(), others.end(), StatBox_playerNameSort);	//sort other players

	//set up header
	page.clearHeader();
	page.writeHeader(" ", COLOR_Green);		//cursor column
	page.writeHeader(fixedWidthString(itoa((int)players.size()), nameWidth, centered), COLOR_Green);
	page.writeHeader("  ");						//banner icon column
	page.writeHeader(fixedWidthString("Squad", squadWidth, leftJustified), COLOR_Green);
	page.writeHeader("  W ", COLOR_Green);
	page.writeHeader("  L ", COLOR_Green);
	page.writeHeader("  R ", COLOR_Green);
	page.writeHeader("   AVE", COLOR_Green);
	
	Uint playerNum = 0;
	//insert myPlayer
	page.write(getStatsString(myPlayer, !isLastPlayer()), COLOR_Yellow);
	pagePlayers.push_back(myPlayer);
	
	//insert team members
	PlayerList::iterator i;
	for(i=team.begin(); i != team.end(); ++i)
	{
		page.write(getStatsString(*i, !isLastPlayer()), COLOR_Yellow);
		pagePlayers.push_back(*i);
	}
	
	//insert others
	for(i=others.begin(); i != others.end(); ++i)
	{
		page.write(getStatsString(*i, !isLastPlayer()), COLOR_Gray);
		pagePlayers.push_back(*i);
	}

	page.flush();
}


void SubspaceStatBox::infoBoxTeamStatistics(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& playerMap, SubspacePlayer* myPlayer)
{
	Uint numTeams = 0;
	SubspacePlayer* p = 0;
		
	page.clear();

	//initialize lists
	map<Uint, Uint> points, wins, losses, flags, teams;

	//gather team data
	SubspacePlayerMap::const_iterator i;
	for(i = playerMap.begin(); i != playerMap.end(); ++i)
	{
		p = (*i).second;
		
		Uint team = p->getTeam();

		++teams[team];						//players on team
		points[team] += p->getPoints();
		wins[team] += p->getWins();
		losses[team] += p->getLosses();
		flags[team] += p->getFlags();

		pagePlayers.push_back(p);
	}

	numTeams = (Uint)teams.size();

	//insert header
	page.clearHeader();
	page.writeHeader(" ");
	page.writeHeader("Freq", COLOR_Green);		
	page.writeHeader("   Points", COLOR_Green);
	page.writeHeader("   Win", COLOR_Green);
	page.writeHeader("  Lose", COLOR_Green);
	page.writeHeader("  Flag", COLOR_Green);
	
	Uint index = 0;
	Uchar color = COLOR_Yellow;

	//write my team first
	Uint teamNum = myPlayer->getTeam();

	page.write(" ");	//cursor column
	page.write( fixedWidthString(itoa(teamNum), 4, rightJustified), color );	//team number
	page.write(	fixedWidthString(itoa(points[teamNum]), 9, rightJustified), color );	//team points
	page.write(	fixedWidthString(itoa(wins[teamNum]), 6, rightJustified), color );				//team wins
	page.write(	fixedWidthString(itoa(losses[teamNum]), 6, rightJustified), color );	//team losses
	page.write(	fixedWidthString(itoa(flags[teamNum]), 6, rightJustified), color );	//team flags

	if(index++ < teams.size()-1)
		page.write("\n");

	color = COLOR_Gray;

	//write other teams
    map<Uint, Uint>::const_iterator j;
	for(j = teams.begin(); j != teams.end(); ++j)
	{
		teamNum = (*j).first;

		if(teamNum == myPlayer->getTeam())
			continue;

		page.write(" ");	//cursor column
		page.write( fixedWidthString(itoa(teamNum), 4, rightJustified), color );	//team number
		page.write(	fixedWidthString(itoa(points[teamNum]), 9, rightJustified), color );	//team points
		page.write(	fixedWidthString(itoa(wins[teamNum]), 6, rightJustified), color );				//team wins
		page.write(	fixedWidthString(itoa(losses[teamNum]), 6, rightJustified), color );	//team losses
		page.write(	fixedWidthString(itoa(flags[teamNum]), 6, rightJustified), color );	//team flags

		if(index++ < teams.size()-1)
			page.write("\n");
	}

	page.flush();
}
