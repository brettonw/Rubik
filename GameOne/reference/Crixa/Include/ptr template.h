//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/28/96
//
//------------------------------------------------------------------------------

#ifndef	PTR_TEMPLATE
#define	PTR_TEMPLATE

//------------------------------------------------------------------------------
template<class T>																																//	template
class RCPtr																																			//	pointer template class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				T					*ptr;																													//	pointer to the reference counted object
	public:																																				//	members available externally
  			RCPtr (void);																														//	constructor
  			RCPtr (T *t);																														//	constructor
  			RCPtr (const RCPtr<T> &ptrt);																						//	constructor
  			~RCPtr (void);																													//	destructor
				operator void * (void) const;																						//	typecasting operator
				T					*operator -> (void) const;																		//	pointer dereference operator
				RCPtr<T>		&operator = (T *t);																					//	assignment operator
				RCPtr<T>		&operator = (const RCPtr<T> &ptrt);													//	assignment operator
};																																							//	end

//------------------------------------------------------------------------------
template<class T>																																//	template
inline	RCPtr<T>::RCPtr<T> (void)																								//	constructor
{																																								//	begin
	ptr = 0;																																			//	empty pointer
}																																								//	end

//------------------------------------------------------------------------------
template<class T>																																//	template
inline	RCPtr<T>::RCPtr<T> (T *t)																								//	constructor
{																																								//	begin
	if (t)																																				//	if the new pointer is valid
		t->AddRef ();																																//	add a reference to the object
	ptr = t;																																			//	save the pointer
}																																								//	end

//------------------------------------------------------------------------------
template<class T>																																//	template
inline	RCPtr<T>::RCPtr<T> (const RCPtr<T> &ptrt)																//	constructor
{																																								//	begin
	if (ptrt.ptr)																																	//	if the other pointer is valid
		ptrt.ptr->AddRef ();																												//	add a reference to it
	ptr = ptrt.ptr;																																//	copy the other pointer
}																																								//	end

//------------------------------------------------------------------------------
template<class T>																																//	template
inline	RCPtr<T>::~RCPtr<T> (void)																							//	destructor
{																																								//	begin
	if (ptr)																																			//	if the pointer is valid
		ptr->RemoveRef ();																													//	remove the reference from the object
}																																								//	end

//------------------------------------------------------------------------------
template<class T>																																//	template
inline	RCPtr<T>::operator void * (void) const																	//	typecasting operator
{																																								//	begin
	return ptr;																																		//	return the source pointer
}																																								//	end

//------------------------------------------------------------------------------
template<class T>																																//	template
inline	T			*RCPtr<T>::operator -> (void) const																//	pointer dereference operator
{																																								//	begin
	return ptr;																																		//	return the source pointer
}																																								//	end

//------------------------------------------------------------------------------
template<class T>																																//	template
RCPtr<T>	&RCPtr<T>::operator = (T *t)																					//	assignment operator
{																																								//	begin
	if (t)																																				//	if the other pointer is valid
		t->AddRef ();																																//	add a reference to it
	if (ptr)																																			//	if this pointer is valid
		ptr->RemoveRef ();																													//	remove a reference from it
	ptr = t;																																			//	copy the other pointer
	return *this;																																	//	return the reference to this pointer
}																																								//	end

//------------------------------------------------------------------------------
template<class T>																																//	template
RCPtr<T>	&RCPtr<T>::operator = (const RCPtr<T> &ptrt)													//	assignment operator
{																																								//	begin
	if (ptrt.ptr)																																	//	if the other pointer is valid
		ptrt.ptr->AddRef ();																												//	add a reference to it
	if (ptr)																																			//	if this pointer is valid
		ptr->RemoveRef ();																													//	remove a reference from it
	ptr = ptrt.ptr;																																//	copy the other pointer
	return *this;																																	//	return the reference to this pointer
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	PTR_TEMPLATE
