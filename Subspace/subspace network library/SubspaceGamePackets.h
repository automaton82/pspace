#ifndef _SUBSPACEGAMEPACKETS_H_
#define _SUBSPACEGAMEPACKETS_H_

// received/executed on the server, client to server packets
enum ServerPacketType
{
	SSGSPT_Unknown	= 0x0000,

	SPT_ArenaLogin	= 0x01,
	SPT_ArenaLogout	= 0x02,
	SPT_Position	= 0x03,
	SPT_WeaponFired	= 0x04,
	SPT_Death		= 0x05,
	SPT_Chat		= 0x07,
	SPT_AttachRequest	= 0x10,
	SPT_Password	= 0x0A,
	SPT_MapRequest	= 0x0C,
	SPT_TeamChange	= 0x0F,
	SPT_FlagRequest	= 0x13,
	SPT_FlagsDropped	= 0x15,
	SPT_RegForm		= 0x17,
	SPT_ShipChange	= 0x18,
	SPT_Checksum	= 0x1A,
	SPT_KothTimerEnded	= 0x1E,		//the client king of the hill timer has ended
	SPT_BrickPlaced	= 0x21,
	
	SSGSPT_NumTypes
};

// received/executed on the client, server to client packets
enum SubspaceClientPacketType
{
	CPT_Unknown	= 0x0000,

	CPT_PlayerID		= 0x01,		// When logging in, this is the id of the client
	CPT_InGame			= 0x02,		// The client is actually in the game, should start sending position packets
	CPT_PlayerEntering	= 0x03,
	CPT_PlayerLeaving	= 0x04,
	CPT_WeaponFired		= 0x05,
	CPT_PlayerDeath		= 0x06,
	CPT_Chat			= 0x07,
	CPT_PlayerPrize		= 0x08,
	CPT_ScoreUpdate		= 0x09,
	CPT_PasswordResponse	= 0x0A,
	CPT_PlayerChangeTeam	= 0x0D,
	CPT_TurretLinkRequest	= 0x0E,
	CPT_ArenaSettings	= 0x0F,
	CPT_TurretLink		= 0x10,
	CPT_FlagPosition	= 0x12,
	CPT_FlagClaim		= 0x13,
	CPT_DestroyTurretLink	= 0x15,
	CPT_FlagDrop			= 0x16,
	CPT_ServerSyncRequest	= 0x18,
	CPT_SpectatePlayer	= 0x1C,
	CPT_PlayerChangeShip	= 0x1D,
	CPT_PlayerBanner	= 0x1F,
	CPT_BrickPlaced		= 0x21,
	CPT_KeepAlive		= 0x27,			// "keep alive" packet - tells server the client is still active (don't time me out!)
	CPT_Position		= 0x28,
	CPT_MapInfo			= 0x29,
	CPT_MapFile			= 0x2A,
	CPT_KothTimerSet	= 0x2B,			// reset the client's king of the hill timer
	CPT_KothReset		= 0x2C,			// the king of the hill game has reset
	CPT_BallPosition	= 0x2E,
	CPT_ArenaList		= 0x2F,
	CPT_PastLogin		= 0x31,
		
	CPT_NumTypes
};

#endif