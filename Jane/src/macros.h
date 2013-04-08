"use strict";

#define AND					&&
#define OR					||
#define NOT					!

#define MATH_PI				"&pi;"

#define	DEFINE_AS(value)	value : #value
#define	COPY_PARAM(paramName, params)											\
	if (#paramName in params) this[#paramName] = params[#paramName]

#define	SUPER				(Object.getPrototypeOf((Object.getPrototypeOf(this))))

#ifdef DEBUG
#define	DEBUGGER			debugger
#define	DEBUGLOG(x)			console.log (x)
#else
#define	DEBUGGER
#define	DEBUGLOG(x)			
#endif

