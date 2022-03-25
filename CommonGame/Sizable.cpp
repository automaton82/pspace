#include "Sizable.h"

Sizable::Sizable() : 
	size_(1)	
{
}

Sizable::~Sizable()
{
}

Uint Sizable::getSize() const
{
	return size_;
}

void Sizable::setSize(Uint size)
{
	size_ = size;
}
