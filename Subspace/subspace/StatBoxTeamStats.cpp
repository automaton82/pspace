#include "StatBoxTeamStats.h"

#include "AsciiUtil.h"
using namespace AsciiUtil;
#include "SubspacePlayer.h"

void StatBoxTeamStats::init(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer)
{	
	cursor_.load("graphics/spectate.bm2", false);
	cursor_.setFrames(1, 11);

	textBox_.setBackgroundColor(backgroundColor_);
	textBox_.setBorderColor(borderColor_);
	textBox_.setFont(font_);
	textBox_.setPadding(1.0, 0.0, 1.0, 1.0);

	textBox_.clear();
	players_.clear();
	teamStatistics(textBox_, players_, playerMap, myPlayer);
	
	topDisplayIndex_ = 0;
	bottomDisplayIndex_ = min(statBoxSize_-1, textBox_.size()-1);
	textBox_.setLinesDisplayed(statBoxSize_);
	//textBox_.etHeader(itoa(players_.size()), COLOR_Green);
}

SubspacePlayer* StatBoxTeamStats::getSelectedPlayer() const
{
	return 0;
}


void StatBoxTeamStats::selectPlayer(SubspacePlayer* player)
{
	Uint j;
	PlayerList::iterator i;
	for(i = players_.begin(), j = 0; i != players_.end(); ++i, ++j)
	{
		if((*i).getTeam() == player->getTeam())
			break;
	}

	if(j >= players_.size())
		selectedPlayerIndex_ = 0;
	else
		selectedPlayerIndex_ = j;
}

void StatBoxTeamStats::teamStatistics(SubspaceTextBox& box, PlayerList& players, const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer)
{
	Uint numTeams = 0;
	SubspacePlayer* p = 0;
		
	box.clear();
	players.clear();

	//initialize lists
	map<Uint, Uint> points, wins, losses, flags;
	map<Uint, vector<SubspacePlayer*> > teams;

	//gather team data
	SubspacePlayerMap::const_iterator i;
	for(i = playerMap.begin(); i != playerMap.end(); ++i)
	{
		p = (*i).second;
		
		Uint team = p->getTeam();

		//++teams[team];						//players on team
		teams[team].push_back(p);
		points[team] += p->getPoints();
		wins[team] += p->getWins();
		losses[team] += p->getLosses();
		flags[team] += p->getFlags();

		/*if(players.size() == 0 || p->getTeam() != players.back().getTeam())
			players.push_back(*p);*/
	}

	numTeams = (Uint)teams.size();

	//insert header
	box.clearHeader();
	box.writeHeader(" ");
	box.writeHeader("Freq", COLOR_Green);		
	box.writeHeader("   Points", COLOR_Green);
	box.writeHeader("   Win", COLOR_Green);
	box.writeHeader("  Lose", COLOR_Green);
	box.writeHeader("  Flag", COLOR_Green);
	
	Uint index = 0;
	Uchar color = COLOR_Yellow;

	Uint teamNum = 0;
	//write my team first
	if(myPlayer)
	{
		teamNum = myPlayer->getTeam();

		box.write(" ");	//cursor column
		box.write( fixedWidthString(itoa(teamNum), 4, leftJustified), color );				//team number
		box.write(	fixedWidthString(itoa(points[teamNum]), 9, rightJustified), color );	//team points
		box.write(	fixedWidthString(itoa(wins[teamNum]), 6, rightJustified), color );		//team wins
		box.write(	fixedWidthString(itoa(losses[teamNum]), 6, rightJustified), color );	//team losses
		box.write(	fixedWidthString(itoa(flags[teamNum]), 6, rightJustified), color );		//team flags

		players.push_back(*myPlayer);

		if(index++ < teams.size()-1)
			box.write("\n");
	}

	color = COLOR_Gray;

	//write other teams
    map<Uint, vector<SubspacePlayer*> >::const_iterator j;
	for(j = teams.begin(); j != teams.end(); ++j)
	{
		teamNum = (*j).first;

		if(myPlayer && teamNum == myPlayer->getTeam())
			continue;

		box.write(" ");	//cursor column
		box.write( fixedWidthString(itoa(teamNum), 4, leftJustified), color );				//team number
		box.write(	fixedWidthString(itoa(points[teamNum]), 9, rightJustified), color );	//team points
		box.write(	fixedWidthString(itoa(wins[teamNum]), 6, rightJustified), color );		//team wins
		box.write(	fixedWidthString(itoa(losses[teamNum]), 6, rightJustified), color );	//team losses
		box.write(	fixedWidthString(itoa(flags[teamNum]), 6, rightJustified), color );		//team flags

        if(index++ < teams.size()-1)
			box.write("\n");

		players.push_back(*teams[teamNum].front());
	}

	box.flush();
}
