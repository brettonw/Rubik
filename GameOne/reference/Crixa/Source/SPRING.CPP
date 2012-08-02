//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/13/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "spring.h"
#include "referee.h"
#include "stationary sequence.h"
#include "fade pixel sequence.h"
#include "view.h"

//------------------------------------------------------------------------------
void	Spring (PiecePtr &a, PiecePtr &b, Real spring, Real damp, Real length)		//	apply spring forces to two objects
{																																								//	begin
	Body			*aBody = a->GetBody (),
						*bBody = b->GetBody ();
	Vector_2d	delta = aBody->TLocation () - bBody->TLocation (),
						relVel = aBody->TVelocity () - bBody->TVelocity ();
	Real			deviation = delta.Normalize () - length,
						velocity = relVel | delta,
						F = (spring * deviation) + (damp * velocity);
	aBody->ApplyForce (-F * delta);
	bBody->ApplyForce (F * delta);
}																																								//	end

//------------------------------------------------------------------------------
void	Spring (PiecePtr &a, CPoint loc, Real spring, Real damp, Real length)			//	apply spring forces to a single relative to a location
{																																								//	begin
	Body			*aBody = a->GetBody ();
	Vector_2d	delta = aBody->TLocation () - loc;
	Real			deviation = delta.Normalize () - length,
						velocity = aBody->TVelocity () | delta,
						F = (spring * deviation) + (damp * velocity);
	aBody->ApplyForce (-F * delta);
}																																								//	end

//------------------------------------------------------------------------------
void	DrawSpring (CPoint a, CPoint b, int dots, Pixel color)										//	draw a spring line between two objects
{																																								//	begin
	Vector_2d	delta = (a - b) / Real (dots);
	Point_2d	point = b + delta;
	for (int i = 1; i < dots; i++)
	{
		gView->Install (new StatSeq (point, new FadeSeq (color, R(0.1))), UNDER_LAYER);
		point = point + delta;
	}
}																																								//	end

//------------------------------------------------------------------------------
