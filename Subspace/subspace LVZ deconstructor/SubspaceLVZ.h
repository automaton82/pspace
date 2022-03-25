#ifndef _SUBSPACEDATA_
#define _SUBSPACEDATA_

#include <fstream>
#include <string>
#include <vector>

using std::ifstream;
using std::string;
using std::vector;

#include "SubspaceLVZData.h"

class SubspaceLVZ
{
public:
	SubspaceLVZ();
	SubspaceLVZ(const string& filename);
	~SubspaceLVZ();

	bool load(const string& filename);
	bool load(const string& filename, const string& outputPath, bool saveOnExtract = true);		// saves extraction to outputPath

	const vector<string>& getImageFiles() const;
	const vector<LVZImageObject>& getImageObjects() const;
	const vector<LVZMapObject>& getMapObjects() const;
	const vector<LVZScreenObject>& getScreenObjects() const;
    
private:

	void clear();
	bool readFileObject(FILE* file, bool saveOnExtract);
	
	size_t loadCLVObject(unsigned char* data);
	size_t loadImageObject(unsigned char* data);
	bool loadScriptObject(unsigned char* data, size_t size);

	bool saveData(const string& filename, unsigned char* data, size_t size);
	static bool uncompressLVZ(unsigned char* compressedData, size_t compressedSize, unsigned char*& data, size_t* size);

	union TypeName
	{
		char bytes[4];
		unsigned __int32 u;
	};
	
    static const TypeName contTypeName;
	static const TypeName objectTypeCLV1;
	static const TypeName objectTypeCLV2;

private:
	static bool verbose_;

	string filename_;
	string outputPath_;
		
	vector<LVZMapObject> mapObjects_;
	vector<LVZScreenObject> screenObjects_;
	vector<LVZImageObject> imageObjects_;
	vector<string> imageFiles_;
	
};

#endif