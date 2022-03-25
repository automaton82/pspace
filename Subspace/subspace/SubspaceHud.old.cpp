#include "SubspaceHud.h"

#include <algorithm>

#include <map>
using std::map;

#include <vector>
using std::vector;

#include "BasicTextBuffer.h"
#include "SubspaceGlobal.h"

#include "SubspaceUtil.h"
#include "WinUtil.h"	

SubspaceHud::SubspaceHud() :
	player_(0),
	map_(0),

	chat_(0),
	//maxChatSize_(100),
	//nameWidth_(10),
	statBoxWidth_(75),
	selectedIndex_(0),
	//numChatLines_(8),

	//chatBox_(lineWidth_, numChatLines_),
	statusBox_(statBoxWidth_, 4, 3000),
	lifeBar_(50, 10),

	showRadar_(true)
{
}

void SubspaceHud::init()
{
	icons_.load("graphics/Icons.bm2", false);
	icons_.setFrameSize(26, 24);

	iconDigits_.load("graphics/icondoor.bm2", false);
	iconDigits_.setFrameSize(11, 13);

	energyBar_.load("graphics/hlthbar.bm2", false);
	energyBar_.setFrameSize(116, 28);

	statusIndicator_.load("graphics/disp.bm2", false);
	statusIndicator_.setFrameSize(84, 68);

	ledDigits_.load("graphics/led.bm2", false);
	ledDigits_.setFrameSize(7, 10);

	bigLedDigits_.load("graphics/engyfont.bm2", false);
	bigLedDigits_.setFrameSize(16, 24);
    
	font_.load("graphics/tallfont.bm2", false);
	font_.setFrameSize(8, 12);

	//chatBox_.setFont(font_);
	statusBox_.setFont(font_);

	lifeBar_.init();

	//initInfoBox();
	statBox_.setFont(font_);
	isInfoBoxActive_ = false;

	iconShiftRight_ = icons_.getFrameWidth() - 4;
	iconShiftLeft_ = -iconShiftRight_;

	shieldIcon_.load("graphics/shield.bm2", false);
	shieldIcon_.setFrameSize(16, 16);
	shieldIcon_.setAnimationPeriod(500);

	superIcon_.load("graphics/super.bm2", false);
	superIcon_.setFrameSize(16, 16);
	superIcon_.setAnimationPeriod(500);

	flagIcon_.load("graphics/dropflag.bm2", false);
	flagIcon_.setFrameSize(16, 16);
	flagIcon_.setAnimationPeriod(500);

	kothIcon_.load("graphics/kingex.bm2", false);
	kothIcon_.setFrameSize(16, 16);
	kothIcon_.setAnimationPeriod(500);

	portalIcon_.load("graphics/warppnt.bm2", false);
	portalIcon_.setFrameSize(16, 16);
	portalIcon_.setAnimationPeriod(500);

	timerLineHeight_ = 16;
	//timerLineStart_ = subspaceGlobal.windowHeight - statusIndicator_.getFrameHeight() - 5;

	//timer_ = 0;
	//radarFlashTime_ = 150;		//milliseconds

	resize();

	chatInput_.setFont(font_);
	chatInput_.setLineWidth(getChatLineWidth());
	chatInput_.setLinesDisplayed(10);
	chatInput_.setLineOffset(0);

	helpBox_.setFont(font_);
	string helpfile = "config/Commands.hlp";
	if(!helpBox_.load(helpfile))
	{
		debugout << "Failed to load help file \"" << helpfile << "\"" << endl;
		//printf("Failed to load help file \"%s\"\n", helpfile.c_str());
	}
	else
	{
		debugout << "Loaded help file \"" << helpfile << "\"" << endl;
		//printf("Loaded help file \"%s\"\n", helpfile.c_str());
	}
}

void SubspaceHud::resize()
{
	this->setOrthoMode(0, 0, subspaceGlobal.windowWidth, subspaceGlobal.windowHeight);

	lifeBar_.setWidth(subspaceGlobal.windowWidth * 147/800);	//TODO: find exact ratio
	//lifeBar_.setWidth(147);

	timerLineStart_ = subspaceGlobal.windowHeight - statusIndicator_.getFrameHeight() - 5;

	initRadar();
}


/***************
 * Player Info *
 ***************/
void SubspaceHud::setPlayer(SubspacePlayer* player)
{
	player_ = player;
}


/***********
 * Info Box*
 ***********/

void SubspaceHud::scrollUp()
{
	if(selectedIndex_ > 0)
        --selectedIndex_;

	statBox_.selectPlayerIndex(selectedIndex_);
}

void SubspaceHud::scrollDown()
{
	if(selectedIndex_ < statBox_.size()-1)
		++selectedIndex_;
		
	statBox_.selectPlayerIndex(selectedIndex_);
}

void SubspaceHud::scrollUpFast()
{
	Uint scroll = min(selectedIndex_, subspaceGlobal.getVar("statBoxSize")-1);
	if(scroll > selectedIndex_)
		selectedIndex_ = 0;
	else
		selectedIndex_ -= scroll;

	selectedIndex_ %= statBox_.size();

	statBox_.selectPlayerIndex(selectedIndex_);
}

void SubspaceHud::scrollDownFast()
{
	Uint scroll = min(statBox_.size(), subspaceGlobal.getVar("statBoxSize")-1);
	if(selectedIndex_+scroll >= statBox_.size())
		selectedIndex_ = statBox_.size()-1;
	else
		selectedIndex_ += scroll;
	selectedIndex_ %= statBox_.size();

	statBox_.selectPlayerIndex(selectedIndex_);
}

void SubspaceHud::setStatboxSize(Uint size)
{
	size = max(1, min(50, size));

	statBox_.setBoxSize(size);
}

void SubspaceHud::refreshStatBox()
{
	statBox_.init(*players_, player_);
}

void SubspaceHud::setPlayerMap(SubspacePlayerMap* playerMap)
{
	players_ = playerMap;
}

SubspacePlayer* SubspaceHud::getSelectedPlayer() const
{
	return statBox_.getSelectedPlayer();
}

void SubspaceHud::initInfoBox(Uint mode)
{
	isInfoBoxActive_ = true;
	helpPage_ = -1;

	switch(mode)
	{
	case 0:
		//infoBox_ = TextTable();
		isInfoBoxActive_ = false;
		//dont display info box
		break;
	/*case 1:	infoBoxPlayerSort();	break;
	case 2:	infoBoxPlayerPoints();  break;
	case 3:	infoBoxPointSort();		break;
	case 4: infoBoxTeamSort();	break;
	case 5:	infoBoxPlayerExtendedSort();	break;
	case 6: infoBoxTeamStatistics();		break;
	default:
		infoBox_ = TextTable();*/
	}

	statBox_.setMode(mode-1);
	Uint val = subspaceGlobal.getVar("statBoxSize");
	val = max(1, min(50, val));
	statBox_.setBoxSize(val);

	if(players_ && player_)
		statBox_.init(*players_, player_);
}

bool subspacePlayerNameLessThan(SubspacePlayer* a, SubspacePlayer* b)
{
	return (a->getName() < b->getName());
}

bool playerNameSort(SubspacePlayer* a, SubspacePlayer* b)
{
	return (a->getName() < b->getName());
}

