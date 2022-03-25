#include "SubspaceMenu.h"

#include "AsciiUtil.h"
using namespace AsciiUtil;

#include "ChatMessage.h"

SubspaceMenu::SubspaceMenu() : TextTable(menuHeight, 1)
{
}

void SubspaceMenu::init()
{
	setBackgroundColor(Color(0.05, 0.1, 0.05, 0.9));

	font_.load("graphics/menutext.bm2", false);
	font_.setFrameSize(8, 12);
	this->setFont(font_);

	this->setColumnWidth(0, menuWidth);
	this->setRowHeight(1);

	this->setWidth(menuWidth);
	this->setHeight(menuHeight);

	this->setPadding(2, 1, 1, 1);

	this->setColumnHeader(0, "-= Menu =-", centered, ChatMessage::COLOR_Green);
	
	this->setCell(0,  0, "Q  = Quit               Ships", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(1,  0, "F1 = Help             1 = Warbird", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(2,  0, "F2 = Stat box         2 = Javelin", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(3,  0, "F3 = Name Tags        3 = Spider", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(4,  0, "F4 = Radar            4 = Leviathan", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(5,  0, "F5 = Messages         5 = Terrier", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(6,  0, "F6 = Help ticker      6 = Weasel", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(7,  0, "F8 = Engine sounds    7 = Lancaster", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(8,  0, " A = Arena List       8 = Shark", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(9,  0, " B = Set banner       S = Spectator", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(10, 0, " I = Ignore macros", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(11, 0, "PgUp/PgDn = Adjust stat box", leftJustified, ChatMessage::COLOR_Gray);
	this->setCell(12, 0, "    Any other key to resume game", leftJustified, ChatMessage::COLOR_Yellow);

	//this->addLine(fixedWidthString("-= Menu =-", menuWidth, centered), ChatMessage::COLOR_Green);
	/*this->addLine("Q = Quit                Ships", ChatMessage::COLOR_Gray, false);
	this->addLine("F1 = Help             1 = Warbird", ChatMessage::COLOR_Gray, false);
	this->addLine("F2 = Stat box         2 = Javelin", ChatMessage::COLOR_Gray, false);
	this->addLine("F3 = Name Tags        3 = Spider", ChatMessage::COLOR_Gray, false);
	this->addLine("F4 = Radar            4 = Leviathan", ChatMessage::COLOR_Gray, false);
	this->addLine("F5 = Messages         5 = Terrier", ChatMessage::COLOR_Gray, false);
	this->addLine("F6 = Help ticker      6 = Weasel", ChatMessage::COLOR_Gray, false);
	this->addLine("F8 = Engine sounds    7 = Lancaster", ChatMessage::COLOR_Gray, false);
	this->addLine("A = Arena List        8 = Shark", ChatMessage::COLOR_Gray, false);
	this->addLine("B = Set banner        S = Spectator", ChatMessage::COLOR_Gray, false);
	this->addLine("I = Ignore macros", ChatMessage::COLOR_Gray);
	this->addLine("PgUp/PgDn = Adjust stat box", ChatMessage::COLOR_Gray);
	this->addLine(fixedWidthString("Any other key to resume game", menuWidth, centered), ChatMessage::COLOR_Yellow, false);*/

}