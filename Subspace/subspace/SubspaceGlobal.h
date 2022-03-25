#ifndef _SUBSPACEGLOBAL_H_
#define _SUBSPACEGLOBAL_H_

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

#include "SoundManager.h"
#include "TextureManager.h"
#include "Thread.h"
#include "TimeStream.h"

#include "SubspaceChatStream.h"
#include "SubspaceSettings.h"

class SubspaceChat;
class SubspaceHud;
class SubspacePlayer;
class SubspaceStatBox;
class SubspaceZone;

//////////////////////////

class SubspaceGlobal
{
private:

	static const int maxMessageNameWidth = 26;		// on set, 2 is subtracted
	static const int maxMessageLines = 48;

public:
	SubspaceGlobal();

	// Accessors
	MessageLocationType getEnterMessageLocation() const;
	MessageLocationType getKillMessageLocation() const;
	MessageLocationType getLeaveMessageLocation() const;
	Uint getMessageLines() const;
	Uint getMessageNameWidth() const;
	SubspacePlayer* getMyPlayer() const;
	NametagModeType getNametagMode() const;
	Uint getStatBoxSize() const;
	Uint getTargetBounty() const;
	SubspacePlayer* getTickedPlayer() const;
	Uint getTickedTeam() const;
	Uint getWindowHeight() const;
	Uint getWindowWidth() const;
	bool isRadarOn() const;

	// Mutators
	void setChat(SubspaceChat* chat);
	void setHud(SubspaceHud* hud);
	void setStatBox(SubspaceStatBox* box);
	void setZone(SubspaceZone* zone);
	
	void setEnterMessageLocation(MessageLocationType mode);
	void setKillMessageLocation(MessageLocationType mode);
	void setLeaveMessageLocation(MessageLocationType mode);
	void setMessageLines(Uint lines);
	void setMessageNameWidth(Uint width);
	void setNametagMode(NametagModeType mode);
	void setShowRadar(bool isOn);
	void setStatBoxSize(Uint size);
	void setTargetBounty(Uint bounty);
	void setWindowHeight(Uint height);
	void setWindowWidth(Uint width);

private:

	void loadDefaultValues();
	void loadChatValues();
	void loadHudValues();
	void loadStatBoxValues();
	void loadZoneValues();
	
private:

	// Mediated objects
	SubspaceChat* chat_;
	SubspaceHud* hud_;
	SubspaceStatBox* statBox_;
	SubspaceZone* zone_;

	// Chat options
	Uint messageNameWidth_;
	Uint messageLines_;

	MessageLocationType enterMessageMode_;
	MessageLocationType killMessageMode_;	//0=off, 1=center, 2=chat
	MessageLocationType leaveMessageMode_;

	// Game options
	NametagModeType nametagMode_;
	bool showRadar_;
	Uint statBoxSize_;
	Uint targetBounty_;

	// Game management
	//TextureManager subspaceTextureManager;

	// Stat box
	SubspacePlayer* tickedPlayer_;
	Uint tickedTeam_;

	// Window management
	Uint windowWidth_, windowHeight_;	
};

extern SubspaceGlobal subspaceGlobal;

////////////////////////////
//Output streams
////////////////////////////

extern TimeStream debugout;				//displayed in console
extern TimeStream packetout;

#endif
