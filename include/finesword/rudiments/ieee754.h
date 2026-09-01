// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_RUDIMENTS_IEEE754_H
#define FINESWORD_RUDIMENTS_IEEE754_H

#include "finesword/config.h"
#include "finesword/rudiments/types.h"

/* Macros for IEEE 754 Floating-Point Interchange Format(s)
 *
 * Format(s) supported:
 * - binary32 & binary64 (abbreviated with F32_* and F64_* prefixes respectively)
 * - binary16 & binary128  (optional formats, abbreviated analogously)
 *
 * Thanks: Big Thanks to Julia REPL for making work with the bit patterns (especially binary128)
 * remotely bearable. Never forget.
 */

// #ifndef __STDC_IEC_559__
//     #error "implementation must be IEEE Std 754/IEC 60559 compliant!"
// #endif

/* Justify thy words~!
 * 
 * Basic formats are:
 * - Binary32, Binary64, Binary128, Decimal64, Decimal128
 * emin SHALL be 1 - emax for all formats
 *
 * ==== BINARY FORMATS ==== 
 *
 * IEEE 754-2019 \S 3.4 Figure 3.1
 * - MSB -------------------------------------------------------------------------------- LSB
 * - [1-bit sign S][w-bit biased exponent E=e+bias][(t=p-1)-bit trailing significant field T]
 * -               E_0.......................E_w-1 d_1..................................d_p-1
 *
 * leading bit d_0 of the significant is implicit.
 *
 * biased exponent E:
 * - [1, 2^w - 2]       -> normal numbers
 * - reserved value 0   -> subnormals
 * - reserved value 2^w -> \pm Inf or NaNs 
 *
 * values:
 * a) NaNs: exponent is all 1s; T is non-zero. d_1 SHALL exclusively distinguish between qNaN and sNaN (see below) 
 * b) \pm Inf: exponent is all 1s; T=0. v=(-1)^S \times   (+\infty)
 * c) normals: 1 <= E <= 2^w - 2.        r=(S, (E-bias),  (1+2^{1-p}\times T)) v=(-1)^S\times 2^{E-bias} \times (1 + 2^{1-p} \times T)
 * d) subnormals: E=0 and T is non-zero. r=(S, emin,      (0+2^{1-p}\times T)) v=(-1)^S\times 2^{emin}   \times (0 + 2^{1-p} \times T)
 * e) \pm 0: E=0 T=0. r=(S, emin, 0) v=(-1)^S \times (+0) (signed zeros)
 *
 * -- NaNs:
 * Encoding:
 * - IEEE 754's preferred encoding (under "should" phrasing)
 *   states that **qNaN SCHOULD be encoded with d1 set to 1**.
 * - A **sNaN SCHOULD be encoded with d1 set to 0**.
 * - If d1 is 0, then some other bit MUST be non-zero to distinguish
 *   NaN from Inf.
 * - NOTE: since it's under SHOULD rather than SHALL phrasing,
 *   we SHALL NOT depend on the recommended encoding.
 * - In the preferred encoding just described, a sNaN SHALL be quieted by
 *   setting d1 to 1, leaving remaining bits of T unchanged.
 * - IEEE 754 does NOT interpret the sign of a NaN.
 * Propagation:
 * - NaN payload SHOULD be preserved (if representable)
 * - if two or more inputs are NaN, then the result's payload SHOULD be
 *   the payload of one of the input NaNs (if representable). Which one is unspecified.
 * - Conversion of a NaN to a fp format of the same or different radix that does not
 *   allow the payload to be preserved SHALL return a qNaN that SHOULD contain some
 *   diagnostic information.
 * - Except when specified otherwise, a NaN result SHALL be canonical.
 * - NOTE: In binary interchange formats, all number and NaN encodings are canonical.
 *
 *
 * NOTE: this project uses 'mantissa' and 'trailing significant' interchangbly
 */


/* --- Binary32 --- */

/* -- Layout -- */

// NOTE: Be aware that int's reliable range is [-32767, +32767] [C99 \S 5.2.4.2.1] 
#define F32_RADIX_B          (2) /* binary by definition */
#define F32_TYPE_WIDTH_K   (32)
#define F32_PRECISION_P     (24) // p = t+1
#define F32_EMAX           (127) // 2^{w - 1} - 1
#define F32_EMIN          (-126) // emin = 1 - emax
#define F32_BIAS           (127) // bias = emax
#define F32_SIGN_WIDTH       (1) // 1
#define F32_EXP_WIDTH_W      (8) // w = k - p
#define F32_MANT_WIDTH_T    (23) // t = k - (w+1)
#define F32_PMIN             (9) // pmin = 1 + ceil(p * log10(2))
/* LSB=index 0; MSB=N-1 */
#define F32_SIGN_BIT_INDEX  (31) // sign_bit_index = w+t = k-1
#define F32_D1_BIT_INDEX    (22) // d1_bit_index = t - 1 

