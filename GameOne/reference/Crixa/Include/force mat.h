//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97:	creation.	(JB)
//									02/06/97:	comments.	(JB)
//
//------------------------------------------------------------------------------

#ifndef FORCE_MAT
#define FORCE_MAT

/*
DESCRIPTION:
a force mat is a device that, when triggered, applies a force to the body
of the object that triggered it.  it currently supports only circular objects,
because it bases the force applied on the radius of the ship.  a better
implementation of the application of the force would be desired, since it
doesn't take into account the actual mass of the object.  this is just a
quick hack, though, so it'll do for now.

there is no graphic associated with the force mat, so the level designer
must present some sort of graphic feedback for the player.

PROBLEMS:
the force mat encourages the same kinds of problems that pinching doors, wedged
ships, etc. cause.  if a force mat applies force to an object so that it is
thrust against a wall, and the force is constantly applied, multiple collisions
occur, bringing the game to a halt.  this is not so much to do with the force
mat, as it is to do with the collision detection, but the problem remains.

the force mat doesn't work on rectangular (polygonal) objects.  since there
aren't any such objects in the world of crixa, this currently isn't a problem,
but may need to be addressed at some later date.
*/

//------------------------------------------------------------------------------
#include "device.h"

//------------------------------------------------------------------------------
class ForceMat : public Device																									//	a force mat
{																																								//	begin
	private:																																			//	private members
	protected:																																		//	protected members
				Vector_2d			dirVec;																										//	direction vector of the mat's force application
	public:																																				//	public members
				ForceMat (Real force, Real angle, int trigger);													//	constructor
virtual	~ForceMat (void);																												//	destructor
virtual	Trigger (const PiecePtr &piece, int code, int code2);										//	trigger
};																																							//	end

//------------------------------------------------------------------------------

#endif // FORCE_MAT