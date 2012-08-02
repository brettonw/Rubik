//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/31/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "joystick POV pointer.h"
#include "math utility.h"

//------------------------------------------------------------------------------
JoystickPOVPointer::JoystickPOVPointer (JoystickDevice *dev, Real f)						//	constructor
{																																								//	begin
	device = dev;																																	//	copy the joystick device pointer
	filter = f;																																		//	copy the filter size
	vector (ZERO, ZERO);																													//	zero the vector to start
}																																								//	end

//------------------------------------------------------------------------------
JoystickPOVPointer::~JoystickPOVPointer (void)																	//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	JoystickPOVPointer::Update (int count)																		//	update the joystick status
{																																								//	begin
	if (PointerDevice::Update (count))																						//	if the update should occur
	{																																							//	begin
		device->Update (count);
		Real	pov = Real (device->GetAxis (POV_AXIS)) * R(1.0e-2),
					x = vector[X], 
					y = vector[Y];
		if ((pov >= ZERO) && (pov < R(360.0)))
		{
			Real	angle = DegreesToRadians (R(450.0) - pov);
			x = COS (angle);
			y = SIN (angle);
		}
		if (FABS (x - vector[X]) < filter)
			x = vector[X];
		if (FABS (y - vector[Y]) < filter)
			y = vector[Y];
		if ((x != vector[X]) || (y != vector[Y]))
		{
			vector (x, y);																														//	store the vector of the joystick position
			TimeStamp ();
		}
		return TRUE;																																//	return true to indicate the update occurred
	}																																							//	end
	return FALSE;																																	//	return false to indicate that no update occurred
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d	JoystickPOVPointer::GetPos (void)																			//	get the joystick position
{																																								//	begin
	return vector;																																//	return the vector position of the joystick
}																																								//	end

//------------------------------------------------------------------------------
