#ifndef _ITEMPOOL_
#define _ITEMPOOL_

#include <vector>
using std::vector;

/*
Description:	A bag of objects - pre-allocates space for N objects, and does not reallocate 
	on creation or deletion.  Order is unimportant

IMPORTANT:  Requires that copy constructor and equals operator are implemented correctly

Note that this class is more sensitive and less robust than RecyclingSet - but is faster
*/

//TODO: needs more testing i think

template <class T>
class ItemPool
{
public:
	ItemPool(size_t size = 1000);	//arbitrary max size
	~ItemPool();

	T* create();			//returns 0 if max size has been reached
	void release(T* object);

	void resize(size_t newSize);	//after resizing, all existing references become invalid
	size_t size() const;			//number of items left in pool
	size_t getMaxSize() const;	//number of items total

	bool validate() const;		//verifies that all items in the pool are valid
	bool isValidItem(T* item) const;
	size_t verifyItems() const;	//returns number of invalid items
	
private:
	
	typedef vector<T> ItemArray;
	typedef vector<T*> RefArray;
	
private:
	
	ItemArray items_;
	RefArray refs_;	
};

template <class T>
ItemPool<T>::ItemPool(size_t size) : 
	items_(0),
	refs_(0)
{
//	resize(size);
}

template <class T>
ItemPool<T>::~ItemPool()
{
	//assert(validate());
}

template <class T>
T* ItemPool<T>::create()
{
	static T blank;
	T* retval = 0;

	if(refs_.size() == 0)	//no more space
		return 0;

	T* temp = &items_.back();

	retval = refs_.back();
	*retval = blank;
	refs_.pop_back();

	return retval;
}

template <class T>
void ItemPool<T>::release(T* object)
{
	assert(object);

	//assert(isValidItem(object));		//check if ref belongs in the pool

	refs_.push_back(object);
}

template <class T>
void ItemPool<T>::resize(size_t newSize)
{
	size_t oldSize = items_.size();
	size_t i;

	items_.resize(newSize);
	refs_.resize(newSize);		

	for(i=0; i < newSize; ++i)	//get new references
	{
		refs_[i] = &items_[i];
	}
}

template <class T>
size_t ItemPool<T>::size() const
{
	return refs_.size();
}

template <class T>
size_t ItemPool<T>::getMaxSize() const
{
	return items_.size();
}

template <class T>
bool ItemPool<T>::validate() const
{
	size_t invalid = verifyItems();
	bool retval = false;

	if(invalid == 0)
		retval = true;

	return retval;
}

template <class T>
bool ItemPool<T>::isValidItem(T* item) const
{
	ItemPool<T>::ItemArray::const_iterator i;
	bool retval = false;

	for(i = items_.begin(); i != items_.end(); ++i)
	{
		if( &(*i) == item )
		{
			retval = true;
			break;
		}
	}
		
	return retval;
}

template <class T>
size_t ItemPool<T>::verifyItems() const
{
	size_t invalid = 0;

	ItemPool<T>::RefArray::const_iterator i;
	for(i=refs_.begin(); i != refs_.end(); ++i)
	{
		if(!isValidItem(*i))
			++invalid;
	}

	return invalid;
}


#endif