/* -- Integer Boundaries -- */

// integral boundaries of unsigned types instead of signed
// pros:
// makes everything far more homogeneous;
// still useful since positive finite patterns are naturally ordered

// 0 10010110 00000000000000000000000
// All floating-point bit patterns >= are integral
#define F32_NO_FRAC_FROM U32_C(0x4B000000) // bit pattern for value of 2^t
// 0 10010111 00000000000000000000000
// All floating-point bit patterns >= are even and not all integers are representable   
#define F32_IS_EVEN_FROM U32_C(0x4B800000) // bit pattern for value of 2^p

/* -- Masks -- */

// 0 00000000 10000000000000000000000
#define F32_D1_BIT_MASK U32_C(0x00400000) // 0x1 << (t-1)
// 1 00000000 00000000000000000000000
#define F32_SIGN_MASK   U32_C(0x80000000) // 0x1 << (t+w)
// 0 11111111 00000000000000000000000
#define F32_EXP_MASK    U32_C(0x7F800000) // ((emax << 1) | 0x1) << t
// 0 00000000 11111111111111111111111
#define F32_MANT_MASK   U32_C(0x007FFFFF) // 2^t - 1
// 0 11111111 11111111111111111111111
#define F32_ABS_MASK    U32_C(0x7FFFFFFF) // ~sign_mask

/* -- Patterns -- */

// due to undetermined polarity
// 0x7FC00000 -- 0 11111111 10000000000000000000000 -- exp_mask | d1_bit_mask
// 0x7F800001 -- 0 11111111 00000000000000000000001 -- exp_mask | 0x1
#if FINESWORD_BINARY_FP_D1_SET_IS_QUIET
    #define F32_MINPAY_POS_QNAN U32_C(0x7FC00000)
    #define F32_MINPAY_POS_SNAN U32_C(0x7F800001)
#else
    #define F32_MINPAY_POS_QNAN U32_C(0x7F800001) // NOTE: HPPA/PA-RISC commonly uses 0x7FA00000
    #define F32_MINPAY_POS_SNAN U32_C(0x7FC00000)
#endif
// /* https://software.intel.com/en-us/articles/floating-point-reference-sheet-for-intel-architecture */
// #define F32_INTEL_R_IND       U32_C(0xFFC00000)

// 0 11111111 00000000000000000000000
#define F32_POS_INF           U32_C(0x7F800000) // exp_mask
// 1 11111111 00000000000000000000000
#define F32_NEG_INF           U32_C(0xFF800000) // exp_mask | sign_mask
// 0 11111110 11111111111111111111111
#define F32_MAX_NORMAL        U32_C(0x7F7FFFFF) // exp_mask - 1
// 0 00000001 00000000000000000000000
#define F32_POS_MIN_NORMAL    U32_C(0x00800000) // 0x1 << t
// 0 00000000 11111111111111111111111
#define F32_MAX_SUBNORMAL     U32_C(0x007FFFFF) // mant_mask
// 0 00000000 00000000000000000000001
#define F32_POS_MIN_SUBNORMAL U32_C(0x00000001) // 0x1


/* --- Binary64 --- */

/* -- Layout -- */

#define F64_RADIX_B          (2)
#define F64_TYPE_WIDTH_K   (64)
#define F64_PRECISION_P     (53)
#define F64_EMAX          (1023)
#define F64_EMIN         (-1022)
#define F64_BIAS          (1023)
#define F64_SIGN_WIDTH       (1)
#define F64_EXP_WIDTH_W     (11)
#define F64_MANT_WIDTH_T    (52)
#define F64_PMIN            (17)
#define F64_SIGN_BIT_INDEX  (63)
#define F64_D1_BIT_INDEX    (51)

/* -- Integer Boundaries -- */

// 0 10000110011 0000000000000000000000000000000000000000000000000000
#define F64_NO_FRAC_FROM U64_C(0x4330000000000000)
// 0 10000110100 0000000000000000000000000000000000000000000000000000
#define F64_IS_EVEN_FROM U64_C(0x4340000000000000)

/* -- Masks -- */

// 0 00000000000 1000000000000000000000000000000000000000000000000000
#define F64_D1_BIT_MASK U64_C(0x0008000000000000)
// 1 00000000000 0000000000000000000000000000000000000000000000000000
#define F64_SIGN_MASK   U64_C(0x8000000000000000)
// 0 11111111111 0000000000000000000000000000000000000000000000000000
#define F64_EXP_MASK    U64_C(0x7FF0000000000000)
// 0 00000000000 1111111111111111111111111111111111111111111111111111
#define F64_MANT_MASK   U64_C(0x000FFFFFFFFFFFFF)
// 0 11111111111 1111111111111111111111111111111111111111111111111111
#define F64_ABS_MASK    U64_C(0x7FFFFFFFFFFFFFFF)

