#ifndef _STATBOXPLAYERSORT_H_
#define _STATBOXPLAYERSORT_H_

#include <map>
using std::map;

#include "SubspaceStatBoxModes.h"

class StatBoxPlayerSort :
	public SubspaceStatBoxMode
{
protected:
	//typedef vector<SubspacePlayer*> PlayerList;
	//typedef map<Uint, SubspacePlayer*> SubspacePlayerMap;
	
public:
	StatBoxPlayerSort();
	virtual ~StatBoxPlayerSort();
	
	virtual void init(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);

	// accessors
	virtual double getDisplayWidth() const;
	virtual double getDisplayHeight() const;
	virtual Uint getNumPlayers() const;
	virtual SubspacePlayer* getSelectedPlayer() const;
	virtual Uint getSelectedTeamID() const;

	virtual Uint size() const;

	// mutators
	virtual void selectPlayer(SubspacePlayer* player);

	virtual void decreaseSize();
	virtual void increaseSize();
	virtual void pageDown();
	virtual void pageUp();
	virtual void scrollDown();
	virtual void scrollUp();
	
	///////////////////////////////////

	virtual void draw() const;		//draws from upper left

protected:

	void drawBanner(Uint index) const;
	void drawBanners() const;
	void drawCursor(Uint index) const;		// this includes spectator mode and king of the hill icons
	void drawCursors() const;

	Uint getCursorIcon(const SubspacePlayer* player, bool selected = false) const;
	double getLineHeight(Uint line) const;
	
	static string getNameString(const SubspacePlayer* player, bool showPoints = false, bool newline = true);
	static void getPlayerList(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer, PlayerList* players, bool excludeMyPlayer = false);
	static void getTeamList(const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer, Uint teamNum, PlayerList* team, PlayerList* others, bool excludeMyPlayer = false);

	static void playerSort(SubspaceTextBox& textBox, PlayerList& players, const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer, bool showPoints = false);

	static const Uint defaultStatBoxSize = 16;

protected: 

	SubspaceTextBox textBox_;

	Uint topDisplayIndex_, bottomDisplayIndex_;
	Uint selectedPlayerIndex_;
	Uint statBoxSize_;

	PlayerList players_;
	bool showBanners_;
	//SubspacePlayer* selectedPlayer_;
};

bool StatBox_playerNameSort(const SubspacePlayer& a, const SubspacePlayer& b);

#endif