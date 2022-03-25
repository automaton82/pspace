#ifndef _SUBSPACEMENUINPUT_H_
#define _SUBSPACEMENUINPUT_H_

#include <map>
using std::map;

#include "CommandGenerator.h"
#include "RequestChain.h"

#include "InputCommands.h"
using InputCommands::InputData;

#include "InputCommandReceiver.h"

#include "SubspaceMenuCommands.h"
#include "SubspaceMenuCommandReceiver.h"

class SubspaceMenu;

class SubspaceMenuInput : 
	public RequestChain<InputCommand>,
	public InputCommandReceiver
	//public CommandGenerator<SubspaceMenuCommandReceiver>
{
private:
	typedef map<InputData, SubspaceMenuCommand*> MenuInputMap;

public:

	SubspaceMenuInput();
	~SubspaceMenuInput();
	
	void setMenu(SubspaceMenu* menu);

	bool doRequest(InputCommand* input);

	void doInputActive(const InputData& input);
	void doInputActivated(const InputData& input);
	void doInputUnactivated(const InputData& input);

	void loadConfigDefault();

private:

	static const InputData MENU_Toggle;
	static const InputData MENU_Quit;
	static const InputData MENU_Help;
	static const InputData MENU_StatBox;
	static const InputData MENU_NameTags;
	static const InputData MENU_Radar;
	static const InputData MENU_Messages;
	static const InputData MENU_HelpTicker;
	static const InputData MENU_EngineSounds;
	static const InputData MENU_ScrollUp;
	static const InputData MENU_ScrollDown;
	static const InputData MENU_ArenaList;
	static const InputData MENU_SetBanner;
	static const InputData MENU_IgnoreMacros;
	static const InputData MENU_ShipWarbird;
	static const InputData MENU_ShipJavelin;
	static const InputData MENU_ShipSpider;
	static const InputData MENU_ShipLeviathan;
	static const InputData MENU_ShipTerrier;
	static const InputData MENU_ShipWeasel;
	static const InputData MENU_ShipLancaster;
	static const InputData MENU_ShipShark;
	static const InputData MENU_Spectator;

private:

	MenuInputMap menuInput_;
	//bool isInputOn_;
	SubspaceMenu* menu_;

	bool handled_;
};

#endif