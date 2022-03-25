#ifndef _SUBSPACEGAMEINPUT_H_
#define _SUBSPACEGAMEINPUT_H_

#include <map>
#include <string>

using std::map;
using std::string;

#include "CommandGenerator.h"
#include "RequestChain.h"

#include "InputCommands.h"
using InputCommands::InputData;

#include "InputCommandReceiver.h"
//#include "SubspaceHudCommands.h"
//#include "SubspacePlayerCommands.h"
class SubspaceHudCommand;
class SubspacePlayerCommand;
#include "SubspaceHudCommandReceiver.h"
#include "SubspacePlayerCommandReceiver.h"

class SubspaceGameInput : 
	public RequestChain<InputCommand>,
	public InputCommandReceiver,
	public CommandGenerator<SubspaceHudCommandReceiver>,
	public CommandGenerator<SubspacePlayerCommandReceiver>
{
private:
	typedef map<InputData, SubspaceHudCommand*> HudInputMap;
	typedef map<InputData, SubspacePlayerCommand*> PlayerInputMap;

public:

	SubspaceGameInput();
	~SubspaceGameInput();

	bool doRequest(InputCommand* input);

	void doInputActive(const InputData& input);
	void doInputActivated(const InputData& input);
	void doInputUnactivated(const InputData& input);

    // input conversions
	void clearConfig();
	void loadConfigDefault();
	bool loadConfigFile(const string& filename);

private:

	// TODO: need to take care of shift functions

	//weapons
	static const InputData GI_FireBrick;
	static const InputData GI_FireBomb;
	static const InputData GI_FireBullet;
	static const InputData GI_FireBurst;		// TODO: make this shifted input
	static const InputData GI_FireDecoy;
	static const InputData GI_FireMine;
	static const InputData GI_FirePortal;
	static const InputData GI_FireRepel;
	static const InputData GI_FireRocket;
	static const InputData GI_FireThor;

	//movement
	static const InputData GI_MoveForward;
	static const InputData GI_MoveBackward;
	static const InputData GI_TurnLeft;
	static const InputData GI_TurnRight;
	static const InputData GI_FastForward;		//afterburner
	static const InputData GI_FastBackward;
	static const InputData GI_FastLeft;				
	static const InputData GI_FastRight;

	//toggles
	static const InputData GI_ToggleAntiwarp;
	static const InputData GI_ToggleCloak;
    static const InputData GI_ToggleMultifire;
	static const InputData GI_ToggleStealth;
	static const InputData GI_ToggleXRadar;

	//misc
	static const InputData GI_Attach;	//turret attachment
	static const InputData GI_Warp;
	static const InputData GI_ToggleBigRadar;

	//menu commands
	static const InputData GI_ToggleHelp;		//TODO: turn these into a chat command
	static const InputData GI_ToggleStatBox;

	//chat
	static const InputData GI_ScrollUp;
	static const InputData GI_ScrollDown;
	static const InputData GI_ScrollUpFast;
	static const InputData GI_ScrollDownFast;

	///////////////////////////////////
	static void initStringInputs();							// sets up "keyName" to KEY_ mappings
	static InputData parseInputData(const string& s);		// converts "keyName" to KEY_ mapping
	static InputData simplifyInput(const InputData& input);		// converts rshift, rctrl to lshift, lctrl

private:
	HudInputMap hudInput_;						// activated, on press
	PlayerInputMap playerInput_;				// active, while pressed
	PlayerInputMap playerInputActivated_;		// activated, on press

    InputData inputState_;
	bool handled_;

	///////////////////////////////////
	
	typedef map<string, InputEventType> StringInputMap;
	static StringInputMap stringInputs_;
	static bool hasStringInputs_;
};

#endif