//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	and:						Bretton Wade
//	Last Revision:	08/16/96
//
//------------------------------------------------------------------------------

#ifndef	POWER_DEVICE
#define POWER_DEVICE

//------------------------------------------------------------------------------
#include "device.h"
#include "device node.h"
#include "sequence ptr.h"

//------------------------------------------------------------------------------
class PowerDevice : public Device																								//	a device that carries a current
{																																								//	begin
	private:																																			//	private members of the class
	protected:																																		//	protected members of the class
		SeqPtr			sequence;																												//	a pointer to a switched sequence
	public:																																				//	public members of the class
				PowerDevice (SeqPtr sequence, int id);																	//	constructor
virtual ~PowerDevice (void);																										//	destructor
virtual	Bool			SwitchOn (void);																							//	turn the device on
virtual	Bool			SwitchOff (void);																							//	turn the device off
};																																							//	end

//------------------------------------------------------------------------------

#endif //	POWER_DEVICE