#include "SubspaceRadar.h"

#include "MathUtil.h"

#include "SubspaceGlobal.h"
#include "SubspaceMap.h"
#include "SubspacePlayer.h"
#include "SubspaceUtil.h"


struct CreateRadarData
{
	CreateRadarData(SubspaceRadar* r, const SubspaceMap* m, Uint w, Uint h) : radar(r), map(m), width(w), height(h)
	{}

	SubspaceRadar* radar;
	const SubspaceMap* map;
	Uint width, height;
};

void createRadarRoutine(void* arg)
{
	CreateRadarData* data = (CreateRadarData*)arg;
    char* pixels = data->radar->createRadarTexture(data->map, data->width, data->height);
	data->radar->notifyRadarCreated(pixels);
}

//////////////////////////////////////

const Color& SubspaceRadar::colorBall1		= colorYellow;
const Color& SubspaceRadar::colorBall2		= colorRed;
const Color& SubspaceRadar::colorFlag1		= colorRed;
const Color& SubspaceRadar::colorFlag2		= colorYellow;
const Color& SubspaceRadar::colorExplosion	= colorDarkRed;
const Color& SubspaceRadar::colorMine		= colorLightGray;
const Color& SubspaceRadar::colorMyPlayer	= colorYellow;
const Color& SubspaceRadar::colorPlayerEnemy	= colorBlue;
const Color& SubspaceRadar::colorPlayerEnemyWeak= colorDarkBlue;
const Color& SubspaceRadar::colorPlayerFlag	= colorRed;
const Color& SubspaceRadar::colorPlayerTeam	= colorYellow;
const Color& SubspaceRadar::colorPrize		= colorGreen;
	

SubspaceRadar::SubspaceRadar() :
	createRadarThread_(createRadarRoutine),
	isCreatingRadar_(false),
	targetBounty_(10),
	tempPixelData_(0),

	flashTime_(150),							// TODO: where did these numbers come from? guesstimates?
	bombFlashTime_(75),
	timer_(0)
{
}


double SubspaceRadar::getDisplayHeight() const
{
	Uint rx, ry;
	tileToRadarCoords(radarTileDisplayWidth_, radarTileDisplayHeight_, &rx, &ry);

	return (double)ry;
}

double SubspaceRadar::getDisplayWidth() const
{
	Uint rx, ry;
	tileToRadarCoords(radarTileDisplayWidth_, radarTileDisplayHeight_, &rx, &ry);

	return (double)rx;
}

int SubspaceRadar::getTargetBounty() const
{
	return targetBounty_;
}

void SubspaceRadar::setDisplayArea(Uint tileX, Uint tileY, Uint tileWidth, Uint tileHeight)
{
	radarTileX_ = tileX;
	radarTileY_ = tileY;

	/*radarDisplayWidth_ = displayWidth;
	radarDisplayHeight_ = displayHeight;*/

	radarTileDisplayWidth_ = tileWidth;		
	radarTileDisplayHeight_ = tileHeight;	
}

void SubspaceRadar::setTargetBounty(int bounty)
{
	targetBounty_ = bounty;
}

void SubspaceRadar::initRadar(const SubspaceMap* map, Uint width, Uint height)	//creates radar of SCREEN size width x height
{
	assert(map);
	assert(width > 0 && height > 0);

	radarWidth_ = width;
	radarHeight_ = height;

	//createRadarThread(map, width, height);

	char* pixels = createRadarTexture(map, width, height);		//TODO: need GLUtil renderstates for creating texture in new thread
	notifyRadarCreated(pixels);

	if(tempPixelData_)		// assumes createThreadRoutine has finished
	{
		createRadarThread_.waitForExit();

		radarTextureData_.loadFromMemory(tempPixelData_, radarWidth_, radarHeight_, false);
		delete [] tempPixelData_;
		tempPixelData_ = 0;

		radarTexture_.setTextureData(&radarTextureData_);

		isCreatingRadar_ = false;
	}
}

