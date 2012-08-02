//----------------------------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//----------------------------------------------------------------------------------------------------

#ifndef	TIMER
#define	TIMER

//----------------------------------------------------------------------------------------------------
#ifndef	_MSC_VER
#include <types.h>
#endif


//----------------------------------------------------------------------------------------------------
class	Timer																																														//	timer class
{																																																			//	begin
	private:																																														//	members internal to this class only
	protected:																																													//	members internal to this class hierarchy
#ifdef	_MSC_VER
				__int64				start;																																					//	the recorded start time
				double				frequency;																																			//	the timer frequency for this system
#else
				UnsignedWide	start;																																					//	the recorded start time
#endif
	public:																																															//	members available externally
				Timer (void);																																									//	constructor
				void			Start (void);																																				//	start the timer
				double		Elapsed (void);																																			//	return the elapsed time
				double		Interval (void);																																		//	return the elapsed time, and reset the timer
				double		Total (void);																																				//	return the total time elapsed
};																																																		//	end

//----------------------------------------------------------------------------------------------------

#endif	//TIMER
