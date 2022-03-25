#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <list>
using std::list;

#include <string>
using std::string;

class Console /*: 
	public InputMessageGenerator,		// TODO: make these?
	public InputMessageListener*/
{
protected:

	typedef list<string> StringList;

public:
	Console();
	~Console();

	// Accessors
	const string& getBuffer() const;
	size_t getMaxBufferSize() const;
	size_t getMaxHistorySize() const;

	// Mutators
	void setMaxBufferSize(size_t size);
	void setMaxHistorySize(size_t size);

	// Command functions
	void clear();
	void nextCommand();
	void previousCommand();

	///////////////////////////////////

	virtual void write(char c);				// TODO: switch this to input keys?
	virtual void write(const string& s);

protected:

	string& buffer();

	void inputBackspace();
	void inputChar(char c);		// only printable chars typically
	void inputEnd();
	void inputEnter();			// sends a command
	void inputHome();
	void inputTab();			// intended for tab completion
	
	StringList::iterator lastCommand();		// last command in the list
	void newCommand();			

	virtual void onPreInput();	// called just before input, implemented by subclass
	virtual void onPostInput();	// called just after input, implemented by subclass
	virtual void tabCompletion();

	static const int defaultHistorySize = 32;
	static const int defaultBufferSize = 1024;

protected:	

	StringList history_;
	size_t maxHistorySize_;
	size_t maxBufferSize_;

	size_t cursorPosition_;
	StringList::iterator currentCommand_;
};

#endif