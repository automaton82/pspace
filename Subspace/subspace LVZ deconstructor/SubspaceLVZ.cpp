#include "SubspaceLVZ.h"

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>
using namespace std;

#include "zlib.h"

const SubspaceLVZ::TypeName SubspaceLVZ::contTypeName = {'C','O','N','T'};
const SubspaceLVZ::TypeName SubspaceLVZ::objectTypeCLV1 = {'C','L','V','1'};
const SubspaceLVZ::TypeName SubspaceLVZ::objectTypeCLV2 = {'C','L','V','2'};

bool SubspaceLVZ::verbose_ = false;

SubspaceLVZ::SubspaceLVZ()
{
}

SubspaceLVZ::SubspaceLVZ(const string& filename)
{
	load(filename);
}

SubspaceLVZ::~SubspaceLVZ()
{
}

bool SubspaceLVZ::load(const string& filename)
{
	return load(filename, "", false);
}

bool SubspaceLVZ::load(const string& filename, const string& outputPath, bool saveOnExtract)
{
	clear();

	outputPath_ = outputPath;

	FILE* file;
	size_t bytesRead = 0;
	
	file = fopen(filename.c_str(), "rb");
	if(!file)
	{	
		cout << "Failed to open file \"" << filename << "\"." << endl;
		return false;
	}

	filename_ = filename;

	// file header
	LVZFileHeader fileHeader;
	bytesRead = fread((char*)&fileHeader, 1, sizeof(fileHeader), file);
	if(bytesRead != sizeof(fileHeader))
	{
		cout << "Invalid file header." << endl;
		return false;
	}
	if(!fileHeader.fileType == contTypeName.u)
	{
		cout << "Not an LVZ file." << endl;
		return false;
	}
	
	// file objects
	for(int i=0; i < fileHeader.count; ++i)
	{
        readFileObject(file, saveOnExtract);
	}

	fclose(file);
	return true;
}

const vector<string>& SubspaceLVZ::getImageFiles() const
{
	return imageFiles_;
}

const vector<LVZImageObject>& SubspaceLVZ::getImageObjects() const
{
	return imageObjects_;
}

const vector<LVZMapObject>& SubspaceLVZ::getMapObjects() const
{
	return mapObjects_;
}

const vector<LVZScreenObject>& SubspaceLVZ::getScreenObjects() const
{
	return screenObjects_;
}

void SubspaceLVZ::clear()
{
	filename_.clear();
	
	mapObjects_.clear();
	screenObjects_.clear();
	imageObjects_.clear();
	imageFiles_.clear();
}

bool SubspaceLVZ::readFileObject(FILE* file, bool saveOnExtract)
{
	// read compressed header
	LVZFileObject fileObject;
	size_t bytesRead = fread((char*)&fileObject, 1, sizeof(fileObject), file);
	if(bytesRead != sizeof(fileObject))
	{
		cout << "Invalid compressed header." << endl;
		return false;
	}
	if(verbose_)
	{
		cout << "File object: compressed=" << fileObject.compressedSize 
				<< "  decompressed=" << fileObject.decompressedSize 
				<< "  timestamp=" << fileObject.timestamp 
				<< endl;
	}

	// read filename
	string filename;
	
	int c = 0;
	fread(&c, 1, 1, file);
	while(c != '\0')
	{
		filename += c;
		fread(&c, 1, 1, file);
	}

	if(verbose_)
	{
		if(filename.size() > 0)
			cout << "Filename: " << filename << endl;
		else
			cout << "Script object." << endl;
	}

	// read compressed data
	unsigned char* compressedData = new unsigned char[fileObject.compressedSize];
	bytesRead = fread((char*)compressedData, 1, fileObject.compressedSize, file);
	if(bytesRead != fileObject.compressedSize)
	{
		cout << "Invalid compressed data." << endl;
		return false;
	}

	// uncompress file data
	unsigned char* data = 0;
	size_t size = 0;
	if(fileObject.compressedSize == fileObject.decompressedSize)
	{
		data = new unsigned char[fileObject.decompressedSize];
		memcpy(data, compressedData, fileObject.compressedSize);
		size = fileObject.decompressedSize;
	}
	else if(!uncompressLVZ(compressedData, fileObject.compressedSize, data, &size))
	{
		return false;
	}

	if(size != fileObject.decompressedSize)
	{
		cout << "Incorrect decompressed: size=" << size << ", decompressedSize=" << fileObject.decompressedSize << endl;
		return false;
	}

	if(fileObject.timestamp == 0 && filename.size() == 0)
	{
		loadScriptObject(data, size);
	}
	else if(saveOnExtract)
	{
		// note: filename_ is the lvz filename, filename is the output object, yes it's confusing
		/*size_t b1 = filename_.find_last_of("/");
		size_t b2 = filename_.find_last_of("\\");

		if(b1 == filename_.npos) b1 = filename_.length();
		if(b2 == filename_.npos) b2 = filename_.length();
		size_t nameStart = min(b1, b2);

		if(nameStart == filename_.npos)
			nameStart = 0;

		string path;
		if(outputPath_.length() == 0)
			path = filename_.substr(0, nameStart+1);
		else
			path = outputPath_;*/
		string path = outputPath_;

		CreateDirectory(path.c_str(), 0);
		if(!saveData(path + filename, data, size))
		{
			return false;
		}
	}
	else
	{
		// cache file
	}

	delete [] compressedData;
	delete [] data;

	return true;
}

