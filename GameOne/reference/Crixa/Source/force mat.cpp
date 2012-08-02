//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97:	creation.	(JB)
//									02/06/97:	comments.	(JB)
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "force mat.h"
#include "circle.h"
#include "referee.h"

//------------------------------------------------------------------------------
ForceMat::ForceMat (Real force, Real angle, int trigger) : Device (trigger)			//	constructor
{																																								//	begin
	dirVec = Vector_2d (angle);																										//	direction vector for the force
	dirVec *= force;																															//	scale the vector accordingly
}																																								//	end

//------------------------------------------------------------------------------
ForceMat::~ForceMat (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
int	ForceMat::Trigger (const PiecePtr &piece, int code, int code2)
{																																								//	begin
	if (code == DEV_TRIGGER && power)																							//	if the device has power and the trigger is a regular trigger
	{																																							//	begin
		Geometry *geometry = piece->GetGeometry ();																	//	look at the piece's geometry
		if (geometry->Type () == CIRCLE_GEOM)																				//	if the geometry is circular
		{																																						//	begin
			Body	*body = piece->GetBody ();																					//	look at the piece's body
			Real	radius = ((Circle *) geometry)->Radius ();													//	determine it's radius
			body->ApplyForce (dirVec * (radius * radius));														//	apply the force to the body
			gReferee->Update (piece);																									//	we've modified the piece, so update it
		}																																						//	end
	}																																							//	end
	return Device::Trigger (piece, code, code2);																	//	trigger the device
}																																								//	end

//------------------------------------------------------------------------------
