//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/30/96
//
//------------------------------------------------------------------------------

#ifndef	MOVIE_CONTROLLER
#define	MOVIE_CONTROLLER

//------------------------------------------------------------------------------
#include "controller.h"
#include "direct x.h"

//------------------------------------------------------------------------------
#define	BLIZZARD_CONTROLLER_ID		3																							//	the id of the blizzard logo movie controller
#define	INTRO_CONTROLLER_ID				4																							//	the id of the intro movie controller

//------------------------------------------------------------------------------
class	MovieController : public Controller																				//	control function class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				char			moviename[256];																								//	the name of the movie to play
				LPDIRECTDRAWSURFACE	primary;																						//	the primary surface
				LPDIRECTDRAWPALETTE	palette;																						//	the palette
	public:																																				//	members available externally
				MovieController (HWND window, cstr movie, int id);											//	constructor
virtual	void			Begin (void);																									//	set up the controller
virtual	void			Loop (void);																									//	do one iteration of a control loop
virtual	void			End (void);																										//	clean up the controller
};																																							//	end

//------------------------------------------------------------------------------
extern	MovieController	*gBlizzardController;																		//	global pointer to the blizzard logo movie controller
extern	MovieController	*gIntroController;																			//	global pointer to the intro movie controller

//------------------------------------------------------------------------------

#endif	//MOVIE_CONTROLLER
