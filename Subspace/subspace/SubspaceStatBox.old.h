#ifndef _SUBSPACESTATBOX_H_
#define _SUBSPACESTATBOX_H_

#include <map>
using std::map;

class SubspacePlayer;
#include "SubspaceSettings.h"
#include "SubspaceTextBox.h"

class SubspaceStatBox
{
private:

	typedef vector<SubspaceTextBox> PageList;
	typedef vector<SubspacePlayer*> PlayerList;
	typedef map<Uint, SubspacePlayer*> SubspacePlayerMap;
	typedef map<Uint, PlayerList> TeamMap;
	typedef vector<PlayerList> TeamList;
	
	enum StatBoxType
	{
		STATBOX_PlayerSort = 0,
		STATBOX_PlayerSortPoints,
		STATBOX_PointSort,
		STATBOX_TeamSort,
		STATBOX_PlayerSortExtended,
		STATBOX_TeamStats,
		
		STATBOX_NumPages
	};

public:
	SubspaceStatBox();
	virtual ~SubspaceStatBox();
	
	void init(const SubspacePlayerMap& players, SubspacePlayer* myPlayer);

	double getDisplayWidth() const;
	double getDisplayHeight() const;
	Uint getSelectedIndex() const;
	SubspacePlayer* getSelectedPlayer() const;
	void selectPlayerIndex(Uint index);
	void setBoxSize(Uint size);
	void setFont(const TextureFont& font);
	void setMode(Uint mode = 1);
	Uint size() const;
	
	void draw() const;		//draws from upper left

private:

	void drawBanner(Uint index) const;
	void drawBanners() const;
	void drawCursor(Uint index) const;
	void drawCursors() const;

	Uint getCursorIcon(SubspacePlayer* player, bool selected = false) const;
	double getLineHeight(Uint line) const;

	static void getPlayerList(const SubspacePlayerMap& playerMap, SubspacePlayer* myPlayer, PlayerList* players, bool excludeMyPlayer = false);
	static void getTeamList(const SubspacePlayerMap& playerMap, SubspacePlayer* myPlayer, Uint teamNum, PlayerList* team, PlayerList* others, bool excludeMyPlayer = false);

	Uint getPlayerIndex(Uint page, SubspacePlayer* player) const;
	Uint getPlayerTeamIndex(Uint page, SubspacePlayer* player) const;
	static string getNameString(SubspacePlayer* player, bool showPoints = false, bool newline = true);
	static string getStatsString(SubspacePlayer* player, bool newline = true);

    static void infoBoxPlayerSort(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer, bool showPoints = false);
	static void infoBoxPlayerPoints(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer);
	static void infoBoxPointSort(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer);
	void infoBoxTeamSort(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer);
	static void infoBoxPlayerExtendedSort(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer);
	static void infoBoxTeamStatistics(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, SubspacePlayer* myPlayer);

private: 

	TextureFont font_;
	TextureAnimated cursor_;

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
	Uint topPlayerIndex_;
	SubspacePlayer* lastSelectedPlayer_;
	Uint statBoxSize_;

	SubspaceTextBox pages_[numPages];
	PlayerList pagePlayers_[numPages];
	Uint lineIndexes_[numPages];
	TeamList teams_;					//specifically for teamsort
};

#endif