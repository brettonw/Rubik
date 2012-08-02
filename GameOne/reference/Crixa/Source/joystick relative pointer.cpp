//------------------------------------------------------------------------------
//	Copyright �1996 qualia, inc. All rights reserved.
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
#include "joystick relative pointer.h"
#include "pi.h"
#include "debugger.h"

//------------------------------------------------------------------------------
JoystickRPointer::JoystickRPointer (JoystickDevice *dev, Real f, Real r)				//	constructor
{																																								//	begin
	device = dev;																																	//	copy the joystick device pointer
	filter = f;																																		//	copy the filter size
	rate = r;																																			//	copy the rotation rate
	angle = ZERO;																																	//	zero the starting angle
	vector (ZERO, ZERO);																													//	zero the vector to start
	SetAxis (X);																																	//	start with the axis at X
}																																								//	end

//------------------------------------------------------------------------------
JoystickRPointer::~JoystickRPointer (void)																			//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	JoystickRPointer::Update (int count)																			//	update the joystick status
{																																								//	begin
	if (PointerDevice::Update (count))																						//	if the update should occur
	{																																							//	begin
		device->Update (count);
		Real	pos = (Real (device->GetAxis (axis)) - joyMid) * scale;								//	compute the normalized axis value
		angle += pos;
		Real	x = COS (angle),
					y = SIN (angle);
		DEBUG_STR ("value (%d), pos (%f), scale (%f)\n", device->GetAxis (axis), pos, scale);
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
Vector_2d	JoystickRPointer::GetPos (void)																				//	get the joystick position
{																																								//	begin
	return vector;																																//	return the vector
}																																								//	end

//------------------------------------------------------------------------------
void	JoystickRPointer::SetAxis (int x, Bool invert)														//	set the axes to check on the joystick
{																																								//	begin
	axis = x;																																			//	copy the x axis
	Real	minx = Real (device->GetAxisMin (x)),
				width = (Real (device->GetAxisMax (x)) - minx) * R(0.5);								//	compute the width of the joystick travel
	scale = (R(1.0) / width) * rate;																							//	compute a scale value for projection into the unit length
	if (invert)																																		//	if the invert flag is set
		scale *= R(-1.0);																														//	multiply the scale factor by -1
	joyMid = minx + width;																												//	compute the mid point of the joystick travel
}																																								//	end

//------------------------------------------------------------------------------
