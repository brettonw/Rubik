//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/02/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "network server.h"
#include "input code.h"
#include "get parameters.h"
#include "timer.h"

//------------------------------------------------------------------------------
#define	SMOOTHING_WINDOW	128

//------------------------------------------------------------------------------
Network	*gServer = 0,																														//	pointer to the server network
				*gClients[NUM_NETWORK_PLAYERS];
int			gNumPlayers;																														//	the actual number of players
Timer		gTimer;
double	gFrames[SMOOTHING_WINDOW];
int			gCurrentFrame;
double	gTotalTime;

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
void	StartNetwork ()																														//	start up network play
{																																								//	begin
	printf ("Waiting for connections...\n");
	PlayerStart	player;
	StartupSynch	startup;
	gClients[0] = gServer->Accept ();
	gClients[0]->Read (&player, sizeof (PlayerStart));													//	write back the player info
	startup.numPlayers = gNumPlayers = player.numPlayers;
	strcpy (startup.names[0], player.name);
	printf ("Got connection 1 of %d\n", gNumPlayers);

	for (int i = 1; i < gNumPlayers; i++)
	{
		gClients[i] = gServer->Accept ();
		gClients[i]->Read (&player, sizeof (PlayerStart));												//	write back the player info
		strcpy (startup.names[i], player.name);
		printf ("Got connection %d\n", i + 1);
	}
	for (i = 0; i < gNumPlayers; i++)
	{
		startup.id = i;
		gClients[i]->Write (&startup, sizeof (StartupSynch));													//	write back the player info
	}
	gCurrentFrame = 0;
	double	rate = 0.05;
	gTotalTime = rate * SMOOTHING_WINDOW;
	for (i = 0; i < SMOOTHING_WINDOW; i++)
		gFrames[i] = rate;
	gTimer.Start ();
}																																								//	end

//------------------------------------------------------------------------------
Bool	DoNetwork (void)																													//	run a frame on the network
{																																								//	begin
	ServerSynch	synch;
	int					totalRead = 0;
	for (int i = 0; i < gNumPlayers; i++)
		if (gClients[i])
		{
			int		read = gClients[i]->Read (&synch.codes[i], sizeof (Code));
			totalRead += read;
			if (!read)
			{
				printf ("Player %d quit\n", i + 1);
				delete gClients[i];
				gClients[i] = 0;
				synch.codes[i].quit = 1;
			}
		}

	synch.rate = Real (gTotalTime * (1.0 / SMOOTHING_WINDOW));
	gTotalTime -= gFrames[gCurrentFrame];
	gFrames[gCurrentFrame] = gTimer.Interval ();
	if (gFrames[gCurrentFrame] > ParmReal (MAXIMUM_FRAME_INTERVAL))
		gFrames[gCurrentFrame] = ParmReal (MAXIMUM_FRAME_INTERVAL);
	gTotalTime += gFrames[gCurrentFrame];
	gCurrentFrame = (gCurrentFrame + 1) & (SMOOTHING_WINDOW - 1);

	if (totalRead > 0)
	{
		for (i = 0; i < gNumPlayers; i++)
			if (gClients[i])
				gClients[i]->Write (&synch, sizeof (Real) + (sizeof (Code) * gNumPlayers));
		return TRUE;
	}
	return FALSE;
}																																								//	end

//------------------------------------------------------------------------------
void	StopNetwork (void)																												//	clean up after network play
{																																								//	begin
	for (int i = 0; i < gNumPlayers; i++)
		if (gClients[i])
			delete gClients[i];
	printf ("Play Rate: %g fps\nGame Over\n\n", SMOOTHING_WINDOW / gTotalTime);
}																																								//	end

//------------------------------------------------------------------------------
void	main (void)																																//	I'm a little raincloud, of course
{																																								//	begin
	gParmBase = new ParmBase ("parameters.h");																		//	build the parameters database
	Network::Open ();
	printf ("Server at %s\n", Network::HostName ());
	gServer = new Network (ParmInteger (SERVER_PORT), FALSE);
	gServer->Listen ();
//	while (1)
	{
		StartNetwork ();
		while (DoNetwork ()) {}
		StopNetwork ();
	}
	delete gServer;
	Network::Close ();
	delete gParmBase;
}																																								//	end

//------------------------------------------------------------------------------
