#include "SubspaceBanner.h"

#include "SubspacePalette.h"

SubspaceBanner::SubspaceBanner()
{
}

SubspaceBanner::SubspaceBanner(const SubspaceBanner& copy)
{
	*this = copy;
}

SubspaceBanner::~SubspaceBanner()
{
}

SubspaceBanner& SubspaceBanner::operator =(const SubspaceBanner& rhs)
{
	if(this != &rhs)
	{
		(*(Texture*)this) = rhs;
		bannerData_ = rhs.bannerData_;

		setTextureData(&bannerData_);
	}
	return *this;
}

bool SubspaceBanner::loadBanner(const char* indexes, Uint numIndexes)
{
	if(numIndexes != 12*8)	//banners must be this size
		return false;

	char* memData = new char[numIndexes * 4];
	Uint height = 8;
	Uint width = 12;

	Uint i, j;
	Uint newIndex = 0, oldIndex = 0;
	
	newIndex = 0;
	for(i=0; i < width; ++i)
	{
		for(j=0; j < height; ++j)
		{
			newIndex = j*width+i;				//for flipping image vertically
			oldIndex = (height-1-j)*width + i;		

			memData[(newIndex*4)+0] = subspacePaletteIndex[(Uchar)indexes[oldIndex]][0];
			memData[(newIndex*4)+1] = subspacePaletteIndex[(Uchar)indexes[oldIndex]][1];
			memData[(newIndex*4)+2] = subspacePaletteIndex[(Uchar)indexes[oldIndex]][2];
			memData[(newIndex*4)+3] = 255;		
		}
	}

	bannerData_.setUseColorKey(false);
	bannerData_.loadFromMemory(memData, 12, 8, false);
	this->setTextureData(&bannerData_);

	delete [] memData;

	return true;
}