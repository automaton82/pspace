#ifndef _HEIGHTMAP_H_
#define _HEIGHTMAP_H_

#include <string>
using std::string;

#include "DataTypes.h"
#include "Material.h"
#include "Vector.h"

//TODO: add terrain mipmaps, i.e. levels of heightmap (every other index, etc.)

class HeightMap
{
public:
	HeightMap();
	~HeightMap();

	double getHeight() const;	//data size x
	double getWidth() const;	//data size y

	double getElevation(double x, double y) const;	//from 0.0 - 1.0
	//Vector getSlopeVector() const;

	//bool load(const string& filename);	//supports bitmaps and pngs
	bool load(const double* data, Uint width, Uint height);	//raw data, vertex[height]

	//void setMipmapLevel(Uint level);

	//void setDrawRange(Uint startX, Uint endX, Uint startY, Uint endY);
	void draw() const;		//draws "unit" height map - length,width,height = 1, NOTE: Z coord unchanged
	
private:

	void calcVertices(const double* data, Uint width, Uint height);
	void calcNormals();
	void calcIndices();

	void clearData();

private:

	//Texture mapData_;
	//Texture displayTexture_;

	//Vector drawStart_;		//start draw range
	//Vector drawEnd_;		//end draw range

	Uint width_;	//columns
	Uint height_;	//rows

	//Vertex array data
	Vector* vertices_;
	Uint numVertices_;

	Vector* normals_;	//same number of normals and vertices
	
	Uint* indices_;
	Uint numIndices_;

	//int listIndex;
};

#endif