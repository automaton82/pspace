#ifndef _RECYCLABLE_
#define _RECYCLABLE_


//////////////////////////////////////////////////////////////////////////////////
//Description:	For use with RecyclingSet - provides necessary functions
//
//IMPORTANT:  Requires that copy is implemented correctly (equals operator)
//////////////////////////////////////////////////////////////////////////////////

class Recyclable
{
public:

	const size_t INVALID_ID;

public:
	Recyclable();
	virtual ~Recyclable();
	
	size_t getAllocId() const;
	void setAllocId(size_t id);

private:
    size_t allocId_;
};

Recyclable::Recyclable() :
	INVALID_ID(-1)
{
	allocId_ = INVALID_ID;
}

Recyclable::~Recyclable()
{
}

size_t Recyclable::getAllocId() const
{
	return allocId_;
}

void Recyclable::setAllocId(size_t id)
{
	allocId_ = id;
}

#endif