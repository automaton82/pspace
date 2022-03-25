#include "SubspaceMenuCommandReceiver.h"

#define emptyCommandFunction(name) void SubspaceMenuCommandReceiver::do##name() {}

emptyCommandFunction(ActivateToggle); 
emptyCommandFunction(Quit);
emptyCommandFunction(Help);
emptyCommandFunction(StatBox);
emptyCommandFunction(NameTags);
emptyCommandFunction(Radar);
emptyCommandFunction(Messages);
emptyCommandFunction(HelpTicker);
emptyCommandFunction(EngineSounds);
emptyCommandFunction(DecreaseStatBoxSize);
emptyCommandFunction(IncreaseStatBoxSize);
emptyCommandFunction(ArenaList);
emptyCommandFunction(SetBanner);
emptyCommandFunction(IgnoreMacros);
emptyCommandFunction(ShipWarbird);
emptyCommandFunction(ShipJavelin);
emptyCommandFunction(ShipSpider);
emptyCommandFunction(ShipLeviathan);
emptyCommandFunction(ShipTerrier);
emptyCommandFunction(ShipWeasel);
emptyCommandFunction(ShipLancaster);
emptyCommandFunction(ShipShark);
emptyCommandFunction(Spectator);
