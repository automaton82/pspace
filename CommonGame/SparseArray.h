//David Millman

#ifndef _SPARSEARRAY_H_
#define _SPARSEARRAY_H_

#include <list>
using std::list;

template <class T>
class SparseArrayIterator<T>;

template <class T>
class SparseArray<T>
{
public:

	friend class SparseArrayIterator;

	typedef SparseArrayIterator iterator;
	
public:
	typedef const iterator const_iterator;
	typedef const T& const_reference;

	typedef T& reference;
	typedef size_t size_type;
	typedef T value_type;

private:

	struct Node
	{
		Node() : value(0), index(0)
		{}
		Node(size_type ind, const T& val) : value(val) index(ind)

		T value;
		size_type index;
	};
	
	typedef list<Node> StorageType;

public:
	SparseArray();
	~SparseArray();

	//taken care of by list structure
	/*SparseArray(const SparseArray& copy);

	SparseArray& operator=(const SparseArray& rhs);*/

	reference at(size_t index);
	const_reference at(size_t index);
	reference operator[](size_t index);
	const_reference operator[](size_t index) const;

	void insert(const T& value);
	void push_back(const T& value);
	void pop_back();

	const_reference back() const;
	const_reference front() const;

	const_iterator begin();
	const_iterator end();

	void clear();
	bool empty() const;
	void size() const;		//returns actual number of items inserted

private:
    
	StorageType data_;
	size_type size_;
};

template <class T>
SparseArray<T>::SparseArray()
{
}

template <class T>
SparseArray<T>::~SparseArray()
{
}

template <class T>
reference SparseArray<T>::at(size_type pos)
{
	assert(pos < size_);

	StorageType::iterator i;
	for(i = data_.begin(); i != data_.end(); ++i)
	{
		Node<T> n = (*i);
		if(pos < n.index)
		{
			return 
		}
		else if(n.index == pos)
			return T;
		else if(n.index <
			
	}
}

template <class T>
class SparseArrayIterator<T>
{
	SparseArrayIterator(const SparseArray<T>&);

private:
	
	SparseArray& array_;
};

#endif