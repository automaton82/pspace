#include "SubspaceGameManager.h"

#include <fstream>

#include "DynamicData.h"
#include "GLUtil.h"
#include "Thread.h"
#include "TimeProfiler.h"

static TimeProfiler& timeProfiler = *TimeProfiler::getInstance();

#include "SubspaceGlobal.h"
#include "SubspaceSound.h"
#include "SubspaceTextures.h"

#include "SubspacePlayer.h"

SubspaceGameManager::SubspaceGameManager()
{
    initGlobal();
	initListeners();
}

SubspaceGameManager::~SubspaceGameManager()
{
	closeLogs();
}

void SubspaceGameManager::init()
{
	loadConfigFile();
	loadGraphics();
	loadSounds();

	font_.load("graphics/tallfont.bm2", false);
	font_.setFrameSize(8, 12);

	console_.setFont(&font_);
	
	initGlobal();
		
	//Arena
	Uint16 port = atoi(configData_.getData("port").c_str());
	string arena = configData_.getData("arena");
	ShipType ship = (ShipType)atoi(configData_.getData("ship").c_str());
	
	bool noconnect = false;
	if(atoi(configData_.getData("noconnect").c_str()))
		noconnect = true;

	initChat();
	initHelp();
	initHud();
	initInput();
	initLogs();
	initStatBox();
	initZone();

	menu_.init();
	
	
	if(!noconnect)
	{
		if(connect())
			login();

	}
	else
	{
		//zone_.loadDefaultSettings();
		//zone_.initArena();

		//zone_.setMyPlayer(0);

		SubspacePlayer* newPlayer = new SubspacePlayer();
		newPlayer->setName("Offline");
		//zone_.addPlayer(newPlayer, 0);
		//zone_.playerChangeShip(0, SHIP_Warbird);

		string defaultMap = configData_.getData("defaultMap");
	}	

	//LVZ Test
	vector<string> files = AsciiUtil::tokenize(configData_.getData("lvzFile"), ",");
	for(vector<string>::iterator f = files.begin(); f != files.end(); ++f)
	{
		lvzManager_.load("zones/", AsciiUtil::trim(*f));
	}
	lvzManager_.setHud(&hud_);
}

void SubspaceGameManager::initLogs()
{

}

void SubspaceGameManager::closeLogs()
{
	timeProfiler.print();
}

void SubspaceGameManager::destroy()
{
	//zone_.disconnect();
}

void SubspaceGameManager::resize()
{
	initChat();
	hud_.resize();
	zone_.resize();
}

bool SubspaceGameManager::connect()
{
	const int defaultTimeout = 100000;

	//Arena
	string ip = configData_.getData("ip");
	Uint16 port = atoi(configData_.getData("port").c_str());
		
	//string username = configData_.getData("username");
	//string password = configData_.getData("password");
	//string arena = configData_.getData("arena");
	//ShipType ship = (ShipType)atoi(configData_.getData("ship").c_str());
	
	//while(!zone_.connect(/*"ST1-"+*/dataFile.getData("username"), dataFile.getData("password"), dataFile.getData("ip"), port, arena, ship, 10000))
	/*while(!network_.connect2(username, password, ip, port, defaultTimeout))
	{
		Sleep(1);		//waiting
	}*/

	return network_.connect2(ip, port, defaultTimeout);
}

bool SubspaceGameManager::login()
{
	string username = configData_.getData("username");
	string password = configData_.getData("password");
	string arena = configData_.getData("arena");
	ShipType ship = (ShipType)atoi(configData_.getData("ship").c_str());

	/*SubspaceServerCommands::Password passwordCmd;
	passwordCmd.newUser = false;
	passwordCmd.user = configData_.getData("username");
	passwordCmd.password = configData_.getData("password");

	network_.SubspaceServerCommandListener::handleCommand(&passwordCmd);*/

	zone_.setStartArena(arena);
	zone_.setStartShip(ship);
	zone_.setLogin(username, password);

	network_.doServerPassword(username, password, false);
	
	return true;
}

