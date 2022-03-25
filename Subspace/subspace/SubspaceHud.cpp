#include "SubspaceHud.h"

#include <algorithm>

#include <map>
using std::map;

#include <vector>
using std::vector;

#include "BasicTextBuffer.h"
#include "WinUtil.h"	

#include "SubspaceGlobal.h"
#include "SubspaceSettings.h"
#include "SubspaceUtil.h"

#include "SubspaceChat.h"
#include "SubspaceConsole.h"
#include "SubspaceHelp.h"
#include "SubspaceLifeBar.h"
#include "SubspaceMenu.h"
#include "SubspaceMap.h"
#include "SubspacePlayer.h"
#include "SubspaceRadar.h"
#include "SubspaceStatBox.h"
#include "SubspaceZone.h"


const double SubspaceHud::lifeBarScreenWidthRatio_ = 147.0 / 800.0;
const double SubspaceHud::radarScreenWidthRatio_ = 208.0 / 1280.0;
const double SubspaceHud::radarScreenHeightRatio_ = 208.0 / 1024.0;	
const double SubspaceHud::bigRadarScreenWidthRatio_ = 620.0 / 1280.0;
const double SubspaceHud::bigRadarScreenHeightRatio_ = 560.0 / 1024.0;
const double SubspaceHud::systemMessageScreenWidthRatio_ = 1.0 / 5.0;
const double SubspaceHud::systemMessageScreenHeightRatio_ = 3.0 / 7.0;	

SubspaceHud::SubspaceHud() :
	chat_(0),
	console_(0),	
	help_(0),
	lvzManager_(0),
	map_(0),
	menu_(0),
	myPlayer_(0),
	statBox_(0),
	zone_(0),		

	lifeBar_(50, 10),
	systemMessageBox_(systemMessageBoxWidth_, 4, 3000),

	showRadar_(true),
	currentRadar_(&radar_)
{

	//statBox_ = new SubspaceStatBox();
}

SubspaceHud::~SubspaceHud()
{
	//delete statBox_;
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

	lifeBar_.init();

	//initInfoBox();
	//statBox_->setFont(font_);
	//statBox_->init(*players_, myPlayer_);
	//isInfoBoxActive_ = false;

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
	//timerLineStart_ = subspaceGlobal.getWindowHeight() - statusIndicator_.getFrameHeight() - 5;

	//timer_ = 0;
	//radarFlashTime_ = 150;		//milliseconds

	resize();

	/*chatInput_.setFont(font_);
	chatInput_.setLineWidth(getChatLineWidth());
	chatInput_.setLinesDisplayed(10);
	chatInput_.setLineOffset(0);*/

	//helpBox_.setFont(font_);
	/*string helpfile = "config/Commands.hlp";
	if(!help_->load(helpfile))
	{
		//debugout << "Failed to load help file \"" << helpfile << "\"" << endl;
		printf("Failed to load help file \"%s\"\n", helpfile.c_str());
	}
	else
	{
		//debugout << "Loaded help file \"" << helpfile << "\"" << endl;
		printf("Loaded help file \"%s\"\n", helpfile.c_str());
	}*/
}

void SubspaceHud::resize()
{
	this->setOrthoMode(0, 0, subspaceGlobal.getWindowWidth(), subspaceGlobal.getWindowHeight());

	lifeBar_.setWidth(subspaceGlobal.getWindowWidth() * lifeBarScreenWidthRatio_);	//TODO: find exact ratio
	//lifeBar_.setWidth(147);

	timerLineStart_ = subspaceGlobal.getWindowHeight() - statusIndicator_.getFrameHeight() - 5;

	initRadar();
	initBigRadar();

	calcOffsets();
}

void SubspaceHud::setConsole(SubspaceConsole* console)
{
	console_ = console;
}

void SubspaceHud::setChat(SubspaceChat* chat)
{
	chat_ = chat;
}

void SubspaceHud::setHelp(SubspaceHelp* help)
{
	help_ = help;
}

void SubspaceHud::setLVZ(SubspaceLVZManager* lvzManager)
{
	lvzManager_ = lvzManager;
}

