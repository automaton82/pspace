//David Millman  

#ifndef _FUNCTOR_H_
#define _FUNCTOR_H_

class Functor0
{
public:
	
	virtual void operator()() = 0;
	virtual void call() = 0;
};

//example:
//start_->setCallback(new Functor0Instance<CMainMenu>(this, &CMainMenu::Start));
//new Functor0Instance<classType>(classInstance, &memberFunc)

template <class T>
class Functor0Instance :
	public Functor0
{
public:
	typedef void(T::*MemberFunc)();

	Functor0Instance(T* instance, MemberFunc function)
		: instance_(instance), function_(function)
	{
	}

	void operator()()
	{
		(*instance_.*function_)();
	}

	void call()
	{
		(*instance_.*function_)();
	}

private:

	MemberFunc function_;
	T* instance_;
};

template <class T>
class Functor1
{
public:
	
	virtual void operator()(T arg) = 0;
	virtual void call(T arg) = 0;
};

template <class T, class TArg>
class Functor1Instance :
	public Functor1<TArg>
{
public:
	typedef void(T::*MemberFunc)(TArg);

	Functor1Instance(T* instance, MemberFunc function)
		: instance_(instance), function_(function)
	{
	}

	void operator()(TArg arg)
	{
		(*instance_.*function_)(arg);
	}

	void call(TArg)
	{
		(*instance_.*function_)(arg);
	}

private:

	MemberFunc function_;
	T* instance_;
};

#endif
