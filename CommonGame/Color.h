//David Millman

#ifndef _COLOR_H_
#define _COLOR_H_

//TODO: add color math operations

class Color
{
public:
	Color();
	Color(double r, double g, double b);				// a=1.0 is opaque
	Color(double r, double g, double b, double a);

	void enable() const;
	void disable() const;	//makes color white, no transparency

	Color& operator +=(const Color& rhs);
	Color& operator -=(const Color& rhs);
	Color& operator *=(const Color& rhs);
	Color& operator /=(const Color& rhs);

	Color& operator +=(const double rhs);
	Color& operator -=(const double rhs);
	Color& operator *=(const double rhs);
	Color& operator /=(const double rhs);

	Color operator -();
	
	friend Color operator +(const Color& lhs, const Color& rhs);
	friend Color operator -(const Color& lhs, const Color& rhs);
	friend Color operator *(const Color& lhs, const Color& rhs);
	friend Color operator /(const Color& lhs, const Color& rhs);

	friend Color operator +(const Color& lhs, const double rhs);
	friend Color operator -(const Color& lhs, const double rhs);
	friend Color operator *(const Color& lhs, const double rhs);
	friend Color operator /(const Color& lhs, const double rhs);

	friend Color operator +(const double lhs, const Color& rhs);
	//friend Color operator -(const double lhs, const Color& rhs);
	friend Color operator *(const double lhs, const Color& rhs);
	//friend Color operator /(const double lhs, const Color& rhs);

	double r, g, b, a;
};

#endif