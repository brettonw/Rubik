//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

#ifndef	PORTAL
#define	PORTAL

//------------------------------------------------------------------------------
#include "collider.h"
#include "line.h"

//------------------------------------------------------------------------------
class Portal : public Piece																											//	a portal piece, linking two colliders
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Collider	*link[2];																											//	the pair of pointers to colliders
	public:																																				//	members available externally
				Portal (Collider *a, Collider *b, Line *line);													//	constructor
virtual	~Portal (void);																													//	destructor
virtual	int				PreCollide (Piece *piece, Sect *sect);												//	do some work before a collision with another piece
virtual	int				PostCollide (Piece *piece, Sect *sect);												//	do some work after a collision with another piece
virtual	void			Damage (Real damage);																					//	damage the piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PORTAL
