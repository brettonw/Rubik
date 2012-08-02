//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	03/25/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
SArrayPtr::SArrayPtr (void)																											//	constructor
{																																								//	begin
	ptr = 0;																																			//	start with an empty representation
}																																								//	end

//------------------------------------------------------------------------------
SArrayPtr::SArrayPtr (cstr name)																								//	constructor
{																																								//	begin
	ptr = new SArray (name);																											//	create a new sequence
	ptr->refcount++;																															//	increment its reference count
}																																								//	end

//------------------------------------------------------------------------------
SArrayPtr::SArrayPtr (const SArrayPtr &sptr)																		//	constructor
{																																								//	begin
	ptr = sptr.ptr;																																//	copy the representation
	if (ptr)																																			//	if the representation is valid
		ptr->refcount++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
SArrayPtr::~SArrayPtr (void)																										//	destructor
{																																								//	begin
	if (ptr)																																			//	if the representation is valid
		if (--(ptr->refcount) == 0)																									//	if the decremented reference is zero
			delete ptr;																																//	delete the representation
}																																								//	end

//------------------------------------------------------------------------------
SArrayPtr	&SArrayPtr::operator = (const SArrayPtr &sptr)												//	assignment operator
{																																								//	begin
	if (sptr.ptr)																																	//	if the new representation is valid
		sptr.ptr->refcount++;																												//	increment its reference count
	if (ptr)																																			//	if the old representation is valid
		if (--(ptr->refcount) == 0)																									//	if the decremented reference is zero
			delete ptr;																																//	delete the representation
	ptr = sptr.ptr;																																//	copy the new representation
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
