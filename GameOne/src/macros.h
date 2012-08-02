"use strict";

#define AND     &&
#define OR      ||
#define NOT     !

#define FUNCTION                                                                \
    writable :      true,                                                       \
    configurable :  false,                                                      \
    enumerable :    true,                                                       \
    value :         function                                                    \
    
#define VALUE(var) {                                                            \
    writable :      true,                                                       \
    configurable :  false,                                                      \
    enumerable :    true,                                                       \
    value :         var                                                         \
}                                                                               \
    
