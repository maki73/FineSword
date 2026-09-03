// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_RUDIMENTS_TYPES_H
#define FINESWORD_RUDIMENTS_TYPES_H

#include "finesword/config.h"
#include "finesword/rudiments/preprocessor.h"


/* Custom Types Header
 *
 * -/ similar to <stdint.h> [C99 \S 7.18]
 *
 * NOTE: UN_C(), IN_C(C), and FN_C() macros correspond to
 * uN, iN, and fN exact-width types respectively, unlike C
 * macros [C99 \S 7.18.4.1] which correspond to int/uint_leastN_t
 * types. Additionally C99 does not define FLOATN_C()-style macros,
 * but we do.
 *
 * Assumptions:
 * - 1 byte = 8 bits
 * - float/double are Binary formats, not Decimal
 */

// #ifndef __STDC_IEC_559__
//     #error "implementation must be IEEE Std 754/IEC 60559 compliant!"
// #endif


/* --- Floating-Point Types --- */

FINESWORD_STATIC_ASSERT(sizeof(float) == 4,   float_is_assumed_to_be_4_bytes);
typedef float  f32; /* Binary32 by contract */
#define F32_C(c) c ## f
FINESWORD_STATIC_ASSERT(sizeof(double) == 8, double_is_assumed_to_be_8_bytes);
typedef double f64; /* Binary64 by contract */
#define F64_C(c) c

// optional
#if FINESWORD_TARGET_16BIT_FLOAT
    /* == WARNING: This relies on C24/C23 optional types == */

    #pragma GCC diagnostic ignored "-Wpedantic"   /* GCC correctly considers it a C99 violation */
    FINESWORD_STATIC_ASSERT(sizeof(_Float16) == 2, _Float16_is_assumed_to_be_2_bytes);
    typedef _Float16 f16; /* Binary16 by contract */
    #define F16_C(c) c ## F16 /* see [C24 Annex H.5.2] */
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    /* == WARNING: This relies on GNU compiler extension == */

    #pragma GCC diagnostic ignored "-Wpedantic"
    /* See: https://gcc.gnu.org/onlinedocs/gcc/Floating-Types.html */
    #ifdef __s390x__
        /* GCC on s390x doesn't expose __float128 alias
         -> use the C23/C24 _Float128 type directly */
        FINESWORD_STATIC_ASSERT(sizeof(_Float128) == 16,   _Float128_is_assumed_to_be_16_bytes);
        typedef _Float128 f128;  /* Binary128 by contract */
        #define F128_C(c) c ## F128
    #else
        FINESWORD_STATIC_ASSERT(sizeof(__float128) == 16, __float128_is_assumed_to_be_16_bytes);
        typedef __float128 f128; /* Binary128 by contract */
        #define F128_C(c) c ## Q
    #endif /* __s390x__ */
#endif


/* --- Integer Types --- */

typedef signed   char         i8; // [C99 \S 6.5.3.4 \P 3] 1 byte by definition
#define I8_C(c)   c
typedef unsigned char         u8;
#define U8_C(c)   c ## U

FINESWORD_STATIC_ASSERT(sizeof(signed   short) == 2,   signed_short_is_assumed_to_be_2_bytes);
typedef signed   short       i16;
#define I16_C(c)  c
FINESWORD_STATIC_ASSERT(sizeof(unsigned short) == 2, unsigned_short_is_assumed_to_be_2_bytes);
typedef unsigned short       u16;
#define U16_C(c)  c ## U

FINESWORD_STATIC_ASSERT(sizeof(signed   int) == 4,   signed_int_is_assumed_to_be_4_bytes);
#define I32_C(c)  c
typedef signed   int         i32;
FINESWORD_STATIC_ASSERT(sizeof(unsigned int) == 4, unsigned_int_is_assumed_to_be_4_bytes);
typedef unsigned int         u32;
#define U32_C(c)  c ## U

FINESWORD_STATIC_ASSERT(sizeof(signed   long long) == 8,   signed_long_long_is_assumed_to_be_8_bytes);
typedef signed   long long   i64;
#define I64_C(c)  c ## LL
FINESWORD_STATIC_ASSERT(sizeof(unsigned long long) == 8, unsigned_long_long_is_assumed_to_be_8_bytes);
typedef unsigned long long   u64;
#define U64_C(c)  c ## ULL

// optional (u16 and i16 already defined)
#if FINESWORD_TARGET_128BIT_FLOAT
    /* == WARNING: This relies on C24/C23 optional types and/or compiler extensions == */

    // -Wpedantic on GCC already ignored earlier
    #if defined(__GNUC__) && !defined(__BITINT_MAXWIDTH__) // only GCC 14+ supports _BitInt() [https://gcc.gnu.org/gcc-14/changes.html]
        typedef unsigned __int128 u128; 
    #else
        typedef unsigned _BitInt(128) u128;
    #endif
    // typedef unsigned __int128 u128;

    /* hi and lo SHALL be of an unsigned type with width <=64-bits */
    #define FINESWORD_U128_C_HI_LO(hi, lo) (((u128)(hi) << 64U) | (u128)(lo))
    // signed type and U128_C() are currently unsupported
    // (no need for signed and internal hi-lo method does the job well)
    /* == WARNING: FAKE for compatibility; use hi_lo for real 128-bit types == */
    #define U128_C(c) ((u128)c)
#endif


/* --- Boolean Types --- */

/* these types collide with the existing C
 * defined names and thus they will be guarded by
 * __bool_true_false_are_defined.
 * NOTE: this might not work cleanly with newer standards
 * since C24/C23 defines bool,true,false as *keywords*. 
 */
#ifndef __bool_true_false_are_defined
    /* wouldn't be exactly correct per C *if* we were <stdbool.h>
     because these are unsuitable for use in #if preprocessing directive(s)
     Nevertheless, defining them as simply '1' and '0'
     would conflict with the Rule 10.3 and [C99 \S 7.16 \P 4]
     even allows to undefine/redefine bool, true, and false macros.
    */
    #define bool _Bool
    #define true  ((_Bool)1)
    #define false ((_Bool)0)
    #define __bool_true_false_are_defined 1
#endif


/* -- Runtime Support Query --- */

bool finesword_supports_decimal(void);
bool finesword_supports_binary (void);

bool finesword_supports_16bit_float (void);
bool finesword_supports_32bit_float (void); // 32 and 64 are usually always true
bool finesword_supports_64bit_float (void); // everything else depends on config.h
bool finesword_supports_128bit_float(void);

#endif /* FINESWORD_RUDIMENTS_TYPES_H */