void SubspaceHud::infoBoxPlayerSort(bool showPoints)
{
	TextTable newBox;
	int numPlayers=0;
	Uint i=0;
	//SubspacePlayerMap::iterator i;
	
	newBox.setFont(this->font_);
	
	newBox.addColumn(1, " ", leftJustified, COLOR_Green); //cursor column
	newBox.addColumn(12, "0", centered, COLOR_Green);	//player names
	if(showPoints)
	{
		newBox.addColumn(2, " ", leftJustified, COLOR_Green); //banner icon column
		newBox.addColumn(6, "Points", rightJustified, COLOR_Green);
	}
	
	vector<SubspacePlayer*> team, others;

	if(players_)
	{
		//sort team members
		getTeamList(player_->getTeam(), &team, &others, true);
		/*for(i = players_.begin(); i != players_.end(); ++i)
		{
			SubspacePlayer* p = (*i).second;
			assert(p);

			if(p->getTeam() == player_->getTeam())
			{
				if(p != player_)
					team.push_back(p);
			}
			else
				others.push_back(p);
		}*/

		newBox.setColumnHeader(1, itoa(team.size()+others.size()+1), centered, COLOR_Green);

		std::sort(team.begin(), team.end(), playerNameSort);
		std::sort(others.begin(), others.end(), playerNameSort);

		newBox.setCell(0, 0, " ", leftJustified, COLOR_Yellow);
		newBox.setCell(0, 1, player_->getName(), leftJustified, COLOR_Yellow);
		if(showPoints)
			newBox.setCell(0, 3, itoa(player_->getPoints()), rightJustified, COLOR_Yellow);

		Uint offset = 1;
		for(i=0; i < team.size(); ++i)
		{
			newBox.addRow();
			newBox.setCell(offset+i, 0, " ", leftJustified, COLOR_Yellow);
			newBox.setCell(offset+i, 1, team[i]->getName(), leftJustified, COLOR_Yellow);
			if(showPoints)
				newBox.setCell(offset+i, 3, itoa(team[i]->getPoints()), rightJustified, COLOR_Yellow);
		}

		//offset += i;
		offset += team.size();
		for(i=0; i < others.size(); ++i)
		{
			newBox.addRow();			
			newBox.setCell(offset+i, 0, " ", leftJustified, COLOR_Gray);
			newBox.setCell(offset+i, 1, others[i]->getName(), leftJustified, COLOR_Gray);
			if(showPoints)
				newBox.setCell(offset+i, 3, itoa(others[i]->getPoints()), rightJustified, COLOR_Gray);
		}
	}
	
	newBox.setWidth(12+1);
	if(showPoints)
		newBox.setWidth(20+1);
	newBox.setHeight(min(20, team.size()+others.size()+1+1));

	newBox.setBackgroundColor(Color(0.05, 0.1, 0.05, 0.9));

	infoBox_ = newBox;
}

void SubspaceHud::infoBoxPlayerPoints()
{
	infoBoxPlayerSort(true);
}

bool playerPointSort(SubspacePlayer* a, SubspacePlayer* b)
{
	if(a->getPoints() > b->getPoints())
		return true;
	else
		return false;
}

void SubspaceHud::infoBoxPointSort()
{
	TextTable newBox;
	int numPlayers=0;
	Uint i=0;
	
	newBox.setFont(this->font_);
	
	newBox.addColumn(1, " ", leftJustified, COLOR_Green); //cursor column
	newBox.addColumn(12, "0", centered, COLOR_Green);	//player names
	newBox.addColumn(12, "Point Sort", rightJustified, COLOR_Green);	//player names
	
	vector<SubspacePlayer*> playerList;

	if(players_)
	{
		//sort team members
		getPlayerList(&playerList);
		/*for(i = players_.begin(); i != players_.end(); ++i)
		{
			SubspacePlayer* p = (*i).second;
			assert(p);
		
            playerList.push_back(p);
		}*/

		newBox.setColumnHeader(1, itoa(playerList.size()), centered, COLOR_Green);

		std::sort(playerList.begin(), playerList.end(), playerPointSort);

		int offset = 0;
		for(i=0; i < playerList.size(); ++i)
		{
			SubspacePlayer* p = playerList[i];
			ColorType color = COLOR_Gray;

			if(p->getTeam() == player_->getTeam())
			{
				color = COLOR_Yellow;
			}

			newBox.setCell(offset+i, 0, " ", leftJustified, color);
			newBox.setCell(offset+i, 1, playerList[i]->getName(), leftJustified, color);
			newBox.setCell(offset+i, 2, itoa(playerList[i]->getPoints()), rightJustified, color);

			if(i != playerList.size()-1)
                newBox.addRow();
		}
	}
	
	newBox.setWidth(12+12+1);
	newBox.setHeight(min(20, playerList.size()+1));

	newBox.setBackgroundColor(Color(0.05, 0.1, 0.05, 0.9));

	infoBox_ = newBox;
}


void SubspaceHud::infoBoxTeamSort()
{
	TextTable newBox;
	int numPlayers = 0;
	int numTeams = 0;
	//Uint i;
	SubspacePlayerMap::iterator i;
	SubspacePlayer* p;
	
	newBox.setFont(this->font_);
	
	newBox.addColumn(21, " ", centered, COLOR_Green); //column
	
	fwStrList fwStrings;
	map<Uint, vector<SubspacePlayer*> > teams;

	if(players_)
	{
		//sort players
		for(i = players_->begin(); i != players_->end(); ++i)
		{
			p = (*i).second;
			assert(p);
			
			teams[p->getTeam()].push_back(p);
			++numPlayers;
		}

		map<Uint, vector<SubspacePlayer*> >::const_iterator j;
		for(j = teams.begin(); j != teams.end(); ++j)
		{
			++numTeams;

			const vector<SubspacePlayer*>& team = (*j).second;
			vector<SubspacePlayer*>::const_iterator k;

			//add team header
			Uint teamNum = (*j).first;
			string teamHeader = itoaFixed(teamNum, 4) + string("-------------") + fixedWidthString(itoa(team.size()), 4, centered);
			
			newBox.setCell(newBox.numRows()-1, 0, teamHeader, leftJustified, COLOR_Red);

			for(k = team.begin(); k != team.end(); ++k)
			{
				p = (*k);

				fwStrings.clear();
				fwStrings.push_back( fwStr(" ", 1) ); //cursor column
				fwStrings.push_back( fwStr(p->getName(), 12, leftJustified) );
				fwStrings.push_back( fwStr(itoa(p->getPoints()), 8, rightJustified) );
				//fwStrings.push_back( fwStr("test", 12, leftJustified) );
				//fwStrings.push_back( fwStr("test2", 12, leftJustified) );

				newBox.addRow();
				Uint color;
				if(player_ && (teamNum == player_->getTeam()))
					color = COLOR_Yellow;
				else
					color = COLOR_Gray;
				newBox.setCell(newBox.numRows()-1, 0, fixedWidthString(fwStrings), leftJustified, color);				
			}

			++j;
			if(j != teams.end())
				newBox.addRow();
			--j;
		}
	}

	//header
	fwStrings.clear();
	fwStrings.push_back( fwStr(itoa(numPlayers), 12, centered) );
	fwStrings.push_back( fwStr("Team Sort", 9, rightJustified) );
	newBox.setColumnHeader(0, fixedWidthString(fwStrings), centered, COLOR_Green);
	
	newBox.setWidth(21);
	newBox.setHeight(min(20, numPlayers+numTeams+1));

	newBox.setBackgroundColor(Color(0.05, 0.1, 0.05, 0.9));

	infoBox_ = newBox;
}