size_t SubspaceLVZ::loadCLVObject(unsigned char* data)
{
	LVZBasicObject* obj = (LVZBasicObject*)&data[0];
	if(obj->isMapObject)
	{
		LVZMapObject* map = (LVZMapObject*)obj;
		mapObjects_.push_back( *map );

		if(verbose_)
			cout << "Map object: [" << map->objectID << "] (" << map->xCoord << "," << map->yCoord << "), image[" << map->imageID << "] " << endl;		
	}
	else
	{
		LVZScreenObject* screen = (LVZScreenObject*)obj;
		screenObjects_.push_back( *screen );
		if(verbose_)
		{
			cout << "Screen object: [" << screen->objectID << "] (" 
				<< screen->xType << "," << screen->yType << ":" 
				<< screen->xCoord << "," << screen->yCoord 
				<< "), image[" << screen->imageID << "] " << endl;
		}
	}

	return sizeof(LVZBasicObject);
}

size_t SubspaceLVZ::loadImageObject(unsigned char* data)
{
	static imgID = 0;
	size_t offset = 0;
	LVZImageObject* img = (LVZImageObject*)&data[0];
	imageObjects_.push_back(*img);

	offset += sizeof(LVZImageObject);
	//offset += 6;

	string filename;
	int c = data[offset++];
	while(c != '\0')
	{
		filename += c;
		c = data[offset++];
	}
	if(verbose_)
		cout << "Image object: [" << imgID++ << "] " << filename << endl;

	imageFiles_.push_back(filename);

	return offset;
}

bool SubspaceLVZ::loadScriptObject(unsigned char* data, size_t size)
{
	size_t objSize = 0, imgSize = 0;
	size_t offset = 0;
	LVZScriptHeader* scriptHeader = (LVZScriptHeader*)&data[0];
	offset += sizeof(LVZScriptHeader);

	if(verbose_)
		cout << scriptHeader->numObjects << " objects, " << scriptHeader->numImages << " images." << endl;

    if(scriptHeader->objectType == objectTypeCLV1.u)
	{
		cout << "CLV1 objects found." << endl;
		for(int i=0; i < scriptHeader->numObjects; ++i)
		{
			objSize = loadCLVObject(&data[offset]);
			if(objSize <= 0)
				return false;
			else
                offset += objSize;
		}
	}
	else if(scriptHeader->objectType == objectTypeCLV2.u)
	{
		cout << "CLV2 objects found." << endl;
		for(int i=0; i < scriptHeader->numObjects; ++i)
		{
			objSize = loadCLVObject(&data[offset]);
			if(objSize <= 0)
				return false;
			else
                offset += objSize;
		}
	}
	else
	{
		cout << "Unknown LVZ script type: " << scriptHeader->objectType << endl;
		return false;
	}

	/*cout << "String: ";
	for(int k=offset; k < size; ++k)
	{
		cout << (char)data[k];
	}
	cout << endl;*/

	for(int j=0; j < scriptHeader->numImages; ++j)
	{
		imgSize = loadImageObject(&data[offset]);
		if(imgSize <= 0)
			return false;
		else
			offset += imgSize;
	}

	return true;
}

bool SubspaceLVZ::saveData(const string& filename, unsigned char* data, size_t size)
{
	FILE* file = 0;
	file = fopen(filename.c_str(), "wb");
	if(!file)
	{
		cout << "Failed to save file \"" << filename << "\"." << endl;
		return false;
	}

	size_t bytesWritten = 0;
	bytesWritten = fwrite(data, 1, size, file);
	if(bytesWritten != size)
	{
		cout << "Error writing to file \"" << filename << "\"." << endl;
		fclose(file);
		return false;
	}

	cout << "Saved file \"" << filename << "\"." << endl;

	fclose(file);
	return true;
}

bool SubspaceLVZ::uncompressLVZ(unsigned char* compressedData, size_t compressedSize, unsigned char*& data, size_t* size)
{
	int status = 0;
	
	assert(compressedSize > 0);
	
	//zlib stuff
	uLongf dsize;
	dsize = compressedSize;
	do
	{
		if(data)
			delete [] data;
		
		dsize *= 2;
		data = new unsigned char[dsize];		

		status = uncompress(data, &dsize, compressedData, compressedSize);
	} 
	while(status == Z_BUF_ERROR);
	*size = dsize;

	if(status < Z_OK)
	{
		cout << "Error decompressing data." << endl;
		return false;
	}
	
	return true;
}