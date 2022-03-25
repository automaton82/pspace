#ifndef _GRADIENTSET_H_
#define _GRADIENTSET_H_

#include <vector>
using std::vector;

#include <assert.h>

#include "Interpolation.h"

//////////////////////////////////////////////////////////////////////////////////
//Description:	Interpolates between values given a set of points
//
//Note: conceptually it's a 2-d graph, where y values are interpolated given x values
//IMPORTANT: make sure min and max points are set (and correct) for gradient values
//////////////////////////////////////////////////////////////////////////////////

//TODO: add a quadratic interpolation function, and support for it
//TODO: add spline interpolation

template <class T>
class GradientSet
{
public:
	struct DataPoint
	{
		DataPoint() :
			x(0), value(T()), dv(T())
		{}

		DataPoint(double X, const T& Value, const T& DV) : 
			x(X), value(Value), dv(DV)
		{}

		double x;
		T value, dv;
	};

	typedef vector<DataPoint> DataPointList;		//most common function will be getValue, needs fast index access

	//typedef T(InterpolationFunc)(const T& a, const T& b, double weight);
	typedef T(InterpolationFunc)(const T& a, const T& b, const T& da, double w);

	// function wrappers, need all the function signatures to match
	static T gs_lerp(const T& a, const T& b, const T& da, double w)	{	return ::lerp(a,b,w);	}
	static T gs_slerp(const T& a, const T& b, const T& da, double w){	return ::slerp(a,b,w);}
	static T gs_querp(const T& a, const T& b, const T& da, double w){	return ::querp(a,b,da,w);	}

//	const T& getDataIndex(size_t index) const;
//	double getPointIndex(size_t index) const;
public:
	GradientSet();
	~GradientSet();

						//TODO: make pre-stored interpolation types, allow parameters to be set?
	void setInterpolationFunction(InterpolationFunc* function);

	size_t insert(double x, const T& value);					//assumes 0.0 - 1.0 scale, but should work for any
	size_t insert(double x, const T& value, const T& dv);		//assumes 0.0 - 1.0 scale, but should work for any
	bool remove(double x);

    T getValue(double x) const;						
	T getMaxValue() const;
	const DataPointList& getDataPoints() const;

	void clear();
	size_t size() const;	//number of data points

	//operations
	GradientSet& operator *= (const GradientSet& rhs);	// scales gradient by value - this is slow
	friend GradientSet operator * (const GradientSet& lhs, const GradientSet& rhs);

	GradientSet& operator *= (const T& rhs);
	friend GradientSet operator * (const GradientSet& lhs, const T& value);
	friend GradientSet operator * (const T& value, const GradientSet& lhs);

	GradientSet& operator += (const T& rhs);
	friend GradientSet operator + (const GradientSet& lhs, const T& value);
	friend GradientSet operator + (const T& value, const GradientSet& lhs);

protected:

	InterpolationFunc* interpFunc_;
	DataPointList dataPoints_;
};

typedef GradientSet<double> Gradient;	//most common gradient


template <class T>
GradientSet<T>::GradientSet() :
	interpFunc_(0)
{
	interpFunc_ = GradientSet<T>::gs_lerp;
}

template <class T>
GradientSet<T>::~GradientSet()
{
}

template <class T>
void GradientSet<T>::setInterpolationFunction(InterpolationFunc* function)
{
	interpFunc_ = function;
}

template <class T>
size_t GradientSet<T>::insert(double x, const T& value)
{
	return insert(x, value, T());
}

template <class T>
size_t GradientSet<T>::insert(double x, const T& value, const T& dv)
{
	//TODO: replace this with a std::find search
	GradientSet<T>::DataPointList::iterator insertPos;
	size_t index;
	
	remove(x);	//clear any existing x values

	GradientSet<T>::DataPointList::iterator i;
	for(i=dataPoints_.begin(), index=0; i != dataPoints_.end(); ++i, ++index)
	{
		DataPoint& dp = (*i);
		if(x < dp.x)
		{
			insertPos = i;
			break;
		}
	}

	dataPoints_.insert(i, DataPoint(x, value, dv));

	return index;
}

template <class T>
bool GradientSet<T>::remove(double x)
{
	GradientSet<T>::DataPointList::iterator i;
	for(i=dataPoints_.begin(); i != dataPoints_.end(); ++i)
	{
		DataPoint& dp = (*i);
		if(x == dp.x)
		{
			dataPoints_.erase(i);
			return true;
		}
	}

	return false;
}