char* SubspaceRadar::createRadarTexture(const SubspaceMap* map, Uint width, Uint height)
{
	Ulong i,j;
	Ulong index;

	assert(map);
	assert(width > 0 && height > 0);		

	width = min(SubspaceMap::maxTileX, width);
	height = min(SubspaceMap::maxTileY, height);

	radarWidth_ = width;
	radarHeight_ = height;

	char* pixels = new char[width * height * 4];
	memset(pixels, 0, width*height*4);

	debugout << "Creating radar..." << ends;
	//printf("Creating radar...");

	Uint w, h;
	double ratioX = (double)width / SubspaceMap::maxTileX;
	double ratioY = (double)height / SubspaceMap::maxTileY;

	const SubspaceMap::BasicMatrix tiles = map->getTileMatrix();
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

			const SubspaceTile& tile = map->getTile(j, i);
			//tileType = tiles[j][i];

			if(tile.isDoor())			
			{
				pixels[index + 0] = (char)173;	//light gray
				pixels[index + 1] = (char)173;
				pixels[index + 2] = (char)173;
				pixels[index + 3] = (char)255;
			}
			else if(tile.isTeamGoal())			
			{
				pixels[index + 0] = (char)255;	//yellow
				pixels[index + 1] = (char)255;
				pixels[index + 2] = 0;
				pixels[index + 3] = (char)255;
			}
			else if(tile.isGoal())		
			{
				pixels[index + 0] = (char)255;	//red
				pixels[index + 1] = 57;
				pixels[index + 2] = 8;
				pixels[index + 3] = (char)255;
			}
			else if(tile.isSafe())
			{
				pixels[index + 0] = 24;	//green
				pixels[index + 1] = 82;
				pixels[index + 2] = 24;
				pixels[index + 3] = (char)255;
			}
			else if(tile.isCollidable() || tile.isAsteroid() || tile.isAsteroidPart())	
			{
				pixels[index + 0] = 90;	//gray
				pixels[index + 1] = 90;
				pixels[index + 2] = 90;
				pixels[index + 3] = (char)255;
			}
		}
	}

	debugout << "done." << endl;
	//printf("done.\n");

	return pixels;
}

bool SubspaceRadar::createRadarThread(const SubspaceMap* map, Uint width, Uint height)
{
	assert(map);

	if(isCreatingRadar_)
		return false;

	isCreatingRadar_ = true;

	static CreateRadarData data(this, map, width, height);
	createRadarThread_.start(&data);	

	return true;
}

void SubspaceRadar::update(double time)
{
	timer_ += time;

	if(tempPixelData_)		// assumes createThreadRoutine has finished
	{
		createRadarThread_.waitForExit();

		radarTextureData_.loadFromMemory(tempPixelData_, radarWidth_, radarHeight_, false);
		delete [] tempPixelData_;
		tempPixelData_ = 0;

		radarTexture_.setTextureData(&radarTextureData_);

		isCreatingRadar_ = false;
	}
}

void SubspaceRadar::draw() const
{
	glDisable(GL_TEXTURE_2D);
	drawBackground();

	glEnable(GL_TEXTURE_2D);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	drawRadarStatic();

	glDisable(GL_TEXTURE_2D);
	drawBorder();
}

void SubspaceRadar::drawBackground() const
{
	double drawWidth = radarTileDisplayWidth_ * radarWidth_/SubspaceMap::maxTileX;
	double drawHeight = radarTileDisplayHeight_ * radarHeight_/SubspaceMap::maxTileY;

	double borderWidth_ = 2;

	//glColor4d(backgroundColor_.r_, backgroundColor_.g_, backgroundColor_.b_, backgroundColor_.a_);

	glColor4d(10.0/255.0, 25.0/255.0, 10.0/255.0, 0.9);
	
	glBegin(GL_QUADS);			//background
		glVertex3d(drawWidth + borderWidth_/2.0, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, drawHeight + borderWidth_/2.0, 0);
		glVertex3d(drawWidth + borderWidth_/2.0, drawHeight + borderWidth_/2.0, 0);
	glEnd();
}

