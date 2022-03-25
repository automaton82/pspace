#include "PngObject.h"

#include <assert.h>

#include "png.h"

PngObject::PngObject() : 
	width_(0), height_(0),
	data_(0), 
	bitDepth_(0)
{
}

PngObject::PngObject(const string& file) : 
	width_(0), height_(0),
	data_(0), 
	bitDepth_(0)
{
	load(file);
}

PngObject::~PngObject()
{

}

unsigned int PngObject::getBitDepth() const
{
	return bitDepth_;
}

char* PngObject::getData() const
{
	return data_;
}

unsigned int PngObject::getImageSize() const
{
	return width_*height_ * sizeof(char);
}

unsigned int PngObject::getHeight() const
{
	return height_;
}

unsigned int PngObject::getWidth() const
{
	return width_;
}

bool PngObject::load(const string& filename)
{
	FILE* file = 0;

	file = fopen(filename.c_str(), "rb");
	if(!file)
		return false;
		
	if(!isPngFile(file))
	{
		fclose(file);
		return false;
	}

	if(!readPngFile(file))
	{
		fclose(file);
		return false;
	}

	fclose(file);

	return true;
}

void PngObject::clearData()
{
	if(data_)
		delete [] data_;
}

bool PngObject::isPngFile(FILE* file)
{
	char buffer[pngBytesToCheck];
	
	size_t bytesRead;

	bytesRead = fread(buffer, 1, pngBytesToCheck, file);	//read signature
	if(bytesRead != pngBytesToCheck)
		return false;

	return (png_sig_cmp((png_bytep)buffer, (png_size_t)0, pngBytesToCheck) == 0);	//returns 0 on success
}

bool PngObject::readPngFile(FILE* file)
{
	png_structp pngPtr;
	png_infop infoPtr;

	pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)0, 0, 0);	//use the default stderr and longjump method
	/*pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		(png_voidp)user_error_ptr, user_error_fn, user_warning_fn);*/
	if(!pngPtr)
		return false;

	infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr)
	{
		png_destroy_read_struct(&pngPtr, png_infopp_NULL, png_infopp_NULL);
		return false;
	}

	png_set_sig_bytes(pngPtr, pngBytesToCheck);	//from the file check

#ifndef png_jmpbuf
#  define png_jmpbuf(pngPtr) ((pngPtr)->jmpbuf)
#endif

	if(setjmp(png_jmpbuf(pngPtr)))	//0 on success, non-zero on failure
	{
        png_destroy_read_struct(&pngPtr, &infoPtr, png_infopp_NULL);
		return false;
	}

	int pngTransforms = PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_PACKING;	//convert to raw 32-bit data

	png_init_io(pngPtr, file);

	int filler = 0xffff;
	png_set_filler(pngPtr, filler, PNG_FILLER_AFTER);	//add alpha channel

	png_read_png(pngPtr, infoPtr, pngTransforms, png_voidp_NULL);

	width_ = png_get_image_width(pngPtr, infoPtr);
	height_ = png_get_image_height(pngPtr, infoPtr);
	bitDepth_ = png_get_bit_depth(pngPtr, infoPtr);

	int colorType = png_get_color_type(pngPtr, infoPtr);
	int channels = png_get_channels(pngPtr, infoPtr);
	
	bitDepth_ *= channels;	//convert channelbit depth to overall bitdepth

	assert(bitDepth_ == 32);	
	
	png_bytepp rowPointers;
	rowPointers = png_get_rows(pngPtr, infoPtr);

	clearData();
	size_t imageSize = width_ * height_ * bitDepth_ / 8;
	data_ = new char[imageSize];
	//memcpy(data_, rowPointers, imageSize);

	for(size_t i=0; i < height_; ++i)	//copy each row
	{
		memcpy(&data_[width_*i*4], rowPointers[height_-i-1], width_ * 4);
	}

	png_destroy_read_struct(&pngPtr, &infoPtr, png_infopp_NULL);	// clean up after the read, and free any memory allocated

	return true;
}