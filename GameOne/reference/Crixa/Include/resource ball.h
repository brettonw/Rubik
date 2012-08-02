//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97:	creation.  (JB)
//									02/06/97:	commenting & descriptions. (JB)
//
//------------------------------------------------------------------------------

#ifndef	RESOURCE_BALL
#define RESOURCE_BALL

/*
DESCRIPTION:
a resource ball is a destructable ball that releases a resource into the world
upon termination.

PROBLEMS:
the explode routine is vis inadequate at the moment.  FIX IT!  FIX IT!
*/

//------------------------------------------------------------------------------
#include "dest ball.h"

//------------------------------------------------------------------------------
class ResBall : public DestBall																									//	a destructable ball that holds a resource
{																																								//	begin
	private:																																			//	private members
	protected:																																		//	protected members
				int				resType;																											//	the type of resource
				int				resAmount;																										//	the amount of the resource contained
	public:																																				//	public members
				ResBall (Player *own, Real armor, CPoint loc, Real rad, SArrayPtr spr,	//	constructor
								 int type, int num);																						//	constructor (continued)
virtual	~ResBall (void);																												//	destructor
virtual	void			Damage (const PiecePtr &piece, Real damage);									//	damage the ball
virtual	void			Explode (void);																								//	explode the ball
};																																							//	end

//------------------------------------------------------------------------------

#endif // RESOURCE BALL