void SubspaceHud::infoBoxPlayerExtendedSort()
{
	TextTable newBox;
	int numPlayers = 0;
	Uint i;
	fwStrList fwStrings;
	
	newBox.setFont(this->font_);
	
	newBox.addColumn(42, " ", centered, COLOR_Green); //column
	
	vector<SubspacePlayer*> team;
	vector<SubspacePlayer*> others;

	if(players_)
	{
		//sort team members
		getTeamList(player_->getTeam(), &team, &others, true);
		/*for(i=0; i < (Uint)players_->size(); ++i)
		{
			SubspacePlayer* p = players_->getItem(i);
			if(p)
			{
				++numPlayers;
				if(p->getTeam() == player_->getTeam())
				{
					if(p != player_)
						team.push_back(p);
				}
				else
					others.push_back(p);
			}
		}*/

		std::sort(team.begin(), team.end(), subspacePlayerNameLessThan);
		std::sort(others.begin(), others.end(), subspacePlayerNameLessThan);

		fwStrings.clear();
		fwStrings.push_back( fwStr(" ", 1, leftJustified) );
		fwStrings.push_back( fwStr(player_->getName(), 12, leftJustified) );
		fwStrings.push_back( fwStr(player_->getSquad(), 8, leftJustified) );
		fwStrings.push_back( fwStr(itoa(player_->getWins()), 5, rightJustified) );
		fwStrings.push_back( fwStr(itoa(player_->getLosses()), 5, rightJustified) );
		fwStrings.push_back( fwStr("???", 5, rightJustified) );
		fwStrings.push_back( fwStr(ftoa(player_->getAveKill(), 1), 6, rightJustified) );
		newBox.setCell(newBox.numRows()-1, 0, fixedWidthString(fwStrings), leftJustified, COLOR_Yellow);

		//int offset = 1;
		for(i=0; i < team.size(); ++i)
		{
			newBox.addRow();

			fwStrings.clear();
			fwStrings.push_back( fwStr(" ", 1, leftJustified) );
			fwStrings.push_back( fwStr(team[i]->getName(), 12, leftJustified) );
			fwStrings.push_back( fwStr(team[i]->getSquad(), 8, leftJustified) );
			fwStrings.push_back( fwStr(itoa(team[i]->getWins()), 5, rightJustified) );
			fwStrings.push_back( fwStr(itoa(team[i]->getLosses()), 5, rightJustified) );
			fwStrings.push_back( fwStr("???", 5, rightJustified) );
			fwStrings.push_back( fwStr(ftoa(team[i]->getAveKill(), 1), 6, rightJustified) );
			newBox.setCell(newBox.numRows()-1, 0, fixedWidthString(fwStrings), leftJustified, COLOR_Yellow);
		}

		//offset += i;
		for(i=0; i < others.size(); ++i)
		{
			newBox.addRow();

			fwStrings.clear();
			fwStrings.push_back( fwStr(" ", 1, leftJustified) );
			fwStrings.push_back( fwStr(others[i]->getName(), 12, leftJustified) );
			fwStrings.push_back( fwStr(others[i]->getSquad(), 8, leftJustified) );
			fwStrings.push_back( fwStr(itoa(others[i]->getWins()), 5, rightJustified) );
			fwStrings.push_back( fwStr(itoa(others[i]->getLosses()), 5, rightJustified) );
			fwStrings.push_back( fwStr("???", 5, rightJustified) );
			fwStrings.push_back( fwStr(ftoa(others[i]->getAveKill(), 1), 6, rightJustified) );
			newBox.setCell(newBox.numRows()-1, 0, fixedWidthString(fwStrings), leftJustified, COLOR_Gray);
		}
	}
	
	//header
	fwStrings.clear();
	fwStrings.push_back( fwStr(" ", 1, leftJustified) );
	fwStrings.push_back( fwStr(itoa(numPlayers), 12, centered) );
	fwStrings.push_back( fwStr("Squad", 8, leftJustified) );
	fwStrings.push_back( fwStr("  W", 5, centered) );
	fwStrings.push_back( fwStr("  L", 5, centered) );
	fwStrings.push_back( fwStr("  R", 5, centered) );
	fwStrings.push_back( fwStr("Ave", 6, rightJustified) );
	newBox.setColumnHeader(0, fixedWidthString(fwStrings), leftJustified, COLOR_Green);
	
	newBox.setWidth(42);
	newBox.setHeight(min(20, team.size()+others.size()+1+1));

	newBox.setBackgroundColor(Color(0.05, 0.1, 0.05, 0.9));

	infoBox_ = newBox;
}


void SubspaceHud::infoBoxTeamStatistics()
{
	TextTable newBox;
	Uint numTeams = 0;
	//Uint i;
	SubspacePlayer* p = 0;
	
	fwStrList fwStrings;
	//TODO: use structure instead of parallel maps
	map<Uint, Uint> points, wins, losses, flags, teams;

	newBox.setFont(this->font_);
	
	newBox.addColumn(34, " ", centered, COLOR_Green); //column

	if(players_)
	{
		//gather team data
		SubspacePlayerMap::iterator i;
		for(i = players_->begin(); i != players_->end(); ++i)
		{
			p = (*i).second;
			assert(p);

			Uint team = p->getTeam();
			++teams[team];
			points[team] += p->getPoints();
			wins[team] += p->getWins();
			losses[team] += p->getLosses();
			flags[team] += p->getFlags();
		}

		numTeams = teams.size();

		map<Uint, Uint>::const_iterator j;
		for(j = teams.begin(); j != teams.end(); ++j)
		{
			//++numTeams;
			
			Uint teamNum = (*j).first;

            fwStrings.clear();
			fwStrings.push_back( fwStr(" ", 1, rightJustified) );
			fwStrings.push_back( fwStr(itoa(teamNum), 5, rightJustified) );
			fwStrings.push_back( fwStr(itoa(points[teamNum]), 9, rightJustified) );
			fwStrings.push_back( fwStr(itoa(wins[teamNum]), 6, rightJustified) );
			fwStrings.push_back( fwStr(itoa(losses[teamNum]), 6, rightJustified) );
			fwStrings.push_back( fwStr(itoa(flags[teamNum]), 6, rightJustified) );			//TODO: implement flags for teams
			
			Uint color;
			if(p && teamNum == p->getTeam())
				color = COLOR_Yellow;
			else
				color = COLOR_Gray;

			newBox.setCell(newBox.numRows()-1, 0, fixedWidthString(fwStrings), leftJustified, color);

			++j;
			if(j != teams.end())
				newBox.addRow();
			--j;
		}
	}

	//header
	fwStrings.clear();
	fwStrings.push_back( fwStr(" ", 1, leftJustified) );
	fwStrings.push_back( fwStr("Freq", 5, rightJustified) );
	fwStrings.push_back( fwStr("Points", 9, rightJustified) );
	fwStrings.push_back( fwStr("Win", 6, rightJustified) );
	fwStrings.push_back( fwStr("Lose", 6, rightJustified) );
	fwStrings.push_back( fwStr("Flag", 6, rightJustified) );
	newBox.setColumnHeader(0, fixedWidthString(fwStrings), leftJustified, COLOR_Green);
		
	newBox.setWidth(34);
	newBox.setHeight(min(20, numTeams+1));

	newBox.setBackgroundColor(Color(0.05, 0.1, 0.05, 0.9));

	infoBox_ = newBox;
}

void SubspaceHud::getPlayerList(PlayerList* players)
{
	SubspacePlayerMap::iterator i;
	for(i = players_->begin(); i != players_->end(); ++i)
	{
		SubspacePlayer* p = (*i).second;
		assert(p);

		players->push_back(p);
	}
}

void SubspaceHud::getTeamList(Uint teamNum, PlayerList* team, PlayerList* others, bool excludeMyPlayer)
{
	SubspacePlayerMap::iterator i;
	for(i = players_->begin(); i != players_->end(); ++i)
	{
		SubspacePlayer* p = (*i).second;
		assert(p);

		if(p->getTeam() == teamNum)
		{
			if(p != player_ || !excludeMyPlayer)
				team->push_back(p);
		}
		else
		{
			others->push_back(p);
		}
	}
}

/******************
 * Help Page Stuff *
 ******************/

void SubspaceHud::toggleHelpPage()
{
	++helpPage_;

	if(helpPage_ >= helpBox_.size())
		helpPage_ = -1;
	else
		helpBox_.setPage(helpPage_);

	isInfoBoxActive_ = false;
}

/************
 * Map Info *
 ************/
void SubspaceHud::setMap(SubspaceMap* map)
{
	map_ = map;

	initRadar();
}

void SubspaceHud::initRadar()
{
	radarDisplayWidth_ = 208;	//size on screen
	radarDisplayHeight_ = 208;

	/*radarWidth_ = (Uint)(subspaceGlobal.windowWidth * 208.0/1280);
	radarHeight_ = (Uint)(subspaceGlobal.windowHeight * 208.0/1024);*/

	Uint radarWidthMax = (Uint)(subspaceGlobal.windowWidth * 208.0/1280 * (double)SubspaceMap::maxTileX/radarDisplayWidth_);
	Uint radarHeightMax = (Uint)(subspaceGlobal.windowHeight * 208.0/1024 * (double)SubspaceMap::maxTileY/radarDisplayHeight_);

	radar_.initRadar(map_, radarWidthMax, radarHeightMax);
}

