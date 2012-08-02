//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/17/96
//	03/09/97	DS		Some additional error trapping for divide by zero
//	03/11/97	DS		Some temporary debugging stuff in Quadratic
//	03/12/97	DS		Some numerical spackle added
//------------------------------------------------------------------------------

#define SECONDINFINITY 1.0E30																										//	silly name for temporary numerical patch
#define SECONDEPSILON 1.0E-12																										//	silly name for temporary numerical patch

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "quadratic.h"
#include "math utility.h"

//------------------------------------------------------------------------------
Bool	Dist (CVector p, CVector v, Real d, Real &root1, Real &root2)							//	compute the two times at which a moving point is some distance from the origin
{																																								//	begin
	Real			a = v | v,																													//	compute the 'a' coefficient of the quadratic
						b = R(2.0) * (p | v),																								//	compute the 'b' coefficient of the quadratic
						c = (p | p) - (d * d);																							//	compute the 'c' coefficient of the quadratic
	return Quadratic (a, b, c, root1, root2);																			//	return teh solution to the quadratic equation
}																																								//	end

//------------------------------------------------------------------------------
Bool	Quadratic (Real a, Real b, Real c, Real &root1, Real &root2)							//	solve the quadratic equation
{																																								//	begin
	Real	radical = (b * b) - (R(4.0) * a * c);																		//	compute the radical for the quadratic formula
	if (radical > ZERO)																														//	if the roots are going to be real numbers, the point does come within the specified distance
	{																																							//	begin
	#if 1																																					//	better way to compute roots on computers
		Real	root = SQRT (radical),																								//	compute the square root of the radical
					q = R(-0.5) * (b + (Sign (b) * root));																//	compute the alternative coefficient

//	root1 = c / q;																															//	original: compute the first root
		if (fabs(q)<SECONDEPSILON) root1 = SECONDINFINITY * Sign(q);								//	ds: temp error wrapper
		else root1 = c / q;			

//	root2 = q / a;																															//	original: compute the second root
		if (fabs(a)<SECONDEPSILON) root2 = SECONDINFINITY * Sign(q);								//	ds: temp error wrapper
		else root2 = q / a;																													//	compute the second root

		if (root1 > root2)																													//	if the second root is smaller than the first
		{																																						//	begin
			Real	tmp = root1;																												//	save the first root
			root1 = root2;																														//	move the second root into the first slot
			root2 = tmp;																															//	move the first root into the second slot
		}																																						//	end
	#else																																					//	normal quadratic equation
		Real	root = SQRT (radical),																								//	compute the square root of the radical
					over2a = R(1.0) / (R(2.0) * a);																				//	compute two times a
		root1 = (-b - root) * over2a;																								//	compute the first intersection time
		root2 = (-b + root) * over2a;																								//	compute the second intersection time
	#endif																																				//	end if (determining method)
		return TRUE;																																//	return true to indicate that the appraoch occurred
	}																																							//	end
	return FALSE;																																	//	return false to indicate no appraoch was detected
}																																								//	end

//------------------------------------------------------------------------------
