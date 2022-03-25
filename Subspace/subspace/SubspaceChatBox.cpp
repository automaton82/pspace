#include "SubspaceChatBox.h"

SubspaceChatBox::SubspaceChatBox(const SubspaceChatDatabase* db) :
	chatDatabase(db)
{

	fontWidth = 8;
	fontHeight = 12;

	numCols = 200;
	numRows = 10;
	
	TextureFont tex;
	tex.load("graphics/tallfont.bm2");
	tex.setTextureInfo(fontWidth, fontHeight);
	tex.setFrameRange(0, 96);

    this->setTexture(tex);
}

void SubspaceChatBox::setChatDatabase(const SubspaceChatDatabase* db)
{
	chatDatabase = db;
}

void SubspaceChatBox::draw() const
{
	const TextureFont* tex = static_cast<const TextureFont*>(this->getTexture());

	//last 10 items
	int rowNum = 0;
	for(int i=chatDatabase->size()-1; i >= 0 && rowNum < numRows; --i)
	{				//start at the last chat message
		const ChatData* data = chatDatabase->getItem(i);
		if(data)
		{
			int offset = 0;
			int width = 10;
			//the number of rows this message takes up
			int textRows = (data->text.size() / (numCols-width) ) + 1;
			int j = 0;

			for(j=0; j < textRows-1; ++j)
			{
				offset += numCols-width;
			}
            
			//the buffer has some allocated space that affects size
			string text = data->text.c_str();
			string sender = data->sender.c_str();
			do
			{
				text = data->text.substr(offset, numCols-width);
				if(sender.length() > 0)
				{
					tex->displayString(makeChatMessage(sender, text, 10).c_str());
				}
				else
				{
					tex->displayString(text.c_str());
				}

				offset -= numCols-width;
				++rowNum;

				glTranslatef(0, fontHeight, 0);
			}
			while(offset >= 0 && rowNum < numRows);// (int)data->text.size());
		}

	}
}

string SubspaceChatBox::makeChatMessage(const string& sender, const string& text, int width) const
{
	string ret;
	
	int start = width - sender.length();
	if(start < 0)
		start = 0;

	for(int i=0; i < width; ++i)
	{
		if(i >= start)
		{
			ret += sender[i - start];
		}
		else
		{
			ret += " ";
		}
	}
	
	ret += "> " + text;

	return ret;
}