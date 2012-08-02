//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/25/96
//
//------------------------------------------------------------------------------

#ifndef	CONSTRAINED_BODY
#define	CONSTRAINED_BODY

//------------------------------------------------------------------------------
#include "body.h"

//------------------------------------------------------------------------------
class ConBody : public Body																											//	a constrained body for the physics model
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Vector_2d	tAxis;																												//	translational constraint axis
	public:																																				//	members available externally
  			ConBody (CPoint loc, Real orient, Real tInertia, Real rInertia);				//	constructor
virtual	~ConBody (void);																												//	destructor
virtual	void			ApplyForce (CVector force);																		//	apply a force to the body at the center of mass
virtual	void			ApplyForce (CVector force, CPoint where);											//	apply a force to the body at some point distant from the center of mass
				void			SetConstraint (CVector axis);																	//	set the constraint axis
};																																							//	end

//------------------------------------------------------------------------------
inline	void	ConBody::SetConstraint (CVector axis)															//	set the constraint axis
{																																								//	begin
	tAxis = axis;																																	//	copy the axis
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	CONSTRAINED_BODY