void SubspaceHud::setMap(const SubspaceMap* map)
{
	if(!map || map->getWidth() == 0 || map->getHeight() == 0)
		return;

    if(map_ != map ||
		mapFilename_ != map->getMapFilename())
	{
		map_ = map;
		mapFilename_ = map->getMapFilename();
		initRadar();
		initBigRadar();
		calcRadarOffsets(*currentRadar_);
	}

}

void SubspaceHud::setMenu(SubspaceMenu* menu)
{
	menu_ = menu;
}

void SubspaceHud::setMyPlayer(const SubspacePlayer* player)
{
	//assert(player);

	myPlayer_ = player;	
}

void SubspaceHud::setPlayers(const SubspacePlayerMap* players)
{
	if(players)
	{
		players_ = players;
		if(statBox_->getNumPlayers() != players->size())
		{
			statBox_->setPlayers(*players_, myPlayer_);
		}
	}
	else
	{
		SubspacePlayerMap blank;
		statBox_->setPlayers(blank, 0);
	}
}

void SubspaceHud::setStatBox(SubspaceStatBox* statBox)
{
	statBox_ = statBox;
}

void SubspaceHud::setZone(SubspaceZone* zone)
{
	zone_ = zone;
}

double SubspaceHud::getLVZScreenCoordX(LVZScreenType type) const
{
	// note: bottom left of screen is 0,0
	switch(type)
	{
	case LVZ_SCREEN_Normal:
		return 0;
	case LVZ_SCREEN_Center:			
		return subspaceGlobal.getWindowWidth()/2.0;	
	case LVZ_SCREEN_BottomRight:	
		return subspaceGlobal.getWindowWidth();		
	case LVZ_SCREEN_StatBoxLowerRight:
		return offsetStatbox_.x + statBox_->getDisplayWidth();		
	case LVZ_SCREEN_SpecialsTopRight:
		return offsetItems_.x + icons_.getFrameWidth();		
	case LVZ_SCREEN_SpecialsBottomRight:
		return offsetItems_.x + icons_.getFrameWidth();		
	case LVZ_SCREEN_BelowEnergy:
		return subspaceGlobal.getWindowWidth()/2.0;		
	case LVZ_SCREEN_ChatTopLeft:
		return 0;		
	case LVZ_SCREEN_RadarTopLeft:
		return offsetRadar_.x;		
	case LVZ_SCREEN_RadarTextTopLeft:
		return offsetRadarText_.x;		
	case LVZ_SCREEN_WeaponTopLeft:
		return offsetWeapons_.x;		
	case LVZ_SCREEN_WeaponBottomLeft:
		return offsetWeapons_.x;
	}

	return 0;	
}

double SubspaceHud::getLVZScreenCoordY(LVZScreenType type) const
{
	// note: bottom left of screen is 0,0
	switch(type)
	{
	case LVZ_SCREEN_Normal:
		return subspaceGlobal.getWindowHeight();
	case LVZ_SCREEN_Center:
		return subspaceGlobal.getWindowHeight()/2.0;		
	case LVZ_SCREEN_BottomRight:
		return 0;
	case LVZ_SCREEN_StatBoxLowerRight:
		return offsetStatbox_.y - statBox_->getDisplayWidth();
	case LVZ_SCREEN_SpecialsTopRight:
		return offsetItems_.y;
	case LVZ_SCREEN_SpecialsBottomRight:
		return offsetItems_.y - icons_.getFrameHeight()*NUM_ITEMS;
	case LVZ_SCREEN_BelowEnergy:
		return offsetLifebar_.y;
	case LVZ_SCREEN_ChatTopLeft:
		return chat_->getDisplayHeight();
	case LVZ_SCREEN_RadarTopLeft:
		return offsetRadar_.y + radar_.getDisplayHeight();
	case LVZ_SCREEN_RadarTextTopLeft:
		return offsetRadarText_.y + font_.getFontHeight();
	case LVZ_SCREEN_WeaponTopLeft:
		return offsetWeapons_.y;
	case LVZ_SCREEN_WeaponBottomLeft:
		return offsetWeapons_.y - icons_.getFrameHeight()*NUM_WEAPONS;
	}

	return subspaceGlobal.getWindowHeight();
}


