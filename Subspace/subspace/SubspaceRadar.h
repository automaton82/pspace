#ifndef _SUBSPACERADAR_H_
#define _SUBSPACERADAR_H_

#include <list>
using std::list;

#include "TextBox.h"
#include "TextTable.h"
#include "TimedTextBox.h"
#include "Thread.h"

#include "SubspaceColors.h"

class SubspaceMap;
class SubspacePlayer;

//TODO: make this class store almost no information

class SubspaceRadar
{
private:

	static const Color& colorBall1;
	static const Color& colorBall2;
	static const Color& colorFlag1;
	static const Color& colorFlag2;
	static const Color& colorExplosion;
	static const Color& colorMine;
	static const Color& colorMyPlayer;
	static const Color& colorPlayerEnemy;
	static const Color& colorPlayerEnemyWeak;
	static const Color& colorPlayerFlag;
	static const Color& colorPlayerTeam;
	static const Color& colorPrize;
	
public:
	SubspaceRadar();

	void initRadar(const SubspaceMap* map, Uint width, Uint height);	//creates radar of SCREEN size width x height
	
	// Accessors
	double getDisplayHeight() const;
	double getDisplayWidth() const;
	int getTargetBounty() const;

	// Mutators
	void setDisplayArea(Uint tileX, Uint tileY, Uint tileWidth, Uint tileHeight);	//x,y is the center of the display area
	void setTargetBounty(int bounty);

	//////////////////////////////////////

	void update(double timeStep);

	void draw() const;
		void drawRadarBall(const Vector& position) const;
		void drawRadarExplosion(const Vector& position) const;
		void drawRadarFlag(const Vector& position) const;
		void drawRadarMine(const Vector& position) const;
		void drawRadarMyPlayer(SubspacePlayer* player) const;
		void drawRadarPlayer(SubspacePlayer* player, bool myTeam = false) const;
		//void drawRadarPlayerWeak(SubspacePlayer* player, bool myTeam = false) const;
		void drawRadarPrize(const Vector& position) const;
	
private:
	
	//void clearRadar();		// TODO: clear the radar when it gets re-init
	char* createRadarTexture(const SubspaceMap* map, Uint width, Uint height);	//creates radar of TILE size width x height
	bool createRadarThread(const SubspaceMap* map, Uint width, Uint height);
	void notifyRadarCreated(char* data);

	friend void createRadarRoutine(void* arg);

	void drawBackground() const;
	void drawBorder() const;
	void drawRadarStatic() const;
	//void drawRadarDynamic() const;	//dynamic radar items, e.g. players, greens, balls, flags

	void drawDynamicRadarItem(const Vector& position) const;

	void tileToRadarCoords(Uint tx, Uint ty, Uint* rx, Uint* ry) const;

private:
	bool isCreatingRadar_;
	Thread createRadarThread_;
	char* tempPixelData_;

	TextureData radarTextureData_;
	Texture radarTexture_;
	//Uint radarDisplayWidth_, radarDisplayHeight_;	//actual size of radar on screen
	Uint radarWidth_, radarHeight_;					//size of entire radar on screen

	Uint radarTileX_, radarTileY_;							//lower left tile that radar is displaying
	Uint radarTileDisplayWidth_, radarTileDisplayHeight_;	//area of radar displayed, in tiles
	
	double flashTime_;
	double bombFlashTime_;
	double timer_;

	int targetBounty_;
};

#endif