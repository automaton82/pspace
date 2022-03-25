#include "SubspaceConsoleCommandReceiver.h"

void SubspaceConsoleCommandReceiver::doWriteChar(char c) {}

//void SubspaceConsoleCommandReceiver::doSaveMacro(int macroID) {}
void SubspaceConsoleCommandReceiver::doSendMacro(int macroID) {}

///////////////////////////////////////
#define emptyCommandFunction(name) void SubspaceConsoleCommandReceiver::do##name() {}

emptyCommandFunction(ClearBuffer);
emptyCommandFunction(SendBuffer); 

emptyCommandFunction(ScrollHistoryUp); 
emptyCommandFunction(ScrollHistoryDown); 

emptyCommandFunction(TabComplete); 
