#include "SubspaceChatCommandReceiver.h"

// Implementation stub for SubspaceChatCommandListener
class SubspaceChatCommandListener : public SubspaceChatCommandReceiver
{
public:
	void doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte) override;
};

void SubspaceChatCommandListener::doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte)
{
}