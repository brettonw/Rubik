//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/03/97
//
//------------------------------------------------------------------------------

#ifndef	PARAMETER_DATABASE
#define	PARAMETER_DATABASE

//------------------------------------------------------------------------------
#include "hash table.h"

//------------------------------------------------------------------------------
class ParmBase																																	//	paramter database class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				HashTable	table;																												//	the table of token pairs
	public:																																				//	members available externally
				ParmBase (void);																												//	constructor
				ParmBase (cstr database);																								//	constructor
				~ParmBase (void);																												//	destructor
				Bool			Load (cstr database);																					//	load from a specified database
				void			Dump (cstr database);																					//	dump the parameters to a specified database file
				cstr			GetString (const char *name);																	//	get a string parameter
				Real			GetReal (const char *name);																		//	get a real valued parameter
				int				GetInteger (const char *name);																//	get an integer valued parameter
				cstr			Assign (const char *name, const char *value);									//	assign a value to a name
};																																							//	end

//------------------------------------------------------------------------------
extern	ParmBase	*gParmBase;																										//	global parameter database

//------------------------------------------------------------------------------

#endif	//	PARAMETER_DATABASE