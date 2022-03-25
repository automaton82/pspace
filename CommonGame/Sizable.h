#ifndef _SIZABLE_H_
#define _SIZABLE_H_

#include "DataTypes.h"

//TODO: make multi-dimensional size, or bounding volume?, or hierarchial
//TODO: incoporate physics model for size

class Sizable
{
public:

	Sizable();
	virtual ~Sizable();

	//acts like radius, could do bounding boxes here?
	virtual Uint getSize() const;
	virtual void setSize(Uint size);

private:
	Uint size_;
};

#endif