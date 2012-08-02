//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/06/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "rotating.h"
#include "pi.h"

//------------------------------------------------------------------------------
Rotating::Rotating (Body *bod, SArrayPtr array) : Visual (bod), sprites (array)	//	constructor
{																																								//	begin
	Real	index = body->RLocation () * OVER_TWO_PI * sprites.Count ();						//	convert the rotation angle to the range 0..1 and multiply by the number of rotations
	sprites.SetCurrent (ushort (index + R(0.5)));																	//	set the current frame to the current rotation
	if (sprites.Count () == 32)																										//	if the sprite array has 32 rotations
		shift = 5;																																	//	set the shift value to 5
	else																																					//	otherwise
		shift = 4;																																	//	set the shift value to 4
}																																								//	end

//------------------------------------------------------------------------------
Rotating::~Rotating (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Rotating::Draw (CPoint where)																							//	draw the object
{																																								//	begin
	Real	index = body->RLocation () * OVER_TWO_PI * sprites.Count ();						//	convert the rotation angle to the range 0..1 and multiply by the number of rotations
	if (FABS (index - sprites.GetCurrent ()) > R(0.6))														//	schmidt trigger to prevent oscillation
		sprites.SetCurrent (ushort (index + R(0.5)));																//	set the current frame to the current rotation
	
	if (mode && lastmode == mode)
		mode = VISNORMAL;
	sprites.Step (body->TLocation () - where, mode);
	lastmode = mode;
	mode = VISNORMAL;

/*
	if (mode)																																			//	check the drawing mode
	{
		if (lastmode!=mode)
			sprites.Step (body->TLocation () - where, VISFLASH);											//	draw the current frame FLASH
		else
			sprites.Step (body->TLocation () - where);																//	draw the current frame FLASH
	}
	else
		sprites.Step (body->TLocation () - where);																	//	draw the current frame
	lastmode = mode;
	mode = VISNORMAL;																															//	reset the flash behavior
*/
}																																								//	end

//------------------------------------------------------------------------------
void	Rotating::Update (void)																										//	update the visual
{																																								//	begin
	Real	index = body->RLocation () * OVER_TWO_PI * sprites.Count ();						//	convert the rotation angle to the range 0..1 and multiply by the number of rotations
	if (FABS (index - sprites.GetCurrent ()) > R(0.6))														//	schmidt trigger to prevent oscillation
		sprites.SetCurrent (ushort (index + R(0.5)));																//	set the current frame to the current rotation
}																																								//	end

//------------------------------------------------------------------------------
