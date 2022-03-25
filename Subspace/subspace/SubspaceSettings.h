#ifndef _SUBSPACESETTINGS_H_
#define _SUBSPACESETTINGS_H_

enum ColorType					// bitmap font colors (tallfont.bmp)
{
	COLOR_Blue = 2,
	COLOR_Gray = 0,
	COLOR_Grey = 0,
	COLOR_Green = 1,
	COLOR_Orange = 6,
	COLOR_Pink = 7,
	COLOR_Purple = 5,
	COLOR_Red = 3,
	COLOR_Yellow = 4
};

enum GraphicLayerType
{
	LAYER0,
	LAYER1,
	LAYER2,
	LAYER3,
	LAYER4,
	LAYER5,
	LAYER6,
	LAYER7,

	NUM_LAYERS
};

enum MessageLocationType
{
	LOCATION_Off = 0,
	LOCATION_Center,
	LOCATION_Chat
};

enum NametagModeType
{
	NAMETAG_NoTags = 0,
	NAMETAG_Others,			//everyone except the player
	NAMETAG_All,

	NAMETAG_Unknown
};

enum ObjectLayerType
{
	UnknownLayer = LAYER0,
	EffectLayer = LAYER1,
	ShipLayer = LAYER2,
	WeaponLayer = LAYER3,
	EffectLayerFar = LAYER4,
	MapItemLayer = LAYER5,
	StarLayer = LAYER6,

	NumObjectLayers
};

#endif
