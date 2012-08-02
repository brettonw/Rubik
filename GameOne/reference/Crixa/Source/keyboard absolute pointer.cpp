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
#include "keyboard absolute pointer.h"
#include "direction.h"
#include "pi.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
void	KeyboardAPointer::DoDoubleTap (Bool down)																	//	handle the double tap process
{																																								//	begin
	tapCount[0]++;
	tapCount[1]++;
	if (down)
	{
		if (!wasDown)
		{
			if (tapCount[tapCurrent] < ParmInteger (DOUBLE_TAP_DELAY))
			{
				multiplier = range;
				tappedCount = 0;
			}
			tapCount[tapCurrent] = 0;
			tapCurrent ^= 0x01;
		}
		else if (++tappedCount >= tapDuration)
			multiplier = R(1.0);
		wasDown = TRUE;
	}
	else
	{
		multiplier = R(1.0);
		wasDown = FALSE;
	}
	vector *= multiplier;
}																																								//	end

//------------------------------------------------------------------------------
KeyboardAPointer::KeyboardAPointer (Keyboard *kbd, Bool cen, Real ran, int tap)	//	constructor
{																																								//	begin
	keyboard = kbd;
	vector (ZERO, ZERO);
	autocenter = cen;
	range = ran;
	tapDuration = tap;
	tapCurrent = 0;
	tapCount[0] = tapCount[1] = 0;
	wasDown = FALSE;
	multiplier = R(1.0);
}																																								//	end

//------------------------------------------------------------------------------
KeyboardAPointer::~KeyboardAPointer (void)																			//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	KeyboardAPointer::Update (int count)																			//	update the pointer status
{																																								//	begin
	if (PointerDevice::Update (count))																						//	if the update should occur
	{																																							//	begin
		Bool	changed = FALSE;
		if (autocenter)
		{
			vector (ZERO, ZERO);
			for (int i = 0; i < keyCount; i++)
				if (keyboard->GetKey (keys[i]))
				{
					vector += table[i];
					keyboard->SetKey (keys[i], FALSE);
					changed = TRUE;
				}
			if (changed)
			{
				TimeStamp ();
				vector.Normalize ();
			}
		}
		else
		{
			Vector_2d	vec (ZERO, ZERO);
			for (int i = 0; i < keyCount; i++)
				if (keyboard->GetKey (keys[i]))
				{
					vec += table[i];
					keyboard->SetKey (keys[i], FALSE);
					changed = TRUE;
				}
			if (changed)
			{
				TimeStamp ();
				vector = vec;
				vector.Normalize ();
			}
		}
		DoDoubleTap (changed);
		return TRUE;																																//	return true to indicate the update occurred
	}																																							//	end
	return FALSE;																																	//	return false to indicate that no update occurred
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d	KeyboardAPointer::GetPos (void)																				//	get the mouse position
{																																								//	begin
	return vector;
}																																								//	end

//------------------------------------------------------------------------------
void	KeyboardAPointer::SetKeys (cstr string)																		//	set the keys
{																																								//	begin
	keyCount = atoi (string);
	Real	rads = TWO_PI / keyCount;
	for (int i = 0; i < keyCount; i++)
	{
		while (!isspace (*string))
			string++;
		string++;
		keys[i] = atoi (string);
		table[i] = Vector_2d (rads * i);
	}
}																																								//	end

//------------------------------------------------------------------------------