/* -- Patterns -- */

// 0x7FF8000000000000 -- 0 11111111111 1000000000000000000000000000000000000000000000000000
// 0x7FF0000000000001 -- 0 11111111111 0000000000000000000000000000000000000000000000000001
#if FINESWORD_BINARY_FP_D1_SET_IS_QUIET
    #define F64_MINPAY_POS_QNAN U64_C(0x7FF8000000000000)
    #define F64_MINPAY_POS_SNAN U64_C(0x7FF0000000000001)
#else
    #define F64_MINPAY_POS_QNAN U64_C(0x7FF0000000000001)
    #define F64_MINPAY_POS_SNAN U64_C(0x7FF8000000000000)
#endif

// 0 11111111111 0000000000000000000000000000000000000000000000000000
#define F64_POS_INF           U64_C(0x7FF0000000000000)
// 1 11111111111 0000000000000000000000000000000000000000000000000000
#define F64_NEG_INF           U64_C(0xFFF0000000000000)
// 0 11111111110 1111111111111111111111111111111111111111111111111111
#define F64_MAX_NORMAL        U64_C(0x7FEFFFFFFFFFFFFF)
// 0 00000000001 0000000000000000000000000000000000000000000000000000
#define F64_POS_MIN_NORMAL    U64_C(0x0010000000000000)
// 0 00000000000 1111111111111111111111111111111111111111111111111111
#define F64_MAX_SUBNORMAL     U64_C(0x000FFFFFFFFFFFFF)
// 0 00000000000 0000000000000000000000000000000000000000000000000001
#define F64_POS_MIN_SUBNORMAL U64_C(0x0000000000000001)


