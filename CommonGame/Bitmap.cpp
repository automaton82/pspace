//David Millman

#include "Bitmap.h"

Bitmap::Bitmap() : colorIndices_(0), rawData_(0), colorData_(0)
{
	currentByte_ = 0;
}

/**
* Windows specific function to load a bitmap from an already open file handle
* @param h handle to load from
* @return true on a successful load, false otherwise
*/
bool Bitmap::load(FILE* file)
{
	/*BOOL readResult;
	
	unsigned long bytesRead = 0;
	
	int numBytes = 0;
	int bytesPerRow = 0;
	int padding = 0;

	readResult = fread(ReadFile(h, &fileHeader_, sizeof(BITMAPFILEHEADER), &bytesRead, NULL);
	if(readResult == FALSE)
	{
		printf("Error reading bitmap file header!\n");
		return false;
	}
	currentByte_ += bytesRead;
	
	if(!isBitmapType(fileHeader_.bfType))
	{
		printf("Unknown file type: %c%c\n", (fileHeader_.bfType & 0xFF00) >> 8, fileHeader_.bfType & 0x00FF);
		return false;
	}

	readResult = ReadFile(h, &infoHeader_, sizeof(BITMAPINFOHEADER), &bytesRead, NULL);
	if(readResult == FALSE)
	{
		printf("Error reading bitmap info header!\n");
		return false;
	}
	currentByte_ += bytesRead;

	if(infoHeader_.biBitCount != 24)
	{
		if(!readColorIndexes(h))
		{
			printf("Error reading color indices\n");
			return false;
		}
	}

	numBytes = infoHeader_.biSizeImage;
		
	rawData_ = new unsigned char[numBytes];
	readResult = ReadFile(h, &rawData_[0], sizeof(unsigned char) * numBytes, &bytesRead, NULL);
	if(readResult == FALSE)
	{
		printf("Error reading color data\n");
		return false;
	}
	currentByte_ += bytesRead;
	
	uncompressRawData();
	convertRawData();*/

	return true;
}

