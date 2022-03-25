#include "SubspaceGameInput.h"

#include "AsciiUtil.h"
#include "DynamicData.h"
#include "TimeProfiler.h"

static TimeProfiler& timeProfiler = *TimeProfiler::getInstance();

#include "SubspaceHudCommands.h"
#include "SubspacePlayerCommands.h"

using namespace SubspaceHudCommands;
using namespace SubspacePlayerCommands;

using InputCommands::InputData;

SubspaceGameInput::StringInputMap SubspaceGameInput::stringInputs_;
bool SubspaceGameInput::hasStringInputs_ = false;

const InputData SubspaceGameInput::GI_FireBrick = InputData(KEY_F4);
const InputData SubspaceGameInput::GI_FireBomb = InputData(KEY_TAB);
const InputData SubspaceGameInput::GI_FireBullet = InputData(INPUT_UNKNOWN, false, true);
const InputData SubspaceGameInput::GI_FireBurst = InputData(KEY_DELETE, true);
const InputData SubspaceGameInput::GI_FireDecoy = InputData(KEY_F5);
const InputData SubspaceGameInput::GI_FireMine = InputData(KEY_TAB, true);
const InputData SubspaceGameInput::GI_FirePortal = InputData(KEY_INSERT, true);
const InputData SubspaceGameInput::GI_FireRepel = InputData(INPUT_UNKNOWN, true, true);
const InputData SubspaceGameInput::GI_FireRocket = InputData(KEY_F3);
const InputData SubspaceGameInput::GI_FireThor = InputData(KEY_F6);

//movement
const InputData SubspaceGameInput::GI_MoveForward = InputData(KEY_UP);
const InputData SubspaceGameInput::GI_MoveBackward = InputData(KEY_DOWN);
const InputData SubspaceGameInput::GI_TurnLeft = InputData(KEY_LEFT);
const InputData SubspaceGameInput::GI_TurnRight = InputData(KEY_RIGHT);
const InputData SubspaceGameInput::GI_FastForward = InputData(KEY_UP, true);		//afterburner
const InputData SubspaceGameInput::GI_FastBackward = InputData(KEY_DOWN, true);
const InputData SubspaceGameInput::GI_FastLeft = InputData(KEY_LEFT, true);				
const InputData SubspaceGameInput::GI_FastRight = InputData(KEY_RIGHT, true);

//toggles
const InputData SubspaceGameInput::GI_ToggleAntiwarp = InputData(KEY_END, true);
const InputData SubspaceGameInput::GI_ToggleCloak = InputData(KEY_HOME, true);
const InputData SubspaceGameInput::GI_ToggleMultifire = InputData(KEY_DELETE, true);
const InputData SubspaceGameInput::GI_ToggleStealth = InputData(KEY_HOME);
const InputData SubspaceGameInput::GI_ToggleXRadar = InputData(KEY_END);

//misc
const InputData SubspaceGameInput::GI_Attach = InputData(KEY_F7);	//turret attachment
const InputData SubspaceGameInput::GI_Warp = InputData(KEY_INSERT);
const InputData SubspaceGameInput::GI_ToggleBigRadar = InputData(KEY_F8, true);

//menu commands
const InputData SubspaceGameInput::GI_ToggleHelp = InputData(KEY_F1);		//TODO: turn these into a chat command
const InputData SubspaceGameInput::GI_ToggleStatBox = InputData(KEY_F2);

//chat
const InputData SubspaceGameInput::GI_ScrollUp = InputData(KEY_PAGEUP);
const InputData SubspaceGameInput::GI_ScrollDown = InputData(KEY_PAGEDOWN);
const InputData SubspaceGameInput::GI_ScrollUpFast = InputData(KEY_PAGEUP, true);
const InputData SubspaceGameInput::GI_ScrollDownFast = InputData(KEY_PAGEDOWN, true);

