/*
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_TYPES_H_
#define _FAITH_EMULATOR_TYPES_H_

#include <stddef.h> // Define NULL, size_t
#include <limits.h> // Define UCHAR_MAX, etc

/*
 *****************************************************
 * Fixed-width integer definitions
 ****************************************************
 */
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS
#include <stdint.h> // Will define intN_t

/*
 *****************************************************
 * nullptr definition
 ****************************************************
 */

#if __cplusplus < 201103L // C++11 defines nullptr
#define nullptr NULL
#endif // __cplusplus < 201103L

#endif // _FAITH_EMULATOR_TYPES_H