/*void SubspaceHud::createRadar(Uint width, Uint height)
{
	Ulong i,j;
	Ulong index;

	if(!map_)
		return;


	if(width==0 && height==0)
		return;

	width = min(SubspaceMap::maxTileX, width);
	height = min(SubspaceMap::maxTileY, height);

	char* pixels = new char[width * height * 4];
	//memset(pixels, 0, width*height*4);

	debugout << "Creating radar..." << ends;
	//printf("Creating radar...");

	
	Uint w, h;
	double ratioX = (double)width / SubspaceMap::maxTileX;
	double ratioY = (double)height / SubspaceMap::maxTileY;

	for(i=0; i < SubspaceMap::maxTileY; ++i)
	{
		for(j=0; j < SubspaceMap::maxTileX; ++j)
		{
			w = j * ratioX;
			h = i * ratioY;

			//index = (i*SubspaceMap::maxTileX *ratioY + j*ratioX) * 4;
			index = (h*width + w) * 4;

			pixels[index + 0] = (char)10;
			pixels[index + 1] = (char)25;
			pixels[index + 2] = (char)10;
			pixels[index + 3] = (char)(255 * 0.9);
		}
	}

	const SubspaceMap::BasicMatrix tiles = map_->getTileMatrix();
	//SubspaceTile::TileType tileType;

	//create texture
	for(i=0; i < SubspaceMap::maxTileY; ++i)
	{
		for(j=0; j < SubspaceMap::maxTileX; ++j)
		{
			w = j * ratioX;
			h = i * ratioY;

			//index = (i*SubspaceMap::maxTileX *ratioY + j*ratioX) * 4;
			index = (h*width + w) * 4;

			//assert(index <= width*height*4);

			const SubspaceTile& tile = map_->getTile(j, i);
			//tileType = tiles[j][i];

			if(tile.isDoor())
			//if(tileType >= SubspaceTile::vieDoorStart && tileType <= SubspaceTile::vieDoorEnd)
			{
				pixels[index + 0] = (char)173;	//light gray
				pixels[index + 1] = (char)173;
				pixels[index + 2] = (char)173;
				pixels[index + 3] = (char)255;
			}
			else if(tile.isTeamGoal())
			//else if(tileType == SubspaceTile::ssbTeamGoal)
			{
				pixels[index + 0] = (char)255;	//yellow
				pixels[index + 1] = (char)255;
				pixels[index + 2] = 0;
				pixels[index + 3] = (char)255;
			}
			else if(tile.isGoal())		
			//else if(tileType == SubspaceTile::vieGoalArea)
			{
				pixels[index + 0] = (char)255;	//red
				pixels[index + 1] = 57;
				pixels[index + 2] = 8;
				pixels[index + 3] = (char)255;
			}
			else if(tile.isSafe())
			//else if(tileType == SubspaceTile::vieSafeZone)
			{
				pixels[index + 0] = 24;	//green
				pixels[index + 1] = 82;
				pixels[index + 2] = 24;
				pixels[index + 3] = (char)255;
			}
			else if(tile.isCollidable() || tile.isAsteroid() || tile.isAsteroidPart())
			//else if((tileType >= SubspaceTile::vieNormalStart && tileType <= SubspaceTile::vieNormalEnd) || (tileType >= SubspaceTile::vieAsteroidStart && tileType <= SubspaceTile::vieAsteroidEnd))
			{
				pixels[index + 0] = 90;	//gray
				pixels[index + 1] = 90;
				pixels[index + 2] = 90;
				pixels[index + 3] = (char)255;
			}
			//else
			//{
			//	pixels[index + 0] = (char)10;
			//	pixels[index + 1] = (char)25;
			//	pixels[index + 2] = (char)10;
				//pixels[index + 3] = (char)255;
			//}
		}
	}

	debugout << "done." << endl;
	//printf("done.\n");

	radarTextureData_.loadFromMemory(pixels, width, height, false);
	delete [] pixels;

	radarTexture_.setTextureData(&radarTextureData_);
}*/

const SubspaceRadar* SubspaceHud::getRadar() const
{
	return &radar_;
}

void SubspaceHud::setShowRadar(bool b)
{
	showRadar_ = b;

	chatInput_.setLineWidth(getChatLineWidth());
	
	if(chat_)
	{
		chat_->setLineWidth(getChatLineWidth());
		//chat_->flush();
	}
}

void SubspaceHud::toggleRadar()
{
	setShowRadar(!showRadar_);
}

/**************
 * Menu Stuff *
 **************/

void SubspaceHud::toggleNametag()
{
	/*subspaceGlobal.nametagOption = (NametagOptionType)((Uint)subspaceGlobal.nametagOption + 1);

	if(subspaceGlobal.nametagOption >= NAMETAG_Unknown)
		subspaceGlobal.nametagOption = NAMETAG_NoTags;*/

	Uint val = subspaceGlobal.getVar("nametags")+1;

	if(val >= NAMETAG_Unknown)
		val = NAMETAG_NoTags;

	subspaceGlobal.setVar("nametags", val);
}

/*****************
 * Chat Messages *
 *****************/

void SubspaceHud::setChat(SubspaceChat* chat)
{
	chat_ = chat;
}


void SubspaceHud::setChatBuffer(string* chatBuffer)
{
	chatBuffer_ = chatBuffer;
}

void SubspaceHud::setMessageLines(Uint lines)
{
	messageLines_ = lines;
}

/*void SubspaceHud::addChatMessage(const ChatMessage& chatMessage)
{
	int length = 0;
	string name;
	Uint n=0;

	if(chatMessage.hasSender())
	{
		//format the name
		for(n=strlen(chatMessage.sender.c_str()); n < nameWidth_; ++n)		//append spaces to short names
			name += ' ';
		for(n=0; n < min(strlen(chatMessage.sender.c_str()), nameWidth_); ++n)
			name += chatMessage.sender[n];

		//set header

		const char* tmp = name.c_str();
		chatBox_.setLineHeader(name + "> ", chatMessage.getNameColor());
	}
	else
	{
		chatBox_.setLineHeader("", 0);
	}

	chatBox_.addLine(chatMessage.text, chatMessage.getTextColor());
}*/

void SubspaceHud::addSystemMessage(const string& message, int color)
{
	statusBox_.setLineHeader("", 0);

	statusBox_.addLine(message, color, false);	//don't need to parse
}

/**********
 * Update *
 **********/

