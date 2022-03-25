#ifndef _DYNAMICDATA_H_
#define _DYNAMICDATA_H_

#include <map>
#include <string>
#include <fstream>

using std::string;
using std::map;
using std::pair;

/*
Description: Loads name/value pairs - non-unique pairs take the last value

All data is of the format:
	name = value

Comment characters: ';'
	;this is a comment
	e.g.:	a = b  ;this is also comment, but not the 'a=b'
*/

class DynamicData
{
public:
	DynamicData();
	DynamicData(const string& filename);
	~DynamicData();

	bool load(const string& filename);
	bool save(const string& filename);

	void setData(const string& name, const string& value);
	const string& getData(const string& name);

private:
	
	bool isEndOfLine(char c);
	bool isComment(char c);
	bool isWhiteSpace(char c);

	typedef map<string, string> ContainerType;
	
	ContainerType data_;
	bool verbose_;					// TODO: make this a parameter
};

#endif

	