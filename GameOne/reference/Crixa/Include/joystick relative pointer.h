//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/31/96
//
//------------------------------------------------------------------------------

#ifndef	JOYSTICK_RELATIVE_POINTER
#define	JOYSTICK_RELATIVE_POINTER

//------------------------------------------------------------------------------
#include "pointer device.h"
#include "joystick device.h"

//------------------------------------------------------------------------------
class JoystickRPointer : public PointerDevice																		//	joystick input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				JoystickDevice	*device;																								//	pointer to the joystick device to extract from
				int							axis;																										//	the actual axis index
				Real						joyMid;																									//	midpoint of the joystick range of motion
				Real						scale;																									//	scaling vector for normalization of the position to screen space
				Real						angle;																									//	the angle accumulator
				Vector_2d				vector;																									//	the vector for the current position
				Real						filter;																									//	the size of the noise filter 
				Real						rate;																										//	the rotation rate relative to the axis change
	public:																																				//	members available externally
				JoystickRPointer (JoystickDevice *dev, Real filter, Real rate);					//	constructor
virtual	~JoystickRPointer (void);																								//	destructor
virtual	Bool						Update (int count);																			//	update the pointer status
virtual	Vector_2d				GetPos (void);																					//	get the pointer position
				void						SetAxis (int axis, Bool invert = FALSE);								//	set the axis to check on the joystick
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	JOYSTICK_RELATIVE_POINTER