//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/21/96
//
//------------------------------------------------------------------------------

#ifndef	TRIGGER_ARRAY
#define	TRIGGER_ARRAY

//------------------------------------------------------------------------------
#include "list.h"
#include "device.h"

//------------------------------------------------------------------------------
class TArray																																		//	array of device lists for triggering
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				List			*devices;																											//	pointer to an array of devices
	public:																																				//	members available externally
				TArray (int count);																											//	constructor
virtual	~TArray (void);																													//	destructor
virtual	int				Trigger (int trigger, const PiecePtr &piece, int v1, int v2);	//	trigger the device
				void			SetTrigger (int trigger, Device *device);											//	set a trigger device
};																																							//	end

//------------------------------------------------------------------------------
extern	TArray	*gTrigger;																											//	global trigger array

//------------------------------------------------------------------------------

#endif	//	TRIGGER_ARRAY
