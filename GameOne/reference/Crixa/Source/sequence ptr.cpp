//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
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
#include "sequence ptr.h"

//------------------------------------------------------------------------------
SeqPtr::SeqPtr (void)																														//	constructor
{																																								//	begin
	ptr = 0;																																			//	start with an empty representation
}																																								//	end

//------------------------------------------------------------------------------
SeqPtr::SeqPtr (Sequence *sequence)																							//	constructor
{																																								//	begin
	ptr = sequence;																																//	copy the sequence epointer
	if (ptr)																																			//	if the representation is valid
		ptr->refcount++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
SeqPtr::SeqPtr (const SeqPtr &sptr)																							//	constructor
{																																								//	begin
	ptr = sptr.ptr;																																//	copy the representation
	if (ptr)																																			//	if the representation is valid
		ptr->refcount++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
SeqPtr::~SeqPtr (void)																													//	destructor
{																																								//	begin
	if (ptr)																																			//	if the representation is valid
		if (--(ptr->refcount) == 0)																									//	if the decremented reference is zero
			delete ptr;																																//	delete the representation
}																																								//	end

//------------------------------------------------------------------------------
SeqPtr	&SeqPtr::operator = (const SeqPtr &sptr)																//	assignment operator
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
