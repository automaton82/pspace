//David Millman

#ifndef _TEXTBUFFER_H_
#define _TEXTBUFFER_H_

#include <list>
using std::list;

#include "TextureFont.h"

class TextBuffer
{
public:

	struct Chunk
	{
		Chunk() : font(0), endline(false)
		{}

		Chunk(const string& t, const Color& c, const TextureFont* tf, bool el) :
			text(t), color(c), font(tf), endline(el)
		{}

		string text;
		Color color;
		const TextureFont* font;
		bool endline;
	};

	typedef list<Chunk> ChunkList;

public:

	TextBuffer();
	virtual ~TextBuffer();

	void clear();
	Uint size() const;	//number of chunks

	void setDefaultFont(const TextureFont& font);
	void write(const string& text, const Color& tint = Color(1.0, 1.0, 1.0), const TextureFont* font = 0);	//assumes font is valid during the life of the buffer
	void writeln(const string& text, const Color& tint = Color(1.0, 1.0, 1.0), const TextureFont* font = 0);	//assumes font is valid during the life of the buffer
	void endline();		//carriage return

	void initRead() const;	//sets cursor position
	bool readChunk(Chunk* chunk) const;
	
protected:

	TextureFont defaultFont_;

	ChunkList chunks_;
	mutable ChunkList::const_iterator readPosition_;
};

#endif