SubspaceGameInput::SubspaceGameInput() /*:
	GI_FireBrick(InputData(KEY_F4)),
	GI_FireBomb(InputData(KEY_TAB)),
	GI_FireBullet(InputData(INPUT_UNKNOWN, false, true)),
	GI_FireBurst(InputData(KEY_DELETE, true)),
	GI_FireDecoy(InputData(KEY_F5)),
	GI_FireMine(InputData(KEY_TAB, true)),
	GI_FirePortal(InputData(KEY_INSERT, true)),
	GI_FireRepel(InputData(INPUT_UNKNOWN, true, true)),
	GI_FireRocket(InputData(KEY_F3)),
	GI_FireThor(InputData(KEY_F6)),

	//movement
	GI_MoveForward(InputData(KEY_UP)),
	GI_MoveBackward(InputData(KEY_DOWN)),
	GI_TurnLeft(InputData(KEY_LEFT)),
	GI_TurnRight(InputData(KEY_RIGHT)),
	GI_FastForward(InputData(KEY_UP, true)),		//afterburner
	GI_FastBackward(InputData(KEY_DOWN, true)),
	GI_FastLeft(InputData(KEY_LEFT, true)),				
	GI_FastRight(InputData(KEY_RIGHT, true)),

	//toggles
	GI_ToggleAntiwarp(InputData(KEY_END, true)),
	GI_ToggleCloak(InputData(KEY_HOME, true)),
    GI_ToggleMultifire(InputData(KEY_DELETE, true)),
	GI_ToggleStealth(InputData(KEY_HOME)),
	GI_ToggleXRadar(InputData(KEY_END)),

	//misc
	GI_Attach(InputData(KEY_F7)),	//turret attachment
	GI_Warp(InputData(KEY_INSERT)),

	//menu commands
	GI_ToggleHelp(InputData(KEY_F1)),		//TODO: turn these into a chat command
	GI_ToggleInfoBox(InputData(KEY_F2)),

	//chat
	GI_ScrollUp(InputData(KEY_PAGEUP)),
	GI_ScrollDown(InputData(KEY_PAGEDOWN)),
	GI_ScrollUpFast(InputData(KEY_PAGEUP, true)),
	GI_ScrollDownFast(InputData(KEY_PAGEDOWN, true))*/
{
	initStringInputs();
	loadConfigDefault();
}

SubspaceGameInput::~SubspaceGameInput()
{
	clearConfig();
}

bool SubspaceGameInput::doRequest(InputCommand* input)
{
	handled_ = false;

	timeProfiler.enter("game input");
	input->execute(this);
	timeProfiler.exit("game input");

	return handled_;
}

void SubspaceGameInput::doInputActive(const InputData& input)
{
	InputData i = simplifyInput(input);

	// TODO: be careful if shift comes in as input first
	// TODO: make it a global state, while input is handled?
	if(playerInput_.find(i) != playerInput_.end())
	{
		SubspacePlayerCommand* cmd = playerInput_[i];
		this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(playerInput_[i]);
		handled_ = true;
	}
}

void SubspaceGameInput::doInputActivated(const InputData& input)
{
	InputData i = simplifyInput(input);

	if(hudInput_.find(i) != hudInput_.end())
	{
		this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(hudInput_[i]);
		handled_ = true;
	}
	else if(playerInputActivated_.find(i) != playerInputActivated_.end())
	{
		this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(playerInputActivated_[i]);
		handled_ = true;
	}
	else
		doInputActive(input);
}

void SubspaceGameInput::doInputUnactivated(const InputData& input)
{
	// do nothing
}

void SubspaceGameInput::clearConfig()
{
	HudInputMap::iterator i;
	for(i = hudInput_.begin(); i != hudInput_.end(); ++i)
	{
		SubspaceHudCommand* hcmd = (*i).second;
		if(hcmd) 
		{
			delete hcmd;
			hcmd = 0;
		}
	}
	hudInput_.clear();

	PlayerInputMap::iterator j;
	for(j = playerInput_.begin(); j != playerInput_.end(); ++j)
	{
		SubspacePlayerCommand* ucmd = (*j).second;
		if(ucmd) 
		{
			delete ucmd;
			ucmd = 0;
		}
	}
	playerInput_.clear();

	PlayerInputMap::iterator k;
	for(k = playerInputActivated_.begin(); k != playerInputActivated_.end(); ++k)
	{
		SubspacePlayerCommand* acmd = (*k).second;
		if(acmd) 
		{
			delete acmd;
			acmd = 0;
		}
	}
	playerInputActivated_.clear();
}

