#include "SubspaceConsoleInput.h"

#include "AsciiUtil.h"
#include "TimeProfiler.h"

static TimeProfiler& timeProfiler = *TimeProfiler::getInstance();

#include "SubspaceConsoleCommands.h"
using namespace SubspaceConsoleCommands;

#include "SubspaceConsoleCommandReceiver.h"

using InputCommands::InputData;

const InputData SubspaceConsoleInput::CONSOLE_ClearBuffer = InputData(KEY_ESCAPE);
const InputData SubspaceConsoleInput::CONSOLE_SendBuffer = InputData(KEY_ENTER); 

const InputData SubspaceConsoleInput::CONSOLE_ScrollHistoryUp = InputData(KEY_UP);
const InputData SubspaceConsoleInput::CONSOLE_ScrollHistoryDown = InputData(KEY_DOWN);

const InputData SubspaceConsoleInput::CONSOLE_TabComplete = InputData(KEY_TAB);

const InputData SubspaceConsoleInput::CONSOLE_SendMacro1 = InputData(KEY_F1, true);
const InputData SubspaceConsoleInput::CONSOLE_SendMacro2 = InputData(KEY_F2, true);
const InputData SubspaceConsoleInput::CONSOLE_SendMacro3 = InputData(KEY_F3, true);
const InputData SubspaceConsoleInput::CONSOLE_SendMacro4 = InputData(KEY_F4, true);
const InputData SubspaceConsoleInput::CONSOLE_SendMacro5 = InputData(KEY_F5, true);
const InputData SubspaceConsoleInput::CONSOLE_SendMacro6 = InputData(KEY_F6, true);
const InputData SubspaceConsoleInput::CONSOLE_SendMacro7 = InputData(KEY_F7, true);
const InputData SubspaceConsoleInput::CONSOLE_SendMacro8 = InputData(KEY_F8, true);
	

SubspaceConsoleInput::SubspaceConsoleInput()
{
	loadConfigDefault();
}

SubspaceConsoleInput::~SubspaceConsoleInput()
{
	ConsoleInputMap::iterator i;
	for(i = consoleInput_.begin(); i != consoleInput_.end(); ++i)
	{
		SubspaceConsoleCommand* cmd = (*i).second;
		if(cmd) 
		{
			delete cmd;
			cmd = 0;
		}
	}
}

bool SubspaceConsoleInput::doRequest(InputCommand* input)
{
	handled_ = false;

	timeProfiler.enter("console input");
	input->execute(this);
	timeProfiler.exit("console input");

	return handled_;
}

void SubspaceConsoleInput::doInputActive(const InputData& input)
{
	// do nothing
}

void SubspaceConsoleInput::doInputActivated(const InputData& input)
{
	if(consoleInput_.find(input) != consoleInput_.end())
	{
		this->invokeCommand(consoleInput_[input]);
	}
	else
	{
		SubspaceConsoleCommands::WriteChar cmd;
		if(input.type < 256)		// is ascii character
		{
			cmd.c = input.type;
			this->invokeCommand(&cmd);
		}
	}

	handled_ = true;
}

void SubspaceConsoleInput::doInputUnactivated(const InputData& input)
{
	// do nothing
}

void SubspaceConsoleInput::loadConfigDefault()
{
	consoleInput_[CONSOLE_ClearBuffer] = new SubspaceConsoleCommands::ClearBuffer();
	consoleInput_[CONSOLE_SendBuffer] = new SubspaceConsoleCommands::SendBuffer(); 

	consoleInput_[CONSOLE_ScrollHistoryUp] = new SubspaceConsoleCommands::ScrollHistoryUp();
	consoleInput_[CONSOLE_ScrollHistoryDown] = new SubspaceConsoleCommands::ScrollHistoryDown();

	consoleInput_[CONSOLE_TabComplete] = new SubspaceConsoleCommands::TabComplete();

	consoleInput_[CONSOLE_SendMacro1] = new SubspaceConsoleCommands::SendMacro(1);
	//consoleInput_[CONSOLE_SendMacro1].macroID = 1;
	consoleInput_[CONSOLE_SendMacro2] = new SubspaceConsoleCommands::SendMacro(2);
	//consoleInput_[CONSOLE_SendMacro2].macroID = 2;
	consoleInput_[CONSOLE_SendMacro3] = new SubspaceConsoleCommands::SendMacro(3);
	//consoleInput_[CONSOLE_SendMacro3].macroID = 3;
	consoleInput_[CONSOLE_SendMacro4] = new SubspaceConsoleCommands::SendMacro(4);
	//consoleInput_[CONSOLE_SendMacro4].macroID = 4;
	consoleInput_[CONSOLE_SendMacro5] = new SubspaceConsoleCommands::SendMacro(5);
	//consoleInput_[CONSOLE_SendMacro5].macroID = 5;
	consoleInput_[CONSOLE_SendMacro6] = new SubspaceConsoleCommands::SendMacro(6);
	//consoleInput_[CONSOLE_SendMacro6].macroID = 6;
	consoleInput_[CONSOLE_SendMacro7] = new SubspaceConsoleCommands::SendMacro(7);
	//consoleInput_[CONSOLE_SendMacro7].macroID = 7;
	consoleInput_[CONSOLE_SendMacro8] = new SubspaceConsoleCommands::SendMacro(8);
	//consoleInput_[CONSOLE_SendMacro8].macroID = 8;

	//consoleInput_[CONSOLE_WriteChar] = new SubspaceConsoleCommands::WriteChar(); 
}