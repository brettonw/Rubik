//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/16/96
//	03/07/97	DS		Removed Status and Mission triggering
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "device.h"
#include "device node.h"
#include "trigger array.h"

//------------------------------------------------------------------------------
Device::Device (int id)																													//	constructor
{																																								//	begin
	power = 0;
	on = FALSE;
	identifier = id;
	route = TRUE;
}																																								//	end

//------------------------------------------------------------------------------
Device::~Device (void)																													//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
int	Device::Trigger (const PiecePtr &piece, int type, int what)									//	trigger the device
{																																								//	begin
	switch (type)																																	//	check the event type
	{																																							//	begin
		case DEV_TRIGGER:																														//	an ordinary trigger
			break;																																		//	end case
		case DEV_POWER:																															//	a power flow trigger
			if (route)																																//	if the power should be propogated
				Propogate (piece, what);																								//	propogate the power message
			power += what;																														//	add the new current flow
			if (power)																																//	if we have power
				SwitchOn ();																														//	turn on the device
			else																																			//	otherwise
				SwitchOff ();																														//	switch off the device
			break;																																		//	end case
	}																																							//	end
	return 0;																																			//	return nothing
}																																								//	end

//------------------------------------------------------------------------------
Bool	Device::SwitchOn (void)																										//	turn the device on
{																																								//	begin
	if (!on)																																			//	if the device is not already on
	{																																							//	begin
		on = TRUE;																																	//	turn on the device
		return TRUE;																																//	return true to indicate that the device has to do work to turn on
	}																																							//	end
	return FALSE;																																	//	return false to indicate that the device is already on
}																																								//	end

//------------------------------------------------------------------------------
Bool	Device::SwitchOff (void)																									//	turn the device off
{																																								//	begin
	if (on)																																				//	if the device is on
	{																																							//	begin
		on = FALSE;																																	//	turn off the device
		return TRUE;																																//	return true to indicate that the device has work to do to turn off
	}																																							//	end
	return FALSE;																																	//	return false to indicate that the device is already off
}																																								//	end

//------------------------------------------------------------------------------
void	Device::Toggle (void)																											//	toggle the device flow state
{																																								//	begin
	if (power)																																		//	if the device has power
		if (route)																																	//	if the flow state is on
		{																																						//	begin
			route = FALSE;																														//	switch the flow state to off
			Propogate (PiecePtr (0), -power);																					//	propogate the state change
		}																																						//	end
		else																																				//	otherwise
		{																																						//	begin
			route = TRUE;																															//	switch the flow state to on
			Propogate (PiecePtr (0), power);																					//	propogate the state change
		}																																						//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Device::Propogate (const PiecePtr &piece, int what)												//	propogate a power message to the children
{																																								//	begin
	DeviceNode *node = (DeviceNode *) children.Head ();														//	get the first child
	while (node)																																	//	while there are children
	{																																							//	begin
		node->device->Trigger (piece, DEV_POWER, what);															//	trigger the child with the power state change
		node = (DeviceNode *) children.Next (node);																	//	get the next child
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Device::Connect (Device *device)																					//	attach a device onto the child chain
{																																								//	begin
	DeviceNode	*node = new DeviceNode (device);																	//	allocate the device node
	children.InsertAfter (node);																									//	insert the device onto the list
}																																								//	end

//------------------------------------------------------------------------------
