//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/19/96
//
//	02/24/97	DS		Added AltDrawMode
//
//------------------------------------------------------------------------------

#ifndef	TIME_SEQUENCE
#define	TIME_SEQUENCE

//------------------------------------------------------------------------------
#include "sprite sequence.h"

//------------------------------------------------------------------------------
class TimeSeq : public SpriteSeq																								//	timed sequence of sprites
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			struct	Frame																														//	private structure for maintaining the sequence
  			{																																				//	begin
  				int			sprite;																												//	which sprite to draw
  				Real		time;																													//	how long to leave it up
  			}					*frames;																											//	array of frames
  			int				count;																												//	the number of frames
  			Bool			loop;																													//	whether or not to loop the sequence at the end
  			int				frame;																												//	the current frame
  			Real			time;																													//	time remaining for this frame
	public:																																				//	members available externally
  			TimeSeq (SArrayPtr sprites, Bool loop = FALSE, Real common = ZERO);			//	constructor
  			TimeSeq (SArrayPtr sprites, Bool loop, int count, ...);									//	constructor
virtual	~TimeSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
virtual	Bool			Step (CPoint where, int mode);																//	draw the next frame
virtual void			Reset (void);																									//	return the sequence to its starting state
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	TIME_SEQUENCE