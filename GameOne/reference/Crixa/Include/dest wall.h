//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/29/96
//  Modified by:    Mark E. Kern  02/17/97    Added DropResource call
//
//------------------------------------------------------------------------------

#ifndef	DEST_WALL
#define DEST_WALL

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sequence ptr.h"
#include "sprite sequence.h"

//------------------------------------------------------------------------------
class DestWall : public Piece																										//	a physical bumper that toggles an object
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Point_2d	location;
				int				layer;
				SpriteSeq	*sequence;																										//	pointer to the sprite sequence
	public:																																				//	members available externally
				DestWall (Player *own, Real armor, CPoint loc, Real width, Real height,	//	constructor
									SeqPtr seq, int layer);																				//	constructor (continued)
				~DestWall (void);																												//	destructor
virtual Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
virtual void			Damage (const PiecePtr &piece, Real damage);
void							DropResource (int type, int number);														// (MK) Resource dropping
};																																							//	end

//------------------------------------------------------------------------------

#endif // DEST_WALL