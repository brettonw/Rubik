//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/28/96
//
//------------------------------------------------------------------------------

#ifndef	REFERENCE_COUNT
#define	REFERENCE_COUNT

//------------------------------------------------------------------------------
class RefCount																																	//	reference counted object superclass
{																																								//	begin
	private:																																			//	members internal to this class only
  			int				refcount;																											//	reference count for the sequence
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				RefCount (void);																												//	constructor
virtual	~RefCount (void);																												//	destructor
				void			AddRef (void);																								//	add a reference to the object
				void			RemoveRef (void);																							//	remove a reference to the object
};																																							//	end

//------------------------------------------------------------------------------
inline	RefCount::RefCount (void)																								//	constructor
{																																								//	begin
	refcount = 0;																																	//	initialize the reference count to zero
}																																								//	end

//------------------------------------------------------------------------------
inline	void	RefCount::AddRef (void)																						//	add a reference to the object
{																																								//	begin
	refcount++;																																		//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
inline	void	RefCount::RemoveRef (void)																				//	remove a reference to the object
{																																								//	begin
	refcount--;																																		//	decrement the reference count
	if (refcount == 0)																														//	if the reference count is zero
		delete this;																																//	delete the object
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	REFERENCE_COUNT
