//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	08/13/96	JB		creation & last undocumented revision.
//	02/12/97	MK		sound emitter id
//	02/20/97	JB		modified to use device emitter object.
//
//------------------------------------------------------------------------------

#ifndef LASER_BARRIER_GEN
#define LASER_BARRIER_GEN

//------------------------------------------------------------------------------
#include "device.h"
#include "piece ptr.h"
#include "laser barrier.h"
#include "sequence ptr.h"
#include "emitter device.h"

//------------------------------------------------------------------------------
class LaserBarrierGen : public Device
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SeqPtr					*sequences;																							//	array of sequences for the laser barrier
				int 						length;																									//	the number of visuals composing the laser barrier
				PiecePtr				laserbarrier;																						//	the piece to use for the collisions
				EmitterDevice		soundEmitter;																						//	the source of the laser barrier's sound
	public:																																				//	members available externally
				LaserBarrierGen (int type, int id, Real x, Real y, Real w, Real h, Real dam);//	constructor
virtual	~LaserBarrierGen (void);																								//	destructor
virtual Bool						SwitchOn (void);																				//	switch the laser barrier on
virtual Bool						SwitchOff (void);																				//	switch the laser barrier off
};																																							//	end

//------------------------------------------------------------------------------

#endif // LASER_BARRIER_GEN