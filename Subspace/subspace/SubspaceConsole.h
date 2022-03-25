#ifndef _SUBSPACECONSOLE_H_
#define _SUBSPACECONSOLE_H_

#include <map>
using std::map;

#include <string>
using std::string;

#include "Console.h"
#include "TextureFont.h"
#include "CommandGenerator.h"

/*#include "SubspaceConsoleCommands.h"
#include "SubspaceChatCommands.h"
#include "SubspaceHudCommands.h"
#include "SubspacePlayerCommands.h"*/

//#include "SubspaceChatCommandReceiver.h"
#include "SubspaceConsoleCommandReceiver.h"

class SubspaceChatCommandReceiver;
class SubspaceHudCommandReceiver;
class SubspacePlayerCommandReceiver;

#include "ChatMessage.h"
#include "SubspaceZone.h"

class SubspaceConsole : 
	public Console,
	public SubspaceConsoleCommandReceiver,
	public CommandGenerator<SubspaceChatCommandReceiver>,
	public CommandGenerator<SubspaceHudCommandReceiver>,
	public CommandGenerator<SubspacePlayerCommandReceiver>
{
private: 

	typedef SubspaceZone::SubspacePlayerMap SubspacePlayerMap;

	enum BufferType
	{
		BT_Basic = 0,
		BT_Private,				// e.g. /hello
		BT_PrivateSpecific,		// e.g. :dude:hello
		BT_Team,				// e.g. 'hi		e.g. //hi
		BT_TeamPrivate,			// e.g. "yo allies
		BT_Channel,				// e.g. ;hello -> 1:channel
		BT_ChannelSpecific,		// e.g. ;3;hello -> 3:channel
		BT_Squad,				// e.g. #yo dawgs					// TODO: implement these
		BT_SquadSpecific,		// e.g. ;#badsquad;you guys suck
		BT_TeamChange,
		BT_Query,				// e.g. ?namelen=3, ?namelen 3
		//BT_VarQuery,			// ?example			// TODO: finish parsing these
		//BT_VarSet,			// ?example=e

		BT_Unknown
	};

	typedef map<int, string> MacroMap;
	typedef void(SubspaceConsole::*ConsoleFunc)(const string* value);
	typedef map<string, ConsoleFunc> ConsoleFuncMap;

public:
	SubspaceConsole();
	~SubspaceConsole();

	// Accessors
	double getDisplayHeight() const;

	// Mutators
	void setDisplayLines(Uint lines);
	void setFont(TextureFont* font);
	void setLineWidth(int lineWidth);
	void setPlayer(int playerID);				// send message to this player
	void setSquadName(string name);
	void setTeam(int teamID);					// send message to this team

	void setPlayerMap(const SubspacePlayerMap* players);	// for player lookups?

	// Player history	
	void addPlayerHistory(const string& player);
	const string& getPlayerFromHistory();		
	void nextPlayerHistory();
	
	// Console commands
	//void doSaveMacro(int macroID);
	void doClearBuffer();
	void doScrollHistoryDown(); 
	void doScrollHistoryUp(); 
	void doSendBuffer(); 
	void doSendMacro(int macroID);
	// void doTabComplete(); 
	void doWriteChar(char c);

	///////////////////////////////////
	void draw() const;

private:
														//TODO: finish implementing this
														// find type, parse for tags, 
	void doCommandString(const string& command);		// actually sends a command
	void commandTeamChange(const string& command);		// e.g. =1234
	void commandQuery(const string& command);			// e.g. ?status
	bool isKnownQuery(const string& name);
	void initQueries();			
		void queryFlags(const string* val);
		void queryMessageLines(const string* val);
		void queryNameLength(const string* val);
		void queryShip(const string* val);
		void queryStatus(const string* val);
		void queryStatus2(const string* val);
		void queryTargetBounty(const string* val);
		void queryTeam(const string* val);
		void queryTicked(const string* val);
		void queryTimeProfiler(const string* val);

	BufferType parseBufferType(const string& s);
	void parsePrivate(const string& s, string* text);
	void parsePrivateSpecific(const string& s, string* sendTo, string* text);
	void parseTeam(const string& s, string* text);
	void parseTeamPrivate(const string& s, string* text);
	void parseChannel(const string& s, string* text);
	void parseChannelSpecific(const string& s, int* channel, string* text);
	void parseSquad(const string& s, string* text);
	void parseSquadSpecific(const string& s, string* toSquad, string* text);
	void parseTeamChange(const string& s, int* toTeam);
	void parseQuery(const string& s, string* query, string* value);
	//void parseVarQuery(const string& s, string* var);			// TODO: implement these
	//void parseVarSet(const string& s, string* var, int* val);

	void parseTags(string s, string* newText, int* soundID);
	string getTag(const string& tagName);

	virtual void onPostInput();		// do something after input
	virtual void tabComplete();		// TODO: this would be cool to implement

	void updateBuffer();
	
	static const int defaultLineWidth = 80;
	static const Uint defaultDisplayLines = 8;

private:
	StringList playerHistory_;
	StringList::iterator playerHistoryPosition_;
	const SubspacePlayerMap* players_;

	int playerID_;
	int teamID_;
	string squadName_;

	MacroMap macroMap_;
	ConsoleFuncMap consoleCommands_;
	
	TextureFont* font_;
	int lineWidth_;
	Uint displayLines_;
	int bufferColor_;
};

#endif