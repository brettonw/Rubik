//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//  02/17/97	MK		Added emitter ids
//	03/09/97	DS		Added private random 
//	03/18/97	DS		patched in timer-based double tapping
//	03/20/97	DS		added virtual Reset() function from Human
//------------------------------------------------------------------------------

#ifndef	LOCAL_HUMAN
#define	LOCAL_HUMAN

//------------------------------------------------------------------------------
#include "human.h"
#include "keyboard.h"
#include "pointer device.h"
#include "joystick device.h"
#include "sprite array ptr.h"
#include "random.h"

//------------------------------------------------------------------------------
class Local : public Human																											//	a human player in the game
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real					tlife, lastlife;																					//	ds:target and last life 
				Real					textralife, lastextralife;																//	ds:target and last extralife
				int						lifedamage, extralifedamage;															//	ds:for handling lasting damage warning
				int						dstart, dend;																							//	ds:used for remembering damage warning
				int						framecounter;																							//	ds:counter for determining blinking intervals

				long					redEMid, yellowEMid;																			//  mk:emitter sound ids and flags for critical damage sounds
				
				int						flipflop;																									//	a flipflop counter to make each frame identifiably unique
				JoystickDevice	*joystick0,
												*joystick1;
				PointerDevice	*mouse,
											*joy1,
											*joy2,
											*keypad,
											*arrows,
											*rel;
				Keyboard			*keyboard;																								//	keyboard used for input
				int						stamp;																										//	time stamp from the tractor drive activation
				int						tapCount[2];
				int						tapCurrent;
				int						tappedCount;
				int						tapDuration;																							//	how long the double tap effect lasts
				Real					doubletapwhen;																						//	ds:used to enforce double-tap time

				Bool					wasDown;
				Real					multiplier;																								//	multiplier for tractor drive effects
				Real					range;																										//	the max value of the multiplier in the double tap process
				Bool					weaponKeyOp;
				int						lastWeapon;																								//	whatever the last selected weapon was
				Real					lastLock;
				Real					lockTime;
				Vector_2d			cameraOffset;																							//	the offset vector to compensate for camera drift
				int						deadCount;
virtual	void					Reset (void);																							//	reset appropriate vars
				Vector_2d			GetInput (void);																					//	get the input from the console
				void					DrawInput (Vector_2d goal);																//	draw the control inputs
				void					DrawStatus (void);																				//	draw the ship status
				void					DoLock (void);																						//	handle the object locking stuff
				void					DoDoubleTap (Bool changed);																//	return the multiplier for the double tap effect
				void					DrawString (SArrayPtr font, cstr text, int xx, int yy);		//	ds: convenience function for drawing strings
	public:																																				//	members available externally
				Local (int id = 0);																											//	constructor
				Local (cstr name, int id);																							//	constructor
virtual	~Local (void);																													//	destructor
				Code					*PrePlay (void);																					//	play one frame
};																																							//	end

//------------------------------------------------------------------------------
extern	Local	*gLocal;																													//	global local human player

//------------------------------------------------------------------------------

#endif	//	LOCAL_HUMAN