//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/27/97
//	03/09/97	DS		Added new event type codes
//
//------------------------------------------------------------------------------

#ifndef	DEVICE
#define	DEVICE

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "list.h"

//------------------------------------------------------------------------------
enum	{	DEV_TRIGGER = 0,
				DEV_POWER = 1, 
				DEV_TOGGLE = 2,
				//	messages for level logic control
				DEV_END_LEVEL = 3,
				DEV_PRIVATE_MESSAGE = 4,
				DEV_PUBLIC_MESSAGE = 5,
				DEV_MISSION_MESSAGE = 6,
				DEV_SHIP_MESSAGE = 7,
				DEV_TEXT_MESSAGE = 10,
				DEV_CDPLAYTRACK = 11,
				DEV_PLAYSOUND = 12
			};																	//	event type codes for the triggers

//------------------------------------------------------------------------------
class Device																																		//	a device in a system
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				power;																												//	power to the device
				Bool			on;																														//	the on/off state of the device
				Bool			route;																												//	the current flow state
				List			children;																											//	child devices for propogating a power event
				int				identifier;																										//	identifier for status reporting
	public:																																				//	members available externally
				Device (int id);																												//	constructor
virtual	~Device (void);																													//	destructor
virtual	int				Trigger (const PiecePtr &piece, int type, int what);					//	trigger the device
virtual	Bool			SwitchOn (void);																							//	turn the device on
virtual	Bool			SwitchOff (void);																							//	turn the device off
virtual	void			Toggle (void);																								//	toggle the route state
				void			Propogate (const PiecePtr &piece, int what);									//	propogate a power message to the children
				void			Connect (Device *device);																			//	attach a device onto the child chain
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	DEVICE
