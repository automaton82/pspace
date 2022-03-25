#ifndef _HEIGHTMAPUTIL_H_
#define _HEIGHTMAPUTIL_H_

#include "HeightMap.h"

		//NOTE: make sure to delete old data and new data when finished with it
double* interpolateVertices(double* vertices, Uint width, Uint height, Uint* newWidth, Uint* newHeight);	//averages 4 points into a new one 

bool loadHeightMap(HeightMap& map, const string& filename, Uint interp = 1);		//supports bmp and png


#endif
