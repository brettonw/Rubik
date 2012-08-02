//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/22/97
//
//------------------------------------------------------------------------------

#ifndef	NETWORK_SERVER
#define	NETWORK_SERVER

//------------------------------------------------------------------------------
#include "network.h"

//------------------------------------------------------------------------------
#define	NUM_NETWORK_PLAYERS	16

//------------------------------------------------------------------------------
void	StartNetwork (int port);																									//	start up network play
Bool	DoNetwork (void);																													//	run a frame on the network
void	StopNetwork (void);																												//	clean up after network play

//------------------------------------------------------------------------------

#endif	//	NETWORK_SERVER
