#ifndef _SPARSEMATRIX_H_
#define _SPARSEMATRIX_H_

#include <map>
using std::map;

template <class T, class Allocator = std::allocator<T> >
class SparseMatrix
{
public:
 
	typedef Allocator						allocator_type;
    typedef typename Allocator::size_type	size_type;
    typedef typename Allocator::difference_type	difference_type;
    typedef typename Allocator::reference		reference;
    typedef typename Allocator::const_reference	const_reference;

    // Our iterator will be a simple pointer
    typedef Allocator::pointer iterator;
    typedef Allocator::const_pointer iterator;

protected:
    Allocator the_allocator;

public:
	SparseMatrix();
	SparseMatrix(size_type size);

	T& at(size_type x, size_type y)
	{
	}

	const T& at(size_type x, size_type y) const
	{
	}

	// iterators go across columns, then down rows
    iterator begin() 
	{ 
	}

    iterator end() 
	{ 
	} 

	void clear();
	void insert(size_type x, size_type y, const T& value)	

	T& operator[][] (size_type x, size_type y);
	const T& operator[][] (size_type x, size_type y) const;

private:

	typedef map< map<T> > TypeVector;

	TypeVector rows_;
};

#endif