//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/13/96
//
//------------------------------------------------------------------------------

#ifndef	DEBUGGER
#define	DEBUGGER

//------------------------------------------------------------------------------
class QDebugger																																	//	mouse input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				char			name[32];																											//	name of the debugger file
				int				count;																												//	debugging count
	public:																																				//	members available externally
				QDebugger (cstr name);																									//	constructor
				void			Soft (cstr format, ...);																			//	message function
				void			Hard (cstr format, ...);																			//	direct to disk message function
				void			Increment (void);																							//	increment the debugging count
				void			Decrement (void);																							//	decrement the debugging count
};																																							//	end

//------------------------------------------------------------------------------
inline	void	QDebugger::Increment (void)																				//	increment the debugging count
{																																								//	begin
	//if (count==0) Soft ("*** Opening Debug Log\n");
	count++;
}																																								//	end

//------------------------------------------------------------------------------
inline	void	QDebugger::Decrement (void)																				//	decrement the debugging count
{																																								//	begin
	//if (count==1) Soft ("\n*** Closing Debug Log\n");
	count--;																																			//	decrement the count
	if (count < 0)																																//	if the count has gone negative
		count = 0;																																	//	keep it at 0
}																																								//	end

//------------------------------------------------------------------------------
extern	QDebugger	gDebugger;																										//	global debugger

//------------------------------------------------------------------------------
//	special aliases for turning the debugger on and off
#if	1																																						//	If I want to use the debugger
#define	DEBUG_STR		gDebugger.Soft																							//	alias for the global debugger
#define	DEBUG_SOFT	gDebugger.Soft																							//	alias for the global debugger
#define	DEBUG_HARD	gDebugger.Hard																							//	alias for the global debugger
#define	DEBUG_ON		gDebugger.Increment																					//	alias for the global debugger
#define	DEBUG_OFF		gDebugger.Decrement																					//	alias for the global debugger
#else																																						//	otherwise, I don't want to use the debugger
inline	void	DEBUG_STR (cstr, ...) {}																					//	null function
inline	void	DEBUG_SOFT (cstr, ...) {}																					//	null function
inline	void	DEBUG_HARD (cstr, ...) {}																					//	null function
inline	void	DEBUG_ON (void) {}																								//	null function
inline	void	DEBUG_OFF (void) {}																								//	null function
#endif																																					//	end whether or not to use the debugger

//------------------------------------------------------------------------------

#endif	//	DEBUGGER