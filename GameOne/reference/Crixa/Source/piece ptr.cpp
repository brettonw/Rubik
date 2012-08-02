//------------------------------------------------------------------------------
//	Copyright �1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/20/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "piece ptr.h"

//------------------------------------------------------------------------------
PiecePtr::PiecePtr (void)																												//	constructor
{																																								//	begin
	ptr = 0;																																			//	start with an empty representation
}																																								//	end

//------------------------------------------------------------------------------
PiecePtr::PiecePtr (Piece *piece)																								//	constructor
{																																								//	begin
	ptr = piece;																																	//	copy the pointer
	if (ptr)																																			//	if the representation is valid
		ptr->refcount++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
PiecePtr::PiecePtr (const PiecePtr &pptr)																				//	constructor
{																																								//	begin
	ptr = pptr.ptr;																																//	copy the representation
	if (ptr)																																			//	if the representation is valid
		ptr->refcount++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
PiecePtr::~PiecePtr (void)																											//	destructor
{																																								//	begin
	if (ptr)																																			//	if the representation is valid
		if (--(ptr->refcount) == 0)																									//	if the decremented reference is zero
			delete ptr;																																//	delete the representation
}																																								//	end

//------------------------------------------------------------------------------
PiecePtr	&PiecePtr::operator = (const PiecePtr &pptr)													//	assignment operator
{																																								//	begin
	if (pptr.ptr)																																	//	if the new representation is valid
		pptr.ptr->refcount++;																												//	increment its reference count
	if (ptr)																																			//	if the old representation is valid
		if (--(ptr->refcount) == 0)																									//	if the decremented reference is zero
			delete ptr;																																//	delete the representation
	ptr = pptr.ptr;																																//	copy the new representation
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
