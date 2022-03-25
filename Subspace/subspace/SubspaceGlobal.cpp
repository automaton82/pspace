#include "SubspaceGlobal.h"

#include "SubspaceChatStream.h"
#include "TimeStream.h"

#include "SubspaceChat.h"
#include "SubspaceHud.h"
#include "SubspaceStatBox.h"
#include "SubspaceZone.h"

SubspaceGlobal subspaceGlobal;

TimeStream debugout;			//displayed in console
TimeStream packetout;

SubspaceChatStream chatout;	//displayed in chat box
SubspaceChatStream systemout;	//displayed in chat box also - different color

SubspaceGlobal::SubspaceGlobal() : 
	chat_(0),
	hud_(0),
	statBox_(0),
	zone_(0)
{
	loadDefaultValues();
}

MessageLocationType SubspaceGlobal::getEnterMessageLocation() const
{
	return enterMessageMode_;
}

MessageLocationType SubspaceGlobal::getKillMessageLocation() const
{
	return killMessageMode_;	
}

MessageLocationType SubspaceGlobal::getLeaveMessageLocation() const
{
	return leaveMessageMode_;
}

Uint SubspaceGlobal::getMessageLines() const
{
	if(chat_)
		return chat_->getLinesDisplayed();
	else
		return messageLines_;	
}

Uint SubspaceGlobal::getMessageNameWidth() const
{
	if(chat_)
		return chat_->getHeaderWidth() - 2;
	else
		return messageNameWidth_;
}

SubspacePlayer* SubspaceGlobal::getMyPlayer() const
{
	if(zone_)
		return zone_->getMyPlayer();
	else
		return 0;
}

NametagModeType SubspaceGlobal::getNametagMode() const
{
	return nametagMode_;
}

Uint SubspaceGlobal::getStatBoxSize() const
{
	if(statBox_)
		return statBox_->size();
	else
		return statBoxSize_;
}

Uint SubspaceGlobal::getTargetBounty() const
{
	return targetBounty_;
}

SubspacePlayer* SubspaceGlobal::getTickedPlayer() const
{
	if(statBox_)
		return statBox_->getSelectedPlayer();
	else
		return tickedPlayer_;
}

Uint SubspaceGlobal::getTickedTeam() const
{
	if(statBox_)
		return statBox_->getSelectedTeamID();
	else
		return tickedTeam_;
}

Uint SubspaceGlobal::getWindowHeight() const
{
	return windowHeight_;
}

Uint SubspaceGlobal::getWindowWidth() const
{
	return windowWidth_;
}

bool SubspaceGlobal::isRadarOn() const
{
	if(hud_)
		return hud_->isRadarOn();
	else
		return showRadar_;
}

void SubspaceGlobal::setChat(SubspaceChat* chat)
{
	assert(chat);
	chat_ = chat;
	loadChatValues();
}

void SubspaceGlobal::setHud(SubspaceHud* hud)
{
	assert(hud);
	hud_ = hud;
	loadHudValues();
}

void SubspaceGlobal::setStatBox(SubspaceStatBox* box)
{
	assert(box);
	statBox_ = box;
	loadStatBoxValues();
}

void SubspaceGlobal::setZone(SubspaceZone* zone)
{
	assert(zone);
	zone_ = zone;
	loadZoneValues();
}

void SubspaceGlobal:: setEnterMessageLocation(MessageLocationType mode)
{
	if(chat_)
	{
	}

	enterMessageMode_ = mode;
}

void SubspaceGlobal:: setKillMessageLocation(MessageLocationType mode)
{
	killMessageMode_ = mode;
}

void SubspaceGlobal:: setLeaveMessageLocation(MessageLocationType mode)
{
	leaveMessageMode_ = mode;
}

void SubspaceGlobal:: setMessageLines(Uint lines)
{
	if(chat_)
	{
		if(lines >= maxMessageLines)
			lines = maxMessageLines;
		chat_->setLinesDisplayed(lines);
		messageLines_ = chat_->getLinesDisplayed();
	}
	else
	{
		messageLines_ = lines;
	}	
}

void SubspaceGlobal:: setMessageNameWidth(Uint width)
{
	if(chat_)
	{
		width += 2;
		if(width >= maxMessageNameWidth)
			width = maxMessageNameWidth;
		chat_->setHeaderWidth(max(2, width));
		messageNameWidth_ = getMessageNameWidth();
	}
	else
	{
		messageNameWidth_ = width;
	}	
}

void SubspaceGlobal::setNametagMode(NametagModeType mode)
{
	nametagMode_ = mode;
}

void SubspaceGlobal::setShowRadar(bool isOn)
{
	if(hud_)
	{
		hud_->setShowRadar(isOn);
		showRadar_ = isOn;
	}
	else
	{
		showRadar_ = isOn;
	}
}

void SubspaceGlobal::setStatBoxSize(Uint size)
{
	if(statBox_)
	{
		statBox_->setBoxSize(size);
        statBoxSize_ = statBox_->size();
	}
	else
	{
		statBoxSize_ = size;
	}
}

void SubspaceGlobal::setTargetBounty(Uint bounty)
{
	targetBounty_ = bounty;
}

void SubspaceGlobal::setWindowHeight(Uint height)
{
	windowHeight_ = height;
}

void SubspaceGlobal::setWindowWidth(Uint width)
{
	windowWidth_ = width;
}

void SubspaceGlobal::loadDefaultValues()
{
	// Chat options
	messageNameWidth_ = 12;
	messageLines_ = 7;

	enterMessageMode_ = LOCATION_Center;
	killMessageMode_ = LOCATION_Center;	//0=off, 1=center, 2=chat
	leaveMessageMode_ = LOCATION_Center;

	// Game options
	targetBounty_ = 10;

	// Window management
	windowWidth_ = 800;
	windowHeight_ = 600;
}

void SubspaceGlobal::loadChatValues()
{
	messageNameWidth_ = chat_->getLineWidth();
	messageLines_ = chat_->getLinesDisplayed();
}

void SubspaceGlobal::loadHudValues()
{
	/*MessageLocationType enterMessageMode_;
	MessageLocationType killMessageMode_;	//0=off, 1=center, 2=chat
	MessageLocationType leaveMessageMode_;*/

	nametagMode_ = hud_->getNametagMode();
	showRadar_ = hud_->isRadarOn();
	targetBounty_ = hud_->getRadarTargetBounty();
}

void SubspaceGlobal::loadStatBoxValues()
{
	statBoxSize_ = statBox_->size();	
	tickedPlayer_ = statBox_->getSelectedPlayer();
	tickedTeam_ = statBox_->getSelectedTeamID();
}

void SubspaceGlobal::loadZoneValues()
{
}