void SubspaceRadar::drawBorder() const
{
	double drawWidth = radarTileDisplayWidth_ * radarWidth_/SubspaceMap::maxTileX;
	double drawHeight = radarTileDisplayHeight_ * radarHeight_/SubspaceMap::maxTileY;

	glColor4d(0.5, 0.5, 0.5, 1.0);

	double borderWidth_ = 1;
	glLineWidth((float)borderWidth_*2.0f);

	glBegin(GL_LINE_LOOP);			//main border
		glVertex3d(-borderWidth_, -borderWidth_, 0);
		glVertex3d(drawWidth, -borderWidth_, 0);
		glVertex3d(drawWidth, drawHeight, 0);
		glVertex3d(-borderWidth_, drawHeight, 0);
		glVertex3d(-borderWidth_, -borderWidth_, 0);
	glEnd();
}

static tempTX_, tempTY_, tempRX_, tempRY_;

void SubspaceRadar::drawRadarStatic() const
{
	glPushMatrix();

		glEnable(GL_TEXTURE_2D);
   	
		radarTexture_.enable();

		double w = (double)radarWidth_;
		double h = (double)radarHeight_;
				
			//TODO: all these calculations would be done much easier with ints
		double xPos = radarTileX_, yPos = radarTileY_;	//middle position

		xPos = min(SubspaceMap::maxTileX-radarTileDisplayWidth_/2, xPos);	//max of half radar screen less than top tile
		yPos = min(SubspaceMap::maxTileY-radarTileDisplayWidth_/2, yPos);
		
		if(radarTileDisplayWidth_/2 > xPos)				//adjust from center
			xPos = 0;
		else
			xPos -= radarTileDisplayWidth_/2.0;

		if(radarTileDisplayHeight_/2 > yPos)
			yPos = 0;
		else
			yPos -= radarTileDisplayHeight_/2.0;

		tempTX_ = xPos;
		tempTY_ = yPos;

		Uint rx, ry;
		tileToRadarCoords(xPos, yPos, &rx, &ry);

		tempRX_ = rx;
		tempRY_ = ry;

		Uint displayWidth, displayHeight;
		tileToRadarCoords(radarTileDisplayWidth_, radarTileDisplayHeight_, &displayWidth, &displayHeight);

		radarTexture_.displayArea(rx/w, ry/h, displayWidth/w, displayHeight/h);

	glPopMatrix();
}

/*void SubspaceRadar::drawRadarDynamic() const
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

void SubspaceRadar::drawDynamicRadarItem(const Vector& pos) const
{
	Uint xt, yt;
	objectToTileCoords(pos, &xt, &yt);

	Uint rxt, ryt;
	tileToRadarCoords(xt, yt, &rxt, &ryt);	//coordinates of object in radar position

	Uint rX_ = tempRX_;	//lower left corner of screen
	Uint rY_ = tempRY_;

	Uint rdw, rdh;	
	tileToRadarCoords(radarTileDisplayWidth_, radarTileDisplayHeight_, &rdw, &rdh);

	if(rxt >= rX_ && rxt < rX_+rdw &&
		ryt >= rY_ && ryt < rY_+rdh)
		/*xt >= rTileX_ && xt < rTileX_+radarTileDisplayWidth_ &&
		yt >= rTileY_ && yt < rTileY_+radarTileDisplayHeight_)*/
	{
		Uint rx, ry;
		if(rX_ > rxt)
			rx = 0;
		else
			rx = rxt - rX_;

		if(rY_ > ryt)
			ry = 0;
		else
			ry = ryt - rY_;

		glDisable(GL_TEXTURE_2D);

		glBegin(GL_POINTS);
			glVertex3d(rx, ry, 0);
		glEnd();
	}
}

void SubspaceRadar::drawRadarBall(const Vector& position) const
{
	Color color;

	if((Uint)(timer_/flashTime_) % 2 == 0)
		color = colorBall1;
	else
		color = colorBall2;

	color.enable();
	glPointSize(2.0);

	drawDynamicRadarItem(position);
}