void SubspaceGameInput::loadConfigDefault()
{
	playerInput_[GI_FireBrick]	= new FireBrick();
	playerInput_[GI_FireBomb]	= new FireBomb();
	playerInput_[GI_FireBullet]	= new FireBullet();
	playerInput_[GI_FireBurst]	= new FireBurst();		
	playerInput_[GI_FireDecoy]	= new FireDecoy();
	playerInput_[GI_FireMine]	= new FireMine();
	playerInput_[GI_FirePortal]	= new FirePortal();
	playerInput_[GI_FireRepel]	= new FireRepel();
	playerInput_[GI_FireRocket]	= new FireRocket();
	playerInput_[GI_FireThor]	= new FireThor();

	//movement
	playerInput_[GI_MoveForward]	= new MoveForward();
	playerInput_[GI_MoveBackward]	= new MoveBackward();
	playerInput_[GI_TurnLeft]		= new TurnLeft();
	playerInput_[GI_TurnRight]		= new TurnRight();
	playerInput_[GI_FastForward]	= new FastForward();		//afterburner
	playerInput_[GI_FastBackward]	= new FastBackward();
	playerInput_[GI_FastLeft]		= new FastLeft();				
	playerInput_[GI_FastRight]		= new FastRight();

	//toggles
	playerInputActivated_[GI_ToggleAntiwarp]	= new ToggleAntiwarp();
	playerInputActivated_[GI_ToggleCloak]	= new ToggleCloak();
	playerInputActivated_[GI_ToggleMultifire]= new ToggleMultifire();
	playerInputActivated_[GI_ToggleStealth]	= new ToggleStealth();
	playerInputActivated_[GI_ToggleXRadar]	= new ToggleXRadar();

	//misc
	playerInputActivated_[GI_Attach]	= new Attach();	//turret attachment
	playerInputActivated_[GI_Warp]	= new Warp();
	hudInput_[GI_ToggleBigRadar]	= new ToggleBigRadar();

	//menu commands
	hudInput_[GI_ToggleHelp]	= new ToggleHelp();		
	hudInput_[GI_ToggleStatBox]	= new ToggleStatBox();

	//chat
	hudInput_[GI_ScrollUp]		= new ScrollUp();
	hudInput_[GI_ScrollDown]	= new ScrollDown();
	hudInput_[GI_ScrollUpFast]	= new ScrollUpFast();
	hudInput_[GI_ScrollDownFast]= new ScrollDownFast();
}

bool SubspaceGameInput::loadConfigFile(const string& filename)
{
	DynamicData inputFile;
	if(!inputFile.load(filename))
		return false;

	#define mapPlayerInput(name) playerInput_[parseInputData(inputFile.getData(#name))] = new name();
	#define mapPlayerInputActivated(name) playerInputActivated_[parseInputData(inputFile.getData(#name))] = new name();
	#define mapHudInput(name) hudInput_[parseInputData(inputFile.getData(#name))] = new name();

	clearConfig();

	// weapons
	mapPlayerInput(FireBrick); 	
	mapPlayerInput(FireBomb); 	
	mapPlayerInput(FireBullet); 
	mapPlayerInput(FireBurst); 
	mapPlayerInput(FireDecoy); 
	mapPlayerInput(FireMine); 
	mapPlayerInputActivated(FirePortal);
	mapPlayerInput(FireRepel); 
	mapPlayerInput(FireRocket);
	mapPlayerInput(FireThor); 

	//movement
	mapPlayerInput(MoveForward); 
	mapPlayerInput(MoveBackward);
	mapPlayerInput(TurnLeft); 
	mapPlayerInput(TurnRight);
	mapPlayerInput(FastForward);
	mapPlayerInput(FastBackward);
	mapPlayerInput(FastLeft); 
	mapPlayerInput(FastRight);

	//toggles
	mapPlayerInputActivated(ToggleAntiwarp);
	mapPlayerInputActivated(ToggleCloak); 	
	mapPlayerInputActivated(ToggleMultifire);
	mapPlayerInputActivated(ToggleStealth); 
	mapPlayerInputActivated(ToggleXRadar); 

	//misc
	mapPlayerInputActivated(Attach);
	mapPlayerInputActivated(Warp);
	mapHudInput(ToggleBigRadar);

	//menu commands
	mapHudInput(ToggleHelp);
	mapHudInput(ToggleStatBox);
	
	//chat
	mapHudInput(ScrollUp);
	mapHudInput(ScrollDown);
	mapHudInput(ScrollUpFast);
	mapHudInput(ScrollDownFast);

	return true;
}