void SubspaceHud::setNameWidth(int width)
{
	width = max(1, min(maxNameLength_, width));
	chat_->setHeaderWidth(width);
}

void SubspaceHud::setChatLines(int lines)
{
	lines = max(0, min(maxChatLines_, lines));
	chat_->setLinesDisplayed(lines);
}

void SubspaceHud::doToggleHelp()
{
	if(help_)
		help_->nextPage();
}

void SubspaceHud::doToggleHelpTicker()
{
}

void SubspaceHud::doToggleNametags()
{
	nametagMode_ = (NametagModeType)(nametagMode_ + 1);
	if(nametagMode_ >= NAMETAG_Unknown)
		nametagMode_ = NAMETAG_NoTags;
}

void SubspaceHud::doToggleRadar()
{
	showRadar_ = !showRadar_;

	double radarWidth = 0;
	if(showRadar_)
		radarWidth = radar_.getDisplayWidth();

	int chatWidth = (subspaceGlobal.getWindowWidth() - radarWidth) / font_.getFontWidth() - 1;
	chat_->setLineWidth(chatWidth);
	console_->setLineWidth(chatWidth);
	// TODO: add changes to chat width
}

NametagModeType SubspaceHud::getNametagMode() const
{
	return nametagMode_;
}

void SubspaceHud::addSystemMessage(const string& msg, int color)
{
	systemMessageBox_.setLineHeader("", 0);
	systemMessageBox_.addLine(msg, color, false);	//don't need to parse
}

bool SubspaceHud::isRadarOn() const
{
	return showRadar_;
}

int SubspaceHud::getRadarTargetBounty() const
{
	return radar_.getTargetBounty();
}

void SubspaceHud::setRadarTargetBounty(int bounty)
{
	radar_.setTargetBounty(bounty);
}

void SubspaceHud::setShowRadar(bool isOn)
{
	showRadar_ = isOn;
}

void SubspaceHud::decreaseStatboxLines(int lines)
{
	// TODO: implement this
}

void SubspaceHud::increaseStatboxLines(int lines)
{
	// TODO: implement this
}

void SubspaceHud::doDecreaseStatBoxSize()
{
	if(!statBox_)
		return;

	Uint size = statBox_->size();
	if(size >= minStatBoxSize_+1)
		--size;

	statBox_->setBoxSize(size);
}

void SubspaceHud::doIncreaseStatBoxSize()
{
	if(!statBox_)
		return;

	Uint size = statBox_->size();
	if(size+1 <= maxStatBoxSize_)
		++size;

	statBox_->setBoxSize(size);
}

void SubspaceHud::doScrollDown()
{
	statBox_->scrollDown();
}

void SubspaceHud::doScrollDownFast()
{
	statBox_->pageDown();
}

void SubspaceHud::doScrollUp()
{
	statBox_->scrollUp();
}

void SubspaceHud::doScrollUpFast()
{
	statBox_->pageUp();
}

void SubspaceHud::doToggleStatBox()
{
	statBox_->toggleMode();
}

void SubspaceHud::doToggleBigRadar()
{
	printf("OFFSETS (%f,%f),(%f,%f)\n", offsetRadar_.x, offsetRadar_.y, offsetRadarText_.x, offsetRadarText_.y);

	if(currentRadar_ == &radar_)
	{
		printf("BIG RADAR\n");
		currentRadar_ = &bigRadar_;
	}
	else
	{
		printf("SMALL RADAR\n");
		currentRadar_ = &radar_;
	}
	
	calcRadarOffsets(*currentRadar_);

	printf("OFFSETS (%f,%f),(%f,%f)\n", offsetRadar_.x, offsetRadar_.y, offsetRadarText_.x, offsetRadarText_.y);
}

