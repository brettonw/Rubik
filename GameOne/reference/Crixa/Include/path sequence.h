//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Modified by:		Bretton Wade
//	Last Revision:	07/19/96
//
//------------------------------------------------------------------------------

#ifndef	PATH_SEQUENCE
#define	PATH_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence ptr.h"

//------------------------------------------------------------------------------
class PathSeq : public Sequence																									//	a movable sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SeqPtr		sequence;																											//	the actual sequence to draw
				Point_2d	location;																											//	the world coordinate location of this object
				struct	Frame																														//	internal frame structure
				{																																				//	begin
					Point_2d	point;																											//	waypoint
					Vector_2d	delta;																											//	delta vector to next waypoint
					Real			time;																												//	time for travelling to next waypoint
				}					*frames;																											//	pointer to an array of frame structures
				int				frame;																												//	the current frame index
				int				count;																												//	the number of frames
				Real			time;																													//	time current frame has been in operation
				Bool			loop;																													//	whether or not to loop the sequence when it is complete
	public:																																				//	members externally available
				PathSeq (SeqPtr sequence, Bool loop, int count, ...);										//	constructor
virtual ~PathSeq (void);																												//	destructor
virtual Bool			Step (CPoint where);																					//	move it move it
virtual void			Reset (void);																									//	return the sequence to its starting state
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PATH_SEQUENCE
