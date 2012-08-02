//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/22/96
//	03/19/97	DS		Made it ALWAYS get from parameter database (to avoid recompiles)
//------------------------------------------------------------------------------

#ifndef	GET_PARAMETERS
#define	GET_PARAMETERS

//------------------------------------------------------------------------------
#define	GET_PARAMETERS_FROM_DATABASE	1																					//	whether or not to get parameters from the database at runtime
#define	WANT_COMMAND_LINE							0																					//	whether or not to try to use the command line
#define	USE_COMMAND_LINE	(GET_PARAMETERS_FROM_DATABASE && WANT_COMMAND_LINE)		//	use the command line only if both are true

//------------------------------------------------------------------------------
//#if	GET_PARAMETERS_FROM_DATABASE
#include "parameter database.h"
#define	ParmString(name)		gParmBase->GetString (#name)
#define	ParmReal(name)			gParmBase->GetReal (#name)
#define	ParmInteger(name)		gParmBase->GetInteger (#name)
/*
#else
#include "parameters.h"
#define	ParmString(name)		name
#define	ParmReal(name)			Real (name)
#define	ParmInteger(name)		name
#endif
*/

//------------------------------------------------------------------------------

#endif	//	GET_PARAMETERS