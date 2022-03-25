#ifndef _SUBSPACEHELP_H_
#define _SUBSPACEHELP_H_

#include <list>
using std::list;

#include "TextureFont.h"
#include "TextTable.h"

//TODO: make this use a general subspace text box formatting

class SubspaceHelp
{
private:

	struct Chunk
	{
		Chunk() : color(0)
		{}

		Chunk(const string& t, Uchar c) : text(t), color(c)
		{}

		string text;
		Uchar color;
	};
	typedef list<Chunk> ChunkList;
	typedef vector<ChunkList> ChunkLines;

	struct Page
	{
		Page() : width(0)
		{}

		Uint width;//, height;
		ChunkLines lines;
	};

	typedef vector<Page> PageList;

public:
	SubspaceHelp();
	~SubspaceHelp();

	Uint getDrawWidth() const;
	Uint getDrawHeight() const;
	Uint getWidth() const;	//in reference to current page
	Uint getHeight() const;
	void setFont(const TextureFont& font);
	void setPage(Uint page);
	Uint size() const;		//number of pages

	bool load(const string& filename);	//help file

	void draw() const;				//draws current page, no bounds checking or word wrap
	
	void update(double time);

private:

	static const char newPageChar = '#';
	static const char newStateChar = '%';

	void cacheChunk(const Chunk& c);
	void cacheNewline();
	void cacheNewpage();
	Uint changeState(const string& state);
	void parseLine(const string& line);

	void drawBackground() const;
	void drawBorder() const;
	void drawText() const;

public:
	TextureFont font_;
	Color backgroundColor_;
	double borderWidth_;
	double padding_;
	
	PageList pages_;
	Uint currentPage_;
	ChunkList* currentLine_;
	Uint lineWidth_;

	//loading
	Uchar currentColor_;
};

#endif