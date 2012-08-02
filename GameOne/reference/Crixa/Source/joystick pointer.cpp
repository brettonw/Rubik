//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/31/96
//	03/09/97	DS		Added joystick validity checking to avoid float exceptions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "joystick pointer.h"
#include "pi.h"
#include "debugger.h"

//------------------------------------------------------------------------------
#include "view.h"

//------------------------------------------------------------------------------
JoystickPointer::JoystickPointer (JoystickDevice *dev, Real f, Bool zm)					//	constructor
{																																								//	begin
	device = dev;																																	//	copy the joystick device pointer
	vector (ZERO, ZERO);																													//	zero the vector to start
	filter = f;																																		//	copy the filter size
	zero = zm;																																		//	behavior in dead zone flag
	devOK = dev->Valid();
	if (devOK)																																		//	make sure underlying device is functional
		SetAxes (X, Y, FALSE, FALSE);																								//	start with the axes at X and Y
	else
		DEBUG_SOFT ("\nJoystickPointer:\n    JoystickDevice is not valid\n");
}																																								//	end

//------------------------------------------------------------------------------
JoystickPointer::~JoystickPointer (void)																				//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	JoystickPointer::Update (int count)																				//	update the joystick status
{																																								//	begin
	if (!devOK) return FALSE;																											//	if device is invalid, then bail
	
	if (PointerDevice::Update (count))																						//	if the update should occur
	{																																							//	begin
		device->Update (count);
		Real	x = (Real (device->GetAxis (axis[X])) - joyMid[X]) * scale[X],				//	compute the normalized x coordinate of the position
					y = -(Real (device->GetAxis (axis[Y])) - joyMid[Y]) * scale[Y];				//	compute the normalized y coordinate of the position
		if (FABS (x - vector[X]) < filter)
			x = (zero) ? ZERO : vector[X];
		if (FABS (y - vector[Y]) < filter)
			y = (zero) ? ZERO : vector[Y];
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
Vector_2d	JoystickPointer::GetPos (void)																				//	get the joystick position
{																																								//	begin
	return vector;																																//	return the vector position of the joystick
}																																								//	end

//------------------------------------------------------------------------------
void	JoystickPointer::SetAxes (int x, int y, Bool inv1, Bool inv2)							//	set the axes to check on the joystick
{																																								//	begin
	axis[X] = x;																																	//	default to the x axis
	axis[Y] = y;																																	//	default to the y axis
	Real			minx = Real (device->GetAxisMin (x)),
						miny = Real (device->GetAxisMin (y)),
						xspan = Real (device->GetAxisMax (x)) - minx,
						yspan = Real (device->GetAxisMax (y)) - miny,
						width = xspan * R(0.5),																							//	compute the width of the joystick travel
						height = yspan * R(0.5);																						//	compute the height of the joystick travel

	if ((width>ZERO)&&(yspan>ZERO))
	{
		Vector_2d	halfSize (width, height),
							minimum (minx, miny);																								//	set up a vector of the bottom left corner
		joyMid = halfSize + minimum;																									//	compute the mid point of the joystick travel
		scale = Vector_2d (R(1.0) / width, R(1.0) / height);													//	compute a scaling vector for projection into the unit vector
		if (inv1)
			scale[X] *= R(-1.0);
		if (inv2)
			scale[Y] *= R(-1.0);
	}
	else
	{
		DEBUG_SOFT ("JoystickPointer::SetAxes()\n");
		DEBUG_SOFT ("    Device axis range is invalid\n");
		devOK=FALSE;
	}
}																																								//	end

//------------------------------------------------------------------------------
