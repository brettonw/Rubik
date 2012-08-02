//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/06/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "world.h"
#include "world grid.h"
#include "invisible.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
PiecePtr	gWorld;																																//	global world pointer

//------------------------------------------------------------------------------
World::World (void) : Piece (0, new Invisible (0), 0, new WGrid)								//	constructor
{																																								//	begin
	armor = ZERO;																																	//	make the world indestructable
	hardness = ParmReal (WORLD_HARDNESS);																					//	set the world hardness value
}																																								//	end

//------------------------------------------------------------------------------
World::~World (void)																														//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Real	World::TraceRay (CPoint origin, CVector direction)												//	return the time at which a ray intersects this piece
{																																								//	begin
	WGrid	*grid = (WGrid *) geometry;																							//	get a pointer to the grid geometry
	return grid->TraceRay (origin, direction);																		//	trace the ray through the grid
}																																								//	end

//------------------------------------------------------------------------------
Bool	World::Contains (CPoint location)																					//	return whether or not the location is contained by the piece
{																																								//	begin
	Point_2d	loc (location * OVER_CELL_SIZE);																		//	compute the test point in grid space
	WGrid	*grid = (WGrid *) geometry;																							//	get a pointer to the grid geometry
	return grid->IsOccupied (int (loc[X]), int (loc[Y]), TRUE);										//	trace the ray through the grid
}																																								//	end

//------------------------------------------------------------------------------
