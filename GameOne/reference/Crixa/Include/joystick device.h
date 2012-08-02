//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/31/96
//
//	03/09/97	DS		Joystick Validity Test
//------------------------------------------------------------------------------

#ifndef	JOYSTICK_DEVICE
#define	JOYSTICK_DEVICE

//------------------------------------------------------------------------------
#include <mmsystem.h>
#include "keyboard.h"

//------------------------------------------------------------------------------
#define	POV_AXIS	8																															//	value to denote the "axis" containing the POV value

//------------------------------------------------------------------------------
class JoystickDevice																														//	joystick input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Keyboard		*keyboard;																									//	pointer to the keyboard
				int					map[32];																										//	array of button to virtual key code maps
				int					updateCount;																								//	the counter for knowing whether or not to update
				int					id;																													//	id of the joystick to use
				JOYINFOEX		info;																												//	joystick info structure
				JOYCAPS			caps;																												//	joystick capabilities record
				MMRESULT		error;																											//	the error state of the joystick
static	int					lookup[32];																									//	table of button ids for lookup
	public:																																				//	members available externally
				JoystickDevice (Keyboard *kbd, int id = 0);															//	constructor
				~JoystickDevice (void);																									//	destructor
				void				Update (int count);																					//	update the pointer status
				uint				GetAxis (int axis);																					//	return the specified index
				uint				GetAxisMin (int axis);																			//	return the min value for the axis
				uint				GetAxisMax (int axis);																			//	return the max value for the axis
				Bool				GetKey (int key);																						//	return whether or not a selected button is pressed
				void				MapButton (int button, int keycode);												//	map a button on the pointer to a virtual key code
				void				Reset (void);																								//	reset the joystick and error conditions
				Bool				Valid (void) { return (error==JOYERR_NOERROR); }						//	allow outside to see if joystick is valid or not
};																																							//	end

//------------------------------------------------------------------------------
inline	uint		JoystickDevice::GetAxis (int axis)															//	return the specified index
{																																								//	begin
	if (error == JOYERR_NOERROR)																									//	if there is no error
		return ((uint *) &info.dwXpos)[axis];																				//	return the requested axis value
	return 0;																																			//	return zero to indicate no value
}																																								//	end

//------------------------------------------------------------------------------
inline	uint	JoystickDevice::GetAxisMin (int axis)															//	return the min value for the axis
{																																								//	begin
	uint	*vals = (axis < 3) ? &caps.wXmin : &caps.wYmax;													//	pointer to a block of range values
	axis <<= 1;																																		//	compute the axis offset
	return vals[axis];																														//	return the min for the requested axis
}																																								//	end

//------------------------------------------------------------------------------
inline	uint	JoystickDevice::GetAxisMax (int axis)															//	return the max value for the axis
{																																								//	begin
	uint	*vals = (axis < 3) ? &caps.wXmin : &caps.wYmax;													//	pointer to a block of range values
	axis <<= 1;																																		//	compute the axis offset
	return vals[axis + 1];																												//	return the max for the requested axis
}																																								//	end

//------------------------------------------------------------------------------
inline	Bool	JoystickDevice::GetKey (int key)																	//	return whether or not a selected button is pressed
{																																								//	begin
	if (error == JOYERR_NOERROR)																									//	if there is no error
		return (info.dwButtons & lookup[key]) ? TRUE : FALSE;												//	return the state of the requested button
	return FALSE;																																	//	return false as the default value
}																																								//	end

//------------------------------------------------------------------------------
inline	void	JoystickDevice::MapButton (int button, int keycode)								//	map a button on the pointer to a virtual key code
{																																								//	begin
	map[button] = keycode;																												//	store the virtual keycode associated with the button
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	JOYSTICK_DEVICE