void SubspaceHud::update(double timeStep)
{
	static int count = 0;

	GameHud::update(timeStep);


	//chat buffer
	chatInput_.clear();

	//double radarWidth = (Uint)(subspaceGlobal.windowWidth * 208.0/1280);
	//Uint lineWidth = (subspaceGlobal.windowWidth-radarWidth)/(double)chatInput_.getFont().getFontWidth() - 1;
	//chatInput_.setLineWidth(lineWidth);

	chatInput_.setLineWidth(getChatLineWidth());
	
	//chatInput_.writeMessage("", 0, *chatBuffer_, ChatMessage::getMessageColor(*chatBuffer_));
	//ChatMessage msg("", *chatBuffer_)
	//chatInput_.writeMessage("", *chatBuffer, ChatMessage::setMessage(
	//chatInput_.flush();

	inputLines_ = chat_->size();

	//chat box
	if(chat_)
	{
		Uint lineOffset = 0;
		Uint lines = 0;

		if(chat_->size() > messageLines_)
			lineOffset = chat_->size() - messageLines_;
		
		if(chatInput_.size() < messageLines_)
			lines =  messageLines_ - chatInput_.size();
			
		if(chat_->size() >= messageLines_)
			lineOffset += chatInput_.size();
		
		chat_->setLinesDisplayed(lines);
		chat_->setLineOffset(lineOffset);

		chat_->update(timeStep);
		//chat_->flush();
	}

	//chatBox_.update(timeStep);	//doesn't actually do anything
	statusBox_.update(timeStep);
	lifeBar_.update(timeStep);

	if(player_)
	{
		if(player_->getMaxPossibleEnergy() == 0)
			lifeBar_.setMaxLifePercent(0);
		else
			lifeBar_.setMaxLifePercent(player_->getMaxEnergy() / player_->getMaxPossibleEnergy());
		//lifeBar_.setMaxLifePercent(0.5);
		if(player_->getMaxEnergy() == 0)
			lifeBar_.setLifePercent(0);
		else
			lifeBar_.setLifePercent(player_->getEnergy() / player_->getMaxEnergy());
	}

	//testing timed messages
	/*if(rand()%50 == 0)
	{
		statusBox_.setMessageTime(rand()%2000 + 1);
		char buffer[10];
		itoa(count++, buffer, 10);
		this->addSystemMessage(buffer, 0);
	}*/

	//update flag timers

	shieldIcon_.advanceFrame(timeStep);
	superIcon_.advanceFrame(timeStep);
	flagIcon_.advanceFrame(timeStep);
	portalIcon_.advanceFrame(timeStep);
	kothIcon_.advanceFrame(timeStep);

	//timer_ += timeStep;
	radar_.update(timeStep);


	//radar
	if(player_)
	{
		Uint tx, ty;
		objectToTileCoords(player_->getPosition(), &tx, &ty);
		
		radar_.setDisplayArea(tx, ty, radarDisplayWidth_, radarDisplayHeight_);
		//radar_.setDisplayArea(tx, ty, 1024, 1024);
	}
}

/***********
 * Display *
 ***********/


void SubspaceHud::draw() const
{
	//TODO: all these pushes and pops shouldn't be necessary, just precautionary, take them out later
	Color color(1.0, 1.0, 1.0, 1.0);
	if(player_)
	{
		glPushMatrix();
			//drawMyPlayerInfo();
			drawAllPlayersInfo();
		glPopMatrix();

		glPushMatrix();
			drawSafetyTime();
		glPopMatrix();

		glColor4d(color.r_, color.b_, color.g_, color.a_);
		glPushMatrix();
    		drawStatus();
		glPopMatrix();

		glColor4d(color.r_, color.b_, color.g_, color.a_);
		glPushMatrix();
			drawLifeBar();
		glPopMatrix();

		if(showRadar_)
		{
			glColor4d(color.r_, color.b_, color.g_, color.a_);
			glPushMatrix();
				drawRadarInfo();
			glPopMatrix();

			glColor4d(color.r_, color.b_, color.g_, color.a_);
			glPushMatrix();
				drawRadar();
			glPopMatrix();
		}

		glColor4d(color.r_, color.b_, color.g_, color.a_);
		glPushMatrix();
	    	drawMyPlayerIcons();
		glPopMatrix();

		glColor4d(color.r_, color.b_, color.g_, color.a_);
		glPushMatrix();
			drawInfoBox();	//player names, etc.
		glPopMatrix();

		glPushMatrix();
			drawHelpBox();	//player names, etc.
		glPopMatrix();
		glColor4d(1.0, 1.0, 1.0, 1.0);

		glPushMatrix();
        	drawTimers();	//flags, shields, etc.
		glPopMatrix();
	}

	/*glPushMatrix();
		drawChat();		//should be displayed over everything else
	glPopMatrix();*/
	
	glPushMatrix();
		//glTranslated(300, 300, 0);
		drawChat2();		//should be displayed over everything else
	glPopMatrix();

	glPushMatrix();
		drawSystemChat();
	glPopMatrix();
}

/*void SubspaceHud::drawChat() const
{
	Uint numChatLines_ = 8;

	//draw system messages
	glPushMatrix();						//TODO: move this into a new function
		glTranslated(subspaceGlobal.windowWidth * 1/5, subspaceGlobal.windowHeight * 3/7, 0);		//TODO: base on actual window sizes
		//glTranslatef(0, 0, 0);		//TODO: base on actual window sizes

		statusBox_.draw();
	glPopMatrix();

	//translate to the top of the buffer
	glTranslated(0, chatBuffer_->size()/lineWidth_ * font_.getFontHeight(), 0);	

	glPushMatrix();
	
	Uint linesDrawn = 0;
	Uint pos = 0;

	string line;
	int color = 0;
	color = getMessageColor(*chatBuffer_);

	//draw chat buffer
	while(pos < chatBuffer_->size() && linesDrawn < numChatLines_)
	{
		line = chatBuffer_->substr(pos, lineWidth_);

		font_.displayString(line.c_str(), color);

		++linesDrawn;
		pos += lineWidth_;
		
		glTranslated(0, -font_.getFontHeight(), 0);
	}
	glPopMatrix();

	//draw chat box
	if(chatBuffer_->size() > 0)		//move back to the chat buffer location
	{
		glTranslated(0, font_.getFontHeight(), 0);	
	}

	//translate to the top of the box
	glTranslated(0, chatBox_.size() * font_.getFontHeight(), 0);

	chatBox_.draw();
}*/

void SubspaceHud::drawChat2() const
{
	glPushMatrix();
		chatInput_.draw();
	glPopMatrix();

	glTranslated(0, chatInput_.size() * font_.getFontHeight(), 0);	//translate to the top of the buffer
	if(chat_)
		chat_->draw();
}

void SubspaceHud::drawSystemChat() const
{
	//draw system messages
	glPushMatrix();						//TODO: move this into a new function
		glTranslated(subspaceGlobal.windowWidth * 1/5, subspaceGlobal.windowHeight * 3/7, 0);		//TODO: base on actual window sizes

		statusBox_.draw();
	glPopMatrix();
}

void SubspaceHud::drawLifeBar() const
{
	int frame = 0;
	
	if(player_->getEnergy() == player_->getMaxEnergy())
		frame = 1;

	glPushMatrix();
		glTranslated(subspaceGlobal.windowWidth/2.0 - energyBar_.getFrameWidth()/2.0, 
			subspaceGlobal.windowHeight - energyBar_.getFrameHeight(), 0);	

		double midWidth = energyBar_.getFrameWidth() / 2;
		double midHeight = energyBar_.getFrameHeight() / 2 + 1;

		//1/8

		glPushMatrix();
			glTranslated(midWidth, midHeight+2, 0);
			lifeBar_.display();
		glPopMatrix();

		glPushMatrix();
			glTranslated(midWidth, midHeight+2, 0);
			glScalef(-1, 1, 1);
			lifeBar_.display();
		glPopMatrix();

		energyBar_.displayFrame(frame);
	glPopMatrix();
}

void SubspaceHud::drawHelpBox() const
{
	if(helpPage_ == -1)
		return;

	int offset = 1;
	glPushMatrix();
		glTranslated(offset, subspaceGlobal.windowHeight-offset - helpBox_.getDisplayHeight(), 0);
		helpBox_.draw();
	glPopMatrix();
}

void SubspaceHud::drawInfoBox() const
{
	if(!isInfoBoxActive_)
		return;

	int offset = 1;
	glPushMatrix();
		glTranslated(offset, subspaceGlobal.windowHeight-offset, 0);
		//infoBox_.display();
		statBox_.draw();
	glPopMatrix();
}

