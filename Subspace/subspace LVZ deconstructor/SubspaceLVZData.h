#ifndef _SUBSPACELVZDATA_
#define _SUBSPACELVZDATA_

typedef unsigned __int32 Uint32;
typedef unsigned __int16 Uint16;
typedef signed __int32 Sint32;
typedef signed __int16 Sint16;

#pragma pack(push, 1)

/*The first 8 bytes of every .lvz file:
4-len str	Type - Should always be 'CONT'. This varifies you have a .lvz. If not, don't continue on. This string is not NULL ended.
i32		Count - How many compressed sections are in this .lvz.*/

struct LVZFileHeader
{
	Uint32 fileType : 32;
	Sint32 count : 32;
};

/*After this, if the count is > 0, there should be a compressed header:
4-len str	Type - Should be 'CONT' also. If not, the compression header is bad, and do not use.
i32		Decompress Size - The size of the data after it would be decompressed. This is the value you should set your out-buffer for the zlib's compression call.
i32		File Time - This can either be a file time, if this compressed section is a file, or it could be 0. If 0, then is a special compressed section. The file time is the standard time_t format used by windows/unix.
i32		Compressed Size - The size of the compressed data in the .lvz. This is how many bytes you'll need to read to get your required data.
Null-end str	File Name - This string is always null ended. If no file time, the File Name should be blank (just the 0x00), but do not always count on it.
* data *	Compressed data - This should be the length of the Compressed size. Read this all in, and then run it through zlib's decompress function.*/

struct LVZFileObject
{
	Uint32 fileType : 32;
	Sint32 decompressedSize : 32;
	Sint32 timestamp : 32;
	Sint32 compressedSize : 32;
};
// immediately followed by a null-terminated string
// immediately followed by zlib compressed data

/*Object Section always starts with this 12 byte header:
4-len str	Type - Can be 'CLV1' or 'CLV2'.
i32		Object Count - How many object definitions are in this Object Section.
i32		Image Count - How many image definitions are in this Object Section.*/

struct LVZScriptHeader
{
	Uint32 objectType : 32;
	Sint32 numObjects : 32;
	Sint32 numImages : 32;
};

/**** For both of CLV1's Screen/Map object, or CLV2's Map object ***
 After the Object Section header, you would loop through the Object Count reading 10 bytes at a time till you hit the Object Count. Each of these 10 byte sections are the Object definitions:
i1		Map Object - A bool-like value. If true, this is a Map Object. If false, then is a Screen Object.
i15		Object ID - The value for this object id, for use with *objon for example.
i16		X Coord - The X coord value, in pixels, this object will be displayed on. More info later.
i16		Y Coord - The Y coord value, in pixels.
i8		Image Number - Which of the Image Definitions this object will use for its graphic.
i8		Layer - Which layer it will be displayed on. Values for this later,
i12		Display Time - How long will display for, in 1/10th of a second.
i4		Display Mode - Which display mode this object uses. Values later.*/

struct LVZBasicObject
{
	Sint16 isMapObject : 1;
	Sint16 objectID : 15;
	Sint16 xCoord : 16;
	Sint16 yCoord : 16;
	Sint16 imageID : 8;
	Sint16 layer : 8;
	Sint16 displayTime : 12;
	Sint16 displayMode : 4;
};

struct LVZMapObject
{
	Sint16 isMapObject : 1;
	Sint16 objectID : 15;
	Sint16 xCoord : 16;
	Sint16 yCoord : 16;
	Sint16 imageID : 8;
	Sint16 layer : 8;
	Sint16 displayTime : 12;
	Sint16 displayMode : 4;
};

/**** CLV2's Screen Object ***
i1		Map Object - A bool-like value. If true, this is a Map Object. If false, then is a Screen Object.
i15		Object ID - The value for this object id, for use with *objon for example.
u4		X Type - Which part of the screen the X coord is offset from (see OffsetTypes lower down)
i12		X Coord - The X coord value, in pixels, this object will be displayed on. More info later.
u4		Y Type - Same as for X
i12		Y Coord - The Y coord value, in pixels.
i8		Image Number - Which of the Image Definitions this object will use for its graphic.
i8		Layer - Which layer it will be displayed on. Values for this later,
i12		Display Time - How long will display for, in 1/10th of a second.
i4		Display Mode - Which display mode this object uses. Values later.*/