void SubspaceHud::update(double timestep)
{
	GameHud::update(timestep);

	systemMessageBox_.update(timestep);

	updateLifebar(timestep);

	//update flag timers
	shieldIcon_.advanceFrame(timestep);
	superIcon_.advanceFrame(timestep);
	flagIcon_.advanceFrame(timestep);
	portalIcon_.advanceFrame(timestep);
	kothIcon_.advanceFrame(timestep);

	//radar
	Uint tx = 0, ty = 0;
	if(myPlayer_)
		objectToTileCoords(myPlayer_->getPosition(), &tx, &ty);

	currentRadar_->update(timestep);	

	bigRadar_.setDisplayArea(tx, ty, bigRadarDisplayWidth_, bigRadarDisplayHeight_);	

	radar_.setDisplayArea(tx, ty, radarDisplayWidth_, radarDisplayHeight_);	
}

void SubspaceHud::draw() const
{
	// TODO: merge hud and zone drawing

	if(lvzManager_)
		lvzManager_->drawScreenObjects(LVZ_LAYER_BelowAll);

	if(lvzManager_)
		lvzManager_->drawScreenObjects(LVZ_LAYER_AfterBackground);

	if(lvzManager_)
		lvzManager_->drawScreenObjects(LVZ_LAYER_AfterTiles);

	if(lvzManager_)
		lvzManager_->drawScreenObjects(LVZ_LAYER_AfterShips);

	if(lvzManager_)
		lvzManager_->drawScreenObjects(LVZ_LAYER_AfterWeapons);

	//TODO: all these pushes and pops shouldn't be necessary, just precautionary, take them out later
	Color color(1.0, 1.0, 1.0, 1.0);
	if(myPlayer_)
	{
		glPushMatrix();
			//drawMyPlayerInfo();
			drawAllPlayersInfo();
		glPopMatrix();

		glPushMatrix();
			drawSafetyTime();
		glPopMatrix();

		if(!myPlayer_->isSpectator())
		{
			glColor4d(color.r_, color.b_, color.g_, color.a_);
			glPushMatrix();
    			drawPlayerStatus();
			glPopMatrix();
		
			glColor4d(color.r_, color.b_, color.g_, color.a_);
			glPushMatrix();
				drawLifeBar();
			glPopMatrix();
		}

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
			drawStatBox();	//player names, etc.
		glPopMatrix();

		glPushMatrix();
			drawHelpBox();	//player names, etc.
		glPopMatrix();
		glColor4d(1.0, 1.0, 1.0, 1.0);

		glPushMatrix();
        	drawTimers();	//flags, shields, etc.
		glPopMatrix();
	}

	if(lvzManager_)
		lvzManager_->drawScreenObjects(LVZ_LAYER_AfterGauges);

	glPushMatrix();
		drawChat();		//should be displayed over everything else
	glPopMatrix();
	
	glPushMatrix();
		drawSystemMessages();
	glPopMatrix();

	if(lvzManager_)
		lvzManager_->drawScreenObjects(LVZ_LAYER_AfterChat);

	if(lvzManager_)
		lvzManager_->drawScreenObjects(LVZ_LAYER_TopMost);

	/*if(lvzManager_)
	{
		glPushMatrix();
			lvzManager_->drawScreenObjects(LVZ_LAYER_AfterBackground);
			lvzManager_->drawScreenObjects(LVZ_LAYER_AfterChat);
			lvzManager_->drawScreenObjects(LVZ_LAYER_AfterGauges);
			lvzManager_->drawScreenObjects(LVZ_LAYER_AfterShips);
			lvzManager_->drawScreenObjects(LVZ_LAYER_AfterTiles);
			lvzManager_->drawScreenObjects(LVZ_LAYER_AfterWeapons);
			lvzManager_->drawScreenObjects(LVZ_LAYER_BelowAll);
			lvzManager_->drawScreenObjects(LVZ_LAYER_TopMost);
		glPopMatrix();
	}*/
}

