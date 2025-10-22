// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED
#include <cstdio>

namespace miniini_private
{

/// @cond PRIVATE

//Ptr to the logging function
extern void(* __Log)(const char * const);

#ifdef INI_DEBUG

#define LOGBUFSIZE 1024

#define WARNING(...) \
{\
    if(__Log)\
    {\
        __Log("MiniINI WARNING:");\
        c __WARNING_BUF [LOGBUFSIZE];\
        snprintf(__WARNING_BUF, LOGBUFSIZE, __VA_ARGS__);\
        __Log(__WARNING_BUF);\
    }\
}

#define ERROR(...) \
{\
    if(__Log)\
    {\
        __Log("MiniINI ERROR:");\
        c __ERROR_BUF[LOGBUFSIZE];\
        snprintf(__ERROR_BUF, LOGBUFSIZE, __VA_ARGS__);\
        __Log(__ERROR_BUF);\
    }\
}

#define LOG(...) \
{\
    if(__Log)\
    {\
        __Log("MiniINI:");\
        c __LOG_BUF[LOGBUFSIZE];\
        snprintf(__LOG_BUF, LOGBUFSIZE, __VA_ARGS__);\
        __Log(__LOG_BUF);\
    }\
}

#else

#define WARNING(...) 0

#define ERROR(...) 0

#define LOG(...) 0

#endif

/// @endcond 

}

///Sets the function to use for MiniINI logging.
/**
 * @param callback Function that will handle logging - must return void and take a single const char* parameter.
 */
void INILogCallback(void(* callback)(const char * const));

#endif
