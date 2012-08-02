//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/21/96
//
//------------------------------------------------------------------------------

#ifndef	LOCK_PTR
#define	LOCK_PTR

//------------------------------------------------------------------------------
#include "lock.h"

//------------------------------------------------------------------------------
class LockPtr																																		//	reference counted pointer to a lock
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			Lock			*ptr;																													//	pointer to the sequence
	public:																																				//	members available externally
  			LockPtr (void);																													//	constructor
  			LockPtr (Lock *lock);																										//	constructor
  			LockPtr (const LockPtr &ptr);																						//	constructor
  			~LockPtr (void);																												//	destructor
				Lock			*operator -> (void) const;																		//	pointer dereference operator
				Lock			*operator () (void) const;																		//	function call operator
				LockPtr		&operator = (const LockPtr &ptr);															//	assignment operator
};																																							//	end

//------------------------------------------------------------------------------
inline	Lock	*LockPtr::operator -> (void) const																//	pointer dereference operator
{																																								//	begin
	return ptr;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Lock	*LockPtr::operator () (void) const																//	function call operator
{																																								//	begin
	return ptr;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	LOCK_PTR