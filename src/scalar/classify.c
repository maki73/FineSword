// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword/scalar/classify.h"
#include "finesword/rudiments/template.h"
#include "finesword/rudiments/astype.h"
#include "finesword/rudiments/ieee754.h"


#define X(tmpl) FINESWORD_TEMPLATE_SPECIALIZE(tmpl)

/* --- isZero() --- */

#define TEMPLATE_ISZERO_FN_1(N) \
bool iszero_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);  \
    xi <<= 1U;                  \
    return                      \
    xi == U##N##_C(0x0);        \
}

X(TEMPLATE_ISZERO_FN_1)

/* --- isSignMinus() --- */

#define TEMPLATE_ISSIGNMINUS_FN_1(N) \
bool issignminus_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);       \
    return (bool)                    \
    (xi >> F##N##_SIGN_BIT_INDEX);   \
}

X(TEMPLATE_ISSIGNMINUS_FN_1)

/* --- isNormal() --- */

#define TEMPLATE_ISNORMAL_FN_1(N) \
bool isnormal_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);    \
    xi &= F##N##_ABS_MASK;        \
    xi -= F##N##_POS_MIN_NORMAL;  \
    return /* N=32: 0 11111110 00000000000000000000000 */ \
    (u##N)xi < (F##N##_EXP_MASK ^ F##N##_POS_MIN_NORMAL); \
}

X(TEMPLATE_ISNORMAL_FN_1)

/* --- isSubnormal() --- */

#define TEMPLATE_ISSUBNORMAL_FN_1(N) \
bool issubnormal_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);       \
    xi &= F##N##_ABS_MASK;           \
    xi -= U##N##_C(1); /* zero's not denormal */ \
    return                           \
    (u##N)xi < F##N##_MAX_SUBNORMAL; \
}

X(TEMPLATE_ISSUBNORMAL_FN_1)

/* --- isFinite() --- */

#define TEMPLATE_ISFINITE_FN_1(N) \
bool isfinite_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);    \
    xi &= F##N##_EXP_MASK;        \
    return                        \
    (u##N)xi != F##N##_EXP_MASK;  \
}

X(TEMPLATE_ISFINITE_FN_1)

/* --- isInfinite() --- */

#define TEMPLATE_ISINFINITE_FN_1(N) \
bool isinfinite_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);      \
    xi &= F##N##_ABS_MASK;          \
    return                          \
    (u##N)xi == F##N##_POS_INF;     \
}

X(TEMPLATE_ISINFINITE_FN_1)

/* --- isNaN() --- */

#define TEMPLATE_ISNAN_FN_1(N) \
bool isnan_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x); \
    xi &= F##N##_ABS_MASK;     \
    return                     \
    (u##N)xi > F##N##_POS_INF; \
}

X(TEMPLATE_ISNAN_FN_1)

/* --- isSignaling() + isquiet_fN() --- */

/* NOTE: isquiet_fN() is not a part of IEEE 754 (see the header file). 
 */


#define TEMPLATE_DETECT_D1_ZERO_NAN_FN_1(N) \
static inline bool detect_d1_zero_nan_f##N (f##N x) { \
    u##N xi = asuint_f##N (x);              \
    xi &= F##N##_ABS_MASK;                  \
    xi -= F##N##_EXP_MASK | U##N##_C(0x1);  \
    return /* N=32: 0 00000000 01111111111111111111111*/ \
    (u##N)xi < (F##N##_MANT_MASK^F##N##_D1_BIT_MASK); \
}

#define TEMPLATE_DETECT_D1_ONE_NAN_FN_1(N) \
static inline bool detect_d1_one_nan_f##N (f##N x) { \
    u##N xi = asuint_f##N (x);             \
    u##N d1_set_nan = F##N##_EXP_MASK | F##N##_D1_BIT_MASK; \
    xi &= d1_set_nan;                      \
    return                                 \
    (u##N)xi == d1_set_nan;                \
}

X(TEMPLATE_DETECT_D1_ZERO_NAN_FN_1)
X(TEMPLATE_DETECT_D1_ONE_NAN_FN_1)

#if FINESWORD_BINARY_FP_D1_SET_IS_QUIET
    /* - recommended binary IEEE 754 encoding branch
     */

    #define TEMPLATE_ISSIGNALING_FN_1(N)    \
    bool issignaling_f##N (f##N x) {        \
        return detect_d1_zero_nan_f##N (x); \
    }

    #define TEMPLATE_ISQUIET_FN_1(N)       \
    bool isquiet_f##N (f##N x) {           \
        return detect_d1_one_nan_f##N (x); \
    }
#else
    /* - nonpreferred encoding branch (opposite polarity; d1=0->qNaN)
    */

    #define TEMPLATE_ISSIGNALING_FN_1(N)   \
    bool issignaling_f##N (f##N x) {       \
        return detect_d1_one_nan_f##N (x); \
    }

    #define TEMPLATE_ISQUIET_FN_1(N)        \
    bool isquiet_f##N (f##N x) {            \
        return detect_d1_zero_nan_f##N (x); \
    }
#endif

X(TEMPLATE_ISSIGNALING_FN_1)
X(TEMPLATE_ISQUIET_FN_1)
