//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/30/97
//	02/27/97	DS		Added "Level Status" call for PlayController() to check
//									when level finishes. Added Level Status() call.
//	03/09/97	DS		repurposed status for ship status messages
//------------------------------------------------------------------------------

#ifndef	LEVEL
#define	LEVEL

//------------------------------------------------------------------------------
#include "device.h"
#include "sequence ptr.h"

//------------------------------------------------------------------------------
#define LV_INIT							0x00		// just starting
#define LV_PLAYING					0x01		// playing, but nothing has happened yet
#define	LV_MISSION_COMPLETE 0x02		// primary mission objective complete
#define LV_NO_MORE_LIVES		0x04		// crixa ship is out of lives
#define	LV_EXITED						0x08		// player successfully escaped the level
#define	LV_QUIT							0x10		// player press QUIT GAME button

//------------------------------------------------------------------------------
class	Level : public Device																											//	level load and unload class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				levelstatus;																									//	level status codes (defined in level.h)
				Bool			playing;																											//	whether the level is playing or done
				SeqPtr		mission;																											//	mission message board
				SeqPtr		description;																									//	room description message board
				SeqPtr		playerstatus;
	public:																																				//	members available externally
				Level (int level);																											//	constructor
virtual	~Level (void);																													//	destructor
virtual	int				Trigger (const PiecePtr &piece, int code, int code2);					//	trigger the device
				Bool			Play (void);																									//	play a frame
				void			Recall (void);																								//	recall the last message that was shown
				void			Brief (void);																									//	brief the player on the mission
				void			Debrief (void);																								//	tell the player how they did
				int				Status (void);																								//	return the status of the playing level
};																																							//	end

//------------------------------------------------------------------------------

#endif	//LEVEL
