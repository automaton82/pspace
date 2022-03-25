#include "SubspaceChat.h"

#include <iostream>

#include "AsciiUtil.h"
#include "SubspaceSettings.h"

#include "SubspacePlayer.h"

const string SubspaceChat::defaultHeaderSeparator_ = "> ";
const string SubspaceChat::defaultName_ = "<unknown>";

SubspaceChat::SubspaceChat() : 
	headerWidth_(defaultHeaderWidth_),
	headerSeparator_(defaultHeaderSeparator_),
	lineWidth_(defaultLineWidth_)
	//players_(0)
{	
	//headerSeparator_ = "> ";
}

SubspaceChat::~SubspaceChat()
{
}

Uint SubspaceChat::countLines() const
{
	Uint lines = 0, msgLines = 0;
		
	MessageList::const_iterator i;
	for(i = messages_.begin(); i != messages_.end(); ++i)
	{
		msgLines = countMessageLines(*i);
		lines += msgLines;
	}

	return lines;
}

double SubspaceChat::getDisplayHeight() const
{
	Uint lines = min(linesDisplayed_, countLines());

	return (double)lines * font_.getFontHeight();
}

const TextureFont& SubspaceChat::getFont() const
{
	return font_;
}

Uint SubspaceChat::getHeaderWidth() const
{
	return headerWidth_;
}

Uint SubspaceChat::getLinesDisplayed() const
{
	return linesDisplayed_;
}

Uint SubspaceChat::getLineWidth() const
{
	return lineWidth_;
}

Uint SubspaceChat::size() const
{
	return (Uint)messages_.size();
}


void SubspaceChat::setFont(const TextureFont& font)
{
	font_ = font;
}

void SubspaceChat::setHeaderWidth(Uint width)
{
	Uint w = min(width, lineWidth_ - headerSeparator_.size() - 1 - 1);	
	headerWidth_ = w;
}

void SubspaceChat::setLineWidth(Uint width)
{
	lineWidth_ = width;	
}

void SubspaceChat::setLinesDisplayed(Uint lines)
{	
	linesDisplayed_ = lines;	
	if(countLines() < linesDisplayed_)
	{
		linesDisplayOffset_ = 0;
	}
}

void SubspaceChat::setLineOffset(Uint offset)
{
	linesDisplayOffset_ = offset;
}

/*void SubspaceChat::setPlayers(const SubspacePlayerMap* map)
{
	players_ = map;
}*/

void SubspaceChat::addMessage(const ChatMessage& m)
{
	Uint size = countLines();
	Uint newLines = countMessageLines(m);
	if(size + newLines > linesDisplayed_)						// if at the end of the buffer
		linesDisplayOffset_ = size+newLines - linesDisplayed_;	// scroll down
		
	messages_.push_back(m);   
}


void SubspaceChat::clear()
{
	messages_.clear();	
}

/*void SubspaceChat::doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte)
{
	string name = defaultName_;
	SubspacePlayerMap::const_iterator i;
	if(players_)
	{
		i = players_->find(playerID);
		if(i != players_->end())
			name = ((*i).second)->getName();
	}
		
	addMessage(ChatMessage(name, message, (ChatType)chatType, soundByte));
}*/

/*void SubspaceChat::doChatOutbound(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte)
{
	doChat(playerID, message, chatType, soundByte);

	SubspaceChatCommands::Chat cmd;
	cmd.playerID = playerID;
	cmd.message = message;
	cmd.type = (ChatType)chatType;
	cmd.soundByte = soundByte;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}*/

void SubspaceChat::writeMessage(const string& sender, const string& text, ChatType chatType)
{
	this->messages_.push_back(ChatMessage(sender, text, chatType));
}

void SubspaceChat::draw() const
{
	Uint line = 0;
	Uint msgLines = 0;
	Uint msgNum = 0, msgLineOffset = 0;
	
	if(messages_.size() == 0)
		return;

	findLine(linesDisplayOffset_, msgNum, msgLineOffset);
	
	// display offset message
	/*for(msgNum; line < linesDisplayed_ && msgNum < messages_.size(); ++msgNum)
	{
		msgLines = drawMessage(messages_[msgNum], msgLineOffset, linesDisplayed_);
		glTranslated(0, -(double)font_.getFontHeight() * msgLines, 0);
		line += msgLines;
	}*/

	// display offset message
	msgLines = drawMessage(messages_[msgNum], msgLineOffset, linesDisplayed_);
	//glTranslated(0, -(double)font_.getFontHeight() * msgLines, 0);
	line += msgLines;
	++msgNum;

	// display other messages
	for(msgNum; line < linesDisplayed_ && msgNum < messages_.size(); ++msgNum)
	{
		msgLines = drawMessage(messages_[msgNum], 0, linesDisplayed_ - line);
		//glTranslated(0, font_.getHeight() * msgLines, 0);
		line += msgLines;
	}
}

void SubspaceChat::update(double time)
{
}

Uint SubspaceChat::countMessageLines(const ChatMessage& msg) const
{
	/*
		<----------lineWidth-------->
		Header   Message
		|-------|-------------------|
	*/

	Uint messageWidth = lineWidth_ - headerWidth_;
	return AsciiUtil::countWrapLines(msg.text, messageWidth);
}

void SubspaceChat::findLine(Uint lineNum, Uint& msgNum, Uint& msgLineOffset) const
{
	Uint msgLines = 0;
	Uint line = 0;

	msgNum = 0;
	
	msgLines = countMessageLines(messages_[msgNum]);
	while(line + msgLines <= lineNum)
	{
		++msgNum;
		line += msgLines;
		msgLines = countMessageLines(messages_[msgNum]);
	}
	
	//msgLineOffset = line+(msgLines-1) - lineNum;
	msgLineOffset = lineNum - line;
}


Uint SubspaceChat::drawMessage(const ChatMessage& msg, Uint lineOffset, Uint maxMsgLines) const
{
	assert(headerWidth_ >= headerSeparator_.size());

	Uint senderWidth = headerWidth_ - headerSeparator_.size();
	Uint messageWidth = lineWidth_ - headerWidth_;
	vector<string> msgText = AsciiUtil::wordWrap(msg.text, messageWidth);
	string headerString;
	Uint linesDisplayed = 0;

	if(msg.sender.size() == 0)
		headerString = "";
    else if(msg.sender.size() > senderWidth)
	{
		headerString = msg.sender.substr(0, senderWidth);
		headerString += headerSeparator_;
	}
	else
	{
		headerString = AsciiUtil::fixedWidthString(msg.sender, senderWidth, AsciiUtil::rightJustified);
		headerString += headerSeparator_;
	}

	vector<string>::iterator i;
	for(i = msgText.begin() + lineOffset; i != msgText.end() && linesDisplayed < maxMsgLines; ++i)
	{
		glTranslated(0, -(double)font_.getFontHeight(), 0);

		font_.displayString(headerString.c_str(), msg.getNameColor());
        glTranslated(headerString.length() * font_.getFontWidth(), 0, 0);

		font_.displayString((*i).c_str(), msg.getTextColor());
		glTranslated(-(double)(headerString.length() * font_.getFontWidth()), 0, 0);

		++linesDisplayed;
	}

	return linesDisplayed;
}