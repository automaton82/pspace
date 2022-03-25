#ifndef _SUBSPACECHAT_H_
#define _SUBSPACECHAT_H_

#include <list>
#include <map>
#include <string>

using std::list;
using std::map;
using std::string;

#include "DataTypes.h"
#include "TextureFont.h"

//#include "CommandGenerator.h"

#include "ChatMessage.h"

class SubspacePlayer;
//#include "SubspaceChatCommandReceiver.h"


// Chat container object, holds all messages and the like - not responsible for chat routing
class SubspaceChat
	//public SubspaceChatCommandReceiver,
	//public CommandGenerator<SubspaceChatCommandReceiver>
{
private:
	
	typedef vector<ChatMessage> MessageList;
	typedef map<Uint, SubspacePlayer*> SubspacePlayerMap;

public:
	
	SubspaceChat();
	~SubspaceChat();

	// Accessors
	Uint countLines() const;					//TODO: cache this?
	double getDisplayHeight() const;
	const TextureFont& getFont() const;
	Uint getHeaderWidth() const;
	Uint getLinesDisplayed() const;
	Uint getLineWidth() const;
	Uint size() const;	//number of messages

	// Mutators
	void setFont(const TextureFont& font);
	void setHeaderWidth(Uint width);			//TODO: add cache for header and line width for currentLine?
	void setLineWidth(Uint width);	//line width in characters - implied word wrap
	void setLinesDisplayed(Uint height);
	void setLineOffset(Uint offset);
//	void setPlayers(const SubspacePlayerMap* map);

	///////////// 
	void addMessage(const ChatMessage& msg);
	void clear();
//	virtual void doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte);
//	void doChatOutbound(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte);
	void writeMessage(const string& sender, const string& text, ChatType chatType = CHAT_Public);

	// Game functionality
	void draw() const;
	void update(double time);

private:
	Uint countMessageLines(const ChatMessage& msg) const;					// implicit word wrap
	void findLine(Uint lineNum, Uint& msgNum, Uint& msgLineOffset) const;	// TODO: cache this?
	Uint drawMessage(const ChatMessage& msg, Uint lineOffset, Uint maxMsgLines) const;	// returns message lines displayed

	static const int defaultLineWidth_ = 100;
	static const int defaultHeaderWidth_ = 10;
	static const string defaultHeaderSeparator_;

	static const string defaultName_;

private:

	TextureFont font_;
	Uint headerWidth_;
	string headerSeparator_;

	Uint lineWidth_;
	Uint linesDisplayed_;		// number of lines drawn
	Uint linesDisplayOffset_;
    
	MessageList messages_;
	
	//const SubspacePlayerMap* players_;
};

#endif