//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/27/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "independent.h"
#include "player node.h"

//------------------------------------------------------------------------------
Independent	*gIndependent;																											//	global independent player

//------------------------------------------------------------------------------
Independent::Independent (void) : Player ("Independent")												//	constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Independent::~Independent (void)																								//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	Independent::Play (void)																									//	play one frame
{																																								//	begin
	PlayerNode	*node = (PlayerNode *) players.Head ();														//	get the first node in the sub-player list
	while (node)																																	//	loop over all of the players in the list
	{																																							//	begin
		PlayerNode	*next = (PlayerNode *) players.Next (node);											//	get the next node
		if (!node->player->Play ())																									//	give the sub-player time
		{																																						//	begin
			players.Remove (node);																										//	remove the sub-player from the list
			delete node;																															//	delete the node
		}																																						//	end
		node = next;																																//	move on to the next node
	}																																							//	end
	return TRUE;																																	//	the independent player is always part of the game
}																																								//	end

//------------------------------------------------------------------------------
void	Independent::Install (Player *player)																			//	install a new sub player
{																																								//	begin
	players.InsertAfter (new PlayerNode (player));																//	allocate and install a new sub-player
}																																								//	end

//------------------------------------------------------------------------------
