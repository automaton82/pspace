//David Millman

#ifndef _SUBSPACEHUD_H_
#define _SUBSPACEHUD_H_

#include <list>

using std::list;

#include "TextureFont.h"
#include "TextureDigits.h"

#include "GameHud.h"
#include "ChatMessage.h"
#include "SubspaceChat.h"
#include "SubspaceHelp.h"
#include "SubspaceMap.h"
#include "SubspaceLifeBar.h"
#include "SubspacePlayer.h"
#include "SubspacePlayerDatabase.h"
#include "SubspaceRadar.h"
#include "SubspaceSettings.h"
#include "SubspaceStatBox.h"

#include "TextBox.h"
#include "TimedTextBox.h"
#include "TextTable.h"


//TODO: make this class store almost no information

class SubspaceHud :
	public GameHud
{
public:
	typedef list<ChatMessage> ChatList;
	typedef map<Uint, SubspacePlayer*> SubspacePlayerMap;

public:
	SubspaceHud();

	void init();
	void resize();

	/*****************
	 * Chat Messages *
	 *****************/

	//void addChatMessage(const ChatMessage& chatMessage);
	void addSystemMessage(const string& message, int color = 0);

	//void setChatSize(Uint size);
	//void setNameWidth(Uint size);
	//void setNumChatLines(Uint size);

	void setChat(SubspaceChat* chat);
												//TODO: make this a stand-alone chat buffer
	void setChatBuffer(string* chatBuffer);		//only keeps pointer to buffer
	//void setChatList(ChatList* chatList);		//only keeps pointer to list

	void setMessageLines(Uint lines);

	/******************
	 * Info Box Stuff *
	 ******************/

	void scrollUp();
	void scrollDown();
	void scrollUpFast();
	void scrollDownFast();

	void setStatboxSize(Uint size);
	SubspacePlayer* getSelectedPlayer() const;

	void refreshStatBox();

	//TODO: make these adjust by max number of points
	//void setPlayerDatabase(SubspacePlayerDatabase* players);
	void setPlayerMap(SubspacePlayerMap* playerMap);
	void initInfoBox(Uint mode = 1);
	void infoBoxPlayerSort(bool showPoints = false);
	void infoBoxPlayerPoints();
	void infoBoxPointSort();
	void infoBoxTeamSort();
	void infoBoxPlayerExtendedSort();
	void infoBoxTeamStatistics();

	typedef vector<SubspacePlayer*> PlayerList;
	void getPlayerList(PlayerList* players);
	void getTeamList(Uint teamNum, PlayerList* team, PlayerList* others, bool excludeMyPlayer = false);

	/******************
	 * Help Page Stuff *
	 ******************/

	void toggleHelpPage();

	/*******************
	 * Map Information *
	 *******************/

    void setMap(SubspaceMap* map);
	void initRadar();
    //void createRadar(Uint width, Uint height);	//texture size in width/height
	const SubspaceRadar* getRadar() const;

	void setShowRadar(bool b);
	void toggleRadar();

	/**************
	 * Menu Stuff *
	 **************/

	void toggleNametag();

	/**********************
	 * Player Information *
	 **********************/
	void setPlayer(SubspacePlayer* player);

	/**********
	 * Update *
	 **********/

	void update(double timeStep);

	/***********
	 * Display *
	 ***********/
    		
	void draw() const;

	//void drawChat() const;		//TODO: clean up this huge ass function
	void drawChat2() const;
		void drawSystemChat() const;
	void drawHelpBox() const;
	void drawInfoBox() const;
	void drawLifeBar() const;

	void drawTimers() const;
	void drawTimer(Uint position, double time, const TextureAnimated& icon, ColorType color, bool usePercent = false) const;
	
	//TODO: this shouldn't re-render a map, find a better way - render to texture?
	//void drawMiniMap() const;	//TODO: allow for the mini map to show dynamic objects, needs lists of objects
	void drawRadar() const;
	void drawRadarInfo() const;
	
	/*void drawRadarDynamic() const;	//dynamic radar items, e.g. players, greens, balls, flags
	void drawDynamicRadarItem(const Vector& position) const;

	void drawRadarBall(const Vector& position) const;
	void drawRadarFlag(const Vector& position) const;
	void drawRadarMyPlayer(SubspacePlayer* player) const;
	void drawRadarPlayer(SubspacePlayer* player, bool myTeam = false) const;
	void drawRadarPrize(const Vector& position) const;*/

	//void drawMyPlayerInfo() const;
	void drawPlayerInfo(SubspacePlayer* player) const;
	void drawPlayerName(SubspacePlayer* player, Uchar color) const;
	void drawAllPlayersInfo() const;	//not implemented yet
	void drawSafetyTime() const;

	void drawMyPlayerIcons() const;
	void drawStatus() const;	//energy, freq, flags, bounty	

	//TODO: make the icons slide in when the player gets the item
	void drawIcon(int icon, int num, int shift = 0) const;	
	void drawItemIcon(int icon, int num) const;

	void drawBlankIcon() const;
	void drawGunIcon() const;				
	void drawBombIcon() const;
	void drawStealthIcon() const;
	void drawCloakIcon() const;
	void drawXRadarIcon() const;
	void drawAntiwarpIcon() const;

private:

	Uint getChatLineWidth() const;
	bool isSpriteOnScreen(const Sprite& sprite) const;	//TODO: actually implement this

private:
	
	//player info
	SubspacePlayer* player_;

	TextureAnimated icons_;
	TextureDigits iconDigits_;

	int iconShiftRight_, iconShiftLeft_;	//see just the ends of the icon

	TextureAnimated energyBar_;
	SubspaceLifeBar lifeBar_;
	TextureAnimated statusIndicator_;	//not actually animated, but could potentially be - energy, bounty, flags, etc.
	TextureDigits ledDigits_;
	TextureDigits bigLedDigits_;

	int timerLineHeight_, timerLineStart_;

	TextureAnimated shieldIcon_;
	TextureAnimated superIcon_;
	TextureAnimated flagIcon_;
	TextureAnimated portalIcon_;
	TextureAnimated kothIcon_;

	//info box
	//SubspacePlayerDatabase* players_;
	SubspacePlayerMap* players_;

    TextTable infoBox_;
	bool isInfoBoxActive_;
	SubspaceStatBox statBox_;	//info box - contains player statistics, points, etc.
	Uint selectedIndex_;

	SubspaceHelp helpBox_;
	Uint helpPage_;

	//map info
	SubspaceMap* map_;

	//radar
	Uint radarDisplayWidth_, radarDisplayHeight_;
	SubspaceRadar radar_;
	bool showRadar_;

	//Chat 
	SubspaceChat* chat_;
	SubspaceChat chatInput_;
	
	//Uint lineWidth_; 
	Uint messageLines_;
	Uint inputLines_;

	string* chatBuffer_;

	//system messages


	Uint statBoxWidth_;
	TimedTextBox statusBox_;

	//TextBox chatBox_;

	TextureFont font_;	
};

#endif