#include "HeightMap.h"

#include <assert.h>

#include <gl/glaux.h>

HeightMap::HeightMap() : 
	//scale_(1, 1, 0.1), 
	//scale_(1, 1, 1), 

	vertices_(0),
	numVertices_(0), 
	indices_(0), 
	numIndices_(0)
{
}

HeightMap::~HeightMap()
{
	//TODO: destroy cached indices and such
}

double HeightMap::getHeight() const
{
	return height_;
}

double HeightMap::getWidth() const
{
	return width_;
}


double HeightMap::getElevation(double x, double y) const
{
	//int height = mapData_.getHeight()+1;
	//int width = mapData_.getWidth()+1;

	if(x < 0 || x >= width_)	//check bounds
		return 0;		
	if(y < 0 || y >= height_)
		return 0;

	Uint r = (Uint)y;		//row
	Uint c = (Uint)x;		//column						
	Uint v = r*width_ + c;
	
	assert(v >= 0 && v < numVertices_);

	double z00=0, z10=0, z11=0, z01=0;
	Uint numValues=0;

	z00 = vertices_[v].z_;
	++numValues;
	
	if(c < width_)
	{
		z10 = vertices_[v+1].z_;	//1 right
		++numValues;
	}

	if(r < height_)
	{
		z01 = vertices_[v+width_].z_;	//1 down
		++numValues;
	}

	if(c < width_ && r < height_)
	{
		z11 = vertices_[v+1+width_].z_;	//1 down, 1 right
		++numValues;
	}

	double xDist = x - vertices_[v].x_;
	double yDist = y - vertices_[v].y_;

	
	//binlinear filter formula 

	double value = 
		(1-xDist)*(1-yDist) * z00 +
		(1-xDist)*yDist * z01 +
		xDist*(1-yDist) * z10 +
		xDist*yDist * z11;

	value *= 4.0/numValues;	//scale based on number of points used

	return value;
}

bool HeightMap::load(const double* data, Uint width, Uint height)
{
	clearData();

	width_ = width;
	height_ = height;

	calcVertices(data, width, height);
	calcNormals();
	calcIndices();

	return true;
}

/*void HeightMap::setDrawRange(Uint startX, Uint endX, Uint startY, Uint endY)
{
	drawStart_.x_ = startX;
	drawStart_.y_ = startY;

	drawEnd_.x_ = endX;
	drawEnd_.y_ = endY;

	//if(drawStart_.x_ > scale_.x_*mapData_.getWidth()) drawStart_.x_ = scale_.x_*mapData_.getWidth();
	//clamp x ranges
	if(drawStart_.x_ > width_) drawStart_.x_ = width_;
	else if(drawStart_.x_ < 0) drawStart_.x_ = 0;

	if(drawEnd_.x_ > width_) drawEnd_.x_ = width_;
	else if(drawEnd_.x_ < 0) drawEnd_.x_ = 0;

	//clamp y ranges
	if(drawStart_.y_ > height_) drawStart_.y_ = height_;
	else if(drawStart_.y_ < 0) drawStart_.y_ = 0;
	
	if(drawEnd_.y_ > height_) drawEnd_.y_ = height_;
	else if(drawEnd_.y_ < 0) drawEnd_.y_ = 0;
}*/

void HeightMap::draw() const
{
	//scale to unit width and height
	glScaled(1.0 / width_, 1.0 / height_, 1.0);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glVertexPointer(3, GL_DOUBLE, sizeof(Vector), &vertices_[0].x_);
	glNormalPointer(GL_DOUBLE, sizeof(Vector), &normals_[0].x_);
	//glTexCoordPointer(2, GL_double, sizeof(Vertex), &Vertices[0].u);

	glDrawElements(GL_TRIANGLE_STRIP, numIndices_, GL_UNSIGNED_INT, &indices_[0]);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/

	glDisable(GL_CULL_FACE);
}

void HeightMap::calcVertices(const double* data, Uint width, Uint height)
{
	numVertices_ = width * height;
	vertices_ = new Vector[numVertices_];	

	Uint v=0;
	for(Uint r=0; r < height; ++r)
	{
		for(Uint c=0; c < width; ++c)
		{
			vertices_[v].x_ = c;
			vertices_[v].y_ = r;
			vertices_[v].z_ = data[v];

			++v;
		}
	}
}


void HeightMap::calcNormals()
{
	Uint v = 0;				//current vertex

	Vector left, right, top, bottom;
	Vector nw, ne, sw, se;

	normals_ = new Vector[numVertices_];

	for(Uint r=0; r < height_; ++r)
	{
		for(Uint c=0; c < width_; ++c)
		{
			v = r*width_ + c;	

			if(c != width_-1)	//last column
				right = (vertices_[v+1] - vertices_[v]).unitVector();
			if(c != 0)			//first column
				left = -(vertices_[v-1] - vertices_[v]).unitVector();	//unit vector loses negative sign, add it back in

			if(r != height_-1)	//last row
				bottom = (vertices_[v+width_] - vertices_[v]).unitVector();
			if(r != 0)			//first row
				top = -(vertices_[v-width_] - vertices_[v]).unitVector();

			nw = left.cross(top);
			ne = right.cross(top);
			sw = left.cross(bottom);
			se = right.cross(bottom);

			normals_[v] = (nw + ne + sw + se).unitVector();
		}
	}
}


void HeightMap::calcIndices()
{
	Uint v = 0;		//current vertex
	Uint i = 0;		//current index

	numIndices_ = width_ * (height_-1) * 2;	//use triangle strips
	indices_ = new Uint[numIndices_];

	for(Uint r=0; r < height_-1; ++r)	//last row has no row below it - can't make triangle strip
	{
		for(Uint c=0; c < width_; ++c)
		{
			if(r % 2 == 0)	//every even row
			{
				v = r*width_ + c;	//start from left

				indices_[i++] = v + width_;		//vertex 1 below
				indices_[i++] = v;
			}
			else			//every odd row
			{
				v = r*width_ + (width_-1 - c);	//start from right

				indices_[i++] = v;
				indices_[i++] = v + width_;		//vertex 1 below
			}
		}
	}

	assert(numIndices_ == i);
}


void HeightMap::clearData()
{
	width_ = 0;
	height_ = 0;
	//scale_ = Vector(1.0, 1.0, 1.0);

	if(vertices_)
		delete [] vertices_;
	numVertices_ = 0;

	if(normals_)
		delete [] normals_;
		
	if(indices_)
		delete [] indices_;
	numIndices_ = 0;
}