void SubspaceRadar::drawRadarExplosion(const Vector& position) const
{
	Color color;
	if((Uint)(timer_/bombFlashTime_) % 2 == 0)
		color = colorExplosion;
	else
		color = colorClear;

	color.enable();
	glPointSize(2.0);

	drawDynamicRadarItem(position);
}

void SubspaceRadar::drawRadarFlag(const Vector& position) const
{
	Color color;

	if((Uint)(timer_/flashTime_) % 2 == 0)		
		color = colorFlag1;
	else
		color = colorFlag2;

	color.enable();
	glPointSize(2.0);

	drawDynamicRadarItem(position);
}

void SubspaceRadar::drawRadarMine(const Vector& position) const
{
	Color color;
	if((Uint)(timer_/bombFlashTime_) % 2 == 0)
		color = colorMine;
	else
		color = colorClear;

	color.enable();
	glPointSize(2.0);

	drawDynamicRadarItem(position);
}

void SubspaceRadar::drawRadarMyPlayer(SubspacePlayer* player) const
{
	float pointSize = 2.0;
	Color color;

	if((Uint)(timer_/flashTime_) % 2 == 0)		//150 ms on/off
		color = colorMyPlayer;
	else
		color = colorClear;

	if(player->getFlags() > 0)
		pointSize = 3.0;

	glPointSize(pointSize);
	color.enable();
	
	drawDynamicRadarItem(player->getPosition());
}

void SubspaceRadar::drawRadarPlayer(SubspacePlayer* player, bool myTeam) const
{
	float pointSize = 2.0;
	Color color;
					
	Uint targetBounty_ = 10;

	if(myTeam)
		color = colorPlayerTeam;
	else
	{
		if(player->getBounty() >= targetBounty_)	//TODO: add target bounty_;
			color = colorPlayerEnemy;
		else
			color = colorPlayerEnemyWeak;

		if(player->getFlags() > 0)
			color = colorPlayerFlag;
	}

	if(player->getFlags() > 0)
		pointSize = 3.0;
			
	glPointSize(pointSize);	
	color.enable();

	drawDynamicRadarItem(player->getPosition());				
}

/*void SubspaceRadar::drawRadarPlayerWeak(SubspacePlayer* player, bool myTeam) const
{
	float pointSize = 2.0;
	Color color;
					
	if(myTeam)
		color = colorPlayerTeam;
	else
	{
		color = colorPlayerEnemyWeak;

		if(player->getFlags() > 0)
			color = colorPlayerFlag;
	}

	if(player->getFlags() > 0)
		pointSize = 3.0;
			
	glPointSize(pointSize);
	color.enable();

	drawDynamicRadarItem(player->getPosition());				
}*/

void SubspaceRadar::drawRadarPrize(const Vector& position) const
{
	Color color = colorPrize;
			
	glPointSize(2.0);
	color.enable();

	drawDynamicRadarItem(position);				
}

void SubspaceRadar::tileToRadarCoords(Uint tx, Uint ty, Uint* rx, Uint* ry) const
{
	double ratioX = radarWidth_ / (double)SubspaceMap::maxTileX;
	double ratioY = radarHeight_ / (double)SubspaceMap::maxTileY;

	(*rx) = (Uint)MathUtil::round(tx * ratioX); 
	(*ry) = (Uint)MathUtil::round(ty * ratioY); 

	/*(*rx) = tx * radarWidth_ / SubspaceMap::maxTileX;
	(*ry) = ty * radarHeight_ / SubspaceMap::maxTileY;*/
}

void SubspaceRadar::notifyRadarCreated(char* pixelData)
{
	//bool result = createRadarThread_.waitForExit();
	tempPixelData_ = pixelData;
		
	/*radarTextureData_.loadFromMemory(pixelData, radarWidth_, radarHeight_, false);
	delete [] pixelData;

	radarTexture_.setTextureData(&radarTextureData_);*/
}