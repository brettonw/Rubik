//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/30/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "joystick device.h"
#include "debugger.h"

//------------------------------------------------------------------------------
int		JoystickDevice::lookup[32] = {	JOY_BUTTON1,  JOY_BUTTON2,  JOY_BUTTON3,	//	table of button ids for lookup
																			JOY_BUTTON4,  JOY_BUTTON5,  JOY_BUTTON6,	//	table of button ids for lookup (continued)
																			JOY_BUTTON7,  JOY_BUTTON8,  JOY_BUTTON9,	//	table of button ids for lookup (continued)
																			JOY_BUTTON10, JOY_BUTTON11, JOY_BUTTON12,	//	table of button ids for lookup (continued)
																			JOY_BUTTON13, JOY_BUTTON14, JOY_BUTTON15,	//	table of button ids for lookup (continued)
																			JOY_BUTTON16, JOY_BUTTON17, JOY_BUTTON18,	//	table of button ids for lookup (continued)
																			JOY_BUTTON19, JOY_BUTTON20, JOY_BUTTON21,	//	table of button ids for lookup (continued)
																			JOY_BUTTON22, JOY_BUTTON23, JOY_BUTTON24,	//	table of button ids for lookup (continued)
																			JOY_BUTTON25, JOY_BUTTON26, JOY_BUTTON27,	//	table of button ids for lookup (continued)
																			JOY_BUTTON28, JOY_BUTTON29, JOY_BUTTON30,	//	table of button ids for lookup (continued)
																			JOY_BUTTON31, JOY_BUTTON32};							//	table of button ids for lookup (continued)

//------------------------------------------------------------------------------
JoystickDevice::JoystickDevice (Keyboard *kbd, int i)														//	constructor
{																																								//	begin
	keyboard = kbd;																																//	copy the pointer to the keyboard device
	id = i;																																				//	save the joystick identifier
	Reset ();																																			//	reset the joystick state
	for (int j = 0; j < 32; j++)																									//	loop over all of the joystick buttons
		map[j] = VK_UNUSEDKEY;																											//	mapping them to the unused key
}																																								//	end

//------------------------------------------------------------------------------
JoystickDevice::~JoystickDevice (void)																					//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	JoystickDevice::Update (int count)																				//	update the joystick status
{																																								//	begin
	if (count != updateCount)																										//	if the update should occur
	{																																						//	begin
		updateCount = count;																											//	save the count for future updates
		if (error == JOYERR_NOERROR)																							//	if there is no error condition
			if ((error = joyGetPosEx (id, &info)) == JOYERR_NOERROR)								//	update the joystick info
				for (int i = 0; i < 32; i++)																					//	loop over all of the buttons
					if (info.dwButtons & lookup[i])																			//	if the button is pressed
						keyboard->SetKey (map[i], TRUE);																	//	map to the appropriate keys
	}																																						//	end
}																																								//	end

//------------------------------------------------------------------------------
void	JoystickDevice::Reset (void)																							//	reset the joystick and error conditions
{																																								//	begin
	error = joyGetDevCaps (id, &caps, sizeof (caps));															//	get the joystick capabilities
	if (error == JOYERR_NOERROR)																									//	if there is no error
	{																																							//	begin
		info.dwSize = sizeof (info);																								//	set up the info size
		info.dwFlags = JOY_RETURNALL;																								//	set the desired data to be extracted from a joystick
		error = joyGetPosEx (id, &info);																						//	update the device
	}																																							//	end

	if (error != JOYERR_NOERROR)																									//	if there is an error
	{																																							//	begin
		caps.wXmin = caps.wXmax = 0;																								//	zero the axes
		caps.wYmin = caps.wYmax = 0;																								//	zero the axes
		caps.wZmin = caps.wZmax = 0;																								//	zero the axes
		caps.wRmin = caps.wRmax = 0;																								//	zero the axes
		caps.wUmin = caps.wUmax = 0;																								//	zero the axes
		caps.wVmin = caps.wVmax = 0;																								//	zero the axes
		info.dwXpos = info.dwYpos = info.dwZpos = 0;																//	zero the axes
		info.dwRpos = info.dwUpos = info.dwVpos = 0;																//	zero the axes
		info.dwPOV = 0;																															//	zero the axes

		DEBUG_STR ("\nJoystickDevice:\n");																						//	report the status of the devices
		switch(error){
			case MMSYSERR_NODRIVER:
				DEBUG_STR("    The joystick driver is not present.\n");
				break;
			case JOYERR_UNPLUGGED:
				DEBUG_STR("    The specified joystick '%d' is not present.\n",id);
				break;
			default:
				DEBUG_STR("    Error %d\n",error);
		}
	}																																							//	end !=JOYERR_NOERR
}																																								//	end

//------------------------------------------------------------------------------
