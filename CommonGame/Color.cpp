//David Millman

#include "Color.h"

#include <windows.h>
#include <gl/gl.h>

Color::Color() : r(0), g(0), b(0), a(0)
{
}

Color::Color(double r, double g, double b) : r(r), g(g), b(b), a(1.0)
{
}

Color::Color(double r, double g, double b, double a) : r(r), g(g), b(b), a(a)
{
}

void Color::enable() const
{
	glColor4d(r, g, b, a);
}

void Color::disable() const
{
	glColor4d(1.0, 1.0, 1.0, 1.0);
}

Color Color::operator -()
{
	//prfloatf("Color negation\n");
	return Color(-r, -g, -b, -a);
}

Color& Color::operator +=(const Color& rhs)
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	a += rhs.a;
	
	return *this;
}

Color& Color::operator -=(const Color& rhs)
{
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	a -= rhs.a;

	return *this;
}

Color& Color::operator *=(const Color& rhs)
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;
	a *= rhs.a;

	return *this;
}

Color& Color::operator /=(const Color& rhs)
{
	r /= rhs.r;
	g /= rhs.g;
	b /= rhs.b;
	a /= rhs.a;

	return *this;
}


Color& Color::operator +=(const double rhs)
{
	r += rhs;
	g += rhs;
	b += rhs;
	a += rhs;

	return *this;
}

Color& Color::operator -=(const double rhs)
{
	r -= rhs;
	g -= rhs;
	b -= rhs;
	a -= rhs;

	return *this;
}

Color& Color::operator *=(const double rhs)
{
	r *= rhs;
	g *= rhs;
	b *= rhs;
	a *= rhs;

	return *this;
}

Color& Color::operator /=(const double rhs)
{
	r /= rhs;
	g /= rhs;
	b /= rhs;
	a /= rhs;

	return *this;
}

Color operator +(const Color& lhs, const Color& rhs)
{
 	Color v(lhs.r+rhs.r, lhs.g+rhs.g, lhs.b+rhs.b, lhs.a+rhs.a);
	return v;
}

Color operator -(const Color& lhs, const Color& rhs)
{
	Color v(lhs.r-rhs.r, lhs.g-rhs.g, lhs.b-rhs.b, lhs.a-rhs.a);
	return v;
}

Color operator *(const Color& lhs, const Color& rhs)
{
	Color v(lhs.r*rhs.r, lhs.g*rhs.g, lhs.b*rhs.b, lhs.a-rhs.a);
	return v;
}

Color operator /(const Color& lhs, const Color& rhs)
{
	Color v(lhs.r/rhs.r, lhs.g/rhs.g, lhs.b/rhs.b, lhs.a-rhs.a);
	return v;
}


Color operator +(const Color& lhs, const double rhs)
{
	Color v(lhs.r+rhs, lhs.g+rhs, lhs.b+rhs, lhs.a+rhs);
	return v;
}

Color operator -(const Color& lhs, const double rhs)
{
	Color v(lhs.r-rhs, lhs.g-rhs, lhs.b-rhs, lhs.a-rhs);
	return v;
}

Color operator *(const Color& lhs, const double rhs)
{
	Color v(lhs.r*rhs, lhs.g*rhs, lhs.b*rhs, lhs.a*rhs);
	return v;
}

Color operator /(const Color& lhs, const double rhs)
{
	Color v(lhs.r/rhs, lhs.g/rhs, lhs.b/rhs, lhs.a/rhs);
	return v;
}

Color operator +(const double lhs, const Color& rhs)
{
	Color v(lhs+rhs.r, lhs+rhs.g, lhs+rhs.b, lhs+rhs.a);
	return v;
}

//this isn't quite right
/*Color operator -(const double lhs, const Color& rhs)
{
	Color v(lhs-rhs.r, lhs-rhs.g, lhs-rhs.b);
	return v;
}*/

Color operator *(const double lhs, const Color& rhs)
{
	Color v(lhs*rhs.r, lhs*rhs.g, lhs*rhs.b, lhs*rhs.a);
	return v;
}

//this isn't quite right either
/*Color operator /(const double lhs, const Color& rhs)
{
	Color v(lhs*rhs.r, lhs*rhs.g, lhs*rhs.b);
	return v;
}*/