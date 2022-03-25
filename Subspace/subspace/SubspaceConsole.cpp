#include "SubspaceConsole.h"

#include "AsciiUtil.h"
#include "TimeProfiler.h"

static TimeProfiler& timeProfiler = *TimeProfiler::getInstance();

#include "SubspaceGlobal.h"
#include "SubspaceUtil.h"

#include "SubspacePlayer.h"
#include "SubspaceChatCommandReceiver.h"
#include "SubspacePlayerCommandReceiver.h"

SubspaceConsole::SubspaceConsole() : 
	bufferColor_(COLOR_Gray),
	lineWidth_(defaultLineWidth),
	displayLines_(defaultDisplayLines),
	players_(0),
	playerID_(0),
	teamID_(0)
{
	initQueries();
}

SubspaceConsole::~SubspaceConsole()
{
}

double SubspaceConsole::getDisplayHeight() const
{
	if(getBuffer().size() == 0)
		return 0;

	Uint lines = min(displayLines_, AsciiUtil::countWrapLines(getBuffer(), lineWidth_));
	return (double)lines * font_->getFontHeight();
}

void SubspaceConsole::setDisplayLines(Uint lines)
{
	displayLines_ = lines;
}

void SubspaceConsole::setFont(TextureFont* font)
{
	font_ = font;
}

void SubspaceConsole::setLineWidth(int lineWidth)
{
	lineWidth_ = lineWidth;
}

void SubspaceConsole::setPlayer(int playerID)
{
	playerID_ = playerID;
}

void SubspaceConsole::setSquadName(string name)
{
	squadName_ = name;
}

void SubspaceConsole::setTeam(int teamID)
{
	teamID_ = teamID;
}

void SubspaceConsole::setPlayerMap(const SubspacePlayerMap* players)
{
	players_ = players;
}

void SubspaceConsole::addPlayerHistory(const string& player)
{
	playerHistory_.push_back(player);
}

const string& SubspaceConsole::getPlayerFromHistory()
{
	static string blank = "";
	if(playerHistory_.size() == 0)
		return blank;
	else
		return *playerHistoryPosition_;
}

void SubspaceConsole::nextPlayerHistory()
{
	if(playerHistory_.size() <= 1)
		return;
	
	if(playerHistoryPosition_ == playerHistory_.begin())
		playerHistoryPosition_ = playerHistory_.end();
	--playerHistoryPosition_;
}

/*void SubspaceConsole::doSaveMacro(int macroID)
{
}
*/

void SubspaceConsole::doClearBuffer()
{
	this->clear();
}

void SubspaceConsole::doSendBuffer()
{
	if(this->getBuffer().size() <= 0)
		return;

	doCommandString(getBuffer());
	newCommand();
	playerHistoryPosition_ = --playerHistory_.end();
}

void SubspaceConsole::doSendMacro(int macroID)
{
	if(this->getBuffer().size() == 0)					// nothing in buffer
	{
		if(macroMap_.find(macroID) != macroMap_.end())	// macro exists
		{
			doCommandString(macroMap_[macroID]);
		}
	}	
	else												// something in buffer
	{
		macroMap_[macroID] = getBuffer();				// save buffer
		doSendBuffer();
	}
}

void SubspaceConsole::doScrollHistoryDown()
{
	this->nextCommand();
}

void SubspaceConsole::doScrollHistoryUp()
{
	this->previousCommand();
}

void SubspaceConsole::doWriteChar(char c)
{
	this->write(c);
}

void SubspaceConsole::draw() const
{
	if(!font_)
		return;

	const string& buffer = this->getBuffer();

	vector<string> lines = AsciiUtil::wordWrap(buffer, lineWidth_, AsciiUtil::Whitespace);
	vector<string>::iterator i;
	Uint offset = 0;
	if(lines.size() > displayLines_)
		offset = lines.size() - displayLines_;

	for(i = lines.begin(); i != lines.end(); ++i)
	{
		glTranslated(0, -(double)font_->getFontHeight(), 0);
		font_->displayString((*i).c_str(), bufferColor_);
	}
}

