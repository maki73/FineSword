// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_CONFIG_H
#define FINESWORD_CONFIG_H

/* Project-Wide Configuration File
 */

/* -- Config -- */

/* target 16/128-bit width interchange floating-point format(s)
 * NOTE: enabling these will make the project rely on non-portable compiler extensions
 * or features introduced only by newer standards (such as C24/C23)
 * 
 * If you're uncertain, do not define both of these macros and let the heuristics below
 * figure out the target platform's supported set.
 */
// #define FINESWORD_TARGET_16BIT_FLOAT  0
// #define FINESWORD_TARGET_128BIT_FLOAT 0

/* use memcpy instead of union-based type punning for astype/reinterpretation utilities
 * NOTE: by enabling it you lose projects self-containment (<string.h> will be included)
 */
#define FINESWORD_ASTYPE_USE_MEMCPY   0

/* let finesword tests use ANSI escape codes for more expressive terminal output
 *
 * Do not define to let the heuristics figure out
 */
// #define FINESWORD_TESTS_USE_ANSI      0

/* Platform, for binary floating-point formats, uses d1 bit (i.e., trailing significant MSB) set to 1 for **quiet** NaN
 *
 * Do not define to let the heuristics figure out
 */
// #define FINESWORD_BINARY_FP_D1_SET_IS_QUIET 0


/* DO NOT TOUCH. TOGGLED ONLY BY THE IMPLEMENTATION MAINTAINERS */
#define FINESWORD_SUPPORTS_DECIMAL    0
#define FINESWORD_SUPPORTS_BINARY     1


/* -- Heuristics -- */

#if !defined(FINESWORD_TARGET_16BIT_FLOAT) && !defined(FINESWORD_TARGET_128BIT_FLOAT)
    #ifdef _WIN32
        #define FINESWORD_TARGET_16BIT_FLOAT  0
        #define FINESWORD_TARGET_128BIT_FLOAT 0
    #else
        // F128
        // GCC on HP-UX PA-RISC does support __float128, but not on the tested NetBSD target it does not
        #if defined(__apple_build_version__) || (defined(__hppa__) && !defined(__hpux__)) || defined(__aarch64__) || defined(__TINYC__)
            #define FINESWORD_TARGET_128BIT_FLOAT 0
        #else
            #define FINESWORD_TARGET_128BIT_FLOAT 1
        #endif

        // F16
        // #if FINESWORD_TYPES_DO_CHECKS // allows to pull std headers
        //     #define __STDC_WANT_IEC_60559_TYPES_EXT__
        //     #include <float.h> 
        //     #ifdef FLT16_MIN
        //         #define FINESWORD_TARGET_16BIT_FLOAT 1
        //     #else
        //         #define FINESWORD_TARGET_16BIT_FLOAT 0
        //     #endif
        // #else // fallback to compiler specific macro
            #ifdef __FLT16_MIN__
                #define FINESWORD_TARGET_16BIT_FLOAT 1
            #else
                #define FINESWORD_TARGET_16BIT_FLOAT 0
            // #endif
        #endif
    #endif
#endif

#ifndef FINESWORD_BINARY_FP_D1_SET_IS_QUIET
    #if defined(__hppa__) || (defined(__mips__) && !defined(__mips_nan2008)) // PA-RISC; NOTE: untested on legacy MIPS
        #define FINESWORD_BINARY_FP_D1_SET_IS_QUIET 0
    #else
        #define FINESWORD_BINARY_FP_D1_SET_IS_QUIET 1
    #endif
#endif

#ifndef FINESWORD_TESTS_USE_ANSI
    #ifdef _WIN32
        #define FINESWORD_TESTS_USE_ANSI      0
    #else
        #define FINESWORD_TESTS_USE_ANSI      1
    #endif
#endif


/* -- ifndef guards -- */

#if !defined(FINESWORD_TARGET_16BIT_FLOAT)  || (FINESWORD_TARGET_16BIT_FLOAT != 0 && FINESWORD_TARGET_16BIT_FLOAT != 1)
    #error "FINESWORD_TARGET_16BIT_FLOAT SHALL be defined as either 1 or 0"
#endif
#if !defined(FINESWORD_TARGET_128BIT_FLOAT) || (FINESWORD_TARGET_128BIT_FLOAT != 0 && FINESWORD_TARGET_128BIT_FLOAT != 1)
    #error "FINESWORD_TARGET_128BIT_FLOAT SHALL be defined as either 1 or 0"
#endif
#if !defined(FINESWORD_ASTYPE_USE_MEMCPY)   || (FINESWORD_ASTYPE_USE_MEMCPY != 0 && FINESWORD_ASTYPE_USE_MEMCPY != 1)
    #error   "FINESWORD_ASTYPE_USE_MEMCPY SHALL be defined as either 1 or 0"
#endif
#if !defined(FINESWORD_TESTS_USE_ANSI)      || (FINESWORD_TESTS_USE_ANSI != 0 && FINESWORD_TESTS_USE_ANSI != 1)
    #error       "FINESWORD_TESTS_USE_ANSI SHALL be defined as either 1 or 0"
#endif
#if !defined(FINESWORD_BINARY_FP_D1_SET_IS_QUIET) || (FINESWORD_BINARY_FP_D1_SET_IS_QUIET != 0 && FINESWORD_BINARY_FP_D1_SET_IS_QUIET != 1)
#error"FINESWORD_BINARY_FP_D1_SET_IS_QUIET SHALL be defined as either 1 or 0"
#endif

#endif /* FINESWORD_CONFIG_H */
