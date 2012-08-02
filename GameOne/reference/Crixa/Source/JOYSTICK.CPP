//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/12/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "joystick.h"
#include "area.h"

//------------------------------------------------------------------------------
int		Joystick::lookup[32] = {JOY_BUTTON1,  JOY_BUTTON2,  JOY_BUTTON3,					//	table of button ids for lookup
															JOY_BUTTON4,  JOY_BUTTON5,  JOY_BUTTON6,					//	table of button ids for lookup (continued)
															JOY_BUTTON7,  JOY_BUTTON8,  JOY_BUTTON9,					//	table of button ids for lookup (continued)
															JOY_BUTTON10, JOY_BUTTON11, JOY_BUTTON12,					//	table of button ids for lookup (continued)
															JOY_BUTTON13, JOY_BUTTON14, JOY_BUTTON15,					//	table of button ids for lookup (continued)
															JOY_BUTTON16, JOY_BUTTON17, JOY_BUTTON18,					//	table of button ids for lookup (continued)
															JOY_BUTTON19, JOY_BUTTON20, JOY_BUTTON21,					//	table of button ids for lookup (continued)
															JOY_BUTTON22, JOY_BUTTON23, JOY_BUTTON24,					//	table of button ids for lookup (continued)
															JOY_BUTTON25, JOY_BUTTON26, JOY_BUTTON27,					//	table of button ids for lookup (continued)
															JOY_BUTTON28, JOY_BUTTON29, JOY_BUTTON30,					//	table of button ids for lookup (continued)
															JOY_BUTTON31, JOY_BUTTON32};											//	table of button ids for lookup (continued)

//------------------------------------------------------------------------------
Joystick::Joystick (int i)																											//	constructor
{																																								//	begin
	id = i;																																				//	save the joystick identifier
	info.dwSize = sizeof (info);																									//	set up the info size
	info.dwFlags = JOY_RETURNBUTTONS | JOY_RETURNX | JOY_RETURNY;									//	set the desired data to be extracted from a joystick
	JOYCAPS		joycaps;																														//	joystick capabilities record
	joyGetDevCaps (id, &joycaps, sizeof (joycaps));																//	get the joystick capabilities
	Real			width = Real (joycaps.wXmax - joycaps.wXmin),												//	compute the width of the joystick travel
						height = Real (joycaps.wYmax - joycaps.wYmin);											//	compute the height of the joystick travel
	Vector_2d	minimum (Real (joycaps.wXmin), Real (joycaps.wYmin));								//	set up a vector of the bottom left corner
	joyMid = minimum + (Vector_2d (width, height) * R(0.5));											//	compute the mid point of the joystick travel
	scale = Vector_2d (SCRN_HEIGHT / width, SCRN_HEIGHT / height);								//	compute a scaling vector for projection into screen space
}																																								//	end

//------------------------------------------------------------------------------
Joystick::~Joystick (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Joystick::Update (void)																										//	update the joystick status
{																																								//	begin
	joyGetPosEx (id, &info);																											//	update the joystick info
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d	Joystick::GetPos (void)																								//	get the joystick position
{																																								//	begin
	Real	x = Real (info.dwXpos) - joyMid[X],																			//	compute the x coordinate of the position
				y = -(Real (info.dwYpos) - joyMid[Y]);																	//	compute the y coordinate of the position
	return Vector_2d (x * scale[X], y * scale[Y]);																//	return a vector of the joystick position in screen space
}																																								//	end

//------------------------------------------------------------------------------
Bool	Joystick::Button (int button)																							//	get a button status
{																																								//	begin
	return info.dwButtons & lookup[button];																				//	return whether or not the queried button is set
}																																								//	end

//------------------------------------------------------------------------------
