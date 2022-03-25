//David Millman

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#define SINGLETON(Type, Variable) Type& Variable = *Singleton<Type>::instance()

//TODO: make a better singleton type

/**
 *Defines a template singleton class
 *
 *@author	David Millman (millmd@rpi.edu)
 *@date		4/14/2003
 */
template <class T>
class Singleton
{
private:
	Singleton() :
	{}

protected:
	~Singleton()
	{
		if(inst)
			delete inst;
	}

public:
	static T* instance()
	{
		static T* inst = 0;

		if(!inst)
			inst = new T();
		return inst;
	}
};

#endif