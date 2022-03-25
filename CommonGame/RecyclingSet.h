#ifndef _RECYCLINGSET_
#define _RECYCLINGSET_

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <assert.h>

/*
Description:	A bag of objects - pre-allocates space for N objects, and does not reallocate 
	on creation or deletion

IMPORTANT:  Requires that copy constructor and equals operator are implemented correctly
			Requires that the data type supports everything in Recyclable.h
Notes: experimentally, the recycling set was almost as fast as just using a vector or flat array
*/


//TODO: DONT USE THIS, IT HAS COPY CONSTRUCTOR ISSUES

template <class T>
class RecyclingSet
{
public:
	RecyclingSet(size_t maxSize = 1000);	//arbitrary max size
	~RecyclingSet();

	T* create();			//returns 0 if max size has been reached
	void release(T* object);

	void resize(size_t newMaxSize);		
	size_t size();		//number of items - not related to index
	size_t getMaxSize();

private:
	
	typedef deque<size_t> SizeList;
	typedef vector<T> ItemArray;
	
	const size_t MAX_ID;
	const size_t INVALID_ID;

	void initFreeValues(size_t oldMaxSize);
	
private:
	ItemArray items_;
	SizeList freeList_;		//next available free spot

	size_t size_;
	size_t maxSize_;
};

template <class T>
RecyclingSet<T>::RecyclingSet(size_t maxSize) :
	MAX_ID(-1),
	INVALID_ID(-1),

	items_(0),
	size_(0),
	maxSize_(0)
{
	resize(maxSize);
}

template <class T>
RecyclingSet<T>::~RecyclingSet()
{
}

template <class T>
void RecyclingSet<T>::initFreeValues(size_t oldMaxSize)
{
	SizeList newFreeList;
	SizeList::iterator i;
	size_t index=0;

	for(i=freeList_.begin(); i != freeList_.end(); ++i)
	{
		if( (*i) >= maxSize_)	//dont add invalid free spots
			break;
		else
			newFreeList.push_back(*i);
	}
	for(index=oldMaxSize; index < maxSize_; ++index)
	{
		newFreeList.push_back(index);
	}

	freeList_.clear();
	freeList_ = newFreeList;
}

template <class T>
T* RecyclingSet<T>::create()
{
	static T blank;
	T* retval = 0;

	if(freeList_.size() == 0)	//no more space
		return 0;
	
    retval = &items_[freeList_.front()];
	*retval = blank;							//clear new value
	retval->setAllocId(freeList_.front());
	freeList_.pop_front();

	++size_;

	return retval;
}

template <class T>
void RecyclingSet<T>::release(T* object)
{
	assert(object);
	assert(object->getAllocId() >= 0 && object->getAllocId() <= maxSize_);

	if(object->getAllocId() < maxSize_)
		freeList_.push_back(object->getAllocId());

	--size_;
}

template <class T>
void RecyclingSet<T>::resize(size_t newMaxSize)
{
	size_t oldSize = maxSize_;
	
	maxSize_ = newMaxSize;
	
	items_.resize(maxSize_);
	initFreeValues(oldSize);
	
	if(size_ > maxSize_)
		size_ = maxSize_;
}

template <class T>
size_t RecyclingSet<T>::size()
{
	return size_;
}

template <class T>
size_t RecyclingSet<T>::getMaxSize()
{
	return maxSize_;
}	


#endif