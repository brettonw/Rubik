//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

#ifndef	KEYBOARD_ABSOLUTE_POINTER
#define	KEYBOARD_ABSOLUTE_POINTER

//------------------------------------------------------------------------------
#include "pointer device.h"
#include "keyboard.h"

//------------------------------------------------------------------------------
class KeyboardAPointer : public PointerDevice																		//	keyboard input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Keyboard		*keyboard;																									//	pointer to the keyboard device
				Bool				autocenter;																									//	whether or not to center the pointer if there is no input
				int					keyCount;																										//	the number of key mappings
				Vector_2d		table[8];																										//	array of vectors mapping to the keys
				int					keys[8];																										//	array of keys
				Vector_2d		vector;																											//	the current vector
				int					tapCount[2];
				int					tapCurrent;
				int					tappedCount;
				int					tapDuration;																								//	how long the double tap effect lasts
				Bool				wasDown;
				Real				range;																											//	the range of the double tap modifier
				Real				multiplier;																									//	double tap multiplier
				void				DoDoubleTap (Bool changed);																	//	handle the double tap process
	public:																																				//	members available externally
				KeyboardAPointer (Keyboard *kbd, Bool center, Real range, int tapDur);	//	constructor
				~KeyboardAPointer (void);																								//	destructor
virtual	Bool				Update (int count);																					//	update the pointer status
virtual	Vector_2d		GetPos (void);																							//	get the pointer position
				void				SetKeys (cstr string);																			//	set the keys
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	KEYBOARD_ABSOLUTE_POINTER