InputData SubspaceGameInput::simplifyInput(const InputData& input)
{
	InputData i = input;
	if(i.type == KEY_LSHIFT || i.type == KEY_RSHIFT)
	{
		i.type = INPUT_UNKNOWN;
		i.shift = true;
	}
	else if(i.type == KEY_LCONTROL || i.type == KEY_RCONTROL)
	{
		i.type = INPUT_UNKNOWN;
		i.ctrl = true;
	}
	/*else if(i.type == INPUT_RALT)
		i.type = INPUT_LALT;*/

	return i;
}	

void SubspaceGameInput::initStringInputs()
{
	if(hasStringInputs_)
		return;

	stringInputs_["UP"]		= KEY_UP; 
	stringInputs_["DOWN"]	= KEY_DOWN; 
	stringInputs_["LEFT"]	= KEY_LEFT; 
	stringInputs_["RIGHT"]	= KEY_RIGHT;

	stringInputs_["UPARROW"]	= KEY_UP;
	stringInputs_["DOWNARROW"]	= KEY_DOWN; 
	stringInputs_["LEFTARROW"]	= KEY_LEFT; 
	stringInputs_["RIGHTARROW"]	= KEY_RIGHT;

	stringInputs_["CONTROL"]	= KEY_RCONTROL;
	stringInputs_["CTRL"]	= KEY_LCONTROL;
	stringInputs_["SHIFT"]		= KEY_LSHIFT;

	stringInputs_["TAB"]		= KEY_TAB;

	stringInputs_["INSERT"]	= KEY_INSERT;
	stringInputs_["DELETE"]	= KEY_DELETE;
	stringInputs_["DEL"]	= KEY_DELETE;
	stringInputs_["HOME"]	= KEY_HOME;
	stringInputs_["END"]	= KEY_END;
	stringInputs_["PAGEUP"]	= KEY_PAGEUP;
	stringInputs_["PGUP"]	= KEY_PAGEUP;
	stringInputs_["PAGEDOWN"] = KEY_PAGEDOWN;
	stringInputs_["PGDOWN"] = KEY_PAGEDOWN;

	stringInputs_["ESCAPE"] = KEY_ESCAPE;
						
	stringInputs_["F1"] = KEY_F1;
	stringInputs_["F2"] = KEY_F2;
	stringInputs_["F3"] = KEY_F3;
	stringInputs_["F4"] = KEY_F4;
	stringInputs_["F5"] = KEY_F5;
	stringInputs_["F6"] = KEY_F6;
	stringInputs_["F7"] = KEY_F7;
	stringInputs_["F8"] = KEY_F8;
	stringInputs_["F9"]	= KEY_F9;
	stringInputs_["F10"] = KEY_F10;
	stringInputs_["F11"] = KEY_F11;
	stringInputs_["F12"] = KEY_F12;

	stringInputs_["MOUSE_LEFT"] = MOUSE_LEFT;
	stringInputs_["MOUSE_RIGHT"] = MOUSE_RIGHT;
	stringInputs_["MOUSE_MIDDLE"] = MOUSE_MIDDLE;

	hasStringInputs_ = true;
}


InputData SubspaceGameInput::parseInputData(const string& s)
{
	InputData retval;

	vector<string> values = AsciiUtil::tokenize(AsciiUtil::toupper(s), AsciiUtil::Whitespace + "+");
	vector<string>::iterator i;
	for(i = values.begin(); i != values.end(); ++i)
	{
		if(*i == "SHIFT")
			retval.shift = true;
		else if(*i == "CTRL" || *i == "CONTROL")
			retval.ctrl = true;
		else if(*i == "ALT" || *i == "ALTERNATE")
			retval.alt = true;
		else
		{
			if(s.size() == 1)		// ascii char
				retval.type = (InputEventType)s[0];
			else
				retval.type = stringInputs_[*i];
		}
	}

	return retval;
}