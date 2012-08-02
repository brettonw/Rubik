//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Biddle
//	Last Revision:	07/19/96
//
//------------------------------------------------------------------------------

#ifndef	BEAM_SEQUENCE
#define	BEAM_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence ptr.h"
#include "piece ptr.h"
#include "pixel.h"

//------------------------------------------------------------------------------
class BeamSeq : public Sequence																									//	a movable sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				struct	Dot																															//	internal dot structure
				{																																				//	begin
					Real			offset;																											//	offset to the sequence location
					SeqPtr		seq;																												//	the actual sequence to draw
				}					*dots;																												//	array of frames
				int				count;																												//	number of dots in the beam
				Point_2d	muzzle;																												//	the muzzle point
				PiecePtr	owner;																												//	the owning piece for extraction of orientation, etc
				Real			damage;																												//	how much damage the beam does
	public:																																				//	members externally available
				BeamSeq (Real dist, Pixel color, Real time, CPoint muz, PiecePtr owner);//	constructor
virtual ~BeamSeq (void);																												//	destructor
virtual Bool			Step (CPoint where);																					//	move it move it
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	BEAM_SEQUENCE