void SubspaceHud::calcOffsets()
{
	// note: most of these coordinates are the lower left corner of the object

	offsetItems_.x = 0;
	offsetItems_.y = subspaceGlobal.getWindowHeight() * 7.0/13.0 + 1;			//start 7/13 of the way up the screen
	
	offsetLifebar_.x = subspaceGlobal.getWindowWidth()/2.0 - energyBar_.getFrameWidth()/2.0;
	offsetLifebar_.y = subspaceGlobal.getWindowHeight() - energyBar_.getFrameHeight();

	printf("calcOffsets\n");
	calcRadarOffsets(*currentRadar_);
		
	offsetStatbox_.x = 1;										// special case, not from lower left
	offsetStatbox_.y = subspaceGlobal.getWindowHeight() - 1;
	
	offsetWeapons_.x = subspaceGlobal.getWindowWidth() - icons_.getFrameWidth();
	offsetWeapons_.y = subspaceGlobal.getWindowHeight() * 7.0/13.0 + 1;			//start 7/13 of the way up the screen
}

void SubspaceHud::calcRadarOffsets(const SubspaceRadar& radar)
{
	printf("OFFSETS (%f,%f),(%f,%f)\n", offsetRadar_.x, offsetRadar_.y, offsetRadarText_.x, offsetRadarText_.y);

	offsetRadar_.x = subspaceGlobal.getWindowWidth() - radar.getDisplayWidth() - 5;
	offsetRadar_.y = 0+5;	//from lower left = 0,0

	offsetRadarText_.x = subspaceGlobal.getWindowWidth() - radar.getDisplayWidth() - 5;
	offsetRadarText_.y = 0+5 + radar.getDisplayHeight() + 3;

	printf("Radar %d OFFSETS (%f,%f),(%f,%f)\n", currentRadar_, offsetRadar_.x, offsetRadar_.y, offsetRadarText_.x, offsetRadarText_.y);
}

void SubspaceHud::initRadar()
{
	printf("INIT RADAR\n");

	static oldRadarWidthMax = 0, oldRadarHeightMax = 0;

	radarDisplayWidth_ = 208;	//tiles on radar display
	radarDisplayHeight_ = 208;

	Uint radarWidthMax = (Uint)(subspaceGlobal.getWindowWidth() * radarScreenWidthRatio_ * (double)SubspaceMap::maxTileX/radarDisplayWidth_);
	Uint radarHeightMax = (Uint)(subspaceGlobal.getWindowHeight() * radarScreenHeightRatio_ * (double)SubspaceMap::maxTileY/radarDisplayHeight_);

	if(map_ && radarWidthMax > 0 && radarHeightMax > 0)
	{
		if(oldRadarWidthMax != radarWidthMax && oldRadarHeightMax != radarHeightMax)
		{
			radar_.initRadar(map_, radarWidthMax, radarHeightMax);

			oldRadarWidthMax = radarWidthMax;
			oldRadarHeightMax = radarHeightMax;
		}
	}

	//calcRadarOffsets(radar_);
}

void SubspaceHud::initBigRadar()
{
	printf("INIT BIG RADAR\n");


	static oldRadarWidthMax = 0, oldRadarHeightMax = 0;

	//bigRadarDisplayWidth_ = SubspaceMap::maxTileX;	//size on screen
	//bigRadarDisplayHeight_ = SubspaceMap::maxTileY;

	bigRadarDisplayWidth_ = SubspaceMap::maxTileX;	//tiles on radar display
	bigRadarDisplayHeight_ = SubspaceMap::maxTileY;

	Uint radarWidthMax = (Uint)(subspaceGlobal.getWindowWidth()*bigRadarScreenWidthRatio_ * (double)SubspaceMap::maxTileX/bigRadarDisplayWidth_);
	Uint radarHeightMax = (Uint)(subspaceGlobal.getWindowHeight()*bigRadarScreenHeightRatio_ * (double)SubspaceMap::maxTileY/bigRadarDisplayHeight_);

	if(map_ && radarWidthMax > 0 && radarHeightMax > 0)
	{
		if(oldRadarWidthMax != radarWidthMax && oldRadarHeightMax != radarHeightMax)
		{
			bigRadar_.initRadar(map_, radarWidthMax, radarHeightMax);
			
			oldRadarWidthMax = radarWidthMax;
			oldRadarHeightMax = radarHeightMax;
		}
	}

	//calcRadarOffsets(bigRadar_);
}

