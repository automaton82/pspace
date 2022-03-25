#include "SubspaceMenu.h"

#include "AsciiUtil.h"
#include "SubspaceSettings.h"

#include "SubspaceHudCommandReceiver.h"
#include "SubspacePlayerCommandReceiver.h"

SubspaceMenu::SubspaceMenu() :
	activated_(false)
{
}

SubspaceMenu::~SubspaceMenu()
{
}

void SubspaceMenu::init()
{
	header_.write("            -= Menu =-", COLOR_Green);
	setBackgroundColor(Color(10.0/255.0, 25.0/255.0, 10.0/255.0, 0.9));

	font_.load("graphics/menutext.bm2", false);
	font_.setFrameSize(8, 12);
	
	header_.setFont(font_);

	this->setPadding(1.0, 0.0, 1.0, 1.0);
	headerPadTop_ = 1.0;
	headerPadBottom_ = 1.0;
	separatorWidth_ = 1.0;

	this->write("Q  = Quit               ", COLOR_Gray); 
	this->write("Ships\n", COLOR_Red);
	this->write("F1 = Help             1 = Warbird\n", COLOR_Gray);
	this->write("F2 = Stat box         2 = Javelin\n", COLOR_Gray);
	this->write("F3 = Name Tags        3 = Spider\n", COLOR_Gray);
	this->write("F4 = Radar            4 = Leviathan\n", COLOR_Gray);
	this->write("F5 = Messages         5 = Terrier\n", COLOR_Gray);
	this->write("F6 = Help ticker      6 = Weasel\n", COLOR_Gray);
	this->write("F8 = Engine sounds    7 = Lancaster\n", COLOR_Gray);
	this->write(" A = Arena List       8 = Shark\n", COLOR_Gray);
	this->write(" B = Set banner       S = Spectator\n", COLOR_Gray);
	this->write(" I = Ignore macros\n", COLOR_Gray);
	this->write("PgUp/PgDn = Adjust stat box\n", COLOR_Gray);
	this->write("    Any other key to resume game", COLOR_Yellow);

	this->flush();
}

bool SubspaceMenu::isActivated() const
{
	return activated_;
}

void SubspaceMenu::draw()
{
	if(activated_)
		SubspaceTextBox::draw();
}

void SubspaceMenu::doActivateToggle() 
{
	activated_ = !activated_;

	// TODO: adjust chat lines here
} 

void SubspaceMenu::doQuit()
{
	// TODO: make this exit?
	//this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doHelp()
{
	SubspaceHudCommands::ToggleHelp cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doStatBox()
{
	SubspaceHudCommands::ToggleStatBox cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doNameTags()
{
	SubspaceHudCommands::ToggleNametags cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doRadar()
{
	SubspaceHudCommands::ToggleRadar cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doMessages() 
{
	SubspaceHudCommands::ToggleHelpTicker cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doHelpTicker() 
{
	SubspaceHudCommands::ToggleHelpTicker cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doEngineSounds()
{
}

void SubspaceMenu::doDecreaseStatBoxSize() 
{
	SubspaceHudCommands::DecreaseStatBoxSize cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doIncreaseStatBoxSize()
{
	SubspaceHudCommands::IncreaseStatBoxSize cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);
}

void SubspaceMenu::doArenaList()
{
	SubspaceHudCommands::ArenaList cmd;
	this->CommandGenerator<SubspaceHudCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doSetBanner()
{
}

void SubspaceMenu::doIgnoreMacros()
{
}

void SubspaceMenu::doShipWarbird()
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 0;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doShipJavelin() 
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 1;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doShipSpider() 
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 2;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doShipLeviathan()
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 3;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doShipTerrier() 
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 4;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doShipWeasel() 
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 5;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doShipLancaster()
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 6;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doShipShark()
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 7;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}

void SubspaceMenu::doSpectator()
{
	SubspacePlayerCommands::ChangeShip cmd;
	cmd.shipID = 8;
	this->CommandGenerator<SubspacePlayerCommandReceiver>::invokeCommand(&cmd);

	activated_ = false;
}