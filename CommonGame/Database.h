#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <vector>
#include <list>
#include <map>

using std::vector;
using std::list;
using std::map;
using std::pair;

//TODO: this thing is such shit - fix it up, add iterators, make it use hashing, make it not crappy

//TODO make virtual database class?
//TODO replace this with a real database class, this one has memory leaks and is slow and faulty

template <class T>
class Database
{
public:
	Database();
	virtual ~Database();

	virtual T* operator[](int itemID);

	virtual int addItem(T* newItem);
	virtual int addItem(T* newItem, int itemID);
	virtual bool deleteItem(int itemID);
	virtual T* removeItem(int itemID);			//removes from database but leaves item in memory
	virtual T* getItem(int itemID);
	virtual const T* getItem(int itemID) const;

	virtual void clear();

	virtual int numItems() const;

	virtual int size() const;

private:
	typedef map<int, T*> ContainerType;
	//vector<T*> data;

	ContainerType data;
	unsigned int numItems_;

	//list<int> freePositions;
};

template <class T>
Database<T>::Database()
{
}

template <class T>
Database<T>::~Database() 
{
	this->clear();
}

template <class T>
T* Database<T>::operator [](int itemID)
{
	/*if(itemID < 0 || itemID >= this->size())
		return NULL;*/

	return data[itemID];
}

template <class T>
int Database<T>::addItem(T* newItem)
{
	if(newItem == NULL)
		return -1;

	int id = static_cast<int>(data.size());
	pair< ContainerType::iterator, bool> result = data.insert( ContainerType::value_type(id, newItem) );

	++numItems_;

	return (*(result.first)).first;
}

template <class T>
int Database<T>::addItem(T* newItem, int itemID)
{
	if(newItem == NULL)
		return -1;

	data[itemID] = newItem;
	++numItems_;

	return itemID;
}

template <class T>
T* Database<T>::getItem(int itemID)
{
	if(itemID < 0 || itemID >= size())
		return NULL;

	return data[itemID];
}

template <class T>
const T* Database<T>::getItem(int itemID) const
{
	if(itemID < 0 || itemID >= size())
		return NULL;

	ContainerType::const_iterator i = data.find(itemID);
	if(i == data.end())
		return NULL;
	
    return (*i).second;
}

template <class T>
void Database<T>::clear()
{
	//vector<T*>::iterator i;
	ContainerType::iterator i;
	for(i = data.begin(); i != data.end(); ++i)
	{
		T* item = (*i).second;

		delete item;
		item = NULL;
	}
	//data.resize(0);
	numItems_ = 0;

	//freePositions.clear();
}

template <class T>
bool Database<T>::deleteItem(int itemID)
{
	T* item = data[itemID];

	if(item)
	{
		delete item;
		data[itemID] = NULL;
		data.erase(itemID);
		return true;
	}
	--numItems_;

	return false;
}

template <class T>
T* Database<T>::removeItem(int itemID)
{
	if(itemID < 0 || itemID > size()-1)
		return NULL;

	T* item = data[itemID];
	data[itemID] = NULL;
	data.erase(itemID);

	--numItems_;

	return item;
}

/*template <class T>
bool Database<T>::addSubscriber(DatabaseSubscriber<T>* subscriber)
{
	if(!subscriber)
		return false;

	subscribers.push_back(subscriber);

	return true;
}

template <class T>
void Database<T>::notifySubscribers(int changeID, bool doDelete)
{
	SubscriberList::iterator i;
	for(i = subscribers.begin(); i != subscribers.end(); ++i)
	{	
		(*i)->setDatabase(this);
		(*i)->updateData(changeID, doDelete);
	}
}*/

template <class T>
int Database<T>::numItems() const
{
	return numItems_;
	//return static_cast<int>(data.size());
}

template <class T>
int Database<T>::size() const
{
	return static_cast<int>(data.size());
}

#endif