void SubspaceHud::updateLifebar(double timestep)
{
	if(myPlayer_)
	{
		if(myPlayer_->getMaxPossibleEnergy() == 0)
			lifeBar_.setMaxLifePercent(0);
		else
			lifeBar_.setMaxLifePercent(myPlayer_->getMaxEnergy() / myPlayer_->getMaxPossibleEnergy());
		//lifeBar_.setMaxLifePercent(0.5);
		if(myPlayer_->getMaxEnergy() == 0)
			lifeBar_.setLifePercent(0);
		else
			lifeBar_.setLifePercent(myPlayer_->getEnergy() / myPlayer_->getMaxEnergy());
	}
	lifeBar_.update(timestep);
}

void SubspaceHud::drawChat() const
{
	// don't draw chat if on big radar
	if(currentRadar_ == &bigRadar_)
		return;

	glPushMatrix();
		glTranslated(0, console_->getDisplayHeight() + chat_->getDisplayHeight(), 0);
		
		chat_->draw();
		console_->draw();
	glPopMatrix();
}

void SubspaceHud::drawHelpBox() const
{
	if(!help_)
		return;

	if(helpPage_ == -1)
		return;

	int offset = 1;
	glPushMatrix();
		glTranslated(offset, subspaceGlobal.getWindowHeight()-offset - help_->getDisplayHeight(), 0);
		help_->draw();
	glPopMatrix();
}

