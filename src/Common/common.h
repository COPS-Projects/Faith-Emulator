/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Taken from :
 * ****** BARLab - Open Source ******
 * Copyright (C) 2012 BARLab
 * Copyright (C) 2012 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_COMMON_H_
#define _FAITH_EMULATOR_COMMON_H_

#include "def.h" // all defines by CMake
#include "arch.h"
#include "types.h"
#include "endianness.h"
#include "err.h"
#include "unicode.h"
#include "basefunc.h"
#include "strres.h"

#ifdef _WIN32
#include <windows.h>

#ifdef _MSC_VER // Visual Studio will complain for linking...
#pragma comment(lib, "WinMM") // for timeGetTime()
#endif

#else
#include <sys/time.h> // for timeGetTime()
#endif

/*
 *****************************************************
 * Safe delete macros
 ****************************************************
 */

// Delete a pointer and set the pointer to NULL.
#define SAFE_DELETE(ptr)                          \
    delete ptr;                                   \
    ptr = nullptr;

// Delete an array and set the pointer to NULL.
#define SAFE_DELETE_ARRAY(ptr)                    \
    delete[] ptr;                                 \
    ptr = nullptr;

/*
 *****************************************************
 * Protection macros
 ****************************************************
 */

// Prohibit the copy of the object
#define PROHIBIT_COPY(__class)                   \
    private:                                        \
        __class(const __class&);                    \
        __class& operator=(const __class&)

/*
 *****************************************************
 * Common macros
 ****************************************************
 */

// Quote strings in macro */
#define STRINGIFY_(str) #str
#define STRINGIFY(str) STRINGIFY_(str)

/*
 *****************************************************
 * Cross-compiling definitions
 ****************************************************
 */

// If using Visual Studio, __FUNCTION__ is not defined, but __func__ is
#ifndef _MSC_VER
#define __FUNCTION__ __func__
#endif // _MSC_VER

// If using Visual Studio, and C++11 is not implemented
// POSIX-compliant platforms define those functions, so C++11 is not required
#if defined(_MSC_VER) && __cplusplus < 201103L
#define snprintf _snprintf
#endif

// If __TIMESTAMP__ is not defined, create it by merging __DATE__ and __TIME__
#ifndef __TIMESTAMP__
#define __TIMESTAMP__ __DATE__" "__TIME__
#endif

// If not on Windows, we must implement a kind-of timeGetTime()
#ifndef _WIN32
inline unsigned int timeGetTime()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    return now.tv_usec / 1000;
}
#endif


#endif // _FAITH_EMULATOR_COMMON_H_
