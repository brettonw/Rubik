//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/30/96
//
//------------------------------------------------------------------------------

#ifndef	SPLASH_CONTROLLER
#define	SPLASH_CONTROLLER

//------------------------------------------------------------------------------
#include "controller.h"
#include "timer.h"

//------------------------------------------------------------------------------
#define	SPLASH_CONTROLLER_ID		2																								//	the id of the splash controller

//------------------------------------------------------------------------------
class	SplashController : public Controller																			//	control function class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Timer			timer;																												//	timer to drop into the game after a certain amount of time
	public:																																				//	members available externally
				SplashController (HWND window);																					//	constructor
virtual	void			Begin (void);																									//	set up the controller
virtual	void			Loop (void);																									//	do one iteration of a control loop
virtual	void			End (void);																										//	clean up the controller
};																																							//	end

//------------------------------------------------------------------------------
extern	SplashController	*gSplashController;																		//	global pointer to the splash controller

//------------------------------------------------------------------------------

#endif	//SPLASH_CONTROLLER
