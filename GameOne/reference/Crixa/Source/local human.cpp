//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/03/97
//
//  02/13/97	MK		Added SoundCode logic to Colorswitch interface 
//									logic to handle alert sounds (REDSOUND, YELLOWSOUND)
//	02/18/97	MK		Added zero inits to both constructors
//	02/21/97	DS		Modified GetInput() to control camera slew vector gCameraSlew
//									Keys I,J,K,M and SPACE
//									Modified RadarPlotter to use gCameraSlew too
//									Modified DrawStat() to take gCameraSlew in account
//	02/27/97	DS		Handle interface-side of "game over"
//	03/03/97	JB		Added planning bomb targetting, limited camera slew
//	03/05/97	DS		Added Ship Lives, modified limited camera slewing
//	03/06/97	DS		Fixed Radar browsing with camera slew, added coordinates
//									Added SLEWMODE (1 for regular behavior, 0 for explore)
//	03/09/97	DS		Added Flashing Ammo Count when low on ammo
//	03/18/97	DS		Added real time double-tap checks, based on 30th of second.
//	03/20/97	DS		Added LevelInterval() call, to be called between levels
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#else
#define	RECT	Rect
#endif

//------------------------------------------------------------------------------
#include "local human.h"
#include "reporter.h"
#include "mouse pointer.h"
#include "joystick pointer.h"
#include "joystick POV pointer.h"
#include "joystick relative pointer.h"
#include "joystick absolute pointer.h"
#include "keyboard relative pointer.h"
#include "keyboard absolute pointer.h"
#include "sprite array mgr.h"
#include "pi.h"
#include "area.h"
#include "weapon ship.h"
#include "circle.h"
#include "shipyard.h"
#include "debugger.h"
#include "view.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "trigger array.h"
#include "sound mgr.h"
#include "rotating.h"
#include "get parameters.h"
#include "world.h"
#include "world grid.h"
#include "display.h"
#include "crixa ship.h"

//------------------------------------------------------------------------------
#define USE_JOYSTICK_CODE			1																									// normal joystick
// the following require USE_JOYSTICK_CODE to be TRUE
#define	USE_SINISTAR_CODE			0
#define	USE_THRUSTMASTER_CODE	0
#define	USE_SPACEBALL_CODE		0

// camera slew mode 
#define	SLEWMODE	1																															// 0 is debug mode, 1 is normal game

#define YELLOWSOUND 56
#define REDSOUND 57

#define HULL_WARN 17																														// values at which to change interface drawing
#define HULL_CRIT 9																															// for warning and critical

#define TAPINTERVAL R(0.03333333)

//------------------------------------------------------------------------------
Local	*gLocal;																																	//	global local human player

