#ifndef _DYNAMICMULTIDATA_H_
#define _DYNAMICMULTIDATA_H_

#include <fstream>
#include <map>
#include <string>
#include <vector>

using std::multimap;
using std::pair;
using std::string;
using std::vector;

/*
Description: Loads name/value pairs - they do not have to be unique

All data is of the format:
	name = value

Comment characters: ';'
	;this is a comment
	e.g.:	a = b  ;this is also comment, but not the 'a=b'
*/


class DynamicMultiData
{
public:
	typedef vector<string> StringVector;

public:
	DynamicMultiData();
	DynamicMultiData(const string& filename);
	~DynamicMultiData();

	bool load(const string& filename, bool verbose = false);	//add support for filenames with spaces, i.e. "file 1.txt" - use quotes
	bool save(const string& filename);

	void setData(const string& name, const string& value);	//really an insert
	StringVector getData(const string& name) const;
	string getFirstData(const string& name) const;

private:
	
	bool isEndOfLine(char c);
	bool isComment(char c);
	bool isWhiteSpace(char c);

	typedef pair<string, string> StringPair;
	typedef multimap<string, string> ContainerType;
	
	ContainerType data_;
};

#endif

	