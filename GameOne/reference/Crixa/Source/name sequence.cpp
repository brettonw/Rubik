//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/027/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "name sequence.h"

//------------------------------------------------------------------------------
NameSeq::NameSeq (Player *play, SArrayPtr array)																//	constructor
{																																								//	begin
	player = play;																																//	copy the player pointer
	font = array;																																	//	copy the sprites array
}																																								//	end

//------------------------------------------------------------------------------
NameSeq::~NameSeq (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	NameSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	char	tex[64],
				*text = &tex[0];
	sprintf (tex, "%s %d/%d", player->Name (), player->GetTurretKills () + player->GetKills () - player->GetSuicides (), player->GetDeaths ());
	int		ix = int (where[X]),
				x = 0,
				y = int (where[Y]);
	while (*text)																																	//	loop up to the current position in the string
	{
		switch (*text)
		{
			case ' ':
				x += font['n']->Width();
				break;
			default:
				font[*text]->Draw (Point_2d (Real (x + ix), Real (y)), TRUE);					//	draw the string up to the current point
				x += font[*text]->Width ();
				break;
		}
		text++;
	}
	return TRUE;																																	//	always return true
}																																								//	end

//------------------------------------------------------------------------------
