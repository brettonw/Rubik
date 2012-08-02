//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/25/96
//
//------------------------------------------------------------------------------

#ifndef	SEMAPHORE
#define	SEMAPHORE

//------------------------------------------------------------------------------
class Semaphore																																	//	an access control device for code
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Bool			grabbed;																											//	semaphore value, 0 if the semaphore is available
	public:																																				//	members available externally
  			Semaphore (void);																												//	constructor
				Bool			Grab (void);																									//	grab the semaphore on the planner
				void			Release (void);																								//	release the semaphore on the planner
};																																							//	end

//------------------------------------------------------------------------------
inline	Semaphore::Semaphore (void)																							//	constructor
{																																								//	begin
	grabbed = FALSE;																															//	the semaphore is available to start with
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SEMAPHORE
