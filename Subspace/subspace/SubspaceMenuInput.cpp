#include "SubspaceMenuInput.h"

#include "TimeProfiler.h"
static TimeProfiler& timeProfiler = *TimeProfiler::getInstance();


#include "SubspaceMenu.h"
#include "SubspaceMenuCommands.h"
using namespace SubspaceMenuCommands;

using InputCommands::InputData;

const InputData SubspaceMenuInput::MENU_Toggle = InputData(KEY_ESCAPE);
const InputData SubspaceMenuInput::MENU_Quit = InputData((InputEventType)'q');
const InputData SubspaceMenuInput::MENU_Help = InputData(KEY_F1);
const InputData SubspaceMenuInput::MENU_StatBox = InputData(KEY_F2);
const InputData SubspaceMenuInput::MENU_NameTags = InputData(KEY_F3);
const InputData SubspaceMenuInput::MENU_Radar = InputData(KEY_F4);
const InputData SubspaceMenuInput::MENU_Messages = InputData(KEY_F5);
const InputData SubspaceMenuInput::MENU_HelpTicker = InputData(KEY_F6);
const InputData SubspaceMenuInput::MENU_EngineSounds = InputData(KEY_F6);
const InputData SubspaceMenuInput::MENU_ScrollUp = InputData(KEY_PAGEUP);
const InputData SubspaceMenuInput::MENU_ScrollDown = InputData(KEY_PAGEDOWN);
const InputData SubspaceMenuInput::MENU_ArenaList = InputData((InputEventType)'a');
const InputData SubspaceMenuInput::MENU_SetBanner = InputData((InputEventType)'b');
const InputData SubspaceMenuInput::MENU_IgnoreMacros = InputData((InputEventType)'i');
const InputData SubspaceMenuInput::MENU_ShipWarbird = InputData((InputEventType)'1');
const InputData SubspaceMenuInput::MENU_ShipJavelin = InputData((InputEventType)'2');
const InputData SubspaceMenuInput::MENU_ShipSpider = InputData((InputEventType)'3');
const InputData SubspaceMenuInput::MENU_ShipLeviathan = InputData((InputEventType)'4');
const InputData SubspaceMenuInput::MENU_ShipTerrier = InputData((InputEventType)'5');
const InputData SubspaceMenuInput::MENU_ShipWeasel = InputData((InputEventType)'6');
const InputData SubspaceMenuInput::MENU_ShipLancaster = InputData((InputEventType)'7');
const InputData SubspaceMenuInput::MENU_ShipShark = InputData((InputEventType)'8');
const InputData SubspaceMenuInput::MENU_Spectator = InputData((InputEventType)'s');

SubspaceMenuInput::SubspaceMenuInput() :
	//isInputOn_(false),
	menu_(false)
{
	loadConfigDefault();
}

SubspaceMenuInput::~SubspaceMenuInput()
{
	MenuInputMap::iterator i;
	for(i = menuInput_.begin(); i != menuInput_.end(); ++i)
	{
		SubspaceMenuCommand* cmd = (*i).second;
		if(cmd) 
		{
			delete cmd;
			cmd = 0;
		}
	}
}

void SubspaceMenuInput::setMenu(SubspaceMenu* menu)
{
	assert(menu);

	menu_ = menu;
}

bool SubspaceMenuInput::doRequest(InputCommand* input)
{
	handled_ = false;

	timeProfiler.enter("menu input");
	input->execute(this);
	timeProfiler.exit("menu input");

	return handled_;
}

void SubspaceMenuInput::doInputActive(const InputData& input)
{
	// do nothing on active
	// holding down button doesn't matter

	//if(!isInputOn_)
	if(!menu_ || !menu_->isActivated())
		return;
}

void SubspaceMenuInput::doInputActivated(const InputData& input)
{
	if(!menu_)
		return;

	if(MENU_Toggle == input)
	{
		menu_->doActivateToggle();
		//isInputOn_ = !isInputOn_;
		handled_ = true;
	}

	//if(!isInputOn_ && !handled_)		// let menu_toggle notifications pass through
	if(!menu_->isActivated() || handled_)
		return;

	if(menuInput_.find(input) != menuInput_.end())
	{
		//this->invokeCommand(menuInput_[input]);
		menu_->handleCommand(menuInput_[input]);
		handled_ = true;
	}
}

void SubspaceMenuInput::doInputUnactivated(const InputData& input)
{
	// do nothing on unactivate
	//if(!isInputOn_)
	if(!menu_)
		return;
}

void SubspaceMenuInput::loadConfigDefault()
{
	menuInput_[MENU_Toggle] = new SubspaceMenuCommands::ActivateToggle();
	menuInput_[MENU_Quit] = new SubspaceMenuCommands::Quit();
	menuInput_[MENU_Help] = new SubspaceMenuCommands::Help();
	menuInput_[MENU_StatBox] = new SubspaceMenuCommands::StatBox(); 
	menuInput_[MENU_NameTags] = new SubspaceMenuCommands::NameTags();
	menuInput_[MENU_Radar] = new SubspaceMenuCommands::Radar();
	menuInput_[MENU_Messages] = new SubspaceMenuCommands::Messages();
	menuInput_[MENU_HelpTicker] = new SubspaceMenuCommands::HelpTicker();
	menuInput_[MENU_EngineSounds] = new SubspaceMenuCommands::EngineSounds();
	menuInput_[MENU_ScrollUp] = new SubspaceMenuCommands::DecreaseStatBoxSize();
	menuInput_[MENU_ScrollDown] = new SubspaceMenuCommands::IncreaseStatBoxSize();
	menuInput_[MENU_ArenaList] = new SubspaceMenuCommands::ArenaList();
	menuInput_[MENU_SetBanner] = new SubspaceMenuCommands::SetBanner();
	menuInput_[MENU_IgnoreMacros] = new SubspaceMenuCommands::IgnoreMacros();
	menuInput_[MENU_ShipWarbird] = new SubspaceMenuCommands::ShipWarbird();
	menuInput_[MENU_ShipJavelin] = new SubspaceMenuCommands::ShipJavelin();
	menuInput_[MENU_ShipSpider] = new SubspaceMenuCommands::ShipSpider();
	menuInput_[MENU_ShipLeviathan] = new SubspaceMenuCommands::ShipLeviathan();
	menuInput_[MENU_ShipTerrier] = new SubspaceMenuCommands::ShipTerrier();
	menuInput_[MENU_ShipWeasel] = new SubspaceMenuCommands::ShipWeasel();
	menuInput_[MENU_ShipLancaster] = new SubspaceMenuCommands::ShipLancaster();
	menuInput_[MENU_ShipShark] = new SubspaceMenuCommands::ShipShark();
	menuInput_[MENU_Spectator] = new SubspaceMenuCommands::Spectator();
}