//------------------------------------------------------------------------------
void	Local::DoDoubleTap (Bool down)																						//	handle the double tap process
{																																								//	begin
	Real currentTime = gReferee->MasterClock();
	if (currentTime > doubletapwhen)																							//	increment counts only if 1/30th second has passed
	{
		tapCount[0]++;
		tapCount[1]++;
		doubletapwhen = currentTime + TAPINTERVAL;
	}

	if (down)
	{
		if (!wasDown)
		{
			if (tapCount[tapCurrent] < ParmInteger (DOUBLE_TAP_DELAY))
			{
				multiplier = range;
				tappedCount = 0;
			}
			tapCount[tapCurrent] = 0;
			tapCurrent ^= 0x01;
		}
		else if (++tappedCount >= tapDuration)
			multiplier = R(1.0);
		wasDown = TRUE;
	}
	else
	{
		multiplier = R(1.0);
		wasDown = FALSE;
	}
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d	Local::GetInput (void)																								//	get the input from the console
{																																								//	begin
	int		early = GetTickCount ();
	keyboard->Update ();																													//	update the keyboard
	mouse->Update (flipflop);
	#if USE_JOYSTICK_CODE
	joy1->Update (flipflop);
	joy2->Update (flipflop);
	#endif
	keypad->Update (flipflop);
	arrows->Update (flipflop);
	rel->Update (flipflop);
	flipflop ^= 1;
	#if USE_SINISTAR_CODE
	PointerDevice	*pointer,
								*dirThrust = keypad; 
	if (mouse->GetTimeStamp () > dirThrust->GetTimeStamp ())
		dirThrust = mouse;
	if (joy1->GetTimeStamp () > dirThrust->GetTimeStamp ())
		dirThrust = joy1;
	pointer = dirThrust;
	#else
	PointerDevice	*pointer = rel,
								*dirThrust = keypad;
	if (mouse->GetTimeStamp () > pointer->GetTimeStamp ())
		pointer = mouse;
	#endif

	#if USE_JOYSTICK_CODE
	if (joy1->GetTimeStamp () > pointer->GetTimeStamp ())
		pointer = joy1;
	if (joy2->GetTimeStamp () > dirThrust->GetTimeStamp ())
		dirThrust = joy2;
	#endif

	Vector_2d	goal = pointer->GetPos ();																					//	get the goal vector from the pointer
	Real			delta = goal.Norm (),																								//	compute the length of the goal vector
						rotate = (delta > EPSILON) ? ((Ship*) crixa())->Point (goal) : ZERO,//	get the rotational force needed to obtain the goal vector
						thrust = ZERO,																											//	start with a zero thrust
						strafe = ZERO;																											//	and a zero strafe
	Bool			hasInput = FALSE;
	if (delta > R(1.0))
	{
		goal = goal / delta;
		delta = R(1.0);
	}

	if (keyboard->GetKey (ParmInteger (TRACTOR_DRIVE_BUTTON_1)) 
	|| keyboard->GetKey (ParmInteger (TRACTOR_DRIVE_BUTTON_2)))
	{
		Rotating	*rot = (Rotating *) crixa->GetVisual ();
		Real			base = ParmReal (CRIXA_BASE_SPEED),
							inverse = R(1.0) - base;
		DoDoubleTap (TRUE);
		Real			speed = ((Ship*) crixa())->GetMaxSpeed () * ((delta * inverse) + base);
		if (multiplier == range)
		{
			speed = ((Ship*) crixa())->GetMaxSpeed ();
			thrust = R(1.0) * multiplier;
		}
		else
			((Ship*) crixa())->EliteGo (rot->GetDirection ()[0] * speed, thrust, strafe);
		stamp = early;
		hasInput = TRUE;
	}
	else
		DoDoubleTap (FALSE);

	Vector_2d	thr = dirThrust->GetPos ();
	if ((dirThrust->GetTimeStamp () > stamp) && ((thr | thr) != ZERO))
	{
		((Ship*) crixa())->EliteGo (thr * ((Ship*) crixa())->GetMaxSpeed (), thrust, strafe);
		hasInput = TRUE;
	}

	thr = arrows->GetPos ();
	if ((thr | thr) != ZERO)
	{
		thrust = thr[Y];
		strafe = thr[X];
		hasInput = TRUE;
	}

	if (keyboard->GetKey (ParmInteger (STOP_BUTTON)) 
	|| ((hasInput == FALSE) && (ParmInteger (CRIXA_STOP_IF_NO_INPUT))))
		((Ship*) crixa())->Stop (thrust, strafe);

	uint	lockCode = LOCK_EMPTY;
	lockTime -= gReferee->Interval ();
	if (lockTime < ZERO)
	{
		if (keyboard->GetKey (ParmInteger (LOCK_POSITION_BUTTON)))
			lockCode = LOCK_POSITION;
		if (keyboard->GetKey (ParmInteger (LOCK_OBJECT_BUTTON)))
			lockCode = LOCK_OBJECT;
	}
	if (keyboard->GetKey (ParmInteger (LOCK_CLEAR_BUTTON)))
		lockCode = LOCK_CLEAR;


	if (keyboard->GetKey (VK_F2))																									//	Reset the Joystick Detection
	{
		joystick0->Reset ();
		joystick1->Reset ();
	}

	int		weapon = ((WeaponShip *) crixa ())->CurrentWeapon ();										//	start with a weapon index of the currently selected weapon
	Bool	keyOp = FALSE;
	if (keyboard->GetKey (ParmInteger (INC_WEAPON_BUTTON)))
	{
		if (!weaponKeyOp)
			do
				weapon = (weapon + 1) % 10;
			while (!((WeaponShip *) crixa ())->WeaponAvailable (weapon));
		keyOp = TRUE;
	}
	if (keyboard->GetKey (ParmInteger (DEC_WEAPON_BUTTON)))
	{
		if (!weaponKeyOp)
			do
				weapon = ((weapon - 1) + 10) % 10;
			while (!((WeaponShip *) crixa ())->WeaponAvailable (weapon));
		keyOp = TRUE;
	}
	weaponKeyOp = keyOp;
	for (int i = 0; i < 10; i++)																									//	loop over all of the number keys
		if (keyboard->GetKey ('0' + i))																							//	if a key is down
		{
			weapon = i;																																//	save the id as a weapon switch code
			keyOp = TRUE;
		}
	if (!keyOp)
		weapon = 10;
	Bool	fire = keyboard->GetKey (ParmInteger (FIRE_BUTTON_1))	|| keyboard->GetKey (ParmInteger (FIRE_BUTTON_2));

	// capture frame
	if (keyboard->GetKey(VK_F5))
		gCaptureFrame = TRUE;

	// keyboard view slew control

	if (keyboard->GetKey('I'))
			gCameraSlew += Vector_2d(R(0.0),R(32.0));
	if (keyboard->GetKey('M'))
			gCameraSlew -= Vector_2d(R(0.0),R(32.0));
	if (keyboard->GetKey('K'))
			gCameraSlew += Vector_2d(R(32.0),R(0.0));
	if (keyboard->GetKey('J'))
			gCameraSlew -= Vector_2d(R(32.0),R(0.0));

	#if SLEWMODE
	// automatic camera recenter / range limit
	Real a = fabs(gCameraSlew[X])+fabs(gCameraSlew[Y]);		// poor mans vector abs value :)
	if (a < R(1.0)) gCameraSlew(ZERO,ZERO);
	else gCameraSlew *= R(0.82);													// magic equilibrium number
	#else
	if (keyboard->GetKey(VK_SPACE))
		gCameraSlew = Vector_2d(R(0.0),R(0.0));
	#endif

	inputs.Enqueue (Code (thrust, strafe, rotate, weapon, fire, lockCode));				//	enqueue the resulting input code
	return goal;																																	//	return the goal vector

}																																								//	end

//------------------------------------------------------------------------------
void	Local::DrawInput (Vector_2d goal)																					//	draw the control inputs
{																																								//	begin
	Point_2d	shipCenter = gCenter + cameraOffset - gCameraSlew;
	SArrayPtr	apoint = gSpriteMgr.Fetch ("apoint"),
						acursor = gSpriteMgr.Fetch ("acursor");
	Real			angle;
	if ((goal | goal) <= EPSILON)
		goal = ((Rotating *) crixa->GetVisual ())->GetDirection ()[0];
	angle = ATAN2 (goal[Y], goal[X]);
	while (angle < ZERO)
		angle += TWO_PI;
	while (angle > TWO_PI)
		angle -= TWO_PI;
	angle *= OVER_TWO_PI;
	int				index = int ((angle * R(32.0)) + R(0.5)) & 31;
	apoint[index]->Draw (shipCenter);
	acursor[0]->Draw ((goal * R(2.0e2)) + shipCenter);

	SArrayPtr targeting = gSpriteMgr.Fetch ("lock");															//	jb addition for his planning bomb

}																																								//	end

//------------------------------------------------------------------------------
void	Local::DrawStatus (void)																									//	draw the ship status
{																																								//	begin
	SArrayPtr	array, array2, array3;
	int				i, j, x, y;																													//	temp variables
	int				critical=FALSE;																											//  critical ship state
	Real			lifebar,extralife;																											//	used for power bars
	Real			power,extrapower;																										//	used for power display
	int				refX = gDisplay->PlayRect().right,																	//	reference point for interface plotting
						refY = gDisplay->PlayRect().bottom,
						eraseY = gDisplay->PlayRect().top;
	RECT			R;																																	//	used in calling EraseRect()

	char			numString[32];																											// used for digit display
	cstr			wnStrings[10]={"springline", "traik", "flechette", "laser",					// weapon names (used with smallcaps)
													 "plasma", "five", "six", "rocket", "torpedo", "mine"};

	/*** WEAPON RELATED VARIABLES **********************************************/
	
	Weapon	*weapon;																															// used to point to weapon
	int cw;																																				// current weapon
	int	ws;																																				// weapon status

	weapon= ((WeaponShip *) crixa ())->GetCurrentWeapon ();												// initialize weapon
	cw = ((WeaponShip *) crixa ())->CurrentWeapon();															// get index number

	lifebar = crixa->GetLife ();																									// retrieve life from crixa piece
	if (lifebar < ZERO) DEBUG_HARD("Weird Life Error\n");
	if (lifebar > R(1.0))
	{
		extralife = lifebar - R(1.0);
		lifebar = R(1.0);
	}
	else extralife = R(0.0);

	if (fabs(tlife - lifebar)<0.04)																								// init for animating life bar
		tlife=lifebar;
	else
		tlife += (tlife < lifebar) ? R(0.04) : R(-0.04);			

	if (fabs(textralife - extralife)<0.05)																				// init for animating armor bar
		textralife=extralife;								
	else
		textralife += (textralife < extralife) ? R(0.05) : R(-0.05);

	power = ((CrixaShip*) crixa())->Power ();																			// retrieve power level from ship
	if (power > R(1.0)) 
	{
		extrapower = power - R(1.0);
		power = R(1.0);
	}
	else extrapower = R(0.0);
	
	if ((lastextralife - extralife) > R(0.0))																			// init for hit feedback on armor
	{
		if (extralifedamage <= 0)																										// Reset the end only if the fadeout has finished
			dend = (int) (lastextralife * 20.0);																			// ending place to show fades
		dstart = (int) (extralife * 20.0);																					// starting place to show fades
		extralifedamage = 16;																												// *ELD* number of frames to show damage
	}

	if (lifebar < lastlife)																													// frame display time for life damage
		lifedamage = 2;																															// *LD* 

	/*** SHIP HULL, CHASSIS, LIFE BAR ******************************************/

	array2 = gSpriteMgr.Fetch("armi");
	i = (int) (tlife * Real (array2->Count () - 1));															// integer value of ship's hull status (
	if (i<HULL_WARN) critical = TRUE;																							// *CRIT* if ship is damaged at certain point, set this flag for special interface handling

	j = 0;																																				// j is offset into grid sprites
	if (i<HULL_WARN) j = 1;																												// yellow grid
	if (i<HULL_CRIT) j = 2;																												// red grid

	if (extralifedamage>5)																												// *ELD* bright green grid flash for armor damage
		j = 4;																																			// bright yellow grid flash
	if (lifedamage)																																// *LD* red grid flash for hull (life) damage
		j = 5;

	// grid backgrounds
	array = gSpriteMgr.Fetch("grid6x");
	array[j]->DrawUnclipped(refX+29,refY-239);																		// ship grid background

	array = gSpriteMgr.Fetch("grid3x");
	array[j]->DrawUnclipped(refX+86,refY-237);																		// green grid background
	array[3]->DrawUnclipped(refX+117,refY-237);																		// blue grid background


	if ((i > HULL_WARN)||(i==0))																									// If we are healthy again or DEAD
	{
		if (yellowEMid) {																														// Turn off Yellow alert if playing
			gSoundMgr->RemoveEMSound(YELLOWSOUND, yellowEMid);
			yellowEMid = 0;
		}
		else if (redEMid) {																													// Turn off Red alert if playing
			gSoundMgr->RemoveEMSound(REDSOUND, redEMid);
			redEMid = NULL;
		}
		else
			redEMid = yellowEMid = NULL;																							// Set all sound flags to null, display is green
	}
	
	// ship hull status
	// we are reusing j as our index into a sprite array.
	// this time, it's for the ship's hull.
	j = 0;	
	if ((i<HULL_WARN) && (i>= HULL_CRIT))																					// If i is between CRIT & WARN (ie yellow alert) 
	{	
		j = 3;																																			// Set index j to 3 for proper hull sprites
		if(redEMid) {
			gSoundMgr->RemoveEMSound(REDSOUND, redEMid);															// Get rid of red alert if already playing
			redEMid = NULL;																														// Reset the id to indicate it has stopped
		}
		if(!yellowEMid)																															// If Yellow alert not already playing
			yellowEMid = gSoundMgr->InsertEMSound(YELLOWSOUND, crixa);								// Install the yellow sound emitter
	}
		
	if (i<HULL_CRIT)
	{
		j = 6;
		if (yellowEMid) {																														// Get rid of yellow alert if already playing
				gSoundMgr->RemoveEMSound(YELLOWSOUND, yellowEMid);
				yellowEMid = NULL;																											// Reset the id to indicate yellow has stopped
		}
		if (!redEMid)
			redEMid = gSoundMgr->InsertEMSound(REDSOUND, crixa);											// Play red alert sound if not already playing
	}

	// these indicators take precedence
	if (extralifedamage) 																													// *ELD* hull drawn in bright yellow
		j = 3;
	if (lifedamage)																																// *LD* hull drawn in bright red
		j = 6;
	
	array = gSpriteMgr.Fetch("hull");
	array[j]->DrawUnclipped(refX+29,refY-232);																		// hull

	if ((!critical) || (framecounter < 6))																				// *CRIT* *FC* for flashing
	{
		array[j+1]->DrawUnclipped(refX+33,refY-227);																// chassis
		if (lifebar > lastlife)
			array2[i]->DrawFlashUnclipped(refX+86,refY-237);
		else
			array2[i]->DrawUnclipped(refX+86,refY-237);																// life/armor bar

		if (j < 6)																																	// draw these elements only if ship isn't about to die
		{																																						// reason: it just looks better
			// ship hull overlays
			array = gSpriteMgr.Fetch("over2");
			array[0]->DrawUnclipped(refX+33,refY-229);																// overlay bits
			array[1]->DrawUnclipped(refX+30,refY-239);																// overlay boxes

			// overlay level numbers
			array=gSpriteMgr.Fetch("dig5mg");
			i=((CrixaShip *)crixa())->MaxLevel();
			array[i]->DrawUnclipped(refX+33,refY-195);																// armor box number
			array[i]->DrawUnclipped(refX+75,refY-195);																// power box number
			array[i]->DrawUnclipped(refX+54,refY-236);																// boost box number
		}
	}

	if ((critical) && (framecounter > 5))																					// draw the node symbol
		array[j+2]->DrawUnclipped(refX+32, refY-226);																// hurty nodes

	// power bar
	// we are reusing i as the index into the power. Everything before
	// this required i to draw the life status properly.
	array = gSpriteMgr.Fetch("powi");
	i = (int) (power * Real (array->Count () - 1));																// compute integer value of power
	array[i]->DrawUnclipped(refX+117,refY-237);																		// power bar

	/*** EXTRALIFE and POWER DOTS *************************************************/

	// Extralife Armor
	array = gSpriteMgr.Fetch("i2x");																							// armor dots
	x = refX+93; 	y = refY-237;																										// starting plot location
	j = 20;																																				// number of dots in a column

	for (i=(int)((textralife)*20); i>0; i--)																			// draw the good dots
	{
		array[0]->DrawUnclipped(x,y);
		j--;																																				// one less dot in column to plot
		y+=3;																																				// next dot up
		if (j<1)
		{
			j = 20;																																		// reset column count
			x+=4;																																			// next column over
			y = refY-237;																															// start at bottom
		}
	}


	// Most Recently Damaged Extralife Armor Dots
	if (extralifedamage)																													// *ELD* flash the dots that represent damage
		for (i=dstart;i<dend;i++)
		{
			x = i/20;																																	// integer math here
			y = i - (x*20);
			x = (x*4)+refX+93;
			y = (y*3)+(refY-237);
			array[5-((extralifedamage>>2))]->DrawUnclipped(x,y);											// *ELD* draw each dot over several frames
		}

	// Power Dots
	x = refX+124; 	y = refY-237;																									// starting plot location
	j = 20;																																				// number of dots in a column
	for (i=(int)(extrapower*20); i>0; i--)
	{
		array[6]->DrawUnclipped(x,y);
		j--;																																				// one less dot in column to plot
		y+=3;																																				// next dot up
		if (j<1)
		{
			j = 20;																																		// reset column count
			x+=4;																																			// next column over
			y = refY-237;																															// start at bottom
		}
	}


	/*** BIG AMMO COUNT ********************************************************/

	array = gSpriteMgr.Fetch("countext");
	array[(cw==0) ? 1:0]->DrawUnclipped(refX+94,refY-368);												// ammo counter border

	int amc = weapon->AmmoCount ();
	if (amc != 0)
	{
		if ( (amc>25) || (cw>6) || (framecounter<6) )																// low ammo check. Flash if we're below 25 rounds
		{																																						// but only if it's a high rate-of-fire weapon
			// choose green or blue depending on the weapon.
			// weapon 0 is the springline, so use blue
			array = (cw==0) ? gSpriteMgr.Fetch("dig23bb") : gSpriteMgr.Fetch("dig23bg");
			sprintf (numString, "%04d", amc);
			x = 94;
			for (i = 0; numString[i]; i++)
			{
				array[numString[i] - '0']->DrawUnclipped (refX+x,refY-358);
				x += 12;
			}
		}
		else																																				// it's not being drawn, erase the area
		{
			SetRect (&R,refX+94,eraseY+335,refX+142,eraseY+358);
			gDisplay->EraseRect(R);
		}
	}
	else																																					// out of ammo!
	{
		if (framecounter<6)																													// *FC* Make flashing empty sign
		{
			array = gSpriteMgr.Fetch("empty");
			array[0]->DrawUnclipped(refX+94,refY-358);																// EMPTY!
		}
		else																																				// erase the background to make sure it flashes
		{
			SetRect (&R,refX+94,eraseY+335,refX+142,eraseY+358);
			gDisplay->EraseRect(R);
		}
	}


	/*** RESOURCE COUNTER ********************************************************/	

	array = gSpriteMgr.Fetch("text7bg");
	SetRect (&R,refX+18,eraseY+450,refX+80,eraseY+460);
	gDisplay->EraseRect(R);																												// erase a rectangular area

	x=22;																																					// start here
	int		resCount = ((CrixaShip *) crixa())->GetResources ();										// how many resources do we have?
	if (resCount < 1E8)																														// handle overflow of resources
	{
		sprintf (numString,"%08d", resCount);
		for (j=0;numString[j];j++)
		{
			array[numString[j]]->DrawUnclipped(refX+x,refY-458);											// number start
			x += 6;
		} 
	}
	else																																					// switch to scientific notation if numbers are too big
	{
		sprintf (numString,"%2.2e", resCount);
		for (j=0;numString[j];j++)
		{
			array[numString[j]]->DrawUnclipped(refX+x,refY-457);											// number start
			x += 6;
		}
	}

		
	/*** SHIP LIVES COUNTER *****************************************************/	

	SetRect (&R,refX+90,eraseY+450,refX+140,eraseY+460);
	gDisplay->EraseRect(R);																												// erase a rectangular area

	x=92;																																					// start here
	int	lifeCount = GetLives ();
	if (lifeCount > 5)																														// if more than five ships, gotta use numbers
	{
		array = gSpriteMgr.Fetch("text7bg");
		sprintf(numString,"%d",lifeCount);
		for (j=0;numString[j];j++)
		{
			array[numString[j]]->DrawUnclipped(refX+x,refY-458);											// number start
			x += 6;
		} 
	}																																							
	else																																					// use actual icons
	{
		array = gSpriteMgr.Fetch("life");
		for (j=0;j<lifeCount;j++)
		{
			array[0]->DrawUnclipped(refX+x,refY-458);
			x += 10;
		}
	}	// lifeCount


	/*** WEAPON NAME IN BOX ****************************************************/
	SetRect (&R, refX+35,eraseY+272,refX+140,eraseY+280);
	gDisplay->EraseRect(R);											
	if ((!shakeFrame) || (gLocalRandom.RndInteger()>768))													//	bigger damage feedback
	{
		array = gSpriteMgr.Fetch("scaps7bg");																					
		DrawString(array,wnStrings[cw],refX+37,refY-278);														// draw the name of the weapon
	}

	
	/*** WEAPON STATUS DISPLAY *************************************************/

	// Big Ugly Logic...is it cleansable?
	// enum {WEAPON_READY = 0, WEAPON_RELOAD, WEAPON_HEAT, WEAPON_EMPTY}

	// erase the area behind!
	SetRect(&R,refX+28,eraseY+325,refX+80,eraseY+422);
	gDisplay->EraseRect(R);

	// weapon sprite arrays
	array = gSpriteMgr.Fetch("wpstat");
	array2 = gSpriteMgr.Fetch("dig5mg");
	array3 = gSpriteMgr.Fetch("wpnum");

	y = 334;																																			// left edge of weapon numbers
	for (i = 1; i<10; i++)																												// check all ten weapons
	{
		if (((WeaponShip *) crixa())->WeaponAvailable(i))															// if weapon is actually on ship
		{
			weapon = ((WeaponShip *) crixa ())->GetWeapon(i);													// get weapon data to display
			ws = weapon->Status();																										// get the status of the weapo
			if (i!=cw)																																// if this isn't the currently selected weapon
			{
				switch (ws)																															// handle all weapon status codes
				{
					case 0:																																// weapon is READY
					case 1:																																// since weapons don't get process time if they're not selected
					case 2:																																// just assume they're ready. 
						// draw weapon number
						array[0]->DrawUnclipped(refX+50,refY-y);														// draw weapon status READY
						array3[i]->DrawUnclipped(refX+37,refY-y);														// weapon numbers

						// draw ammo count
						x = 56;
						sprintf (numString,"%05d", weapon->AmmoCount());
						for (j=0;numString[j];j++)
						{
							array2[numString[j]-'0']->DrawUnclipped(refX+x,refY-y+2);					// draw digits
							x += 4;
						}
						break;
					case 3:
						array[ws]->DrawUnclipped(refX+50,refY-y);														// armed status EMPTY
    				array3[i+33]->DrawUnclipped(refX+37,refY-y);												// weapon numbers
						break;
				}
			}
			else																																			// this is the currently selected weapon
			{	
				switch (ws)																															// draw both status and weapon number indicator
				{
					case 0:																																// weapon is ARMED and READY
						array[4]->DrawUnclipped(refX+50,refY-y);														// armed status
						array3[i+11]->DrawUnclipped(refX+37,refY-y);												// weapon numbers
						break;
					case 1:																																// weapon is RELOADING
					case 2:																																// weapon is OVERHEATED
						array[ws]->DrawUnclipped(refX+50,refY-y);														// armed status RELOAD or HEAT
    				array3[i+22]->DrawUnclipped(refX+37,refY-y);												// weapon numbers
						break;
					case 3:																																// weapon is EMPTY
						if (framecounter < 6)																								// *FC* make EMPTY status flash
						{
							array[ws]->DrawUnclipped(refX+50,refY-y);													// armed status EMPTY
    					array3[i+33]->DrawUnclipped(refX+37,refY-y);											// weapon numbers
						}
						break;
				}
			}
		}
		else																																				// oh, we don't have this weapon
		{
			array[5]->DrawUnclipped(refX+50,refY-y);																	// blank weapon status
			array3[44]->DrawUnclipped(refX+37,refY-y);																// blank weapon number indicator
		}
		y += 11;
	}

	/*** TRACTOR BEAM ACTIVATION ***********************************************/

	array = gSpriteMgr.Fetch("wptract");
	if (cw)
	{
		SetRect (&R, refX+94,eraseY+406,refX+142,eraseY+422);
		gDisplay->EraseRect(R);
	}
	else
		array[0]->DrawUnclipped(refX+94,refY-422);


	/*** WEAPON LOCK ON SHIPS **************************************************/
	// weapon lock interface
	if ((!shakeFrame) || (gLocalRandom.RndInteger()>768))
	{
		if (lock ())
			gView->Install (new StatSeq (lock->TrueLocation (), new TimeSeq (gSpriteMgr.Fetch ("lock"), FALSE, 1, 0, ZERO)), HUD_LAYER_2);
	}

	/*** RADAR PLOTTER *********************************************************/

	// code from here on in is unmodified, except to make the radar
	// plot in a screen-size independent way

	// added camera slew control...to remove it, search for gCameraSlew

	Reporter	reporter (gReferee, FULL_INTERSECT);
	PiecePtr	piece;																														//	get the first piece
	#if SLEWMODE
	CPoint	loc = crixa->GetBody ()->TLocation ();
	#else
	CPoint	loc = crixa->GetBody ()->TLocation () + gCameraSlew;								//	make radar follow slew
	#endif

	//	radar
	array = gSpriteMgr.Fetch ("rback");																					//	draw the background
	array[0]->DrawUnclipped (refX+21, refY-136);

	if ((!shakeFrame) || (gLocalRandom.RndInteger()>512))												//	bigger damage feedback
	{
		array2 = gSpriteMgr.Fetch ("rsyd");
		array3 = gSpriteMgr.Fetch ("rods");
		
		//	draw the world grid onto the radar
		WGrid			*grid = (WGrid *) gWorld->GetGeometry ();
		#if SLEWMODE
		Vector_2d	gridpos = (crixa->GetBody ()->TLocation ())* OVER_CELL_SIZE;											//	project the ship location into grid space
		#else
		Vector_2d	gridpos = (crixa->GetBody ()->TLocation () + gCameraSlew )* OVER_CELL_SIZE;				//	use this for radar follow gCameraSlew
		#endif
		Pixel			bright = MakePixel (0, 25, 0);
		x = refX + 81;
		y = refY - 76;
		int		xx = int (gridpos[X]),
					yy = int (gridpos[Y]),
					xodd = xx & 1,
					yodd = yy & 1;

		for (j = -60 + yodd; j <= 60; j += 2)
		{
			int		k = yy + j,
						kk = y + j;
			for (i = -60 + xodd; i <= 60; i += 2)
				if (grid->IsOccupied (xx + i, k, FALSE))
					(*gDisplay) (i + x, kk) = bright;
		}

		for (j = -15 + yodd; j <= 15; j += 2)
		{
			int		k = yy + j,
						kk = y + j;
			for (i = -15; i <= 15; i++)
				if (grid->IsOccupied (xx + i, k, FALSE))
					(*gDisplay) (i + x, kk) = bright;
		}
		for (j = -15 + (yodd ^ 1); j <= 15; j += 2)
		{
			int		k = yy + j,
						kk = y + j;
			for (i = -15 + xodd; i <= 15; i += 2)
				if (grid->IsOccupied (xx + i, k, FALSE))
					(*gDisplay) (i + x, kk) = bright;
		}
		bright = MakePixel (19, 31, 19);
		Vector_2d	angle = Vector_2d (crixa->GetBody ()->RLocation ()) * R(4.0);
		(*gDisplay) (x + int (angle[X]), y + int (angle[Y])) = bright;

		Point_2d	RadarPt (Real (refX + 82), Real (refY - 75));
		
		reporter.Reset ();
		piece = reporter.GetPiece ();	
		while (piece ())																															//	loop over all of the pieces
		{																																							//	begin
			if (piece () != crixa ())
			{
				Body			*body = piece->GetBody ();
				if (body)
				{
					Vector_2d	delta = (body->TLocation () - loc) * OVER_CELL_SIZE;
					if ((FABS (delta[X]) < R(58.0)) && (FABS (delta[Y]) < R(58.0)))
					{
						if (piece->GetOwner () == this)
							array = array3;
						else
							array = array2;
						if (piece->GetGeometry ()->Type () == CIRCLE_GEOM)
						{
							Real	rad = ((Circle*) piece->GetGeometry ())->Radius () * R(5.0e-2);	// don't show anything smaller than 20 pixels
							if (rad > R(1.0))
								rad = R(1.0);
							rad *= Real (array->Count () - 1);
							array[int (rad)]->Draw (RadarPt + delta, FALSE);
						}
					}
				}
			}
			reporter.Next ();																														//	advance to the next piece
			piece = reporter.GetPiece ();																								//	grab a pointer to the piece
		}																																							//	end
	}

	/*** CRIXA SECTOR COORDINATES *********************************************/

	array = gSpriteMgr.Fetch("scaps5bgs");
	#if SLEWMODE
	Point_2d cpos = crixa->GetBody()->TLocation();
	#else
	Point_2d cpos = crixa->GetBody()->TLocation() + gCameraSlew;
	#endif
	int	sectorX = int (cpos[X]), sectorY = int (cpos[Y]);
	sprintf(numString,"%02d %02d",sectorX>>9, sectorY>>9);
	DrawString(array,numString,refX+24,refY-134);

	/*** CLEANUP OF LAST VARIABLES ********************************************/
	lastlife = lifebar;
	lastextralife = extralife;

	if (lifedamage > 0)
		--lifedamage;
	if (extralifedamage > 0)
		--extralifedamage;
	if (--framecounter < 0)
		framecounter = 8;
}																																								//	end

//------------------------------------------------------------------------------
void	Local::DoLock (void)																											//	handle the object locking stuff
{																																								//	begin
	if (!lock ())
		lastLock = ZERO;
	else if (lock->Quality () != lastLock)
	{
		if (lock->Quality () >= R(0.9))
		{
			if (lastLock <= R(0.9))
			{
				gSoundMgr->PlaySound (20);
				lockTime = R(1.0);
			}
		}
		else
			gSoundMgr->PlaySound (23);
		lastLock = lock->Quality ();
	}
}																																								//	end

//------------------------------------------------------------------------------
void	Local::DrawString (SArrayPtr font, cstr text, int xx, int yy)							//	draw a string (ds 01/23/97)
{
	int x = xx;
	for (int i = 0; text[i]; i++)
	{
		font[text[i]]->Draw (Point_2d ( Real (x), Real (yy)), FALSE);										//	draw the string up to the current point
		x += font[text[i]]->Width();
	}
}																																								//	end

//------------------------------------------------------------------------------
Local::Local (int ident) : Human (ident)																				//	constructor
{																																								//	begin
	keyboard = new Keyboard;																											//	allocate the keyboard
	mouse = new MousePointer;
	joystick0 = new JoystickDevice (keyboard, 0);
	joystick1 = new JoystickDevice (keyboard, 1);
	((JoystickDevice *) joystick0)->MapButton (0, ParmInteger (JOY_1_BUTTON_0));
	((JoystickDevice *) joystick0)->MapButton (1, ParmInteger (TRACTOR_DRIVE_BUTTON_1));
//((JoystickDevice *) joystick0)->MapButton (1, ParmInteger (JOY_1_BUTTON_1));
	((JoystickDevice *) joystick1)->MapButton (0, ParmInteger (JOY_2_BUTTON_0));
	((JoystickDevice *) joystick1)->MapButton (1, ParmInteger (JOY_2_BUTTON_1));
	for (int i = 2; i < 32; i++)
	{
		((JoystickDevice *) joystick0)->MapButton (i, ParmInteger (FIRE_BUTTON_2));
		((JoystickDevice *) joystick1)->MapButton (i, ParmInteger (FIRE_BUTTON_2));
	}
	#if	USE_SPACEBALL_CODE
	joy1 = new JoystickPointer (joystick0, ParmReal (JOYSTICK_1_FILTER));
	((JoystickPointer *) joy1)->SetAxes (0, 2, FALSE, TRUE);
	joy2 = new JoystickRPointer (joystick0, ParmReal (JOYSTICK_2_FILTER), ParmReal (SPACEBALL_ROTATION_RATE));
	((JoystickRPointer *) joy2)->SetAxis (ParmInteger (SPACEBALL_ROTATION_AXIS), TRUE);
	#elif USE_THRUSTMASTER_CODE
	joy1 = new JoystickPointer (joystick0, ParmReal (JOYSTICK_1_FILTER));
	joy2 = new JoystickPOVPointer (joystick0, ParmReal (JOYSTICK_2_FILTER));
	((JoystickDevice *) joystick0)->MapButton (1, ParmInteger (INC_WEAPON_BUTTON));
	((JoystickDevice *) joystick0)->MapButton (2, ParmInteger (LOCK_OBJECT_BUTTON));
	((JoystickDevice *) joystick0)->MapButton (3, ParmInteger (LOCK_CLEAR_BUTTON));
	#elif USE_JOYSTICK_CODE
	joy1 = new JoystickPointer (joystick0, ParmReal (JOYSTICK_1_FILTER),FALSE);
	joy2 = new JoystickPointer (joystick1, ParmReal (JOYSTICK_2_FILTER));
	#endif
	keypad = new KeyboardAPointer (keyboard, TRUE, ParmReal (DIRECTION_MULTIPLIER), ParmInteger (DOUBLE_TAP_DIR_DUR));
	((KeyboardAPointer *) keypad)->SetKeys (ParmString (KEYBOARD_ABS_PTR_2_KEYS));
	//keypad->MapButton (0, ParmInteger (TRACTOR_DRIVE_BUTTON_2));
	arrows = new KeyboardAPointer (keyboard, TRUE, ParmReal (THRUST_MULTIPLIER), ParmInteger (DOUBLE_TAP_THRUST_DUR));
	((KeyboardAPointer *) arrows)->SetKeys (ParmString (KEYBOARD_ABS_PTR_1_KEYS));
	rel = new KeyboardRPointer (keyboard, ParmInteger (KEYBOARD_REL_PTR_RATE));
	weaponKeyOp = FALSE;
	range = ParmReal (TRACTOR_MULTIPLIER);
	stamp = 0;
	multiplier = R(1.0);
	tapCurrent = 0;
	tapCount[0] = tapCount[1] = 0;
	tapDuration = ParmInteger (DOUBLE_TAP_TRACTOR_DUR);
	wasDown = FALSE;
	lastWeapon = -1;
	lastLock = ZERO;
	lockTime = ZERO;
	flipflop = 1;
	deadCount = 0;
	tlife = R(1.0);
	textralife = ZERO;
	lifedamage = 0;																																// (MK) Init lifedamage, extralifedamage, dstart & dend to 0
	extralifedamage = 0;
	dstart = 0;
	dend = 0;
	doubletapwhen=ZERO;
}																																								//	end

//------------------------------------------------------------------------------
Local::Local (cstr name, int ident) : Human (name, ident)												//	constructor
{																																								//	begin
	keyboard = new Keyboard;																											//	allocate the keyboard
	mouse = new MousePointer;
	joystick0 = new JoystickDevice (keyboard, 0);
	joystick1 = new JoystickDevice (keyboard, 1);
	((JoystickDevice *) joystick0)->MapButton (0, ParmInteger (JOY_1_BUTTON_0));
	((JoystickDevice *) joystick0)->MapButton (1, ParmInteger (TRACTOR_DRIVE_BUTTON_1));
//((JoystickDevice *) joystick0)->MapButton (1, ParmInteger (JOY_1_BUTTON_1));
	((JoystickDevice *) joystick1)->MapButton (0, ParmInteger (JOY_2_BUTTON_0));
	((JoystickDevice *) joystick1)->MapButton (1, ParmInteger (JOY_2_BUTTON_1));
	for (int i = 2; i < 32; i++)
	{
		((JoystickDevice *) joystick0)->MapButton (i, ParmInteger (FIRE_BUTTON_2));
		((JoystickDevice *) joystick1)->MapButton (i, ParmInteger (FIRE_BUTTON_2));
	}
	#if	USE_SPACEBALL_CODE
	joy1 = new JoystickPointer (joystick0, ParmReal (JOYSTICK_1_FILTER));
	((JoystickPointer *) joy1)->SetAxes (0, 2, FALSE, TRUE);
	joy2 = new JoystickRPointer (joystick0, ParmReal (JOYSTICK_2_FILTER), ParmReal (SPACEBALL_ROTATION_RATE));
	((JoystickRPointer *) joy2)->SetAxis (ParmInteger (SPACEBALL_ROTATION_AXIS), TRUE);
	#elif USE_THRUSTMASTER_CODE
	joy1 = new JoystickPointer (joystick0, ParmReal (JOYSTICK_1_FILTER));
	joy2 = new JoystickPOVPointer (joystick0, ParmReal (JOYSTICK_2_FILTER));
	((JoystickDevice *) joystick0)->MapButton (1, ParmInteger (INC_WEAPON_BUTTON));
	((JoystickDevice *) joystick0)->MapButton (2, ParmInteger (LOCK_OBJECT_BUTTON));
	((JoystickDevice *) joystick0)->MapButton (3, ParmInteger (LOCK_CLEAR_BUTTON));
	#elif USE_JOYSTICK_CODE
	joy1 = new JoystickPointer (joystick0, ParmReal (JOYSTICK_1_FILTER),FALSE);
	joy2 = new JoystickPointer (joystick1, ParmReal (JOYSTICK_2_FILTER));
	#endif
	keypad = new KeyboardAPointer (keyboard, TRUE, ParmReal (DIRECTION_MULTIPLIER), ParmInteger (DOUBLE_TAP_DIR_DUR));
	((KeyboardAPointer *) keypad)->SetKeys (ParmString (KEYBOARD_ABS_PTR_2_KEYS));
	//keypad->MapButton (0, ParmInteger (TRACTOR_DRIVE_BUTTON_2));
	arrows = new KeyboardAPointer (keyboard, TRUE, ParmReal (THRUST_MULTIPLIER), ParmInteger (DOUBLE_TAP_THRUST_DUR));
	((KeyboardAPointer *) arrows)->SetKeys (ParmString (KEYBOARD_ABS_PTR_1_KEYS));
	rel = new KeyboardRPointer (keyboard, ParmInteger (KEYBOARD_REL_PTR_RATE));
	weaponKeyOp = FALSE;
	range = ParmReal (TRACTOR_MULTIPLIER);
	stamp = 0;
	multiplier = R(1.0);
	tapCurrent = 0;
	tapCount[0] = tapCount[1] = 0;
	tapDuration = ParmInteger (DOUBLE_TAP_TRACTOR_DUR);
	wasDown = FALSE;
	lastWeapon = -1;
	lastLock = ZERO;
	lockTime = ZERO;
	flipflop = 1;
	deadCount = 0;
	tlife = R(1.0);
	textralife = ZERO;
	lifedamage = 0;																															 // (MK) Init lifedamage, extralifedamage, dstart & dend to 0
	extralifedamage = 0;
	dstart = 0;
	dend = 0;
	doubletapwhen=ZERO;
}																																								//	end

//------------------------------------------------------------------------------
Local::~Local (void)																														//	destructor
{																																								//	begin
	delete mouse;																																	//	free the pointer device
	#if USE_JOYSTICK_CODE
	delete joy1;																																	//	free the pointer device
	delete joy2;																																	//	free the pointer device
	#endif
	delete joystick0;
	delete joystick1;
	delete keypad;																																//	free the pointer device
	delete arrows;																																//	free the pointer device
	delete rel;																																		//	free the pointer device
	delete keyboard;																															//	free the keyboard
}																																								//	end

//------------------------------------------------------------------------------
void Local::Reset (void)																												//	call this whenever a level is about to begin
{
	doubletapwhen = gReferee->MasterClock();																			//	reset double tap timer for new level, since referee is different
	Human::Reset();																																//	initialize everyone else
}
//------------------------------------------------------------------------------
Code	*Local::PrePlay (void)																										//	play one frame
{																																								//	begin
	if (gReferee->Confirm (crixa))																								//	if the crixa ship is a valid piece
	{																																							//	begin
		cameraOffset = crixa->GetBody ()->TLocation () - Camera ();									//	compute the camera offset vector to compensate for camera drift
		Vector_2d	goal = GetInput ();																								//	get an input from the console
		DrawInput (goal);																														//	draw the input
		DrawStatus ();																															//	draw the ship status
		DoLock ();
		int		weapon = ((WeaponShip *) crixa ())->CurrentWeapon ();
		if (weapon != lastWeapon)
		{
			lastWeapon = weapon;
			gSoundMgr->PlaySound (17);
		}
	}																																							//	end
	else // crixa ship is dead
	{
		keyboard->Update ();																												//	get keyboard update (frames)
		if (++deadCount > ParmInteger (CRIXA_DEAD_COUNT))														//	wait for time delay CRIXA_DEAD_COUNT
			if (keyboard->GetKey (VK_SPACE))																					//	if there are, then check for the space bar to continue
			{
				PiecePtr	unused;																												//	next call returns this variable, which we don't care about
				if (!gReferee->Contains (loc, unused))																	//	Make sure human isn't intersecting any other piece 																															
					inputs.Enqueue (Code (ZERO, ZERO, ZERO, 0, 0, LOCK_CLEAR));						//	then enqueue an empty input code
				deadCount = 0;																													//	rest time counter
				
				tlife = R(1.0);																													//	related to interface life bar drawing
				textralife = ZERO;																											//	ditto
			}
			else
			{	
				SArrayPtr sprite;
				if (lives>0)
					sprite = gSpriteMgr.Fetch ("destroyed");															//	display the "press space bar to resume" sprite
				else
					sprite = gSpriteMgr.Fetch ("gameover");																//	you're dead
				sprite[0]->Draw (gCenter);
			}	
		DrawStatus ();																															//	keep drawing the ship status regardless
	}
	return inputs.Dequeue ();
}																																								//	end

//------------------------------------------------------------------------------
