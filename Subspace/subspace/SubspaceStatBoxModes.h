#ifndef _SUBSPACESTATBOXMODES_H_
#define _SUBSPACESTATBOXMODES_H_

#include <map>
using std::map;

class SubspacePlayer;
#include "SubspaceSettings.h"
#include "SubspaceTextBox.h"

class SubspaceStatBoxMode
{
public:

	typedef vector<SubspacePlayer> PlayerList;
	typedef map<Uint, SubspacePlayer*> SubspacePlayerMap;

public:
	SubspaceStatBoxMode();
	virtual ~SubspaceStatBoxMode();
	
	virtual void init(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);

	// accessors
	virtual double getDisplayWidth() const = 0;
	virtual double getDisplayHeight() const = 0;

	virtual Uint getNumPlayers() const = 0;
	virtual SubspacePlayer* getSelectedPlayer() const = 0;
	virtual Uint getSelectedTeamID() const = 0;

	virtual Uint size() const = 0;

	// mutators
	virtual void selectPlayer(SubspacePlayer* player) = 0;
	void setBoxSize(Uint size);
	void setFont(const TextureFont& font);
	
	virtual void decreaseSize() = 0;
	virtual void increaseSize() = 0;
	virtual void pageDown() = 0;
	virtual void pageUp() = 0;
	virtual void scrollDown() = 0;
	virtual void scrollUp() = 0;

	virtual void draw() const = 0;		//draws from upper left

protected: 

	TextureFont font_;
	TextureAnimated cursor_;

	Color backgroundColor_;
	Color borderColor_;
	
    static const Uint bannerWidth = 2;
	static const Uint nameWidth = 12;
	static const Uint pointsWidth = 6;
	static const Uint squadWidth = 10;

	static const Uchar teamColor = COLOR_Yellow;
	static const Uchar statColor = COLOR_Gray;

	Uint statBoxSize_;
};

#endif