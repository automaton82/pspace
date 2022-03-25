#include "TimeProfiler.h"

#include <assert.h>
#include <algorithm>

//TimeProfiler timeProfiler;

//string TimeProfiler::logFilename_;		// TODO: why is this a problem if it's a static member?
//TimeProfiler::TimeEntry* TimeProfiler::currentEntry_;
//TimeProfiler::TimeEntry TimeProfiler::topLevel_;

TimeProfiler::TimeProfiler()
//	logFilename_("time.txt")
//	currentEntry_(&topLevel_)
{
	logFilename_ = "time.txt";
	/*topLevel_.time.start();
	currentEntry_ = &topLevel_;*/
	printf("Created time profiler (%s).\n", logFilename_.c_str());
	init();
}

TimeProfiler::TimeProfiler(const string& logFile)
//	logFilename_(logFile)
//	currentEntry_(&topLevel_)
{	
	logFilename_ = logFile;
	/*topLevel_.time.start();
	currentEntry_ = &topLevel_;*/
	printf("Created time profiler (%s).\n", logFilename_.c_str());
	
	init();
}

TimeProfiler::~TimeProfiler()
{
	pauseAll();

	print();

	deleteAll();
}

TimeProfiler* TimeProfiler::getInstance()
{
	static TimeProfiler* inst = 0;

	if(!inst)
		inst = new TimeProfiler();
	
	return inst;	
}

void TimeProfiler::init()
{
	//logFilename_ = "time.txt";
	topLevel_.time.start();
	currentEntry_ = &topLevel_;
	//printf("Created time profiler (%s).\n", logFilename_.c_str());
}

const string& TimeProfiler::getFilename() const
{
	return logFilename_;
}

void TimeProfiler::enter(const string& name)
{
	assert(currentEntry_);
		
	//TimeMap::const_iterator i = currentEntry_->children.find(name);
	int i = findChildEntry(currentEntry_, name);	
	if(i == -1)						// child not found
	{
		TimeEntry* newEntry = new TimeEntry();
		newEntry->parent = currentEntry_;
		newEntry->time.start();
		newEntry->name = name;

		//currentEntry_->children[name] = newEntry;
		currentEntry_->addChild(newEntry);
		currentEntry_ = newEntry;			

		printf("NEW CURRENT ENTRY: %x, %s\n", currentEntry_, name.c_str());
	}
	else
	{
		//currentEntry_ = currentEntry_->children[name];
		currentEntry_ = currentEntry_->children[i];
		currentEntry_->time.unpause();

		//printf("EXISTING CURRENT ENTRY: %x\n", currentEntry_);
	}
}

void TimeProfiler::exit()
{
	assert(currentEntry_);

	if(currentEntry_->parent)
	{
		currentEntry_->time.pause();
		currentEntry_ = currentEntry_->parent;	

		//printf("BASIC EXIT CURRENT ENTRY: %x\n", currentEntry_);
	}
}

void TimeProfiler::exit(const string& name)
{
	bool found = false;
    TimeEntry* i = 0;

	assert(currentEntry_);

	i = currentEntry_->parent;
	while(i)
	{
		//if(i->children.find(name) != i->children.end())
		if(findChildEntry(i, name) != -1)
		{
			found = true;
			break;
		}		
		i = i->parent;
	}

	if(found)
	{
		currentEntry_ = i;
		//printf("EXIT FIND CURRENT ENTRY: %x\n", currentEntry_);
		
		for(int j=0; j < currentEntry_->numChildren; ++j)
			pauseHelper(currentEntry_->children[j]);
		//TimeMap::iterator c;		
		//for(c = currentEntry_->children.begin(); c != currentEntry_->children.end(); ++c)			
            //pauseHelper((*c).second);
	}
}

void TimeProfiler::deleteAll()
{
	/*TimeMap::iterator i;
	for(i = topLevel_.children.begin(); i != topLevel_.children.end(); ++i)
	{
		deleteHelper((*i).second);	
	}*/
}

void TimeProfiler::deleteHelper(TimeEntry* entry)
{
	/*TimeMap::iterator i;
	for(i = entry->children.begin(); i != entry->children.end(); ++i)
	{
		deleteHelper((*i));
		delete (*i);
	}*/
}

int TimeProfiler::findChildEntry(TimeEntry* entry, const string& name)
{
	int i=0;
	for(i=0; i < entry->numChildren; ++i)
	{
		if(entry->children[i]->name == name)
			break;
	}
	if(i >= entry->numChildren)
		return -1;
	else
		return i;
}

void TimeProfiler::pauseAll()
{
	pauseHelper(&topLevel_);
}

void TimeProfiler::print()
{
	std::ofstream of(logFilename_.c_str());
	if(!of.is_open())
		return;
	
	of << "total: " << topLevel_.time.getElapsedTime()/1000.0 << std::endl;
	printHelper(of, &topLevel_, 1);	
	of.close();
}

void TimeProfiler::pauseHelper(TimeEntry* entry)
{
	entry->time.pause();		// covers top level

	for(int i=0; i < entry->numChildren; ++i)
		pauseHelper(entry->children[i]);

	//TimeMap::iterator i;
	//for(i = entry->children.begin(); i != entry->children.end(); ++i)
		//pauseHelper((*i).second);					
}

void TimeProfiler::printHelper(std::ofstream& log, TimeEntry* entry, int tabs)
{
	for(int i=0; i < entry->numChildren; ++i)
	{
		for(int t=0; t < tabs; ++t)
			log << "\t";

		log << entry->children[i]->name << ": " << entry->children[i]->time.getElapsedTime()/1000.0 << std::endl;		// convert ms to s
		printHelper(log, entry->children[i], tabs+1);	
	}

	/*TimeMap::iterator i;
	for(i = entry->children.begin(); i != entry->children.end(); ++i)
	{
		for(int t=0; t < tabs; ++t)
			log << "\t";

		log << (*i).first << ": " << (*i).second->time.getElapsedTime()/1000.0 << std::endl;		// convert ms to s
		printHelper(log, (*i).second, tabs+1);	
	}*/
}