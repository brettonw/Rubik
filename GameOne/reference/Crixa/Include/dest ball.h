//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97:	creation.  (JB)
//									02/06/97:	some finishing touches. (JB)
//
//------------------------------------------------------------------------------

#ifndef	DEST_BALL
#define DEST_BALL

/*
DESCRIPTION:
a destructable ball is just what it sounds like, a round object that can be
moved around the world, that can bump into things, and can be destroyed.
this class is merely an abstract base class which can be used to cheaply
create new types of balls, such as the resource ball.
*/

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class DestBall : public Piece																										//	a destructable, movable ball
{																																								//	begin
	private:																																			//	private members
	protected:																																		//	protected members
				SArrayPtr	sptr;																													//	pointer to the piece's graphic
	public:																																				//	public members
				DestBall (Player *own, Real armor, CPoint loc, Real rad, SArrayPtr spr);
virtual	~DestBall (void);
virtual	Bool			Step (Real time);																							//	step the ball
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	
virtual	void			Explode (void) = 0;																						//	
};

//------------------------------------------------------------------------------

#endif // DEST_BALL