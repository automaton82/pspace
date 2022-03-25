#ifndef _REQUESTCHAIN_H_
#define _REQUESTCHAIN_H_

#include <list>
using std::list;


// template implementation of the Chain of Responsibility design pattern
template <class RequestInput>
class RequestChain
{
public:
	
	RequestChain() : 
	  next_(0)
	{
	}

	void setNext(RequestChain* next)
	{
		next_ = next;
	}

	void handleRequest(RequestInput* input)
	{
		if(!doRequest(input) && next_)
			next_->handleRequest(input);
	}

	virtual bool doRequest(RequestInput* input)		// true=stop chaining, false=pass on
	{
		return false;
	}

	
private:

	RequestChain<RequestInput>* next_;
};

#endif