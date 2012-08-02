//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/20/96
//
//	02/24/97	DS		Added AltDrawMode
//	02/27/97	JB vs DS		Fixed Flashing
//	03/06/97	DS		Modified Draw to support modes above 1.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "sequence visual.h"

//------------------------------------------------------------------------------
SeqVisual::SeqVisual (Body *bod, const SeqPtr &seq) : Visual (bod)							//	constructor
{																																								//	begin
	sequence = seq;																																//	copy the sequence
}																																								//	end

//------------------------------------------------------------------------------
SeqVisual::~SeqVisual (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	SeqVisual::Draw (CPoint where)																						//	draw the object
{																																								//	begin
	if (sequence ())
	{
		if (mode && (lastmode == mode) && (mode == VISFLASH))
			mode = VISNORMAL;
		if (!sequence->Step (body->TLocation () - where, mode))
			sequence = SeqPtr (0);
		lastmode = mode;
		//mode = VISNORMAL;	I don't think this is needed DS
	}
}																																								//	end

//------------------------------------------------------------------------------
