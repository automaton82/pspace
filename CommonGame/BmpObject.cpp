#include "BmpObject.h"

#include "GLObject.h"	//for auxDIBImageLoad()

BmpObject::BmpObject() : 
	width_(0), height_(0),
	data_(0), 
	bitDepth_(0)
{
}

BmpObject::BmpObject(const string& file) : 
	width_(0), height_(0),
	data_(0), 
	bitDepth_(0)
{
	load(file);
}

BmpObject::~BmpObject()
{

}

unsigned int BmpObject::getBitDepth() const
{
	return bitDepth_;
}

char* BmpObject::getData() const
{
	return data_;
}

unsigned int BmpObject::getImageSize() const
{
	return width_*height_ * bitDepth_/8 * sizeof(char);
}

unsigned int BmpObject::getHeight() const
{
	return height_;
}

unsigned int BmpObject::getWidth() const
{
	return width_;
}

bool BmpObject::load(const string& filename)
{
	FILE* file = 0;

	file = fopen(filename.c_str(), "rb");
	if(!file)
		return false;
	fclose(file);
		
	if(!readBmpFile(filename))
		return false;

	//scaleAlpha();

	return true;
}

void BmpObject::scaleAlpha()
{
	int newPad = 0;//(4 - (w*4)%4)==4 ? 0 : (4 - (w*4)%4);
	
	Uint i,j;
	for(i = 0; i < height_; i++)
	{
		// read in the color components
		for(j = 0; j < width_; j++)
		{
			unsigned long index = ((i * (width_+newPad) + j) * 4);
			
			//max
			data_[index + 3] = (char)max(max(data_[index + 0], data_[index + 1]), data_[index + 2]);

			//avg
			//data_[index + 3] = (char)((data_[index + 0] + data_[index + 1] + data_[index + 2]) / 3);
			
			/*data_[index + 0] = (char)255;
			data_[index + 1] = (char)255;
			data_[index + 2] = (char)255;*/
			
		}
	}
}

void BmpObject::addAlphaChannel()
{
	int newPad = 0;//(4 - (w*4)%4)==4 ? 0 : (4 - (w*4)%4);
	int oldPad = 0;//(4 - (w*3)%4)==4 ? 0 : (4 - (w*3)%4);

	char* newData = new char[(width_+newPad)*height_ * 4];

	Uint i,j;
	for(i = 0; i < height_; i++)
	{
		// read in the color components
		for(j = 0; j < width_; j++)
		{
			unsigned long newIndex = ((i * (width_+newPad) + j) * 4);
			unsigned long oldIndex = ((i * (width_+oldPad) + j) * 3);

			newData[newIndex + 0] = data_[oldIndex + 0];
			newData[newIndex + 1] = data_[oldIndex + 1];
			newData[newIndex + 2] = data_[oldIndex + 2];
			newData[newIndex + 3] = (char)255;
		}
	}

	clearData();
	data_ = newData;
}

void BmpObject::clearData()
{
	if(data_)
		delete [] data_;
}

bool BmpObject::readBmpFile(const string& filename)
{
	AUX_RGBImageRec* TextureImage[1];               // Create Storage Space For The Textures
	memset(TextureImage, 0, sizeof(AUX_RGBImageRec*) * 1);      // Set The Pointer To NULL
	
	if(!(TextureImage[0] = auxDIBImageLoad(filename.c_str())))
	{
		return false;
	}
	
	width_ = TextureImage[0]->sizeX;
	height_ = TextureImage[0]->sizeY;
	bitDepth_ = 24;	//TODO: way to find this out?

	clearData();
	data_ = new char[getImageSize()];

	memcpy(data_, TextureImage[0]->data, getImageSize());

	addAlphaChannel();

	return true;
}