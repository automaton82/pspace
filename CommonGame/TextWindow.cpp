#include "TextWindow.h"

#include "AsciiUtil.h"
using namespace AsciiUtil;

TextWindow::TextWindow() : 
	textBuffer_(0),
	wordWrap_(true)
{
}

TextWindow::~TextWindow()
{
}

void TextWindow::setTextBuffer(TextBuffer* buffer)
{
	textBuffer_ = buffer;

	cacheBuffer();
}

void TextWindow::setWordWrap(bool b)
{
	wordWrap_ = b;
}

void TextWindow::onResize()
{
	cacheBuffer();
}

void TextWindow::draw() const
{
	drawBackground();

	if(!textBuffer_)
		return;

	Color c(1.0, 1.0, 1.0, 1.0);
	c.enable();

	glPushMatrix();
		drawBuffer();
	glPopMatrix();

	drawBorder();
}

void TextWindow::drawBuffer() const
{
	double maxHeight = 0;

	glEnable(GL_TEXTURE_2D);

	glTranslated(0, getHeight(), 0);
	BufferList::const_iterator i;
	for(i = bufferCache_.begin(); i != bufferCache_.end(); ++i)
	{
		const TextBuffer& buffer = (*i);

		TextBuffer::Chunk chunk;

		glPushMatrix();
		maxHeight = 0;
		buffer.initRead();
		while(buffer.readChunk(&chunk))
		{	
			if(chunk.font)
			{
				if(chunk.font->getFontHeight() > maxHeight)
					maxHeight = chunk.font->getFontHeight();

				glTranslated(0, -(double)chunk.font->getFontHeight(), 0);
				chunk.font->displayString(chunk.text.c_str());
				glTranslated(chunk.text.size() * chunk.font->getFontWidth(), (double)chunk.font->getFontHeight(), 0);
			}
		}
		glPopMatrix();

		glTranslated(0, -maxHeight, 0);
	}
}

void TextWindow::update(double time)
{
}

void TextWindow::cacheBuffer()
{
	double currHeight=0, currWidth=0;	
	vector<string> words;
	vector<string>::iterator w;
	Uint offset = 0;
	size_t chars = 0;
	string whitespace;

	double windowWidth = getWidth();
	double windowHeight = getHeight();

	double textWidth, textHeight;
	double wordWidth;

	bufferCache_.clear();

	if(!textBuffer_)
		return;

	textBuffer_->initRead();

	TextBuffer::Chunk chunk;
	TextBuffer line;
	while(textBuffer_->readChunk(&chunk))
	{
		if(!chunk.font)
			continue;

		textWidth = chunk.text.size() * chunk.font->getFontWidth();
		textHeight = chunk.font->getFontHeight();

		words = tokenizeWithTokens(chunk.text, " ");

		for(w = words.begin(); w != words.end(); ++w)
		{
			wordWidth = (*w).size() * chunk.font->getFontWidth();

			if(currHeight + textHeight >= windowHeight)
				break;

			if(currWidth + wordWidth > windowWidth)		//string goes past end
			{
				if(wordWrap_)
				{
					if( (*w)[0] == ' ' || wordWidth >= windowWidth)		//whitespace - split whitespace, or split word if too big
					{
						chars = (Uint)(windowWidth - currWidth)/chunk.font->getFontWidth();	//characters that can fit in window

						line.write( (*w).substr(0, chars), chunk.color, chunk.font);	

						if( (*w)[0] == ' ')
							(*w) = "";
						else
							(*w) = (*w).substr(chars);
					}

					currHeight += textHeight;	//move to next line
					currWidth = 0;
					
					if(currHeight >= windowHeight)
						break;
					
					line.endline();
					bufferCache_.push_back(line);
					line.clear();
					
					--w;
				}
				else
				{
					chars = (Uint)(windowWidth - currWidth)/chunk.font->getFontWidth();	//characters that can fit in window
					line.write( (*w).substr(0, chars), chunk.color, chunk.font);
					line.endline();
					break;
				}
			}
			else										//string fits
			{
				line.write((*w), chunk.color, chunk.font);	//add allowable number of characters	
				currWidth += wordWidth;
			}
		}		

		if(chunk.endline)
		{
			currHeight += textHeight;
			currWidth = 0;

			line.endline();
			bufferCache_.push_back(line);
			line.clear();
		}
	}
	
	if(line.size() > 0)
	{
		bufferCache_.push_back(line);
	}
}
