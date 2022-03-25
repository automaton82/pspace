//David Millman

#ifndef _RECT_H_
#define _RECT_H_

template <class T>
class RectangleType
{
public:
	RectangleType();
	RectangleType<T>(T Top, T Left, T Bottom, T Right) :
		top(Top), left(Left), bottom(Bottom), right(Right)
	{}

	bool intersects(const RectangleType<T>& rhs)
	{
		if( ((rhs.left >= left && rhs.left <= rhs.right) || (left >= rhs.left && left <= rhs.right)) &&	//check x dimension
			((rhs.bottom >= bottom && rhs.bottom <= top) || (bottom >= rhs.bottom && bottom <= rhs.top))
			)
 		{
			return true;
		}
		else
			return false;
	}

	T top, left, bottom, right;		
};

typedef RectangleType<double> Rect;

#endif