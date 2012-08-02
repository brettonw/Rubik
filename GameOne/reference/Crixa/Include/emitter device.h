//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//
//	02/20/97	JB		creation.
//	02/25/97	JB		added kNoSound constant and emitting boolean.
//
//------------------------------------------------------------------------------

#ifndef EMITTER_DEVICE
#define EMITTER_DEVICE

//------------------------------------------------------------------------------
#include "device.h"
#include "piece ptr.h"

//------------------------------------------------------------------------------
const int kNoSound = -1;

//------------------------------------------------------------------------------
class EmitterDevice : public Device																							//	a type of piece expressly used for emitting sound
{																																								//	begin
	private:																																			//	private members
	protected:																																		//	protected members
				int					startSound;																									//	start up sound for turning the piece on
				int					stopSound;																									//	stopping sound for turning the piece off
				int					emSound;																										//	looping, emitted sound
				long				emID;																												//	emitter identification
				PiecePtr		emPiece;																										//	piece used for spacial sound positioning
				Bool				emitting;
	public:																																				//	public members
				EmitterDevice (int id, Point_2d loc, int emSnd,													//	constructor
											 int startSnd = kNoSound, int stopSnd = kNoSound);				//	constructor (continued)
virtual	~EmitterDevice (void);																									//	destructor
virtual	Bool				SwitchOn (void);																						//	switch the emitter on
virtual	Bool				SwitchOff (void);																						//	switch the emitter off
				Bool				IsEmitting (void);																					//	return whether the device is currently emitting
};																																							//	end

//------------------------------------------------------------------------------
inline	Bool	EmitterDevice::IsEmitting (void)
{
	return emitting;
}

//------------------------------------------------------------------------------

#endif // EMITTER_DEVICE
