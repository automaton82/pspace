//David Millman

#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

const Vector& Camera::getViewPoint()
{
	return viewPoint;
}

void Camera::setViewPoint(const Vector& v)
{
	viewPoint = v;
}