struct LVZScreenObject
{
	Sint16 isMapObject : 1;
	Sint16 objectID : 15;
	Uint16 xType : 4;
	Sint16 xCoord : 12;
	Uint16 yType : 4;
	Sint16 yCoord : 12;
	Sint16 imageID : 8;
	Sint16 layer : 8;
	Sint16 displayTime : 12;
	Sint16 displayMode : 4;
};

/*Layer Values:
0 = BelowAll
1 = AfterBackground
2 = AfterTiles
3 = AfterWeapons
4 = AfterShips
5 = AfterGauges
6 = AfterChat
7 = TopMost*/

enum LVZLayerType
{
	LVZ_LAYER_BelowAll = 0,
	LVZ_LAYER_AfterBackground,
	LVZ_LAYER_AfterTiles,
	LVZ_LAYER_AfterWeapons,
	LVZ_LAYER_AfterShips,
	LVZ_LAYER_AfterGauges,
	LVZ_LAYER_AfterChat,
	LVZ_LAYER_TopMost,

	LVZ_NumLayers
};

/*
Display Mode Values:
0 = ShowAlways
1 = EnterZone
2 = EnterArena
3 = Kill
4 = Death
5 = ServerControlled
*/

enum LVZDisplayModeType
{
	LVZ_DISPLAYMODE_ShowAlways = 0,
	LVZ_DISPLAYMODE_EnterZone,
	LVZ_DISPLAYMODE_EnterArena,
	LVZ_DISPLAYMODE_Kill,
	LVZ_DISPLAYMODE_Death,
	LVZ_DISPLAYMODE_ServerControlled
};

/*Types for X/Y's (CLV2 ScreenObjects Only):
 0 = Normal (no letters in front)
 1 = C - Screen center
 2 = B - Bottom right corner
 3 = S - Stats box, lower right corner
 4 = G - Top right corner of specials
 5 = F - Bottom right corner of specials
 6 = E - Below energy bar & spec data
 7 = T - Top left corner of chat
 8 = R - Top left corner of radar
 9 = O - Top left corner of radar's text (clock/location)
10 = W - Top left corner of weapons
11 = V - Bottom left corner of weapons*/

enum LVZScreenType
{
	LVZ_SCREEN_Normal = 0,
	LVZ_SCREEN_Center, // = 'C',
	LVZ_SCREEN_BottomRight, // = 'B',
	LVZ_SCREEN_StatBoxLowerRight, // = 'S',
	LVZ_SCREEN_SpecialsTopRight, // = 'G',
	LVZ_SCREEN_SpecialsBottomRight, // = 'F',
	LVZ_SCREEN_BelowEnergy, // = 'E',
	LVZ_SCREEN_ChatTopLeft, // = 'T',
	LVZ_SCREEN_RadarTopLeft, // = 'R',
	LVZ_SCREEN_RadarTextTopLeft, // = 'O',
	LVZ_SCREEN_WeaponTopLeft, // = 'W',
	LVZ_SCREEN_WeaponBottomLeft, // = 'V'

	LVZ_SCREEN_NumTypes
};

/* After finish looping through the Object Count, you now would loop through for the Image Count. The Image Definition are not pre-determined lengths though. Image Definition:
i16		X Count - How many columns are in this image. Used for animations.
i16		Y Count - How many rows are in this image.
i16		Display Time - How long to display each frame for. This is in 1/10th of a second.
Null-end str	File Name - This will be null ended. This is which file this image uses. The file name is not required to be included with the .lvz, and maybe in another .lvz package, or a non-zone downloaded image that users place in their folders themselves. So do not expect this file to always be in this .lvz or to be in the folder.
*/

struct LVZImageObject
{
	Sint16 xCount : 16;
	Sint16 yCount : 16;
	Sint16 displayTime : 16;
};
// immediately followed by a null-terminated string

#pragma pack(pop)

#endif