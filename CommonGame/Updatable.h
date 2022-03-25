#ifndef _UPDATEABLE_H_
#define _UPDATEABLE_H_

class Updatable
{
public:

	virtual ~Updatable()
	{}

	virtual void update(double timeMultiplier) = 0;
};

#endif