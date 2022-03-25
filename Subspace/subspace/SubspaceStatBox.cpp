#include "SubspaceStatBox.h"

SubspaceStatBox::SubspaceStatBox() : 
	currentMode_(STATBOX_Off),
	tempDisplay_(false)
{
	for(int i=0; i < STATBOX_NumModes; ++i)
		modes_[i] = 0;	

	init();
}

SubspaceStatBox::~SubspaceStatBox()
{
	clearModes();
}
	

void SubspaceStatBox::init()
{
	clearModes();

	modes_[STATBOX_Off] = new StatBoxPlayerSort();		// this shouldn't be necessary but just precautionary
	modes_[STATBOX_PlayerSort] = new StatBoxPlayerSort();
	modes_[STATBOX_PlayerSortPoints] = new StatBoxPlayerSortPoints();
	modes_[STATBOX_PointSort] = new StatBoxPointSort();
	modes_[STATBOX_TeamSort] = new StatBoxTeamSort();
	modes_[STATBOX_PlayerSortExtended] = new StatBoxPlayerSortExtended();
	modes_[STATBOX_TeamStats] = new StatBoxTeamStats();

}

double SubspaceStatBox::getDisplayWidth() const
{
	return modes_[currentMode_]->getDisplayWidth();
}

double SubspaceStatBox::getDisplayHeight() const
{
	return modes_[currentMode_]->getDisplayHeight();
}

Uint SubspaceStatBox::getNumPlayers() const
{
	return modes_[currentMode_]->getNumPlayers();
}

SubspacePlayer* SubspaceStatBox::getSelectedPlayer() const
{
	return modes_[currentMode_]->getSelectedPlayer();
}

int SubspaceStatBox::getSelectedTeamID() const
{
	return modes_[currentMode_]->getSelectedTeamID();
}

Uint SubspaceStatBox::size() const
{
	return modes_[currentMode_]->size();
}

void SubspaceStatBox::setBoxSize(Uint size)
{
	for(int i=0; i < STATBOX_NumModes; ++i)
        modes_[i]->setBoxSize(size);
}

void SubspaceStatBox::setFont(const TextureFont& font)
{
	for(int i=0; i < STATBOX_NumModes; ++i)
	{
		assert(modes_[i]);
        modes_[i]->setFont(font);
	}
}

void SubspaceStatBox::setMode(Uint mode)
{
	if(tempDisplay_)
	{
		displayTimer_.stop();
		tempDisplay_ = false;
	}

	currentMode_ = (StatBoxType)mode;

	SubspacePlayer* currentPlayer = modes_[currentMode_]->getSelectedPlayer();

	if(mode >= STATBOX_NumModes)
		currentMode_ = (StatBoxType)(STATBOX_NumModes - 1);

	modes_[currentMode_]->selectPlayer(currentPlayer);

}

void SubspaceStatBox::setPlayers(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer)
{
	for(int i=0; i < STATBOX_NumModes; ++i)
	{
		assert(modes_[i]);
		modes_[i]->init(players, myPlayer);
	}
}

void SubspaceStatBox::pageDown()
{
	modes_[currentMode_]->pageDown();
	tempDisplay();	
}

void SubspaceStatBox::pageUp()
{
	modes_[currentMode_]->pageUp();
	tempDisplay();
}

void SubspaceStatBox::scrollDown()
{
	modes_[currentMode_]->scrollDown();
	tempDisplay();
}

void SubspaceStatBox::scrollUp()
{
	modes_[currentMode_]->scrollUp();
	tempDisplay();
}

void SubspaceStatBox::toggleMode()
{
	if(tempDisplay_)
	{
		displayTimer_.stop();
		tempDisplay_ = false;
	}

	static SubspacePlayer* p = 0;
	SubspacePlayer* modePlayer = modes_[currentMode_]->getSelectedPlayer();
	if(modePlayer != 0)
		p = modePlayer;

	currentMode_ = (StatBoxType)((int)(currentMode_) + 1);
	if(currentMode_ >= STATBOX_NumModes)
		currentMode_ = STATBOX_Off;

	modes_[currentMode_]->selectPlayer(p);
}

void SubspaceStatBox::draw() const
{
	if(currentMode_ == STATBOX_Off && !tempDisplay_)
		return;

	if(modes_[currentMode_])
		modes_[currentMode_]->draw();
}

void SubspaceStatBox::update(double timestep)
{
	if(tempDisplay_ && displayTimer_.getElapsedTime() > defaultDisplayTime)
	{
		tempDisplay_ = false;
		displayTimer_.stop();
		currentMode_ = STATBOX_Off;
	}
}

void SubspaceStatBox::clearModes()
{
	for(int i=0; i < STATBOX_NumModes; ++i)
	{
		if(modes_[i])
		{
			delete modes_[i];
			modes_[i] = 0;
		}
	}
}

void SubspaceStatBox::tempDisplay()
{
	displayTimer_.start();
		
	if(currentMode_ == STATBOX_Off)
	{
		tempDisplay_ = true;
		currentMode_ = STATBOX_PlayerSort;
	}
}