#ifndef _SUBSPACEHUD_H_
#define _SUBSPACEHUD_H_

#include <list>

using std::list;

#include "GameHud.h"

#include "Sprite.h"
#include "TextBox.h"
#include "TextTable.h"
#include "TextureFont.h"
#include "TextureDigits.h"
#include "TimedTextBox.h"

#include "SubspaceHudCommandReceiver.h"

#include "ChatMessage.h"
#include "SubspaceLifeBar.h"
#include "SubspaceLVZData.h"
#include "SubspaceRadar.h"
#include "SubspaceSettings.h"
//#include "SubspaceStatBox.h"

class SubspaceChat;
class SubspaceConsole;
class SubspaceHelp;
class SubspaceLVZManager;
class SubspaceMap;
class SubspaceMenu;
class SubspacePlayer;
class SubspaceRadar;
class SubspaceStatBox;
class SubspaceZone;


//TODO: make this class store almost no information

class SubspaceHud :
	public GameHud,
	public SubspaceHudCommandReceiver
{
public:
	typedef list<ChatMessage> ChatList;
	typedef map<Uint, SubspacePlayer*> SubspacePlayerMap;

	static const double lifeBarScreenWidthRatio_;
	static const double radarScreenWidthRatio_;
	static const double radarScreenHeightRatio_;	
	static const double bigRadarScreenWidthRatio_;
	static const double bigRadarScreenHeightRatio_;	
	static const double systemMessageScreenWidthRatio_;
	static const double systemMessageScreenHeightRatio_;


	static const int maxNameLength_ = 24;
	static const int maxChatLines_ = 32;
	static const int maxStatBoxSize_ = 32;
	static const int minStatBoxSize_ = 2;

	static const int NUM_ITEMS = 7;				// total number of icons for items (repels, rockets, etc.)
	static const int NUM_WEAPONS = 6;			// total number of icons for weapons (guns, bombs, etc.)

	struct offsetData
	{
		offsetData() : x(0), y(0)
		{}

		double x, y;
	};

public:
	SubspaceHud();
	~SubspaceHud();

	void init();		// loads textures, etc.
	void resize();

	// Object views
	void setConsole(SubspaceConsole* console);			// chat buffer
	void setChat(SubspaceChat* chat);
	void setHelp(SubspaceHelp* help);
	void setLVZ(SubspaceLVZManager* lvzManager);
	void setMap(const SubspaceMap* map);				// makes radar
	void setMenu(SubspaceMenu* menu);			
	void setMyPlayer(const SubspacePlayer* player);		// main player stats
	void setPlayers(const SubspacePlayerMap* players);	// all players nametags	
	void setStatBox(SubspaceStatBox* statBox);
	void setZone(SubspaceZone* zone);

	double getLVZScreenCoordX(LVZScreenType type) const;
	double getLVZScreenCoordY(LVZScreenType type) const;

	// Chat operations
	void setNameWidth(int width);
	void setChatLines(int lines);

	// Help page operations
	virtual void doToggleHelp();
	virtual void doToggleHelpTicker();	

	// Menu operations
	virtual void doToggleNametags();
	virtual void doToggleRadar();
	NametagModeType getNametagMode() const;

	// Message operations
	void addSystemMessage(const string& msg, int color = 0);

	// Radar operations
	bool isRadarOn() const;
	int getRadarTargetBounty() const;
	void setRadarTargetBounty(int bounty);
	void setShowRadar(bool isOn);

	// Statbox operations
	void decreaseStatboxLines(int num = 1);
	void increaseStatboxLines(int num = 1);
	virtual void doDecreaseStatBoxSize();
	virtual void doIncreaseStatBoxSize();
	virtual void doScrollDown();
	virtual void doScrollDownFast();
	virtual void doScrollUp();
	virtual void doScrollUpFast();
	virtual void doToggleStatBox();
	virtual void doToggleBigRadar();
	
	//////////////////////////////////////
	void update(double timestep);
	void draw() const;

private:

	void calcOffsets();
	void calcRadarOffsets(const SubspaceRadar& radar);

	// Chat
	
	// Radar
	void initRadar();
	void initBigRadar();

	//
	void updateLifebar(double timestep);

	// Drawing functions
	// hud layout
	void drawChat() const;			// draws chat and console
	void drawHelpBox() const;
	void drawLifeBar() const;
	void drawMyPlayerIcons() const;
	void drawPlayerStatus() const;	//energy, freq, flags, bounty	
	void drawRadar() const;		//TODO: this shouldn't re-render a map, find a better way - render to texture?
	void drawRadarInfo() const;
	void drawStatBox() const;
	void drawSystemMessages() const;
	void drawTimers() const;
	void drawTimer(Uint position, double time, const TextureAnimated& icon, ColorType color, bool usePercent = false) const;

	// dynamic hud drawing
	void drawAllPlayersInfo() const;		
	void drawPlayerInfo(SubspacePlayer* player) const;
	void drawPlayerNametag(SubspacePlayer* player, Uchar color) const;
	void drawSafetyTime() const;
	
	//TODO: make the icons slide in when the player gets the item
	void drawIcon(int icon, int num, int shift = 0) const;	
	void drawItemIcon(int icon, int num) const;

	void drawAntiwarpIcon() const;
	void drawBombIcon() const;
	void drawBlankIcon() const;
	void drawCloakIcon() const;
	void drawGunIcon() const;
	void drawStealthIcon() const;
	void drawXRadarIcon() const;

	bool isSpriteOnScreen(const Sprite& sprite) const;	//TODO: actually implement this

private:
	TextureFont font_;

	offsetData offsetItems_;			// repels, rockets, etc.
	offsetData offsetLifebar_;
	offsetData offsetRadar_;
	offsetData offsetRadarText_;
	offsetData offsetStatbox_;
	offsetData offsetWeapons_;	

	// view objects
	SubspaceConsole* console_;
	SubspaceChat* chat_;
	SubspaceHelp* help_;
	const SubspaceMap* map_;
	SubspaceMenu* menu_;
	const SubspacePlayer* myPlayer_;
	const SubspacePlayerMap* players_;
	SubspaceStatBox* statBox_;		//statistics box - contains player statistics, points, etc.
	SubspaceZone* zone_;	

	// help
	int helpPage_;

	// icons
	TextureAnimated icons_;
	TextureDigits iconDigits_;
	int iconShiftRight_, iconShiftLeft_;	//see just the ends of the icon

	TextureAnimated energyBar_;
	SubspaceLifeBar lifeBar_;
	TextureAnimated statusIndicator_;	//not actually animated, but could potentially be - energy, bounty, flags, etc.
	TextureDigits ledDigits_;
	TextureDigits bigLedDigits_;

	TextureAnimated shieldIcon_;
	TextureAnimated superIcon_;
	TextureAnimated flagIcon_;
	TextureAnimated portalIcon_;
	TextureAnimated kothIcon_;

	// lvz
	SubspaceLVZManager* lvzManager_;

	// menu
	NametagModeType nametagMode_;

	// radar
	string mapFilename_;
	SubspaceRadar radar_, bigRadar_;	
	int radarDisplayWidth_, radarDisplayHeight_;
	int bigRadarDisplayWidth_, bigRadarDisplayHeight_;
	bool showRadar_;
	SubspaceRadar* currentRadar_;

	// system messages
	TimedTextBox systemMessageBox_;
	int systemMessageBoxWidth_;
    
	// timers
	int timerLineHeight_, timerLineStart_;
};

#endif