void SubspaceConsole::doCommandString(const string& command)
{
	SubspaceChatCommands::ServerChat cmd;		// send to the server
	int intPlayerID;							// for channelSpecific
	string sendTo;								// for privateSpecific
	
	BufferType type = parseBufferType(command);
	
	cmd.type = CHAT_Public;
	cmd.soundByte = 0;

	switch(type)
	{
	case BT_Basic: 
		cmd.type = CHAT_Public;
		cmd.playerID = 0;
		cmd.message = command;
		break;
	case BT_Private:			// e.g. /hello
		cmd.type = CHAT_Private;
		cmd.playerID = playerID_;
		parsePrivate(command, &cmd.message);
		break;				
	case BT_PrivateSpecific:	// e.g. :dude:hello
		cmd.type = CHAT_Private;
		parsePrivateSpecific(command, &sendTo, &cmd.message);
		assert(players_);
		cmd.playerID = getPlayerID(*players_, sendTo);
		if(cmd.playerID == INVALID_PLAYER_ID)
			cmd.playerID = 0;
		break;		
	case BT_Team: 
		cmd.type = CHAT_Team;
		cmd.playerID = 0;
		parseTeam(command, &cmd.message);
		break;
	case BT_TeamPrivate:
		cmd.type = CHAT_TeamPrivate;
		cmd.playerID = teamID_;
		parseTeamPrivate(command, &cmd.message);
		break;
	case BT_Channel:			// e.g. ;hello -> 1:channel
		cmd.type = CHAT_Channel;
		cmd.playerID = 1;						// channel id
		parseChannel(command, &cmd.message);
		break;				
	case BT_ChannelSpecific:	// e.g. ;3;hello -> 3:channel
		cmd.type = CHAT_Channel;
		parseChannelSpecific(command, &intPlayerID, &cmd.message);
		cmd.playerID = intPlayerID;
		break;
	case BT_Squad:
		break;
	case BT_SquadSpecific:
		break;
	case BT_TeamChange:
		commandTeamChange(command);
		return;
		break;
	case BT_Query:
		commandQuery(command);
		return;
		break;
	/*case BT_VarQuery:	// ?example			// TODO: finish parsing these
		break;
	case BT_VarSet:		// ?example=e
		break;				*/
	}

	int sound;
	parseTags(cmd.message, &cmd.message, &sound);
	cmd.soundByte = sound;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::commandTeamChange(const string& command)
{
	SubspacePlayerCommands::ChangeTeam cmd;
	parseTeamChange(command, &cmd.teamID);
	
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::commandQuery(const string& command)
{
	string name, value;
	parseQuery(command, &name, &value);

	if(name.size() == 0)
		return;

	ConsoleFuncMap::iterator i = consoleCommands_.find(name);
	if(i != consoleCommands_.end())
	{
		((*this).*((*i).second)) (&value);
	}
	else
	{
		SubspaceChatCommands::ServerChat cmd;
		cmd.message = command;
		cmd.playerID = 0;
		cmd.soundByte = 0;
		cmd.type = CHAT_Public;

		this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
	}
}

bool SubspaceConsole::isKnownQuery(const string& name)
{
	if(name.size() == 0)
		return false;

	ConsoleFuncMap::iterator i = consoleCommands_.find(name);
	if(i != consoleCommands_.end())
	{
		return true;
	}

	return false;	
}

void SubspaceConsole::initQueries()
{
	consoleCommands_["freq"] = queryTeam;
	consoleCommands_["lines"] = queryMessageLines;
	consoleCommands_["messagelines"] = queryMessageLines;
	consoleCommands_["namelen"] = queryNameLength;
	consoleCommands_["namelength"] = queryNameLength;
	consoleCommands_["namewidth"] = queryNameLength;
	consoleCommands_["ship"] = queryShip;
	consoleCommands_["status"] = queryStatus;
	consoleCommands_["status2"] = queryStatus2;
	consoleCommands_["team"] = queryTeam;
	consoleCommands_["target"] = queryTargetBounty;
	consoleCommands_["targetbounty"] = queryTargetBounty;
	consoleCommands_["tick"] = queryTicked;
	consoleCommands_["ticked"] = queryTicked;
	consoleCommands_["timeprofile"] = queryTimeProfiler;
	consoleCommands_["timeprofiler"] = queryTimeProfiler;

}

void SubspaceConsole::queryFlags(const string* val)
{

}

void SubspaceConsole::queryMessageLines(const string* val)
{
	if(val && AsciiUtil::isNumeric(*val))
	{
		int intVal = atoi(val->c_str());
		if(intVal >= 0)
			subspaceGlobal.setMessageLines(intVal);
	}

	SubspaceChatCommands::Chat cmd;
	cmd.message = "Message Lines: " + AsciiUtil::itoa(subspaceGlobal.getMessageLines());
	cmd.type = CHAT_Query;
	cmd.playerID = -1;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::queryNameLength(const string* val)
{
	if(val && AsciiUtil::isNumeric(*val))
	{
		int intVal = atoi(val->c_str());
		if(intVal >= 0)
			subspaceGlobal.setMessageNameWidth(intVal);
	}

	SubspaceChatCommands::Chat cmd;
	cmd.message = "Message Name Length: " + AsciiUtil::itoa(subspaceGlobal.getMessageNameWidth());
	cmd.type = CHAT_Query;
	cmd.playerID = -1;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::queryShip(const string* val)
{
	if(val && AsciiUtil::isNumeric(*val))
	{
		int intVal = atoi(val->c_str());
		if(intVal >= 0)
		{
			SubspacePlayerCommands::ChangeShip shipCmd;
			shipCmd.shipID = intVal;

			this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&shipCmd);
		}		
	}

	SubspacePlayer* myPlayer = subspaceGlobal.getMyPlayer();
	if(myPlayer)
	{
		SubspaceChatCommands::Chat cmd;
		cmd.message = "Ship: " + AsciiUtil::itoa(myPlayer->getShipType());
		cmd.type = CHAT_Query;
		cmd.playerID = -1;

		this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
	}
}

void SubspaceConsole::queryStatus(const string* val)
{
	SubspaceChatCommands::Chat cmd;

	SubspacePlayer* player = subspaceGlobal.getMyPlayer();
	if(!player || player->getShipType() == SHIP_Spectator)
		return;

	cmd.message = "Recharge:" + AsciiUtil::itoa((int)player->getRechargePercent())	+ "%  " +
				  "Thruster:" + AsciiUtil::itoa((int)player->getThrustPercent())	+ "%  " +
				  "Speed:"    + AsciiUtil::itoa((int)player->getSpeedPercent())		+ "%  " + 
				  "Rotation:" + AsciiUtil::itoa((int)player->getRotationPercent())	+ "%  " +
				  "Shrapnel:" + AsciiUtil::itoa((int)player->getWeapons().bombs.shrapnel);
	cmd.type = CHAT_Query;
	cmd.playerID = -1;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::queryStatus2(const string* val)
{
	SubspaceChatCommands::Chat cmd;

	SubspacePlayer* player = subspaceGlobal.getMyPlayer();
	if(!player || player->getShipType() == SHIP_Spectator)
		return;

	const ShipStatus& status = player->getStatus();
	cmd.message = "Recharge:" + AsciiUtil::itoa((int)status.rechargeRate)	+ "  " +
				  "Thruster:" + AsciiUtil::itoa((int)status.thrustPower)	+ "  " +
				  "Speed:"    + AsciiUtil::itoa((int)status.topSpeed)		+ "  " + 
				  "Rotation:" + AsciiUtil::itoa((int)status.turnRate)		+ "  " +
				  "Shrapnel:" + AsciiUtil::itoa((int)player->getWeapons().bombs.shrapnel);
	cmd.type = CHAT_Query;
	cmd.playerID = -1;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::queryTargetBounty(const string* val)
{
	if(val && AsciiUtil::isNumeric(*val))
	{
		int intVal = atoi(val->c_str());
		if(intVal >= 0)
			subspaceGlobal.setTargetBounty(intVal);
	}

	SubspaceChatCommands::Chat cmd;
	cmd.message = "Target Bounty: " + AsciiUtil::itoa(subspaceGlobal.getTargetBounty());
	cmd.type = CHAT_Query;
	cmd.playerID = -1;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::queryTeam(const string* val)
{
	SubspaceChatCommands::Chat cmd;
	SubspacePlayer* player = subspaceGlobal.getMyPlayer();
	if(!player)
		return;
    			
	cmd.message = "team: " + AsciiUtil::itoa(player->getTeam());
	cmd.type = CHAT_Query;
	cmd.playerID = -1;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::queryTicked(const string* val)
{
	SubspaceChatCommands::Chat cmd;
	SubspacePlayer* player = subspaceGlobal.getTickedPlayer();
	int playerID = -1;
	if(player)
		playerID = player->getId();
		
	cmd.message = "PLAYER[" + AsciiUtil::itoa(playerID) + "]=" + player->getName() 
							+ "  TEAM=" + AsciiUtil::itoa(subspaceGlobal.getTickedTeam());
	cmd.type = CHAT_Query;
	cmd.playerID = -1;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceConsole::queryTimeProfiler(const string* val)
{
	SubspaceChatCommands::Chat cmd;
	
	timeProfiler.print();
	cmd.message = "saved time profile to \"" + timeProfiler.getFilename() + "\"";
	cmd.type = CHAT_Query;
	cmd.playerID = -1;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}

SubspaceConsole::BufferType SubspaceConsole::parseBufferType(const string& s)
{
	BufferType type = BT_Unknown;

	size_t i = s.find_first_not_of(AsciiUtil::Whitespace);
	if(i == s.npos)
		return type;

	switch(s[i])
	{
	case '/':
		type = BT_Private;
		
		if(i+1 < s.size())
		{
			switch(buffer()[i+1])
			{
			case '/':
				type = BT_Team;
				break;			
			}
		}
		break;
	case '\'':
		type = BT_Team;
		break;
	case '\"':
		type = BT_TeamPrivate;
		break;
	case ';':
		type = BT_Channel;
		if(s.find_first_of(';', 1) != s.npos)
			type = BT_ChannelSpecific;
		break;
	case ':':
		if(s.find_first_of(':', 1) != s.npos)	// must be a second :
		{
			if(s[1] == '#')						// squad specific
				type = BT_SquadSpecific;
			else
				type = BT_PrivateSpecific;
		}
		break;
	case '#':
		type = BT_Squad;
		break;
	case '=':
		type = BT_TeamChange;
		break;
	case '?':
		type = BT_Query;
		break;
	default:
		type = BT_Basic;
	}

	return type;
}

void SubspaceConsole::parsePrivate(const string& s, string* text)
{
	size_t i = s.find_first_of('/');	// first '/'
	*text = s.substr(i+1); 
}

void SubspaceConsole::parsePrivateSpecific(const string& s, string* sendTo, string* text)
{
	size_t i = s.find_first_of(':');		// first ':'
	size_t i2 = s.find_first_of(':', i+1);	// second ':'
	
	*sendTo = s.substr(i+1, i2-i);
	*text = s.substr(i2+1);					// second ':' to end
}

void SubspaceConsole::parseTeam(const string& s, string* text)
{
	size_t i = s.find_first_of("\'/");		// first ' or /
	if(s[i] == '/')
		i = s.find_first_of('/', i+1);
	
	*text = s.substr(i+1);
}

void SubspaceConsole::parseTeamPrivate(const string& s, string* text)
{
	size_t i = s.find_first_of('\"');
	*text = s.substr(i+1);
}

void SubspaceConsole::parseChannel(const string& s, string* text)
{
	size_t i = s.find_first_of(';');	// first ';'
	*text = s.substr(i+1);
}

void SubspaceConsole::parseChannelSpecific(const string& s, int* channel, string* text)
{
	size_t i = s.find_first_of(';');		// first ';'
	size_t i2 = s.find_first_of(';', i+1);	// second ';'
	
	int c = max(1, atoi(s.substr(i+1, i2-i).c_str()));
	*channel = c;
	*text = s.substr(i2+1);					// second ';' to end
}

void SubspaceConsole::parseSquad(const string& s, string* text)
{
	size_t i = s.find_first_of('#');	// first '#'
	*text = s.substr(i+1);
}

void SubspaceConsole::parseSquadSpecific(const string& s, string* toSquad, string* text)
{
	size_t i = s.find_first_of(':');		// first '#'
	size_t i2 = s.find_first_of(':', i+1);	// second '#'
	
	*toSquad = s.substr(i+1, i2-i);
	*text = s.substr(i2+1);					// second '#' to end
}

void SubspaceConsole::parseTeamChange(const string& s, int* toTeam)
{
	size_t i = s.find_first_of('=');
	size_t i2 = s.find_first_of(AsciiUtil::Whitespace, i+1);

	*toTeam = atoi(s.substr(i+1, i2-i).c_str());
}

void SubspaceConsole::parseQuery(const string& s, string* query, string* value)
{
	size_t i = s.find_first_of('?');
	size_t i2 = s.find_first_of(AsciiUtil::Whitespace + "=><,.@!$%*", i+1);

	*query = s.substr(i+1, i2-i-1);
	*value = s.substr(i2+1);
}

void SubspaceConsole::parseTags(string s, string* newText, int* soundID)
{
	// TODO: check this function is ok

	newText->clear();	//if s is a const string&, this causes a problem if s and newtext are the same variable

	size_t i = 0;
	size_t i2 = s.find_first_of('%', 0);
	newText->append(s.substr(i, i2-i));
	i = i2;
	
	while(i < s.size())
	{
		if(AsciiUtil::isNumeric(s[i+1]))	// e.g. %151
		{
			i2 = s.find_first_not_of("1234567890", i+1);
            *soundID = atoi(s.substr(i+1, i2-i).c_str());
			// insert nothing into new text
		}
		else
		{
			i2 = s.find_first_of(AsciiUtil::Whitespace, i+1);
			string tagVal = getTag(s.substr(i+1, i2-i));
			if(tagVal.size() > 0)
				newText->append(tagVal);					// insert altered tag
			else
				newText->append("%"+s.substr(i+1, i2-i));	// insert original tag
		}

		i = i2;
		size_t i2 = s.find_first_of('%', 0);
		newText->append(s.substr(i, i2-i));
		i = i2;
	}
}

string SubspaceConsole::getTag(const string& tagName)
{
	// TODO: finish implementing this
	string retval;

	return "<tag>";
}

void SubspaceConsole::onPostInput()
{
	updateBuffer();
}

void SubspaceConsole::tabComplete()
{
}

void SubspaceConsole::updateBuffer()
{
	size_t i=0, i2=0;
	string tmp;
	string name, value;
	BufferType type = parseBufferType(buffer());

	switch(type)
	{
	case BT_Basic:
		bufferColor_ = COLOR_Gray;
		break;
	case BT_Private:
		bufferColor_ = COLOR_Green;
		break;
	case BT_Team:
		bufferColor_ = COLOR_Yellow;
		break;
	case BT_TeamPrivate:
		bufferColor_ = COLOR_Gray;
		break;
	case BT_Channel:
	case BT_ChannelSpecific:
		bufferColor_ = COLOR_Orange;
		break;
	case BT_Squad:
		i = buffer().find_first_of('#');
		buffer().erase(i, 1);
		buffer().insert(i, ":#"+squadName_+":");
		cursorPosition_ = buffer().size();

		bufferColor_ = COLOR_Green;
		break;
	case BT_PrivateSpecific:						// guaranteed to have two ':'
		i = buffer().find_first_of(':');
		i2 = buffer().find_first_of(':', i+1);
		if(i2+1 < buffer().size() && buffer()[i2+1] == ':')	// third ':' found
		{
			buffer().erase(i+1, i2-i);					// erase sender
			tmp = getPlayerFromHistory().c_str();
			buffer().insert(i, tmp);	// insert player from history
			cursorPosition_ = buffer().size();
			nextPlayerHistory();						// move to next person
		}

		bufferColor_ = COLOR_Green;
		break;
	case BT_TeamChange:
		bufferColor_ = COLOR_Pink;
		break;
	case BT_Query:
		parseQuery(buffer(), &name, &value);
		if(isKnownQuery(name))
			bufferColor_ = COLOR_Pink;
		else
			bufferColor_ = COLOR_Gray;
		break;
	}

}