/*********
 * Input *
 *********/

void SubspaceGameManager::handleInput(InputEvent input, int x, int y)
{
	static bool verbose = false;

	static bool lShift = false;
	static bool rShift = false;
	static bool lCtrl = false;
	static bool rCtrl = false;
	static bool lAlt = false;
	static bool rAlt = false;	
	
	static InputCommands::InputActivated activated;
	static InputCommands::InputActive active;
	static InputCommands::InputUnactivated unactivated;
	InputCommand* inputCmd;

	InputCommands::InputData inputData;
		    
	inputData.type = input.type;
	inputData.shift = lShift || rShift;
	inputData.ctrl = lCtrl || rCtrl;
	inputData.alt = lAlt || rAlt;

	///////////////////////////////////
	#define handleToggleInput(var)						\
		if(input.isActivated())							\
		{												\
			var = true;									\
			activated.input = inputData;				\
			inputCmd = &activated;						\
			if(verbose) printf(#var " activated: %d\n", var);			\
		}												\
		else if(input.isActive())						\
		{												\
			var = true;									\
			active.input = inputData;					\
			inputCmd = &active;							\
		}												\
		else if(input.isUnactivated())					\
		{												\
			var = false;								\
			unactivated.input = inputData;				\
			inputCmd = &unactivated;					\
			if(verbose) printf(#var " unactivated: %d\n", var);			\
		}
	///////////////////////////////////

	switch(input.type)
	{
	case KEY_LSHIFT:
		inputData.shift = true;		// shift keys should always be "shift-ed"
		handleToggleInput(lShift);
		break;
	case KEY_RSHIFT:				// TODO: there is a toggle bug in DirectX for the right shift key
		inputData.shift = true;
		//handleToggleInput(rShift);
		if(input.isActivated())							
		{												
			rShift = true;									
			activated.input = inputData;				
			inputCmd = &activated;						
			if(verbose) printf("rShift activated: %d\n", rShift);		
		}												
		else if(input.isActive())						
		{												
			rShift = true;									
			active.input = inputData;					
			inputCmd = &active;							
		}												
		else if(input.isUnactivated())					
		{												
			rShift = false;								
			unactivated.input = inputData;				
			inputCmd = &unactivated;					
			if(verbose) printf("rShift unactivated: %d\n", rShift);		
		}
		break;
	/*case KEY_LCONTROL:				// control keys should always be "control-ed"
		inputData.ctrl = true;			// ignore control'ed inputs
		handleToggleInput(lCtrl);
		break;
	case KEY_RCONTROL:
		inputData.ctrl = true;
		handleToggleInput(rCtrl);
		break;*/
	/*case KEY_LALT:					// alt keys should always be "alt-ed"
		inputData.alt = true;
		handleToggleInput(lAlt);
		break;
	case KEY_RALT:
		inputData.alt = true;
		handleToggleInput(rAlt);
		break;*/
	default:
		if(input.isActivated())
		{
			activated.input = inputData;			
			inputCmd = &activated;
		}
		else if(input.isActive())
		{
			active.input = inputData;			
			inputCmd = &active;
		}
		else if(input.isUnactivated())
		{
			unactivated.input = inputData;	
			inputCmd = &unactivated;
		}
	}

	
	menuInput_.handleRequest(inputCmd);
}

void SubspaceGameManager::update(double timestep)
{
	timeProfiler.enter("chat");
	updateChat();
	timeProfiler.exit("chat");
	
	timeProfiler.enter("console");
	updateConsole();
	timeProfiler.exit("console");
	
	timeProfiler.enter("hud");
	updateHud();
	timeProfiler.exit("hud");

	timeProfiler.enter("lvz");
	lvzManager_.update(timestep);
	timeProfiler.exit("lvz");

	timeProfiler.enter("network");
	updateNetwork();
	timeProfiler.exit("network");

	timeProfiler.enter("hud");
	hud_.update(timestep);
	timeProfiler.exit("hud");
	
	timeProfiler.enter("statBox");
	statBox_.update(timestep);
	timeProfiler.exit("statBox");
	
	timeProfiler.enter("zone");
	zone_.update(timestep);
	timeProfiler.exit("zone");	
}

void SubspaceGameManager::display()
{
	timeProfiler.enter("zone");
	glPushMatrix();
		zone_.draw();
	glPopMatrix();
	timeProfiler.exit("zone");


	timeProfiler.enter("hud");
	/*TextureFont font;							// TODO: move this
	font.load("graphics/tallfont.bm2", false);
	font.setFrameSize(8, 12);*/

	hud_.display();

	glPushMatrix();
		GLUtil::setOrthoMode(0, 0, subspaceGlobal.getWindowWidth(), subspaceGlobal.getWindowHeight());
		GLUtil::enterOrthoMode();
			glPushMatrix();
				glTranslated((subspaceGlobal.getWindowWidth()-menu_.getDisplayWidth())/2.0, subspaceGlobal.getWindowHeight() - menu_.getDisplayHeight(), 0);
				menu_.draw();
			glPopMatrix();
		GLUtil::exitOrthoMode();
	glPopMatrix();

	timeProfiler.exit("hud");
}


void SubspaceGameManager::initChat()
{
	TextureFont font;
	font.load("graphics/tallfont.bm2", false);
	font.setFrames(16, 48);
	
	chat_.setFont(font);
	chat_.setLinesDisplayed(8);
	//chat_.setLineOffset(0);

	double radarWidth = (Uint)(subspaceGlobal.getWindowWidth() * 208.0/1280);
	Uint lineWidth = (Uint)((subspaceGlobal.getWindowWidth()-radarWidth)/(double)font.getFontWidth() - 1);
	chat_.setLineWidth(lineWidth);
	chat_.setHeaderWidth(subspaceGlobal.getMessageNameWidth());

	//chatout.setChatBuffer(&chat_);

	chatAdapter_.setChat(&chat_);
	chatAdapter_.setMyPlayer(zone_.getMyPlayer());
	chatAdapter_.setPlayers(zone_.getPlayers());
}

void SubspaceGameManager::initInput()
{
	memset(InputEvent::inputStates, 0, sizeof(bool) * INPUT_NumInputs);

	// input chain of responsibility
    menuInput_.setNext(&gameInput_);
	gameInput_.setNext(&consoleInput_);
	
	menuInput_.loadConfigDefault();
	if(!gameInput_.loadConfigFile("config/keyConfig.txt"))
		gameInput_.loadConfigDefault();
	consoleInput_.loadConfigDefault();
}

void SubspaceGameManager::initHelp()
{
	string helpfile = "config/Commands.hlp";

	help_.setFont(font_);
	if(!help_.load(helpfile))
	{
		//debugout << "Failed to load help file \"" << helpfile << "\"" << endl;
		printf("Failed to load help file \"%s\"\n", helpfile.c_str());
	}
	else
	{
		//debugout << "Loaded help file \"" << helpfile << "\"" << endl;
		printf("Loaded help file \"%s\"\n", helpfile.c_str());
	}
}

void SubspaceGameManager::initHud()
{
	hud_.init();

	hud_.setConsole(&console_);
	hud_.setChat(&chat_);
	hud_.setHelp(&help_);
	//hud_.setMap(zone_.getMap());
	hud_.setLVZ(&lvzManager_);
	hud_.setMenu(&menu_);
	//hud_.setPlayer(zone_.getMyPlayer());
	hud_.setStatBox(&statBox_);
	hud_.setZone(&zone_);	
}

void SubspaceGameManager::initStatBox()
{
	statBox_.setFont(font_);
}

void SubspaceGameManager::initGlobal()
{
	subspaceGlobal.setChat(&chat_);
	subspaceGlobal.setHud(&hud_);
	subspaceGlobal.setStatBox(&statBox_);
	subspaceGlobal.setZone(&zone_);

	//Chat
	subspaceGlobal.setMessageNameWidth(atoi(configData_.getData("namelen").c_str()));
	subspaceGlobal.setMessageLines(atoi(configData_.getData("lines").c_str()));

		//Game
	//subspaceGlobal.nametagOption = (NametagOptionType)atoi(configData_.getData("nametag").c_str());
	subspaceGlobal.setNametagMode((NametagModeType)atoi(configData_.getData("nametags").c_str()));
	subspaceGlobal.setShowRadar(atoi(configData_.getData("showRadar").c_str()) != 0);
	subspaceGlobal.setStatBoxSize(atoi(configData_.getData("statBoxSize").c_str()));
	subspaceGlobal.setTargetBounty(atoi(configData_.getData("targetBounty").c_str()));
}

void SubspaceGameManager::initZone()
{
	zone_.init();
	zone_.setLVZ(&lvzManager_);

	const string zoneBaseDir = "zones/";
	string zoneDir = configData_.getData("zonedir") + "/";

	CreateDirectory((zoneBaseDir+zoneDir).c_str(), 0);		// TODO: check for existing directory?

	zone_.setZonePath(zoneBaseDir + zoneDir);
}

void SubspaceGameManager::initListeners()
{
	//menuInput_.addListener(&menu_);
	menuInput_.setMenu(&menu_);
	gameInput_.CommandGenerator<SubspacePlayerCommandReceiver>::addListener(&playerAdapter_);
	gameInput_.CommandGenerator<SubspaceHudCommandReceiver>::addListener(&hud_);
	consoleInput_.addListener(&console_);

	playerAdapter_.setZone(&zone_);

	chatAdapter_.CommandGenerator<SubspaceChatCommandReceiver>::addListener(&network_);
	console_.CommandGenerator<SubspaceChatCommandReceiver>::addListener(&chatAdapter_);
	console_.CommandGenerator<SubspacePlayerCommandReceiver>::addListener(&playerAdapter_);
	menu_.CommandGenerator<SubspaceHudCommandReceiver>::addListener(&hud_);
	menu_.CommandGenerator<SubspacePlayerCommandReceiver>::addListener(&playerAdapter_);
	network_.CommandGenerator<SubspaceChatCommandReceiver>::addListener(&chatAdapter_);
	network_.CommandGenerator<SubspaceClientCommandListener>::addListener(&zone_);
	zone_.CommandGenerator<SubspaceServerCommandListener>::addListener(&network_);
}

bool SubspaceGameManager::loadConfigFile()
{
	configData_.load("config/config.txt");	
	return true;
}

bool SubspaceGameManager::loadGraphics()
{
	//string zonedir = configData_.getData("zonedir");
	string zonedir = "";
    loadSubspaceTextures(zonedir);
	return true;
}

bool SubspaceGameManager::loadSounds()
{
	SoundManager::init();
	Sound::setScale(160.0);		// roughly 20 tiles

	//string zonedir = configData_.getData("zonedir");
	string zonedir = "";
	loadSubspaceSounds(zonedir);
	return true;
}

void SubspaceGameManager::updateChat()
{
	chatAdapter_.setMyPlayer(zone_.getMyPlayer());
	chatAdapter_.setPlayers(zone_.getPlayers());
}

void SubspaceGameManager::updateConsole()
{
	SubspacePlayer* player = statBox_.getSelectedPlayer();
	if(player)
	{
        console_.setPlayer(player->getId());
        console_.setSquadName(player->getSquad());
	}
	console_.setPlayerMap(zone_.getPlayers());
	console_.setTeam(statBox_.getSelectedTeamID());
}

void SubspaceGameManager::updateHud()
{
	static oldPlayerSize = 0;

	hud_.setMyPlayer(zone_.getMyPlayer());
	hud_.setMap(zone_.getMap());

	if(zone_.getPlayers() && zone_.getPlayers()->size() != oldPlayerSize)
	{
		oldPlayerSize = zone_.getPlayers()->size();
		hud_.setPlayers(zone_.getPlayers());
	}	
	hud_.setZone(&zone_);
}

void SubspaceGameManager::updateNetwork()
{
	network_.executeInboundCommands();
}

