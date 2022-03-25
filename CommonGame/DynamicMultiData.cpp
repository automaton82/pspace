#include "DynamicMultiData.h"

DynamicMultiData::DynamicMultiData()
{
}

DynamicMultiData::DynamicMultiData(const string& filename)
{
	load(filename);
}

DynamicMultiData::~DynamicMultiData()
{
}

bool DynamicMultiData::load(const string& filename, bool verbose)
{
	char c = 0;
    string name, value;
	std::ifstream file(filename.c_str());

	if(!file)
		return false;

	while(file && !file.eof())
	{
		name.clear();
		value.clear();

		file.get(c);
		while(c != '=' && !isEndOfLine(c) && !isComment(c) && !file.eof())		//look for end of line or comments or '='
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
			setData(name, value);
			if(verbose)
				printf("%s = %s\n", name.c_str(), value.c_str());
		}
	}

	return true;
}

bool DynamicMultiData::save(const string& filename)
{
	return false;
}

void DynamicMultiData::setData(const string& name, const string& value)
{
	data_.insert(StringPair(name, value));
}

DynamicMultiData::StringVector DynamicMultiData::getData(const string& name) const
{
	StringVector retval;
	ContainerType::const_iterator lower = data_.lower_bound(name);
	ContainerType::const_iterator upper = data_.upper_bound(name);

	for(lower; lower != upper; ++lower)
	{
		string val = (*lower).second;
		retval.push_back(val);
	}
	
	return retval;
}

string DynamicMultiData::getFirstData(const string& name) const
{
	string retval;
	ContainerType::const_iterator lower = data_.lower_bound(name);
	ContainerType::const_iterator upper = data_.upper_bound(name);

	if(lower != upper)				//take first value
		retval = (*lower).second;

	return retval;
}

bool DynamicMultiData::isEndOfLine(char c)
{
	if(c == '\n' || c == '\r')
		return true;
	else 
		return false;
}

bool DynamicMultiData::isComment(char c)
{
	if(c == ';')
		return true;
	else
		return false;
}

bool DynamicMultiData::isWhiteSpace(char c)
{
	if(	c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return true;
	else
		return false;
}