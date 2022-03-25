#include "HeightMapUtil.h"

#include "AsciiUtil.h"
#include "BmpObject.h"
#include "PngObject.h"

double* interpolateVertices(double* vertices, Uint width, Uint height, Uint* newWidth, Uint* newHeight)
{
	Uint newW = width+1;
	Uint newH = height+1;

	Uint v = 0, oldV = 0;
	double nw=0, ne=0, sw=0, se=0;	//data values
	Uint numValues = 0;

	double* newData = new double[newW * newH];

	for(Uint r=0; r < newH; ++r)
	{
		for(Uint c=0; c < newW; ++c)
		{
			numValues = 0;

			nw = ne = sw = se = 0;

			if(r!=0 && c!=0)	//top left corner
			{
				oldV = (r-1)*width + (c-1);
				nw = vertices[oldV];
				++numValues;
			}

			if(r!=0 && c!=width)	//top right corner
			{
				oldV = (r-1)*width + c;
				ne = vertices[oldV];
				++numValues;
			}

			if(r!=height && c!=width)	//bottom right corner
			{
				oldV = r*width + c;
				se = vertices[oldV];
				++numValues;
			}

			if(r!=height && c!=0)	//bottom left corner
			{
				oldV = r*width + (c-1);
				sw = vertices[oldV];
				++numValues;
			}

			v = r*newW + c;
			newData[v] = (nw + ne + se + sw) / (numValues);
		}
	}

	*newWidth = newW;
	*newHeight = newH;

	return newData;
}

	//new data has same width and height
double* rgbaToDouble(const char* data, Uint width, Uint height)
{
	Uint size = width * height;
	double* newData = new double[size];
	Uint index = 0;
	
	for(Uint i=0; i < size; ++i)
	{
		index = i * 4;

		newData[i] = ((Uchar)data[index] + (Uchar)data[index+1] + (Uchar)data[index+2] /*+ (Uchar)data[index+3]*/) / 3.0;
		newData[i] /= 255;
	}

	return newData;
}


bool loadHeightMap(HeightMap& map, const string& filename, Uint interp)		//supports bmp and png
{
	bool retval = false;
	
	vector<string> tokens = AsciiUtil::tokenize(filename, ".");

	if(tokens.size() == 0)
		return false;

	double* data;
	double* iData;
	Uint width, height;

	string ext = AsciiUtil::tolower(tokens.back());
	if(ext == "bmp" || ext == "bm2")
	{
		BmpObject bmp;
		retval = bmp.load(filename);
		if(retval)
		{
			width = bmp.getWidth();
			height = bmp.getHeight();

			data = rgbaToDouble(bmp.getData(), width, height);
			for(Uint i=0; i < interp; ++i)
			{
				iData = interpolateVertices(data, width, height, &width, &height);
				delete [] data;

				data = iData;
			}

			map.load(data, width, height);
			delete [] data;
		}
	}
	else if(ext == "png")
	{
		PngObject png;	
		retval = png.load(filename);
		if(retval)
		{
			width = png.getWidth();
			height = png.getHeight();

			data = rgbaToDouble(png.getData(), width, height);
			for(Uint i=0; i < interp; ++i)
			{
				iData = interpolateVertices(data, width, height, &width, &height);
				delete [] data;

				data = iData;
			}

			map.load(data, width, height);
			delete [] data;
		}
	}

	return retval;
}