/**
* Loads a bitmap from a file
* @param filename the file to load
* @return true on a successful load, false otherwise
*/
bool Bitmap::loadFromFile(std::string filename)
{
	bool result;
	HANDLE fileHandle = CreateFile(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if(fileHandle == INVALID_HANDLE_VALUE)
	{
		printf("Error opening file: %s\n", filename.c_str());
		CloseHandle(fileHandle);
		result = false;
	}
	else
	{
		result = loadFromHandle(fileHandle);
		CloseHandle(fileHandle);
	}
		
	return result;
}	

/**
* Windows specific function to load a bitmap from an already open file handle
* @param h handle to load from
* @return true on a successful load, false otherwise
*/
bool Bitmap::loadFromHandle(HANDLE h)
{
	BOOL readResult;
	
	unsigned long bytesRead = 0;
	
	int numBytes = 0;
	int bytesPerRow = 0;
	int padding = 0;

	readResult = ReadFile(h, &fileHeader_, sizeof(BITMAPFILEHEADER), &bytesRead, NULL);
	if(readResult == FALSE)
	{
		printf("Error reading bitmap file header!\n");
		return false;
	}
	currentByte_ += bytesRead;
	
	if(!isBitmapType(fileHeader_.bfType))
	{
		printf("Unknown file type: %c%c\n", (fileHeader_.bfType & 0xFF00) >> 8, fileHeader_.bfType & 0x00FF);
		return false;
	}

	readResult = ReadFile(h, &infoHeader_, sizeof(BITMAPINFOHEADER), &bytesRead, NULL);
	if(readResult == FALSE)
	{
		printf("Error reading bitmap info header!\n");
		return false;
	}
	currentByte_ += bytesRead;

	if(infoHeader_.biBitCount != 24)
	{
		if(!readColorIndexes(h))
		{
			printf("Error reading color indices\n");
			return false;
		}
	}

	numBytes = infoHeader_.biSizeImage;
		
	rawData_ = new unsigned char[numBytes];
	readResult = ReadFile(h, &rawData_[0], sizeof(unsigned char) * numBytes, &bytesRead, NULL);
	if(readResult == FALSE)
	{
		printf("Error reading color data\n");
		return false;
	}
	currentByte_ += bytesRead;
	
	uncompressRawData();
	convertRawData();

	return true;
}

bool Bitmap::isBitmapType(int t)
{
	if(t == 19778) return true;
	else return false;
}

bool Bitmap::isCompressed(int c)
{
	if(c == BI_RGB) return false;
	else return true;
}

bool Bitmap::readColorIndexes(HANDLE h)
{
	BOOL readResult;
	DWORD bytesRead;
	int colors = (int)pow(2, infoHeader_.biBitCount);
		
	colorIndices_ = new RGBQUAD[colors];
	readResult = ReadFile(h, &colorIndices_[0], sizeof(RGBQUAD) * colors, &bytesRead, NULL);

	if(readResult == FALSE)	return false;
	currentByte_ += bytesRead;

	return true;
}

/**
* Used to convert the loaded bitmap data to something that opengl can use
* @return true on a successful conversion, false otherwise
*/
bool Bitmap::convertRawData()
{
	int bitsPerPixel = infoHeader_.biBitCount;
	int numPixels = infoHeader_.biWidth * infoHeader_.biHeight;
	int pixelsPerByte = 8 / infoHeader_.biBitCount;
	
	colorData_ = new unsigned char[numPixels * 3];
	if(colorIndices_)
	{
		if(bitsPerPixel == 1 || bitsPerPixel == 4 || bitsPerPixel == 8)
		{
			for(int n=0; n < numPixels; n++)
			{
				///BYTES: [0xFF][0XFF][0xFF][0xFF]
				//1 bit per pixel -> 8 pixels per byte
				unsigned char currentByte = rawData_[n / pixelsPerByte];
				unsigned char index = ( currentByte >> (pixelsPerByte-1 - n%pixelsPerByte)*pixelsPerByte ) & (unsigned char)pow(2,bitsPerPixel)-1;

				int x = n % infoHeader_.biWidth;
				int y = n / infoHeader_.biWidth;
				int p1 = (y*infoHeader_.biWidth + x)*3;
				int p2 = ((infoHeader_.biHeight-1-y)*infoHeader_.biWidth + x)*3;
								
				colorData_[p1]		= colorIndices_[index].rgbRed;
				colorData_[p1 + 1]	= colorIndices_[index].rgbGreen;
				colorData_[p1 + 2]	= colorIndices_[index].rgbBlue;
			}
		}
		else
		{
		
			printf("Unhandled bit depth\n");
			return false;
		}
	}
	else
	{
		for(int n=0; n < numPixels; n++)
		{
			int x = n % infoHeader_.biWidth;
			int y = n / infoHeader_.biWidth;
			int p1 = (y*infoHeader_.biWidth + x)*3;
			int p2 = (y*infoHeader_.biWidth + x)*3;

			//image is actually upside down, but opengl reads it that way for textures
			colorData_[p1 + 2]	= rawData_[p2];
			colorData_[p1 + 1]	= rawData_[p2 + 1];
			colorData_[p1]		= rawData_[p2 + 2];
		}
	}

	return true;
}

unsigned char* Bitmap::getTileData(int n, int tileWidth, int tileHeight)
{
	int width = infoHeader_.biWidth;
	int height = infoHeader_.biHeight;
	int x = n*tileWidth % infoHeader_.biWidth;
	int y = n*tileHeight / infoHeader_.biWidth * tileHeight;

	//flip y axis
	y = infoHeader_.biHeight - y - tileHeight;
	unsigned char* tileData;

	if(n < 0 || n >= width*height) return 0;

	tileData = new unsigned char[width * height * 3];
	for(int j=0; j < tileHeight; j++)
	{
		for(int i=0; i < tileWidth; i++)
		{
			int dataPixNew = ((j)*tileWidth + i) * 3;
			int dataPix = ((y+j)*width + (x+i)) * 3;

			if(y+j < height && x+i < width)
			{
				tileData[dataPixNew]	= colorData_[dataPix];
				tileData[dataPixNew+1]	= colorData_[dataPix+1];
				tileData[dataPixNew+2]	= colorData_[dataPix+2];
			}
			else
			{
				tileData[dataPixNew] = 255;
				tileData[dataPixNew+1] = 255;
				tileData[dataPixNew+2] = 255;
			}
		}
	}

	return tileData;
}

bool Bitmap::uncompressRawData()
{
	unsigned char* newData;
	if(infoHeader_.biCompression == BI_RGB)
	{
		//nothing
	}
	else if(infoHeader_.biCompression == BI_RLE8)
	{
		unsigned long totalBytes = infoHeader_.biWidth*infoHeader_.biHeight * infoHeader_.biBitCount/8;
		newData = new unsigned char[totalBytes] ;
		unsigned long index = 0;
		for(unsigned long i=0; i < infoHeader_.biSizeImage; i+=2)
		{
			unsigned char repeats = rawData_[i];
			unsigned char color = rawData_[i+1];
			
			for(unsigned char r=0; r < repeats; r++)
			{
				newData[index++] = color;
			}
			//printf("index: %d\n", index);
		}

		if(rawData_) delete [] rawData_;
		rawData_ = newData;
		
		infoHeader_.biCompression = BI_RGB;
	}
	else
	{
		printf("Unhandled compression type: %d\n", infoHeader_.biCompression);
		return false;
	}

	int bytesPerRow = infoHeader_.biWidth * infoHeader_.biBitCount / 8;
	int numBytes = bytesPerRow * infoHeader_.biHeight;
	int padding = 4 - (bytesPerRow % 4);
	
	if(padding == 4) padding = 0;

	newData = new unsigned char[numBytes];

	//TODO: add support for color indices

	for(int r=0; r < infoHeader_.biHeight; r++)
	{	
		for(int c=0; c < bytesPerRow; c++)
		{
			newData[r*bytesPerRow + c] = rawData_[r*bytesPerRow + c];
		}
	}

	if(rawData_) delete [] rawData_;
	rawData_ = newData;

	return true;
}