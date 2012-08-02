//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/31/96
//	03/09/97	DS		Joystick error checking
//	03/14/97	DS		Zero Stick Behavior Added for filtering
//------------------------------------------------------------------------------

#ifndef	JOYSTICK_POINTER
#define	JOYSTICK_POINTER

//------------------------------------------------------------------------------
#include "pointer device.h"
#include "joystick device.h"

//------------------------------------------------------------------------------
class JoystickPointer : public PointerDevice																		//	joystick input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				JoystickDevice	*device;																								//	pointer to the joystick device to extract from
				int							axis[2];																								//	the actual axis indices
				Vector_2d				joyMid;																									//	midpoint of the joystick range of motion
				Vector_2d				scale;																									//	scaling vector for normalization of the position to screen space
				Vector_2d				vector;																									//	the vector for the current position
				Real						filter;																									//	the size of the noise filter 
				Bool						devOK;																									//	status of the connected device
				Bool						zero;
	public:																																				//	members available externally
				JoystickPointer (JoystickDevice *dev, Real filter, Bool zero = TRUE);		//	constructor
virtual	~JoystickPointer (void);																								//	destructor
virtual	Bool						Update (int count);																			//	update the pointer status
virtual	Vector_2d				GetPos (void);																					//	get the pointer position
				void						SetAxes (int axis1, int axis2, Bool inv1, Bool inv2);		//	set the axes to check on the joystick
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	JOYSTICK_POINTER