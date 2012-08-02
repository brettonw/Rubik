//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/14/97
//
//------------------------------------------------------------------------------

#ifndef	PLAY_CONTROLLER
#define	PLAY_CONTROLLER

//------------------------------------------------------------------------------
#include "controller.h"
#include "level.h"
#include "timer.h"

//------------------------------------------------------------------------------
#define	PLAY_CONTROLLER_ID			1																								//	the id of the play controller
#define	FRAME_SMOOTHING_WINDOW	32																							//	how many frame sto smooth the frame time over

//------------------------------------------------------------------------------
class	PlayController : public Controller																				//	control function class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				levelID;																											//	the level to load next
				Level			*level;																												//	the level to play
	public:																																				//	members available externally
				PlayController (HWND window);																						//	constructor
virtual	void			Begin (void);																									//	set up the controller
virtual	void			Loop (void);																									//	do one iteration of a control loop
virtual	void			End (void);																										//	clean up the controller
				void			PreLoad (void);																								//	pre-load sprites from the common directory
};																																							//	end

//------------------------------------------------------------------------------
extern	PlayController	*gPlayController;																				//	global pointer to the play controller

//------------------------------------------------------------------------------

#endif	//PLAY_CONTROLLER
