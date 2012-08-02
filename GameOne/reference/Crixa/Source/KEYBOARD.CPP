//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/19/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "keyboard.h"

//------------------------------------------------------------------------------
Keyboard::Keyboard (void)																												//	constructor
{																																								//	begin
	for (int i = 0; i < 256; i++)
		keyboard[i] = 0;
	keyboard[VK_NUMLOCK] = 0x01;
	SetKeyboardState (keyboard);																									//	set the keyboard state
}																																								//	end

//------------------------------------------------------------------------------
Bool	Keyboard::GetKey (int key)																								//	get a key status
{																																								//	begin
	if (key == VK_CAPITAL)																												//	if the caps lock key is being tested
		return (keyboard[key] & 0x01) ? TRUE : FALSE;																//	return true if the low bit is set
	return (keyboard[key] & 0x80) ? TRUE : FALSE;																	//	return true if the high bit is set
}																																								//	end

//------------------------------------------------------------------------------
void	Keyboard::SetKey (int key, Bool state)																		//	set a key status
{																																								//	begin
	keyboard[key] = state ? 0x80 : 0;																							//	set the key to the state
}																																								//	end

//------------------------------------------------------------------------------
void	Keyboard::Update (void)																										//	get the keyboard state
{																																								//	begin
	GetKeyboardState (keyboard);																									//	load the keyboard state
}																																								//	end

//------------------------------------------------------------------------------