/*void SubspaceHud::drawMiniMap() const	
{
	int drawWidth = 100;
	int drawHeight = drawWidth;// * 3 / 4;

	glPushMatrix();

		glTranslated(subspaceGlobal.windowWidth-25, 0+25, 0);		
		
		//scale down map
		glScaled(1.0/16.0, 1.0/16.0, 1.0);
		
		//draw map
		//glTranslatef(-200, 0, 0);
		glTranslated(-drawWidth*2*(int)SubspaceMap::tileWidth, 0, 0);
		//energyBar_.displayFrame(0);

		Vector pos = player_->getPosition();

		if(map_)
		{
			glDisable(GL_TEXTURE_2D);


			//draw background
			glColor4d(0.05, 0.1, 0.05, 0.9);
			glBegin(GL_QUADS);
				glVertex3d(0, 0, 0);
				glVertex3d(drawWidth*2*16, 0, 0);
				glVertex3d(drawWidth*2*16, drawHeight*2*16, 0);
				glVertex3d(0, drawHeight*2*16, 0);
			glEnd();

			glEnable(GL_TEXTURE_2D);
			glColor4d(1.0, 1.0, 1.0, 1.0);		//TODO: change this stuff to use map variables
			//map_->setDrawRange(pos.x_/16-drawWidth,  pos.y_/16-drawHeight, drawWidth*2, drawHeight*2);
			int xstart = max(0, pos.x_/16-drawWidth);
			int ystart = max(0, pos.y_/16-drawHeight);
			int xend = max(0, pos.x_/16+drawWidth);
			int yend = max(0, pos.y_/16+drawHeight);

			map_->setDrawRange(xstart, ystart, xend, yend);
			//map_->setDrawRange(pos.x_/16-drawWidth,  pos.y_/16-drawHeight, pos.x_/16+drawWidth, pos.y_/16+drawHeight);

			//map_->setDrawInfo(0,  0, 1024, 1024);
			map_->draw();
			//map_->drawTile(384, 401);

			glDisable(GL_TEXTURE_2D);

			//draw border
			glColor4f(0.5, 0.5, 0.5, 1.0);
			glLineWidth(1);
			glBegin(GL_LINE_LOOP);
				glVertex3d(0, 0, 0);
				glVertex3d(drawWidth*2*16, 0, 0);
				glVertex3d(drawWidth*2*16, drawHeight*2*16, 0);
				glVertex3d(0, drawHeight*2*16, 0);
				glVertex3d(0, 0, 0);
			glEnd();

			//TODO: add dynamic display of objects
			//draw ship at center
			glColor4d(0.7, 0.7, 0.0, 1.0);
			glBegin(GL_QUADS);
				glVertex3d(drawWidth*16, drawHeight*16, 0);
				glVertex3d(drawWidth*16+32, drawHeight*16, 0);
				glVertex3d(drawWidth*16+32, drawHeight*16+32, 0);
				glVertex3d(drawWidth*16, drawHeight*16+32, 0);
			glEnd();

			glColor4d(1.0, 1.0, 1.0, 1.0);
		}

	glPopMatrix();
}*/

void SubspaceHud::drawRadar() const
{
	glPushMatrix();
		glTranslated(subspaceGlobal.windowWidth - (radar_.getDisplayWidth()+5), 0+5, 0);	//from lower left = 0,0

		glPushMatrix();
			radar_.draw();
		glPopMatrix();

		/*if(player_)
		{
			radar_.drawRadarMyPlayer(player_);
		}

		if(players_)
		{
			SubspacePlayerMap::const_iterator i;
			for(i = players_->begin(); i != players_->end(); ++i)
			{
				SubspacePlayer* p = (*i).second;
				if(p != player_)
				{
					if(p->getTeam() == player_->getTeam())
						radar_.drawRadarPlayer(p, true);
					else
						radar_.drawRadarPlayer(p, false);
				}
			}
		}*/
	glPopMatrix();


	/*glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();

		glEnable(GL_TEXTURE_2D);

		glTranslated(subspaceGlobal.windowWidth - (radarWidth_+5), 0+5, 0);	//from lower left = 0,0
		//glTranslated(100, 100, 0);
        	
		radarTexture_.enable();

		double xPos=0, yPos=0;
		Uint xt, yt;
		if(player_)
		{
			objectToTileCoords(player_->getPosition(), &xt, &yt);
			xPos = xt;
			yPos = yt;
			//xPos = floor(xPos);
			//yPos = floor(yPos);

			xPos = max(0, (double)xPos - radarDisplayWidth_/2.0);
			yPos = max(0, (double)yPos - radarDisplayHeight_/2.0);

			xPos = min(SubspaceMap::maxTileX-radarDisplayWidth_, xPos);
			yPos = min(SubspaceMap::maxTileY-radarDisplayHeight_, yPos);
		}

		double w = (double)SubspaceMap::maxTileX;
		double h = (double)SubspaceMap::maxTileY;

		radarX_ = xPos;
		radarY_ = yPos;
		
		radarTexture_.displayArea(xPos/w, yPos/h, radarDisplayWidth_/w, radarDisplayHeight_/h);

		glDisable(GL_TEXTURE_2D);

		//draw border
		glColor4f(0.5, 0.5, 0.5, 1.0);
		int lineWidth = 2;
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
			glVertex3d(-1, 0, 0);
			glVertex3d(radarWidth_ + 1, 0, 0);
			glVertex3d(radarWidth_ + 1, radarHeight_ + 1, 0);
			glVertex3d(-1, radarHeight_ + 1, 0);
			glVertex3d(-1, 0, 0);
		glEnd();*/

	glPopMatrix();
}

void SubspaceHud::drawRadarInfo() const
{
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

		Uint xPos, yPos;
		if(player_)
		{
			objectToTileCoords(player_->getSubspacePosition(), &xPos, &yPos);
		}

		xPos = min(1024, max(0, xPos));
		yPos = min(1024, max(0, yPos));

		//coordinates
		string coordStr;
		coordStr += 'A' + xPos/51;
		coordStr += itoa(1 + yPos/51);
		
		//time
		string timeStr = getLocalTime();

		string info;
		info = timeStr + "  " + coordStr;

		glTranslated(subspaceGlobal.windowWidth - info.length()*font_.getFrameWidth() - 5, radar_.getDisplayHeight()+4, 0);
		font_.displayString(info.c_str(), COLOR_Gray);

	glPopMatrix();
}

/*void SubspaceHud::drawRadarDynamic() const
{
	Color color;

	//draw player
	if(player_)
	{
		drawRadarMyPlayer(player_);
	}

	//draw players
	if(players_)
	{
		SubspacePlayerMap::iterator i;
		for(i = players_->begin(); i != players_->end(); ++i)
		{
			SubspacePlayer* p = (*i).second;
			assert(p);

			if(p != player_)
			{
				if(p->getTeam() == player_->getTeam())
					drawRadarPlayer(p, true);
				else
					drawRadarPlayer(p);	
			}
		}
	}

	//draw flags

	//draw greens
	//TODO: find a good way to do this
}*/

