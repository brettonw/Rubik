//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/17/96
//
//------------------------------------------------------------------------------

#ifndef	QUADRATIC
#define	QUADRATIC

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
Bool	Dist (CVector p, CVector v, Real d, Real &root1, Real &root2);						//	compute the two times at which a moving point is some distance from the origin
Bool	Quadratic (Real a, Real b, Real c, Real &root1, Real &root2);							//	solve the quadratic equation

//------------------------------------------------------------------------------

#endif	//	QUADRATIC
