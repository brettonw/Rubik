//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Biddle
//	Last Revision:	02/04/97
//
//------------------------------------------------------------------------------

#ifndef	LIGHTNING_SEQUENCE
#define	LIGHTNING_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence ptr.h"
#include "pixel.h"
#include "piece ptr.h"
#include "list.h"

//------------------------------------------------------------------------------
class LightSeq : public Sequence																								//	a lightning sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				List			nodes;
				int				nodeCount;
				Point_2d	origin;
				Vector_2d	direction;
				int				length;
				int				step;
				void			DrawLine (int c, Point_2d o, CVector d, const Pixel &color);	//	draw a line from a point in a general direction
				void			Branch (CPoint where, CVector direction);
	public:																																				//	members externally available
				LightSeq (CPoint origin, CVector direction, int length);								//	constructor
virtual ~LightSeq (void);																												//	destructor
virtual Bool			Step (CPoint where);																					//	move it move it
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	LIGHTNING_SEQUENCE