template <class T>
T GradientSet<T>::getValue(double x) const
{
	DataPoint a, b;
	T retval;

	if(dataPoints_.size() == 0)
		return T();

	if(x < dataPoints_.front().x)		//special case for endpoints
		return dataPoints_.front().value;
	else if(x > dataPoints_.back().x)
		return dataPoints_.back().value;
	else if(dataPoints_.size() == 1)	//special case for 1 value
		return dataPoints_.front().value;


	a = b = dataPoints_.front();

	GradientSet<T>::DataPointList::const_iterator i;
	for(i=++dataPoints_.begin(); i != dataPoints_.end(); ++i)	//start at second point
	{
		const DataPoint& dp = (*i);

		a = b;		//a equals last point that was less than X
		b = dp;		//b equals the new point

		if(x <= dp.x)
			break;
	}

	assert(interpFunc_);

	double weight = (x - a.x) / (b.x - a.x);
	//   a----x--------b
	retval = (*interpFunc_)(a.value, b.value, weight);

	return retval;
}

template <class T>
T GradientSet<T>::getMaxValue() const
{
	T maxVal;

	GradientSet<T>::DataPointList::const_iterator i;
	for(i=dataPoints_.begin(); i != dataPoints_.end(); ++i)	//start at second point
	{
		const GradientSet<T>::DataPoint& dp = (*i);

		if(i == dataPoints_.begin())
			maxVal = dp.value;
		else if(dp.value > maxVal)
			maxVal = dp.value;
	}

	return maxVal;
}

template <class T>
const GradientSet<T>::DataPointList& GradientSet<T>::getDataPoints() const
{
	return dataPoints_;
}
	
template <class T>
void GradientSet<T>::clear()
{
	dataPoints_.clear();
}

template <class T>
size_t GradientSet<T>::size() const
{
	return dataPoints_.size();
}

template <class T>
GradientSet<T>& GradientSet<T>::operator *=(const GradientSet<T>& rhs)
{
	*this = *this * rhs;

	return *this;
}

template <class T>
GradientSet<T> operator *(const GradientSet<T>& lhs, const GradientSet<T>& rhs)
{
	GradientSet<T> combined;

	GradientSet<T>::DataPointList::const_iterator i;
	for(i=lhs.dataPoints_.begin(); i != lhs.dataPoints_.end(); ++i)
	{
		const GradientSet<T>::DataPoint& dp = (*i);
		combined.insert(dp.x, dp.value * rhs.getValue(dp.x));
	}

	for(i=rhs.dataPoints_.begin(); i != rhs.dataPoints_.end(); ++i)
	{
		const GradientSet<T>::DataPoint& dp = (*i);
		combined.insert(dp.x, dp.value * lhs.getValue(dp.x));
	}

	return combined;
}

template <class T>
GradientSet<T>& GradientSet<T>::operator *=(const T& rhs)
{
	GradientSet<T>::DataPointList::iterator i;
	for(i=dataPoints_.begin(); i != dataPoints_.end(); ++i)	//start at second point
	{
		DataPoint& dp = (*i);

		dp.value *= rhs;
	}

	return *this;
}

template <class T>
GradientSet<T> operator*(const GradientSet<T>& lhs, const T& rhs)
{
	GradientSet<T> retval = lhs;
	retval *= rhs;

	return retval;
}

template <class T>
GradientSet<T> operator*(const T& lhs, const GradientSet<T>& rhs)
{
	GradientSet<T> retval = rhs;
	retval *= lhs;

	return retval;
}

template <class T>
GradientSet<T>& GradientSet<T>::operator+=(const T& lhs)
{
	GradientSet<T>::DataPointList::iterator i;
	for(i=dataPoints_.begin(); i != dataPoints_.end(); ++i)	//start at second point
	{
		GradientSet<T>::DataPoint& dp = (*i);

		dp.value += lhs;
	}

	return *this;
}

template <class T>
GradientSet<T> operator+(const GradientSet<T>& lhs, const T& rhs)
{
	GradientSet<T> retval = lhs;
	retval += rhs;

	return retval;
}

template <class T>
GradientSet<T> operator+(const T& lhs, const GradientSet<T>& rhs)
{
	GradientSet<T> retval = rhs;
	retval += lhs;

	return retval;
}


/*template <class T>
double GradientSet<T>::getPointIndex(size_t index) const
{
	assert(index < dataPoints_.size());

	return dataPoints_[index].x;
}

template <class T>
const T& GradientSet<T>::getDataIndex(size_t index) const
{
	assert(index < dataPoints_.size());

	return dataPoints_[index].value;
}*/


#endif