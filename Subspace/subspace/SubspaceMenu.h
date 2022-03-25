#ifndef _SUBSPACEMENU_H_
#define _SUBSPACEMENU_H_

#include "CommandGenerator.h"
class SubspaceHudCommandReceiver;
class SubspacePlayerCommandReceiver;

#include "SubspaceMenuCommandReceiver.h"
#include "SubspaceTextBox.h"

class SubspaceMenu :
	public SubspaceTextBox,
	public SubspaceMenuCommandReceiver,
	public CommandGenerator<SubspaceHudCommandReceiver>,
	public CommandGenerator<SubspacePlayerCommandReceiver>
{
public:
	SubspaceMenu();
	~SubspaceMenu();

	virtual void init();
	bool isActivated() const;
	virtual void draw();

	virtual void doActivateToggle(); 
	virtual void doQuit();
	virtual void doHelp();
	virtual void doStatBox();
	virtual void doNameTags();
	virtual void doRadar();
	virtual void doMessages();
	virtual void doHelpTicker();
	virtual void doEngineSounds();
	virtual void doDecreaseStatBoxSize();
	virtual void doIncreaseStatBoxSize();
	virtual void doArenaList();
	virtual void doSetBanner();
	virtual void doIgnoreMacros();
	virtual void doShipWarbird();
	virtual void doShipJavelin();
	virtual void doShipSpider();
	virtual void doShipLeviathan();
	virtual void doShipTerrier();
	virtual void doShipWeasel();
	virtual void doShipLancaster();
	virtual void doShipShark();
	virtual void doSpectator();

private:

	bool activated_;
};

#endif