// optional
#if FINESWORD_TARGET_16BIT_FLOAT
    /* --- Binary16 --- */

    /* -- Layout -- */

    #define F16_RADIX_B          (2)
    #define F16_TYPE_WIDTH_K   (16)
    #define F16_PRECISION_P     (11)
    #define F16_EMAX            (15)
    #define F16_EMIN           (-14)
    #define F16_BIAS            (15)
    #define F16_SIGN_WIDTH       (1)
    #define F16_EXP_WIDTH_W      (5)
    #define F16_MANT_WIDTH_T    (10)
    #define F16_PMIN             (5)
    #define F16_SIGN_BIT_INDEX  (15)
    #define F16_D1_BIT_INDEX     (9)

    /* -- Integer Boundaries -- */

    // 0 11001 0000000000
    #define F16_NO_FRAC_FROM U16_C(0x6400)
    // 0 11010 0000000000
    #define F16_IS_EVEN_FROM U16_C(0x6800)

    /* -- Masks -- */

    // 0 00000 1000000000
    #define F16_D1_BIT_MASK U16_C(0x0200)
    // 1 00000 0000000000
    #define F16_SIGN_MASK   U16_C(0x8000)
    // 0 11111 0000000000
    #define F16_EXP_MASK    U16_C(0x7C00)
    // 0 00000 1111111111
    #define F16_MANT_MASK   U16_C(0x03FF)
    // 0 11111 1111111111
    #define F16_ABS_MASK    U16_C(0x7FFF)

    /* -- Patterns -- */

    // 0x7E00 -- 0 11111 1000000000
    // 0x7C01 -- 0 11111 0000000001
    #if FINESWORD_BINARY_FP_D1_SET_IS_QUIET
        #define F16_MINPAY_POS_QNAN U16_C(0x7E00)
        #define F16_MINPAY_POS_SNAN U16_C(0x7C01)
    #else
        #define F16_MINPAY_POS_QNAN U16_C(0x7C01)
        #define F16_MINPAY_POS_SNAN U16_C(0x7E00)
    #endif

    // 0 11111 0000000000
    #define F16_POS_INF           U16_C(0x7C00)
    // 1 11111 0000000000
    #define F16_NEG_INF           U16_C(0xFC00)
    // 0 11110 1111111111
    #define F16_MAX_NORMAL        U16_C(0x7BFF)
    // 0 00001 0000000000
    #define F16_POS_MIN_NORMAL    U16_C(0x0400)
    // 0 00000 1111111111
    #define F16_MAX_SUBNORMAL     U16_C(0x03FF)
    // 0 00000 0000000001
    #define F16_POS_MIN_SUBNORMAL U16_C(0x0001)
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    /* --- Binary128 --- */

    /* -- Layout -- */

    // 16383 < 32767
    #define F128_RADIX_B          (2)
    #define F128_TYPE_WIDTH_K  (128)
    #define F128_PRECISION_P    (113)
    #define F128_EMAX         (16383)
    #define F128_EMIN        (-16382)
    #define F128_BIAS         (16383)
    #define F128_SIGN_WIDTH       (1)
    #define F128_EXP_WIDTH_W     (15)
    #define F128_MANT_WIDTH_T   (112)
    #define F128_PMIN            (36)
    #define F128_SIGN_BIT_INDEX (127)
    #define F128_D1_BIT_INDEX   (111)

    /* -- Integer Boundaries -- */

    // 0x406F 0000000000000000000000000000
    #define F128_NO_FRAC_FROM \
        FINESWORD_U128_C_HI_LO(U64_C(0x406F000000000000), U64_C(0x0000000000000000))
    // 0x4070 0000000000000000000000000000
    #define F128_IS_EVEN_FROM \
        FINESWORD_U128_C_HI_LO(U64_C(0x4070000000000000), U64_C(0x0000000000000000))

    /* -- Masks -- */

    // 0x0000 8000000000000000000000000000
    #define F128_D1_BIT_MASK \
        FINESWORD_U128_C_HI_LO(U64_C(0x0000800000000000), U64_C(0x0000000000000000))
    // 0x8000 0000000000000000000000000000
    #define F128_SIGN_MASK \
        FINESWORD_U128_C_HI_LO(U64_C(0x8000000000000000), U64_C(0x0000000000000000))
    // 0x7FFF 0000000000000000000000000000
    #define F128_EXP_MASK \
        FINESWORD_U128_C_HI_LO(U64_C(0x7FFF000000000000), U64_C(0x0000000000000000))
    // 0x0000 FFFFFFFFFFFFFFFFFFFFFFFFFFFF
    #define F128_MANT_MASK \
        FINESWORD_U128_C_HI_LO(U64_C(0x0000FFFFFFFFFFFF), U64_C(0xFFFFFFFFFFFFFFFF))
    // 0x7FFF FFFFFFFFFFFFFFFFFFFFFFFFFFFF
    #define F128_ABS_MASK \
        FINESWORD_U128_C_HI_LO(U64_C(0x7FFFFFFFFFFFFFFF), U64_C(0xFFFFFFFFFFFFFFFF))

    /* -- Patterns -- */

    // 0x7FFF 8000000000000000000000000000
    // 0x7FFF 0000000000000000000000000001
    #if FINESWORD_BINARY_FP_D1_SET_IS_QUIET
        #define F128_MINPAY_POS_QNAN  \
            FINESWORD_U128_C_HI_LO(U64_C(0x7FFF800000000000), U64_C(0x0000000000000000))
        #define F128_MINPAY_POS_SNAN  \
            FINESWORD_U128_C_HI_LO(U64_C(0x7FFF000000000000), U64_C(0x0000000000000001))
    #else
        #define F128_MINPAY_POS_QNAN  \
            FINESWORD_U128_C_HI_LO(U64_C(0x7FFF000000000000), U64_C(0x0000000000000001))
        #define F128_MINPAY_POS_SNAN  \
            FINESWORD_U128_C_HI_LO(U64_C(0x7FFF800000000000), U64_C(0x0000000000000000))
    #endif

    // 0x7FFF 0000000000000000000000000000
    #define F128_POS_INF \
        FINESWORD_U128_C_HI_LO(U64_C(0x7FFF000000000000), U64_C(0x0000000000000000))
    // 0xFFFF 0000000000000000000000000000
    #define F128_NEG_INF \
        FINESWORD_U128_C_HI_LO(U64_C(0xFFFF000000000000), U64_C(0x0000000000000000))
    // 0x7FFE FFFFFFFFFFFFFFFFFFFFFFFFFFFF
    #define F128_MAX_NORMAL  \
        FINESWORD_U128_C_HI_LO(U64_C(0x7FFEFFFFFFFFFFFF), U64_C(0xFFFFFFFFFFFFFFFF))
    // 0x0001 0000000000000000000000000000
    #define F128_POS_MIN_NORMAL \
        FINESWORD_U128_C_HI_LO(U64_C(0x0001000000000000), U64_C(0x0000000000000000))
    // 0x0000 FFFFFFFFFFFFFFFFFFFFFFFFFFFF
    #define F128_MAX_SUBNORMAL \
        FINESWORD_U128_C_HI_LO(U64_C(0x0000FFFFFFFFFFFF), U64_C(0xFFFFFFFFFFFFFFFF))
    // 0x0000 0000000000000000000000000001
    #define F128_POS_MIN_SUBNORMAL  \
        FINESWORD_U128_C_HI_LO(U64_C(0x0000000000000000), U64_C(0x0000000000000001))
#endif
 
#endif /* FINESWORD_RUDIMENTS_IEEE754_H */

