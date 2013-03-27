"use strict";

#define DERIVE_EXPR(className)                                                  \
var className = Object.create(JaneDataReference, {                              \
    typename : {                                                                \
        value :         #className,                                             \
        writable :      false,                                                  \
        configurable :  false,                                                  \
        enumerable :    true                                                    \
    }})                                                                         \
    
#define DERIVE_EXPR_ALIAS(className, typeName)                                  \
var className = Object.create(JaneDataReference, {                              \
    typename : {                                                                \
        value :         #typeName,                                              \
        writable :      false,                                                  \
        configurable :  false,                                                  \
        enumerable :    true                                                    \
    }})                                                                         \
    
#define JDR(typeName)															\
    Object.create(typeName).init												\
    
#define AND					&&
#define OR					||
#define NOT					!

#define MATH_PI				"&pi;"

#define	DEFINE_AS(value)	value : #value
#define	COPY_PARAM(paramName, params)											\
	if (#paramName in params) this[#paramName] = params[#paramName]
