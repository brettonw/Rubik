//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/04/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "network play.h"
#include "local human.h"
#include "display.h"
#include "referee.h"
#include "random.h"
#include "get parameters.h"
#include "debugger.h"

//------------------------------------------------------------------------------
Network	*gClient = 0;																														//	pointer to the client network
Human		*gPlayers[NUM_NETWORK_PLAYERS];																					//	array of pointers to players
int			gNumPlayers;																														//	the actual number of players

//------------------------------------------------------------------------------
struct	PlayerStart
{
	int		numPlayers;
	char	name[16];
	PlayerStart (void) {}
	PlayerStart (int numPlayers, cstr name);
};

//------------------------------------------------------------------------------
struct	StartupSynch
{
	int		id;
	int		numPlayers;
	char	names[NUM_NETWORK_PLAYERS][16];
	StartupSynch (void) {}
};

//------------------------------------------------------------------------------
struct	ServerSynch
{
	Real	rate;
	Code	codes[NUM_NETWORK_PLAYERS];
};

//------------------------------------------------------------------------------
inline	PlayerStart::PlayerStart (int num, cstr nam)														//	constructor
{																																								//	begin
	numPlayers = num; strcpy (name, nam);
}																																								//	end

//------------------------------------------------------------------------------
void	StartNetwork (char *server, int port, Bool net)														//	start up network play
{																																								//	begin
	if (net)
	{
		gDisplay->SetRate (R(0.05));
		gClient = new Network (0, FALSE);
		gClient->Connect (server, port);
		PlayerStart		player (net, ParmString (PLAYER_NAME));
		StartupSynch	synch;
		gClient->Write (&player, sizeof (PlayerStart));															//	read the player info
		gClient->Read (&synch, sizeof (StartupSynch));															//	read the player info
		gNumPlayers = synch.numPlayers;
		for (int i = 0; i < gNumPlayers; i++)
			if (i == synch.id)
				gPlayers[i] = gLocal = new Local (synch.names[i], i);
			else
				gPlayers[i] = new Human (synch.names[i], i);
		gRandom.SetIndex (0);
	}
	else
	{
		gPlayers[0] = gLocal = new Local (ParmString (PLAYER_NAME), 0);
		gNumPlayers = 1;
	}
}																																								//	end

//------------------------------------------------------------------------------
void	DoNetwork (Code *code)																										//	run a frame on the network
{																																								//	begin
	if (gClient)
	{
		ServerSynch	ssynch;
		Code				psynch;
		if (code)
			psynch = *code;
		gClient->Write (&psynch, sizeof (Code));
		gClient->Read (&ssynch, sizeof (Real) + (sizeof (Code) * gNumPlayers));
		gReferee->SetInterval (ssynch.rate);
		for (int i = 0; i < gNumPlayers; i++)
		{
			if (gPlayers[i])
			{
				if (ssynch.codes[i].valid)
					gPlayers[i]->Input (&ssynch.codes[i]);
				else if (ssynch.codes[i].quit)
					gPlayers[i]->Quit ();
				gPlayers[i]->Play ();
			}
		}
	}
	else
	{
		if (code)
			gLocal->Input (code);
		gLocal->Play ();
	}
}																																								//	end

//------------------------------------------------------------------------------
void	StopNetwork (void)																												//	clean up after network play
{																																								//	begin
	for (int i = 0; i < gNumPlayers; i++)
		if (gPlayers[i])
			delete gPlayers[i];
	if (gClient)
		delete gClient;
}																																								//	end

//------------------------------------------------------------------------------

