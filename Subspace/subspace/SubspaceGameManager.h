#ifndef _SUBSPACEGAMEMANAGER_H_
#define _SUBSPACEGAMEMANAGER_H_

//#include "CommandRouter.h"
#include "DynamicData.h"

#include "InputEvent.h"
#include "SubspaceEvent.h"

#include "SubspaceConsoleInput.h"
#include "SubspaceGameInput.h"
#include "SubspaceMenuInput.h"

#include "SubspaceChat.h"
#include "SubspaceChatAdapter.h"
#include "SubspaceConsole.h"
#include "SubspaceGameProtocol.h"
#include "SubspaceHelp.h"
#include "SubspaceHud.h"
#include "SubspaceLVZManager.h"
#include "SubspaceMenu.h"
#include "SubspacePlayerAdapter.h"
#include "SubspaceStatBox.h"
#include "SubspaceZone.h"

class SubspaceGameManager
{
public:

	//typedef CommandRouter<SubspaceClientCommandListener, SubspaceClientCommand> ClientCommandRouter;	// for commands executed on clients
	//typedef CommandRouter<SubspaceServerCommandListener, SubspaceServerCommand> ServerCommandRouter;	// for commands executed on servers 
	//typedef CommandRouter<SubspaceChatCommandListener, SubspaceChatCommand> ChatCommandRouter;			// two way - in and out of client

public:

	SubspaceGameManager();
	~SubspaceGameManager();

	void closeLogs();
	void destroy();
	void init();
	void initLogs();
	void resize();

	bool connect();
	bool login();

	// Input
	void handleInput(InputEvent input, int x, int y);

	///////////////////////////////////

	void update(double timeStep);
	void display();

private:

	// initialization
	void initChat();
	void initHelp();
	void initHud();
	void initInput();
	void initStatBox();
	void initZone();

	void initGlobal();
	void initListeners();

	bool loadConfigFile();
	bool loadGraphics();
	bool loadSounds();
	
	void updateChat();
	void updateConsole();
	void updateHud();
	void updateNetwork();

private:
	DynamicData configData_;

	SubspaceMenuInput menuInput_;
	SubspaceGameInput gameInput_;
	SubspaceConsoleInput consoleInput_;

	// Game systems
	SubspaceChat chat_;
	SubspaceChatAdapter chatAdapter_;
	SubspaceConsole console_;
	SubspaceGameProtocol network_;
	SubspaceHelp help_;
	SubspaceHud hud_;
	SubspaceLVZManager lvzManager_;
	SubspaceMenu menu_;
	SubspacePlayerAdapter playerAdapter_;
	SubspaceStatBox statBox_;
	SubspaceZone zone_;

	//std::ofstream debuglog_;
	//std::ofstream packetlog_;

	TextureFont font_;
};

#endif