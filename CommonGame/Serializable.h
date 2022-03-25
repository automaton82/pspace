#ifndef _SERIALIZABLE_H_
#define _SERIALIZABLE_H_

#include <string>
#include "DataTypes.h"

using std::string;

class Serializable
{
public:

	virtual ~Serializable()
	{}

	virtual string serialize() const = 0;
	virtual void deserialize(const string& s) = 0;

	/*virtual Uint getSerializationSize() = 0;
	virtual void getSerializationData(string& begin) = 0;*/
};

#endif