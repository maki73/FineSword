// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_RUDIMENTS_EXCEPTION_H
#define FINESWORD_RUDIMENTS_EXCEPTION_H

#include "finesword/rudiments/ieee754.h"
#include "finesword/rudiments/types.h"
#include "finesword/config.h"
#include "finesword/rudiments/template.h"

#pragma STDC FENV_ACCESS ON /* unnecessary, but sets the right tone */

/* Status Flag Control Helpers
 *
 * NOTE: we explicitly account only for
 * the 5 IEEE 754 exceptions:
 *  invalid operation
 *  division by zero
 *  overflow
 *  underflow
 *  inexact
 * [IEEE 754 \S\S 7.2-7.6]
 */

#define FORALL(x) FINESWORD_TEMPLATE_SPECIALIZE(x) /* don't forget to undef */

/* --- Invalid --- */

/* Sadly, we generally cannot use addition(x, -0.0)
 * for both signaling invalid operation exception and
 * quieting the result when isSignaling(x).
 * (e.g., roundTowardNegative addition(+0.0, -0.0) = -0.0, not +0.0)
 */

/* SHALL signal invalid operation exception iff x is sNaN */
#define FINESWORD_TEMPLATE_FE_INVALID_IFF_SNAN_FPU_FN_1(N) \
static inline void                                         \
fe_invalid_iff_snan_fpu_f##N (f##N x) {                    \
    f##N z = F##N##_C(0.0);                                \
    volatile f##N r = x + z; /* addition(x, 0.0) */        \
    (void)r;                                               \
}

FORALL(FINESWORD_TEMPLATE_FE_INVALID_IFF_SNAN_FPU_FN_1)
#undef FINESWORD_TEMPLATE_FE_INVALID_IFF_SNAN_FPU_FN_1


/* --- Quieter --- */

/* SHALL quiet the input iff it is a signaling NaN (not signaling exceptions)
 SHOULD preserve the payload */
#if FINESWORD_BINARY_FP_D1_SET_IS_QUIET
    /* - recommended binary IEEE 754 encoding branch
     * "a signaling NaN SHALL be quieted by setting d1 to 1,
     * leaving the remaining bits of T unchanged." [IEEE 754 \S 6.2.1]
     */

    #define FINESWORD_TEMPLATE_QUIET_IFF_SNAN_BITWISE_UN_1(N) \
    static inline u##N                                        \
    quiet_iff_snan_bitwise_u##N (u##N x) {                    \
        /* detect any NaN */                                  \
        bool is_nan = (x & F##N##_ABS_MASK) > F##N##_POS_INF; \
        u##N is_nan_bit = (u##N)is_nan; /* either 1 or 0 */   \
        u##N m = (u##N)(is_nan_bit << F##N##_D1_BIT_INDEX);   \
        /* no-op for qNaN */                                  \
        return x | m;                                         \
    }
#else
    /* - nonpreferred encoding branch (opposite polarity; d1=0->qNaN)
     * NOTE: payload preservation is under 'SHOULD' phrasing [IEEE 754 \S 6.2]
     */

    #define FINESWORD_TEMPLATE_QUIET_IFF_SNAN_BITWISE_UN_1(N) \
    static inline u##N                                        \
    quiet_iff_snan_bitwise_u##N (u##N x) {                    \
        u##N ax = x & F##N##_ABS_MASK;                        \
        bool is_snan = ax >= F##N##_MINPAY_POS_SNAN;          \
        bool is_pay0 = ax == F##N##_MINPAY_POS_SNAN;          \
        u##N is_snan_bit = (u##N)is_snan;                     \
        u##N is_pay0_bit = (u##N)is_pay0;                     \
        /* if signaling, toggle d1 (e.i., set it to 0) */     \
        u##N toggle = (u##N)(is_snan_bit << F##N##_D1_BIT_INDEX); \
        x ^= toggle;                                          \
        /* smallest change to make it NaN rather than Inf */  \
        return x | is_pay0_bit;                               \
    }
#endif

FORALL(FINESWORD_TEMPLATE_QUIET_IFF_SNAN_BITWISE_UN_1)
#undef FINESWORD_TEMPLATE_QUIET_IFF_SNAN_BITWISE_UN_1

#undef FORALL

#endif /* FINESWORD_RUDIMENTS_EXCEPTION_H */
