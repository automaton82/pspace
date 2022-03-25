#ifndef _DISPLAYABLE_H_
#define _DISPLAYABLE_H_

class Displayable
{
public:

	Displayable();
	virtual ~Displayable();

	virtual void display() const = 0;

	//void setDisplayID(int id);	//set by display manager
	//int getDisplayID() const;

private:

	//int displayId;
};

#endif