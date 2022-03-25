#ifndef _OBJECT_H_
#define _OBJECT_H_

//TODO: is this class even needed - what's it used for besdies game objects?
class Object
{
public:
	virtual ~Object()
	{}
	
	//do psuedo-garbage collection? maybe not here
	virtual bool isGarbage() const = 0;
	virtual void setGarbage(bool b) = 0;
};

#endif