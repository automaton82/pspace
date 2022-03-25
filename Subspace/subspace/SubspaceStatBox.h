#ifndef _SUBSPACESTATBOX_H_
#define _SUBSPACESTATBOX_H_

#include <map>
using std::map;

#include "Timer.h"

#include "SubspaceSettings.h"
#include "SubspaceTextBox.h"

#include "SubspaceStatBoxModes.h"
#include "StatBoxPlayerSort.h"
#include "StatBoxPlayerSortPoints.h"
#include "StatBoxPointSort.h"
#include "StatBoxTeamSort.h"
#include "StatBoxPlayerSortExtended.h"
#include "StatBoxTeamStats.h"

class SubspacePlayer;

class SubspaceStatBox
{
private:

	enum StatBoxType
	{
		STATBOX_Off = 0,
		STATBOX_PlayerSort,
		STATBOX_PlayerSortPoints,
		STATBOX_PointSort,
		STATBOX_TeamSort,
		STATBOX_PlayerSortExtended,
		STATBOX_TeamStats,
		
		STATBOX_NumModes
	};

	typedef SubspaceStatBoxMode::SubspacePlayerMap SubspacePlayerMap;


public:
	SubspaceStatBox();
	virtual ~SubspaceStatBox();
	
	void init();

	// accessors
    double getDisplayWidth() const;
	double getDisplayHeight() const;

	Uint getNumPlayers() const;
	SubspacePlayer* getSelectedPlayer() const;
	int getSelectedTeamID() const;

	Uint size() const;

	// mutators
	void setBoxSize(Uint size);
	void setFont(const TextureFont& font);
	void setMode(Uint mode = 1);
	void setPlayers(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);

	void pageDown();
	void pageUp();
	void scrollDown();
	void scrollUp();
	void toggleMode();

	void draw() const;					//draws from upper left
	void update(double timestemp);		// for display timer

private:

	void clearModes();
	void tempDisplay();

	static const int defaultDisplayTime = 3000;

private: 

	TextureFont font_;
	TextureAnimated cursor_;

	StatBoxType currentMode_;
	SubspaceStatBoxMode* modes_[STATBOX_NumModes];

	Timer displayTimer_;
	bool tempDisplay_;

};

#endif