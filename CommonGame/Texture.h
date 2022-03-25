//David Millman

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <assert.h>
#include <string>
#include <math.h>
using std::string;

//#include <GL/gl.h>
#include <GL/glaux.h>

#include "Point.h"
#include "TextureData.h"
#include "TextureManager.h"

class Texture
{
public:
	struct VertexData
	{
		double tlX, tlY;	// top left
		double trX, trY;	// top right
		double brX, brY;	// bottom right
		double blX, blY;	// bottom left
	};

	struct TexCoordData
	{
		double tlX, tlY;	// top left
		double trX, trY;	// top right
		double brX, brY;	// bottom right
		double blX, blY;	// bottom left
	};

public:

	Texture();
	virtual ~Texture();

	int getTextureID()	const;

	Uint getWidth()	const;
	Uint getHeight() const;

	const char* getData()	const;
	void getVertices(VertexData& vd) const;
	void getTexCoords(TexCoordData& td) const;

	void setTextureData(TextureData* data);
	const TextureData* getTextureData() const;

	bool load(const string& filename, bool stretch = true);
	bool unload();
	virtual void initAfterLoad();

	///enables the texture for displaying in openGL
	bool enable() const;

	void display() const;
	void displayAll() const;	//includes padding
	void displayArea(double x, double y, double width, double height) const;	//all in range 0.0 - 1.0

protected:

	double adjustWidth_, adjustHeight_;
	string filename_;

private:

	//int textureId_;
	TextureData* texData_;
};

#endif