void SubspaceHud::drawLifeBar() const
{
	int frame = 0;
	
	if(myPlayer_->getEnergy() == myPlayer_->getMaxEnergy())		//protects against divide by 0
		frame = 1;

	glPushMatrix();
		glTranslated(offsetLifebar_.x, offsetLifebar_.y, 0);

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

void SubspaceHud::drawMyPlayerIcons() const
{
	//TODO add global width and height data

	if(!myPlayer_)
		return;

	//draw item icons - left side
	glPushMatrix();
		glTranslated(offsetItems_.x, offsetItems_.y, 0);

		drawItemIcon(30, myPlayer_->getItems().bursts);	//draw bursts
		
		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(31, myPlayer_->getItems().repels);	//draw repels

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(40, myPlayer_->getItems().decoys);	//draw decoys

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(41, myPlayer_->getItems().thors);	//draw thors

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(42, myPlayer_->getItems().bricks);	//draw bricks

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(43, myPlayer_->getItems().rockets);	//draw rockets

		glTranslated(0, -(double)icons_.getFrameHeight() - 1, 0);
		drawItemIcon(46, myPlayer_->getItems().portals);	//draw portals
	glPopMatrix();

	//draw status icons - right side
	glPushMatrix();
		glTranslated(offsetWeapons_.x, offsetWeapons_.y, 0);

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


void SubspaceHud::drawPlayerStatus() const
{
	glPushMatrix();
		glTranslated(subspaceGlobal.getWindowWidth() - statusIndicator_.getFrameWidth(), subspaceGlobal.getWindowHeight(), 0);

		double energyFontExtra = 5;
		//draw main box
		glTranslated(0, -(double)(statusIndicator_.getFrameHeight() + energyFontExtra), 0);
		statusIndicator_.displayFrame(0);
		
		//draw bounty
		glPushMatrix();
			glTranslated(44, 6, 0);
			ledDigits_.displayNumber(myPlayer_->getBounty(), 4);
		glPopMatrix();

		//draw flags
		glPushMatrix();
			glTranslated(37, 18, 0);
			ledDigits_.displayNumber(myPlayer_->getFlags(), 2);
		glPopMatrix();

		//TODO: add team flags

		//draw freq
		glPushMatrix();
			glTranslated(44, 30, 0);
			ledDigits_.displayNumber(myPlayer_->getTeam(), 4);
		glPopMatrix();

		//draw energy value
		glPushMatrix();
			glTranslated(6, 49, 0);
			bigLedDigits_.displayNumber((Uint)myPlayer_->getEnergy(), 4);
		glPopMatrix();

	glPopMatrix();
}

void SubspaceHud::drawRadar() const
{
	glPushMatrix();
		//glTranslated(subspaceGlobal.getWindowWidth() - (radar_.getDisplayWidth()+5), 0+5, 0);	//from lower left = 0,0
		glTranslated(offsetRadar_.x, offsetRadar_.y, 0);

		glPushMatrix();
			currentRadar_->draw();
			if(zone_)
				zone_->drawRadarItems(*currentRadar_);			
		glPopMatrix();
	glPopMatrix();
}

void SubspaceHud::drawRadarInfo() const
{
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

		Uint xPos, yPos;
		if(myPlayer_)
		{
			objectToTileCoords(myPlayer_->getSubspacePosition(), &xPos, &yPos);
		}

		xPos = min(1024, max(0, xPos));		// 1024 is the max tile
		yPos = min(1024, max(0, yPos));

		//coordinates
		string coordStr;
		coordStr += 'A' + xPos/51;
		coordStr += itoa(1 + yPos/51);
		
		//time
		string timeStr = getLocalTime();

		string info;
		info = timeStr + "  " + coordStr;

		//glTranslated(subspaceGlobal.getWindowWidth() - info.length()*font_.getFrameWidth() - 5, radar_.getDisplayHeight()+4, 0);
		glTranslated(offsetRadarText_.x, offsetRadarText_.y, 0);
		font_.displayString(info.c_str(), COLOR_Gray);

	glPopMatrix();
}


void SubspaceHud::drawStatBox() const
{
	glPushMatrix();
		glTranslated(offsetStatbox_.x, offsetStatbox_.y, 0);
		statBox_->draw();
	glPopMatrix();
}

void SubspaceHud::drawSystemMessages() const
{
	glPushMatrix();						
		glTranslated(
			subspaceGlobal.getWindowWidth() * systemMessageScreenWidthRatio_, 
			subspaceGlobal.getWindowHeight() * systemMessageScreenHeightRatio_, 
			0);		
		systemMessageBox_.draw();
	glPopMatrix();
}

void SubspaceHud::drawTimers() const
{
	/*drawTimer(0, 5000, shieldIcon_, COLOR_Gray);		//shields
	drawTimer(1, 5000, superIcon_, COLOR_Gray);		//super
	drawTimer(2, 5000, flagIcon_, COLOR_Red);		//flags
	drawTimer(3, 5000, portalIcon_, COLOR_Yellow);		//portal
	drawTimer(4, 5000, kothIcon_, COLOR_Yellow);		//koth*/

	drawTimer(0, myPlayer_->getShieldsPercent(),	shieldIcon_,	COLOR_Yellow,	true);	//shields
	drawTimer(1, myPlayer_->getSuperTime(),		superIcon_,		COLOR_Yellow);			//super
	drawTimer(2, myPlayer_->getFlagTime(),		flagIcon_,		COLOR_Red);				//flags
	drawTimer(3, myPlayer_->getPortalTime(),		portalIcon_,	COLOR_Yellow);			//portal
	drawTimer(4, myPlayer_->getKothTime(),		kothIcon_,		COLOR_Yellow);			//koth
}

void SubspaceHud::drawTimer(Uint position, double time, const TextureAnimated& icon, ColorType color, bool usePercent) const
{
	glPushMatrix();
		glTranslated(subspaceGlobal.getWindowWidth(), (double)timerLineStart_, 0);
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

void SubspaceHud::drawAllPlayersInfo() const
{
	if(subspaceGlobal.getNametagMode() == NAMETAG_NoTags)
		return;

	SubspacePlayerMap::const_iterator i;
	for(i = players_->begin(); i != players_->end(); ++i)
	{
		if((*i).second != myPlayer_ ||									//not my player
			subspaceGlobal.getNametagMode() == NAMETAG_All )	//or my player is drawn
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
	if(p->getTeam() == myPlayer_->getTeam())
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
		if(player == myPlayer_)
		{
			double lifePercent = p->getEnergy()/(double)p->getMaxEnergy();

			if(lifePercent < 0.25)	//25% life
			{
				font_.displayString((itoa((int)p->getEnergy()).c_str()), COLOR_Red);
				glTranslated(0, -(double)font_.getFontHeight(), 0);
			}
			else if(lifePercent < 0.5)
			{
				font_.displayString((itoa((int)p->getEnergy()).c_str()), COLOR_Yellow);
				glTranslated(0, -(double)font_.getFontHeight(), 0);
			}			
		}

		//write player
		glPushMatrix();
			drawPlayerNametag(player, color);
		glPopMatrix();

		glTranslated(0, -(double)font_.getFontHeight(), 0);

		//write turrets
		const SubspacePlayerList& turrets = player->getTurrets();
		SubspacePlayerList::const_iterator i;
		for(i = turrets.begin(); i != turrets.end(); ++i)
		{
			glPushMatrix();
				drawPlayerNametag(*i, color);
			glPopMatrix();
		}
	glPopMatrix();
}

void SubspaceHud::drawPlayerNametag(SubspacePlayer* p, Uchar color) const
{
	if(!p)
		return;

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
	if(!myPlayer_->isSafe() || myPlayer_->isSpectator())
		return;

	double safeTime = myPlayer_->getSafeTime();	//milliseconds

	string safetyString = "Safety Time Left: " + timeFormat(safeTime/1000, true, true, true);
	
	Vector screen;
	objectToScreenCoords(myPlayer_->getPosition(), &screen);
	glPushMatrix();
		glTranslated(screen.x_ + 5, screen.y_ + 35, 0);
		font_.displayString(safetyString.c_str(), COLOR_Yellow);

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


void SubspaceHud::drawAntiwarpIcon() const
{
	int offset = 38;

	if(!myPlayer_->getStatus().antiwarp)
		offset += 1;

	if(myPlayer_->getItems().antiwarp)
		drawIcon(offset, 1);
	else
		drawIcon(offset, 1, iconShiftRight_);
}


void SubspaceHud::drawBlankIcon() const
{
	//draw blank icon here
}


void SubspaceHud::drawBombIcon() const
{
	int offset = 18;

	if(myPlayer_->getWeapons().bombs.level == 0)
	{
		drawIcon(offset, 1, iconShiftRight_);
		return;
	}

	offset += myPlayer_->getWeapons().bombs.level-1;

	if(myPlayer_->getItems().proximity)
		offset += 3;

	if(myPlayer_->getWeapons().bombs.shrapnel > 0)
		offset += 6;

	drawIcon(offset, 1);
}


void SubspaceHud::drawCloakIcon() const
{
	int offset = 34;

	if(!myPlayer_->getStatus().cloak)
		offset += 1;

	if(myPlayer_->getItems().cloak)
		drawIcon(offset, 1);
	else
		drawIcon(offset, 1, iconShiftRight_);
}

void SubspaceHud::drawGunIcon() const
{
	int offset = 0;

	if(myPlayer_->getWeapons().guns.level == 0)
	{
		drawIcon(offset, 1, iconShiftRight_);
		return;
	}

	offset += myPlayer_->getWeapons().guns.level-1;

	if(myPlayer_->getItems().multifire)
	{
		offset += 6;

		if(myPlayer_->getWeapons().guns.multifire)
			offset -= 3;
	}

	if(myPlayer_->getWeapons().guns.bouncing)
		offset += 9;

	drawIcon(offset, 1);
}


void SubspaceHud::drawStealthIcon() const
{
	int offset = 32;

	if(!myPlayer_->getStatus().stealth)
		offset += 1;

	if(myPlayer_->getItems().stealth)
		drawIcon(offset, 1);
	else
		drawIcon(offset, 1, iconShiftRight_);
}


void SubspaceHud::drawXRadarIcon() const
{
	int offset = 36;

	if(!myPlayer_->getStatus().xRadar)
		offset += 1;

	if(myPlayer_->getItems().xRadar)
		drawIcon(offset, 1);
	else
		drawIcon(offset, 1, iconShiftRight_);
}


bool SubspaceHud::isSpriteOnScreen(const Sprite& sprite) const
{
	// TODO: figure this out
	
	return true;
}