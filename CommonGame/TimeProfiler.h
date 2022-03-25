#ifndef _TIMEPROFILER_H_
#define _TIMEPROFILER_H_

#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

using std::list;
using std::map;
using std::string;
using std::vector;

#include "Timer.h"

class TimeProfiler
{
private:

	static const int MAX_CHILDREN = 25;

	struct TimeEntry;

	typedef list<string> StringList;
	//typedef map<string, TimeEntry*> TimeMap;	

	struct TimeEntry
	{
		TimeEntry() : parent(0), numChildren(0)
		{}

		string name;
		Timer time;		
		TimeEntry* parent;
		
		TimeEntry* children[MAX_CHILDREN];
		int numChildren;

		void addChild(TimeEntry* e)
		{
			if(numChildren < MAX_CHILDREN - 1)
			{
				children[numChildren] = e;
				++numChildren;
			}
		}
	};		

	TimeEntry topLevel_;	
	TimeEntry* currentEntry_;				// map tree gets rebalanced, need to use a static data structure	

	TimeProfiler();
	TimeProfiler(const string& logFile);
	~TimeProfiler();

public:
	static TimeProfiler* getInstance();

	void init();
	const string& getFilename() const;

	void enter(const string& name);
	void exit();
	void exit(const string& name);	
	
	void pauseAll();
	void print();

private:

	void deleteAll();
	void deleteHelper(TimeEntry* entry);
	
	int findChildEntry(TimeEntry* entry, const string& name);

	void pauseHelper(TimeEntry* entry);	
	void printHelper(std::ofstream& log, TimeEntry* entry, int tabs);

private:

	string logFilename_;
};

//extern TimeProfiler timeProfiler;
//TimeProfiler& timeProfiler = *TimeProfiler::getInstance();

#endif