/*void SubspaceHud::drawDynamicRadarItem(const Vector& pos) const
{
	int radarDrawX = subspaceGlobal.windowWidth - radarWidth_ - 5;
	int radarDrawY = 5;
	
	Uint xt, yt;

	double w = radarWidth_ / (double)radarDisplayWidth_;
	double h = radarHeight_ / (double)radarDisplayHeight_;
	
	objectToTileCoords(pos, &xt, &yt);
	if(xt >= radarX_ && xt < radarX_+radarDisplayWidth_ &&
		yt >= radarY_ && yt < radarY_+radarDisplayHeight_)
	{
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_POINTS);
			glVertex3d(radarDrawX + (xt-radarX_)*w, radarDrawY + (yt-radarY_)*h, 0);
		glEnd();
	}
}

void SubspaceHud::drawRadarBall(const Vector& position) const
{
	Color color;

	if((Uint)(timer_/radarFlashTime_) % 2 == 0)
		color = Color(255/255.0, 255/255.0, 57/255.0);	//yellow
	else
		color = Color(255/255.0, 57/255.0, 74/255.0);		//red

	color.enable();
	glPointSize(2.0);

	drawDynamicRadarItem(position);
}

void SubspaceHud::drawRadarFlag(const Vector& position) const
{
	Color color;

	if((Uint)(timer_/radarFlashTime_) % 2 == 0)		
		color = Color(255/255.0, 57/255.0, 74/255.0);		//red
	else
		color = Color(255/255.0, 255/255.0, 57/255.0);	//yellow

	color.enable();
	glPointSize(2.0);

	drawDynamicRadarItem(position);
}

void SubspaceHud::drawRadarMyPlayer(SubspacePlayer* player) const
{
	assert(player);

	float pointSize = 2.0;
	Color color;

	if((Uint)(timer_/150) % 2 == 0)		//150 ms on/off
		color = Color(255/255.0, 255/255.0, 57/255.0, 1);
	else
		color = Color(255/255.0, 255/255.0, 57/255.0, 0);

	if(player->getFlags() > 0)
		pointSize = 3.0;

	glPointSize(pointSize);
	color.enable();
	
	drawDynamicRadarItem(player->getPosition());
}

void SubspaceHud::drawRadarPlayer(SubspacePlayer* player, bool myTeam) const
{
	float pointSize = 2.0;
	Color color;
					
	Uint targetBounty_ = 10;

	if(myTeam)
		color = Color(255/255.0, 255/255.0, 57/255.0);
	else
	{
		if(player->getBounty() > targetBounty_)	//TODO: add target bounty_;
			color = Color(165/255.0, 165/255.0, 255/255.0);
		else
			color = Color(82/255.0, 82/255.0, 214/255.0);

		if(player->getFlags() > 0)
			color = Color(255/255.0, 57/255.0, 74/255.0);
	}

	if(player->getFlags() > 0)
		pointSize = 3.0;

			
	glPointSize(pointSize);
	color.enable();

	drawDynamicRadarItem(player->getPosition());				
}

void SubspaceHud::drawRadarPrize(const Vector& position) const
{
	Color color = Color(90/255.0, 206/255.0, 74/255.0);
			
	glPointSize(2.0);
	color.enable();

	drawDynamicRadarItem(position);				
}*/

/*void SubspaceHud::drawMyPlayerInfo() const
{
	BasicTextBuffer text;
	
	text.setFont(font_);

	if(!player_)
		return;

	if(player_->isSpectator())
		return;

	if(!isSpriteOnScreen(*player_))
		return;

	SubspacePlayer* p = player_;

	//write energy
	double lifePercent = p->getEnergy()/(double)p->getMaxEnergy();

	if(lifePercent < 0.25)	//25% life
		text.write(itoa((int)p->getEnergy()) + "\n", COLOR_Orange);
	else if(lifePercent < 0.5)
		text.write(itoa((int)p->getEnergy()) + "\n", COLOR_Yellow);

	if(subspaceGlobal.nametagOption == NAMETAG_All)
	{
		text.write(p->getName() + "(" + itoa(p->getBounty()) + ")" + "\n", COLOR_Yellow);
	}
	text.flush();

	Vector screenPos;
	objectToScreenCoords(p->getPosition(), &screenPos);
	
	//draw text at lower right corner
	glPushMatrix();
		glTranslated(screenPos.x_, screenPos.y_, 0);
		glTranslated(p->getDisplayWidth()/2.0 - 4, -(double)p->getDisplayHeight()/2.0 + 4, 0);	//translate to bottom right of ship
		glTranslated(0, -text.getDisplayHeight(), 0);	//translate to bottom of text buffer

		const SubspaceBanner* banner = p->getBanner();
		if(banner)
		{
			banner->display();
			glTranslated(banner->getWidth(), 0, 0);
		}
		text.draw();
	glPopMatrix();
}	*/

void SubspaceHud::drawAllPlayersInfo() const
{
	if(subspaceGlobal.getVar("nametags") == NAMETAG_NoTags)
		return;

	SubspacePlayerMap::const_iterator i;
	for(i = players_->begin(); i != players_->end(); ++i)
	{
		if((*i).second != player_ ||									//not my player
			subspaceGlobal.getVar("nametags") == NAMETAG_All )	//or my player is drawn
		drawPlayerInfo((*i).second);	
	}
}

void SubspaceHud::drawPlayerInfo(SubspacePlayer* player) const
{
	BasicTextBuffer text;
	
	text.setFont(font_);

	if(!player)
		return;

	if(player->isSpectator() || player->isDead() || player->isTurret())
		return;

	if(!isSpriteOnScreen(*player))
		return;

	SubspacePlayer* p = player;

	Uchar color = COLOR_Blue;
	if(p->getTeam() == player_->getTeam())
		color = COLOR_Yellow;
	else if(p->getFlags() > 0)
		color = COLOR_Red;

	Vector screenPos;
	objectToScreenCoords(p->getPosition(), &screenPos);

	//draw text at lower right corner
	glPushMatrix();
		glTranslated(screenPos.x_, screenPos.y_, 0);
		glTranslated((double)p->getDisplayWidth()/2.0 - 4, -(double)p->getDisplayHeight()/2.0 - 4, 0);	//translate to bottom right of ship
		glTranslated(0, -text.getDisplayHeight(), 0);	//translate to bottom of text buffer

		//write energy
		if(player == player_)
		{
			double lifePercent = p->getEnergy()/(double)p->getMaxEnergy();

			if(lifePercent < 0.25)	//25% life
				font_.displayString((itoa((int)p->getEnergy()).c_str()), COLOR_Red);
			else if(lifePercent < 0.5)
				font_.displayString((itoa((int)p->getEnergy()).c_str()), COLOR_Yellow);				
			glTranslated(0, -(double)font_.getFontHeight(), 0);
		}

		//write player
		glPushMatrix();
			drawPlayerName(player, color);
		glPopMatrix();

		glTranslated(0, -(double)font_.getFontHeight(), 0);

		//write turrets
		const SubspacePlayerList& turrets = player->getTurrets();
		SubspacePlayerList::const_iterator i;
		for(i = turrets.begin(); i != turrets.end(); ++i)
		{
			glPushMatrix();
				drawPlayerName(*i, color);
			glPopMatrix();
		}
	glPopMatrix();
}

void SubspaceHud::drawPlayerName(SubspacePlayer* p, Uchar color) const
{
    string nameStr = p->getName() + "(" + itoa(p->getBounty());

	if(p->getFlags() > 0)
		nameStr += ":" + itoa(p->getFlags());
	nameStr += ")";

	const SubspaceBanner* banner = p->getBanner();
		
	if(banner->getWidth() > 0)
	{
		double vOffset = (font_.getFontHeight() - banner->getHeight())/2.0 + 1;
		glPushMatrix();
			glTranslated(0, vOffset, 0);
			banner->display();
		glPopMatrix();

		Uint size = banner->getWidth() / font_.getFontWidth() + 1;
		glTranslated(size * font_.getFontWidth(), 0, 0);
	}

	font_.displayString(nameStr.c_str(), color);
}

void SubspaceHud::drawSafetyTime() const
{
	if(!player_->isSafe() || player_->isSpectator())
		return;

	double safeTime = player_->getSafeTime();	//milliseconds

	string safetyString = "Safety Time Left: " + timeFormat(safeTime/1000, true, true, true);
	/*Uint hours = safeTime / (3600 * 1000);
	safeTime -= hours * 3600*1000;
	Uint minutes = safeTime / (60 * 1000);
	safeTime -= minutes * 60*1000;
	Uint seconds = safeTime / (1000);
	string safetyString = printfString("Safety Time Left: %s:%s:%s", itoaFixed(hours, 2).c_str(), itoaFixed(minutes, 2).c_str(), itoaFixed(seconds, 2).c_str());*/

	Vector screen;
	objectToScreenCoords(player_->getPosition(), &screen);
	glPushMatrix();
		glTranslated(screen.x_ + 5, screen.y_ + 35, 0);
		font_.displayString(safetyString.c_str(), COLOR_Yellow);

	glPopMatrix();
}

