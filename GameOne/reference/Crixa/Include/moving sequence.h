//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Modified by:		Bretton Wade
//	Last Revision:	07/19/96
//
//------------------------------------------------------------------------------

#ifndef	MOVE_SEQUENCE
#define	MOVE_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence ptr.h"

//------------------------------------------------------------------------------
class MoveSeq : public Sequence																									//	a movable sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SeqPtr		sequence;																											//	the actual sequence to draw
				Point_2d	location;																											//	the world coordinate location of this object
				struct	Frame																														//	internal frame structure
				{																																				//	begin
					Vector_2d	velocity;																										//	velocity to be added at waypoint
					Vector_2d	delta;																											//	change in velocity over time period
					Real			time;																												//	time for travelling to next waypoint
				}					*frames;																											//	array of frames
				int				frame;																												//	the current frame index
				int				count;																												//	the number of frames
				Real			time;																													//	time current frame has been in operation
				Bool			loop;																													//	whether or not to loop the sequence when it is complete
	public:																																				//	members externally available
				MoveSeq (SeqPtr sequence, CPoint loc, Bool loop, int count, ...);				//	constructor
virtual ~MoveSeq (void);																												//	destructor
virtual Bool			Step (CPoint where);																					//	move it move it
virtual void			Reset (void);																									//	return the sequence to its starting state
				CPoint		Location (void) const;																				//	return the location of the moving sequence
				void			SetVelocity (CVector vel);																		//	set the velocity vector for the current frame
};																																							//	end

//------------------------------------------------------------------------------
inline	CPoint	MoveSeq::Location (void) const																	//	return the location of the moving sequence
{																																								//	begin
	return location;																															//	return the location
}																																								//	end

//------------------------------------------------------------------------------
inline	void	MoveSeq::SetVelocity (CVector vel)																//	set the velocity vector for the current frame
{																																								//	begin
	frames[frame].velocity = vel;																									//	copy the new vector
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	MOVE_SEQUENCE