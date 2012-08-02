//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "keyboard relative pointer.h"
#include "direction.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
KeyboardRPointer::KeyboardRPointer (Keyboard *kbd, int rat)											//	constructor
{																																								//	begin
	keyboard = kbd;
	rate = rat;
	current = 0;
	SetKeys (ParmString (KEYBOARD_REL_PTR_KEYS));
}																																								//	end

//------------------------------------------------------------------------------
KeyboardRPointer::~KeyboardRPointer (void)																			//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	KeyboardRPointer::Update (int count)																			//	update the pointer status
{																																								//	begin
	if (PointerDevice::Update (count))																						//	if the update should occur
	{																																							//	begin
		if (keyboard->GetKey (left))
		{
			current += rate;
			keyboard->SetKey (left, FALSE);
			TimeStamp ();
		}
		if (keyboard->GetKey (right))
		{
			current -= rate;
			keyboard->SetKey (right, FALSE);
			TimeStamp ();
		}
		current &= (DIRECTION_COUNT - 1);
		return TRUE;																																//	return true to indicate the update occurred
	}																																							//	end
	return FALSE;																																	//	return false to indicate that no update occurred
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d	KeyboardRPointer::GetPos (void)																				//	get the mouse position
{																																								//	begin
	return gDirection[current][0];
}																																								//	end

//------------------------------------------------------------------------------
void	KeyboardRPointer::SetKeys (cstr string)																		//	set the left and right keys from a string
{																																								//	begin
	left = atoi (string);
	while (!isspace (*string))
		string++;
	string++;
	right = atoi (string);
}																																								//	end

//------------------------------------------------------------------------------