void SubspaceHud::drawMyPlayerIcons() const
{
	//TODO add global width and height data

	if(!player_)
		return;

	//draw item icons - left side
	int yStart = subspaceGlobal.windowHeight * 7/13 + 1;			//start 2/3 of the way up the screen

	glPushMatrix();
		glTranslated(0, yStart , 0);

		drawItemIcon(30, player_->getItems().bursts);	//draw bursts
		
		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(31, player_->getItems().repels);	//draw repels

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(40, player_->getItems().decoys);	//draw decoys

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(41, player_->getItems().thors);	//draw thors

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(42, player_->getItems().bricks);	//draw bricks

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(43, player_->getItems().rockets);	//draw rockets

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(46, player_->getItems().portals);	//draw portals

		/*drawItemIcon(30, 3);	//draw bursts
		
		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(31, 3);	//draw repels

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(40, 3);	//draw decoys

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(41, 3);	//draw thors

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(42, 3);	//draw bricks

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(43, 3);	//draw rockets

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(46, 3);	//draw portals*/
	glPopMatrix();

	//draw status icons - right side
	glPushMatrix();
		glTranslated(subspaceGlobal.windowWidth - icons_.getFrameWidth(), yStart , 0);

		drawGunIcon();
		
		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawBombIcon();

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawStealthIcon();

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawCloakIcon();

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawXRadarIcon();

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawAntiwarpIcon();
	glPopMatrix();
}

void SubspaceHud::drawStatus() const
{
	glPushMatrix();
		//TODO: make this use actual window sizes
		glTranslated(subspaceGlobal.windowWidth - statusIndicator_.getFrameWidth(), subspaceGlobal.windowHeight, 0);

		double energyFontExtra = 5;
		//draw main box
		glTranslated(0, -(double)(statusIndicator_.getFrameHeight() + energyFontExtra), 0);
		statusIndicator_.displayFrame(0);
		
		//draw bounty
		glPushMatrix();
			glTranslated(44, 6, 0);
			ledDigits_.displayNumber(player_->getBounty(), 4);
		glPopMatrix();

		//draw flags
		glPushMatrix();
			glTranslated(37, 18, 0);
			ledDigits_.displayNumber(player_->getFlags(), 2);
		glPopMatrix();

		//TODO: add team flags

		//draw freq
		glPushMatrix();
			glTranslated(44, 30, 0);
			ledDigits_.displayNumber(player_->getTeam(), 4);
		glPopMatrix();

		//draw energy value
		glPushMatrix();
			glTranslated(6, 49, 0);
			bigLedDigits_.displayNumber(player_->getEnergy(), 4);
		glPopMatrix();

	glPopMatrix();
}

void SubspaceHud::drawTimers() const
{
	/*drawTimer(0, 5000, shieldIcon_, COLOR_Gray);		//shields
	drawTimer(1, 5000, superIcon_, COLOR_Gray);		//super
	drawTimer(2, 5000, flagIcon_, COLOR_Red);		//flags
	drawTimer(3, 5000, portalIcon_, COLOR_Yellow);		//portal
	drawTimer(4, 5000, kothIcon_, COLOR_Yellow);		//koth*/

	//glTranslated(0, subspaceGlobal.windowHeight/2.0, 0);

	drawTimer(0, player_->getShieldsPercent(), shieldIcon_, COLOR_Yellow, true);		//shields
	drawTimer(1, player_->getSuperTime(), superIcon_, COLOR_Yellow);		//super
	drawTimer(2, player_->getFlagTime(), flagIcon_, COLOR_Red);		//flags
	drawTimer(3, player_->getPortalTime(), portalIcon_, COLOR_Yellow);		//portal
	drawTimer(4, player_->getKothTime(), kothIcon_, COLOR_Yellow);		//koth
}

void SubspaceHud::drawTimer(Uint position, double time, const TextureAnimated& icon, ColorType color, bool usePercent) const
{
	glPushMatrix();
		glTranslated(subspaceGlobal.windowWidth, (double)timerLineStart_, 0);
		glTranslated(0, -(double)(timerLineHeight_ + 1), 0);	//font is displayed from bottom->up
		glTranslated(0, -(double)(timerLineHeight_ + 1) * position, 0);

		//draw timer
		if(time > 0)
		{
			glTranslated(-(double)icon.getFrameWidth(), 0, 0);
			icon.display();

			string timeStr;
			if(usePercent)
				timeStr = ftoa(time * 10, 1) + "%";
			else
				timeStr = ftoa(time / 1000, 1);

			glTranslated(-(int)font_.getFrameWidth() * (int)timeStr.length(), 0, 0);
			font_.displayString(timeStr.c_str(), color);
		}
	glPopMatrix();
}

void SubspaceHud::drawIcon(int icon, int num, int shift) const
{
	//display the icon
	/*if(num > 0)
	{*/
		glPushMatrix();
			glTranslated((double)shift, 0, 0);
			icons_.displayFrame(icon);
		glPopMatrix();
	//}

	//move down and right for the digit
	glPushMatrix();
		glTranslated((double)(icons_.getFrameWidth()-2), (double)(icons_.getFrameHeight()-iconDigits_.getFrameHeight()) / 2.0, 0);

		if(num > 1 && num < 10)
			iconDigits_.displayFrame(num);
		else if(num >= 10)
			iconDigits_.displayFrame(10);
	glPopMatrix();
}

void SubspaceHud::drawItemIcon(int icon, int num) const
{
	if(num > 0)
		drawIcon(icon, num, 0);
	else
		drawIcon(icon, num, iconShiftLeft_);
}

void SubspaceHud::drawBlankIcon() const
{
	//draw blank icon here

	


}

void SubspaceHud::drawGunIcon() const
{
	int offset = 0;

	if(player_->getWeapons().guns.level == 0)
	{
		drawIcon(offset, 1, iconShiftRight_);
		return;
	}

	offset += player_->getWeapons().guns.level-1;

	if(player_->getItems().multifire)
	{
		offset += 6;

		if(player_->getWeapons().guns.multifire)
			offset -= 3;
	}

	if(player_->getWeapons().guns.bouncing)
		offset += 9;

	drawIcon(offset, 1);
}

void SubspaceHud::drawBombIcon() const
{
	int offset = 18;

	if(player_->getWeapons().bombs.level == 0)
	{
		drawIcon(offset, 1, iconShiftRight_);
		return;
	}

	offset += player_->getWeapons().bombs.level-1;

	if(player_->getItems().proximity)
		offset += 3;

	if(player_->getWeapons().bombs.shrapnel > 0)
		offset += 6;

	drawIcon(offset, 1);
}

void SubspaceHud::drawStealthIcon() const
{
	int offset = 32;

	if(!player_->getStatus().stealth)
		offset += 1;

	if(player_->getItems().stealth)
		drawIcon(offset, 1);
	else
		drawIcon(offset, 1, iconShiftRight_);
}

void SubspaceHud::drawCloakIcon() const
{
	int offset = 34;

	if(!player_->getStatus().cloak)
		offset += 1;

	if(player_->getItems().cloak)
		drawIcon(offset, 1);
	else
		drawIcon(offset, 1, iconShiftRight_);
}

void SubspaceHud::drawXRadarIcon() const
{
	int offset = 36;

	if(!player_->getStatus().xRadar)
		offset += 1;

	if(player_->getItems().xRadar)
		drawIcon(offset, 1);
	else
		drawIcon(offset, 1, iconShiftRight_);
}

void SubspaceHud::drawAntiwarpIcon() const
{
	int offset = 38;

	if(!player_->getStatus().antiwarp)
		offset += 1;

	if(player_->getItems().antiwarp)
		drawIcon(offset, 1);
	else
		drawIcon(offset, 1, iconShiftRight_);
}

bool SubspaceHud::isSpriteOnScreen(const Sprite& sprite) const
{
	//Rect screenRect(

	return true;
}

Uint SubspaceHud::getChatLineWidth() const
{
	double radarWidth = radar_.getDisplayWidth();//(Uint)(subspaceGlobal.windowWidth * 208.0/1280);
	double lineWidth = 0;

	if(showRadar_)
	{
		lineWidth = (subspaceGlobal.windowWidth-radarWidth)/(double)chatInput_.getFont().getFontWidth() - 1;
	}
	else
	{
		lineWidth = (subspaceGlobal.windowWidth)/(double)chatInput_.getFont().getFontWidth() - 1;
	}

	return (Uint)lineWidth;
}
