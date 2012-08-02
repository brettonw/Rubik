//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	07/22/96
//
//------------------------------------------------------------------------------

#ifndef	DEVICE_NODE
#define DEVICE_NODE

//------------------------------------------------------------------------------
#include "list.h"
#include "device.h"

//------------------------------------------------------------------------------
class DeviceNode : public ListNode																							//	a node in a list of devices
{																																								//	begin
	private:																																			//	private members of the class
	protected:																																		//	protected members of the class
	public:																																				//	public members of the class
				Device *device;																													//	a device associated with this node
				DeviceNode (Device *dev);																								//	constructor
virtual	~DeviceNode (void);																											//	destructor
};																																							//	end

//------------------------------------------------------------------------------
#endif //	DEVICE_NODE