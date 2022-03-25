#ifndef _SUBSPACECONSOLEINPUT_H_
#define _SUBSPACECONSOLEINPUT_H_

#include <map>
using std::map;

#include "CommandGenerator.h"
#include "RequestChain.h"

#include "InputCommandReceiver.h"

class SubspaceConsoleCommand;
class SubspaceConsoleCommandReceiver;


class SubspaceConsoleInput : 
	public RequestChain<InputCommand>,
	public InputCommandReceiver,
	public CommandGenerator<SubspaceConsoleCommandReceiver>
{
private: 
	typedef map<InputData, SubspaceConsoleCommand*> ConsoleInputMap;

public:
	
	bool doRequest(InputCommand* input);

	void doInputActive(const InputData& input);
	void doInputActivated(const InputData& input);
	void doInputUnactivated(const InputData& input);

	void loadConfigDefault();

private:

	static const InputData CONSOLE_ClearBuffer;
	static const InputData CONSOLE_SendBuffer; 

	static const InputData CONSOLE_ScrollHistoryUp;
	static const InputData CONSOLE_ScrollHistoryDown;

	static const InputData CONSOLE_TabComplete;

	static const InputData CONSOLE_SendMacro1;
	static const InputData CONSOLE_SendMacro2;
	static const InputData CONSOLE_SendMacro3;
	static const InputData CONSOLE_SendMacro4;
	static const InputData CONSOLE_SendMacro5;
	static const InputData CONSOLE_SendMacro6;
	static const InputData CONSOLE_SendMacro7;
	static const InputData CONSOLE_SendMacro8;

	//static const InputData CONSOLE_WriteChar; 
	
private:

	ConsoleInputMap consoleInput_;

	bool handled_;
};


#endif