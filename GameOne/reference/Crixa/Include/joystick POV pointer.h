//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/31/96
//
//------------------------------------------------------------------------------

#ifndef	JOYSTICK_POV_POINTER
#define	JOYSTICK_POV_POINTER

//------------------------------------------------------------------------------
#include "pointer device.h"
#include "joystick device.h"

//------------------------------------------------------------------------------
class JoystickPOVPointer : public PointerDevice																	//	joystick input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				JoystickDevice	*device;																								//	pointer to the joystick device to extract from
				Vector_2d				vector;																									//	the vector for the current position
				Real						filter;																									//	the size of the noise filter 
	public:																																				//	members available externally
				JoystickPOVPointer (JoystickDevice *dev, Real filter);									//	constructor
virtual	~JoystickPOVPointer (void);																							//	destructor
virtual	Bool						Update (int count);																			//	update the pointer status
virtual	Vector_2d				GetPos (void);																					//	get the pointer position
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	JOYSTICK_POV_POINTER