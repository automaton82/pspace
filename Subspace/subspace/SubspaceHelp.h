#ifndef _SUBSPACEHELP_H_
#define _SUBSPACEHELP_H_

#include <list>
using std::list;

#include "SubspaceTextBox.h"
#include "TextureFont.h"

class SubspaceHelp
{
public:
	SubspaceHelp();
	~SubspaceHelp();

	// Accessors
	double getDisplayWidth() const;
	double getDisplayHeight() const;
	/*Uint getWidth() const;			//in reference to current page
	Uint getHeight() const;*/
	Uint size() const;					//number of pages

	// Mutators
	void setFont(const TextureFont& font);
	
	///////////////////////////////////
	bool load(const string& filename);	//help file
	void nextPage();
	void previousPage();
	void setPage(Uint page);

	///////////////////////////////////
	void draw() const;					//draws current page, no bounds checking or word wrap	
	void update(double time);

private:

	static const char newPageChar = '#';
	static const char newStateChar = '%';

	void cacheNewpage();
	Uint changeState(const string& state);
	void parseLine(const string& line);

public:
	TextureFont font_;
		
	typedef vector<SubspaceTextBox> PageList;
	PageList pages_;
	Uint currentPage_;

	Color backgroundColor_;
	double borderWidth_;
	//double padding_;

	//loading
	Uchar currentColor_;
};

#endif