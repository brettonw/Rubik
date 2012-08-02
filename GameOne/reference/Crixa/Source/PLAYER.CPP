//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//	02/21/97	JB		added team identification initialization
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "player.h"

//------------------------------------------------------------------------------
Player::Player (cstr nam)																												//	constructor
{																																								//	begin
	strcpy (name, nam);																														//	copy the name
	kills = deaths = turretkills = resources = suicides = score = 0;							//	zero the scoring stats
	team = 0;																																			//	make the player neutral
}																																								//	end

//------------------------------------------------------------------------------
Player::~Player (void)																													//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
