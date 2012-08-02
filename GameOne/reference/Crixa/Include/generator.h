//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	07/30/96
//
//------------------------------------------------------------------------------

#ifndef	POWER_GENERATOR
#define POWER_GENERATOR

//------------------------------------------------------------------------------
#include "power device.h"
#include "sequence ptr.h"
#include "point.h"

//------------------------------------------------------------------------------
class Generator : public PowerDevice																						//	a generator that delivers power
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				output;																												//	the generator's power output level
	public:																																				//	members available externally
				Generator (SeqPtr swseq, int out = 1);																	//	constructor
virtual	~Generator (void);																											//	destructor
virtual	int				Trigger (const PiecePtr &piece, int code = 0, int code2 = 0);	//	trigger the device
};																																							//	end

//------------------------------------------------------------------------------

#endif // POWER_GENERATOR