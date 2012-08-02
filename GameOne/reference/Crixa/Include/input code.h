//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/21/97
//
//------------------------------------------------------------------------------

#ifndef	INPUT_CODE
#define	INPUT_CODE

//------------------------------------------------------------------------------
enum	{ LOCK_EMPTY = 0, LOCK_POSITION = 1, LOCK_OBJECT = 2, LOCK_CLEAR = 3};		//	lock control codes
enum	{ KORVES_EMPTY = 0, KORVES_COME = 1, KORVES_GO = 2, KORVES_MOVE = 3,			//	korves base control codes
				KORVES_KILL = 4};																												//	korves base control codes (continued)

//------------------------------------------------------------------------------
class Code																																			//	an input code
{																																								//	begin
	private:																																			//	members internal to this class only
static	Real			scale;																												//	the multiplier for converting the thrust values to scales
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				short			thrust;																												//	the thrust along the normal axis
				short			strafe;																												//	sideways thrust
				short			rotate;																												//	rotational thrust
				uint			weapon:4;																											//	current weapon selection
				uint			fire:1;																												//	whether or not to fire
				uint			lock:2;																												//	lock code
				uint			valid:1;																											//	whether or not this code is valid
				uint			quit:1;																												//	true if the player has quit
				uint			unused:7;																											//	unused bits
	public:																																				//	members available externally
				Code (void);																														//	constructor
				Code (Real thr, Real str, Real rot, uint weap, uint fire, uint lock);		//	constructor
				Code (const Code &code);																								//	copy constructor
				Real			Thrust (void);																								//	return the thrust scale
				Real			Strafe (void);																								//	return the strafe scale
				Real			Rotate (void);																								//	return the rotate scale
};																																							//	end

//------------------------------------------------------------------------------
inline	Code::Code (void)																												//	constructor
{																																								//	begin
	valid = 0;																																		//	set the valid code to FALSE
	quit = 0;																																			//	set the quit code to FALSE
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Code::Thrust (void)																								//	return the thrust scale
{																																								//	begin
	return thrust * scale;																												//	return the reconstituted value
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Code::Strafe (void)																								//	return the strafe scale
{																																								//	begin
	return strafe * scale;																												//	return the reconstituted value
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Code::Rotate (void)																								//	return the rotate scale
{																																								//	begin
	return rotate * scale;																												//	return the reconstituted value
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	INPUT_CODE
