#ifndef ERRORMESSAGE_HPP
#define ERRORMESSAGE_HPP

#include <cstdarg>

extern "C" int vsprintf( char *str, const char *format, va_list ap );

#include "LoopPrivate.hpp"

inline char *errorMessage( const char *fmt, ... ) {
    va_list ap;
    va_start( ap, fmt );

    vsprintf( LoopData->errorBuffer, fmt, ap );

    return LoopData->errorBuffer;
}

#endif
