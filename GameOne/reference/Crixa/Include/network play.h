//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/22/97
//
//------------------------------------------------------------------------------

#ifndef	NETWORK_PLAY
#define	NETWORK_PLAY

//------------------------------------------------------------------------------
#include "network.h"
#include "human.h"

//------------------------------------------------------------------------------
#define	NUM_NETWORK_PLAYERS	16

//------------------------------------------------------------------------------
extern	Network	*gClient;																												//	pointer to the client network
extern	Human		*gPlayers[NUM_NETWORK_PLAYERS];																	//	array of pointers to players
extern	int			gNumPlayers;																										//	the actual number of players

//------------------------------------------------------------------------------
void	StartNetwork (char *server, int port, Bool net);													//	start up network play
void	DoNetwork (Code *code);																										//	run a frame on the network
void	StopNetwork (void);																												//	clean up after network play

//------------------------------------------------------------------------------

#endif	//	NETWORK_PLAY