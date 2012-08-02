//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

#ifndef	KEYBOARD_RELATIVE_POINTER
#define	KEYBOARD_RELATIVE_POINTER

//------------------------------------------------------------------------------
#include "pointer device.h"
#include "keyboard.h"

//------------------------------------------------------------------------------
class KeyboardRPointer : public PointerDevice																		//	keyboard input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Keyboard	*keyboard;																										//	pointer to the keyboard device
				int				rate;																													//	the rate at which the pointer steps through the direction table
				int				current;																											//	the current index into the table
				int				left;																													//	the key code for rotating left
				int				right;																												//	the key code for rotating right
	public:																																				//	members available externally
				KeyboardRPointer (Keyboard *keyboard, int rate);												//	constructor
				~KeyboardRPointer (void);																								//	destructor
virtual	Bool				Update (int count);																					//	update the pointer status
virtual	Vector_2d		GetPos (void);																							//	get the pointer position
				void				SetKeys (cstr string);																			//	set the left and right keys from a string
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	KEYBOARD_RELATIVE_POINTER