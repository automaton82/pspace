// David Millman

#include "DynamicData.h"

DynamicData::DynamicData() :
	verbose_(true)
{
}

DynamicData::DynamicData(const string& filename) : 
	verbose_(true)
{
	load(filename);
}

DynamicData::~DynamicData()
{
}

bool DynamicData::load(const string& filename)
{
	char c = 0;
    string name, value;
	std::ifstream file(filename.c_str());

	if(!file)
		return false;

	if(verbose_)
	{
		printf("Loading \"%s\"\n", filename.c_str()); 
	}

	while(file && !file.eof())
	{
		name.clear();
		value.clear();

		file.get(c);
		while(c != '=' && !isEndOfLine(c) && !isComment(c) && !file.eof())		//look for end of line or comments
		{
			if(!isWhiteSpace(c))	//ignore whitespace
				name += c;
			file.get(c);
		}

		if(name.length() > 0)	//found named value
		{
			file.get(c);
			while(!isEndOfLine(c) && !isComment(c) && !file.eof())		//look for end of line or comments
			{
				if(!isWhiteSpace(c))	//ignore whitespace
					value += c;
				file.get(c);
			}
		}
		
		while(!isEndOfLine(c) && !file.eof())		//ignore rest of line
			file.get(c);
					
		if(name.length() > 0)	//insert value
		{
			data_[name] = value;
			if(verbose_)
                printf("  %s = %s\n", name.c_str(), value.c_str());
		}
	}

	return true;
}

bool DynamicData::save(const string& filename)
{
	return false;
}

void DynamicData::setData(const string& name, const string& value)
{
	data_[name] = value;
}

const string& DynamicData::getData(const string& name)
{
	return data_[name];
}

bool DynamicData::isEndOfLine(char c)
{
	if(c == '\n' || c == '\r')
		return true;
	else 
		return false;
}

bool DynamicData::isComment(char c)
{
	if(c == ';')
		return true;
	else
		return false;
}

bool DynamicData::isWhiteSpace(char c)
{
	if(	c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return true;
	else
		return false;
}