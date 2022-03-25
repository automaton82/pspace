#include "SubspaceEvent.h"


SubspaceEvent::SubspaceEvent() :
	type(EVENT_Unknown),
	param(0)
{
}

SubspaceEvent::SubspaceEvent(SubspaceEventType type, void* param) :
	type(type),
	param(param)
{
}