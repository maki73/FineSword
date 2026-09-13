// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_LOVECRAFT_H
#define FINESWORD_LOVECRAFT_H

#include "finesword/rudiments/preprocessor.h"
#include "finesword/rudiments/ieee754.h"
#include "finesword/rudiments/types.h"
#include "finesword/rudiments/astype.h"
#include "finesword/config.h"


/* Eldritch Metaprogramming Horrors
*/

/* sign SHALL be either '+' or '-'
 * 
 * payload SHALL be 0 <= payload <= 2^{T-1} - 1 where T := FN_MANT_WIDTH_T
 * and also != 0 iff it would otherwise produce an Inf
 * 
 * Thou SHALT NOT do
 * fN aNaN = Q/SNANN(sign, payload) + 1;
 * or
 * fN aNaN1 = Q/SNANN(sign1, payload1) fN aNaN2 = Q/SNANN(sign2, payload2)
 */

/* NOT NOT's quiet; don't cast to uN */
#define FINESWORD_STATIC_ASSERT_QSNANN_2(N, payload, is_quiet_operator) \
    FINESWORD_STATIC_ASSERT(                                            \
         ((payload) >= 0)                                               \
        &&                                                              \
         ((payload) <= (u##N)((U##N##_C(0x1) << (u##N)F##N##_D1_BIT_INDEX) - U##N##_C(1))) \
        &&                                                              \
        (((payload) != U##N##_C(0x0)) || (is_quiet_operator FINESWORD_BINARY_FP_D1_SET_IS_QUIET)) \
    , check_qsnan##N##_payload_valid)


#define FINESWORD_TEMPLATE_QSNANN_1(N, sign, payload, is_quiet_operator) \
    asfloat_u##N ((u##N)(                                                \
        /* sign: - \mapsto 0x80000000; + \mapsto 0x00000000 */           \
        (((u##N)0x3 << (u##N)((u##N)F##N##_SIGN_BIT_INDEX-U##N##_C(1)))  \
        sign                                                             \
         ((u##N)0x1 << (u##N)((u##N)F##N##_SIGN_BIT_INDEX-U##N##_C(1)))) \
        | /* exponent and d1 */                                          \
        ((u##N)(is_quiet_operator FINESWORD_BINARY_FP_D1_SET_IS_QUIET)   \
        << (u##N)F##N##_D1_BIT_INDEX) | F##N##_EXP_MASK                  \
        | /* mantissa */                                                 \
        (u##N)(payload)                                                  \
    ));                                                                  \
    { FINESWORD_STATIC_ASSERT_QSNANN_2(N, payload, is_quiet_operator); }


#define QNAN32(sign, payload) FINESWORD_TEMPLATE_QSNANN_1(32, sign, payload, !!)
#define SNAN32(sign, payload) FINESWORD_TEMPLATE_QSNANN_1(32, sign, payload, ! )

#define QNAN64(sign, payload) FINESWORD_TEMPLATE_QSNANN_1(64, sign, payload, !!)
#define SNAN64(sign, payload) FINESWORD_TEMPLATE_QSNANN_1(64, sign, payload, ! )

#if FINESWORD_TARGET_16BIT_FLOAT
    #define QNAN16(sign, payload) FINESWORD_TEMPLATE_QSNANN_1(16, sign, payload, !!)
    #define SNAN16(sign, payload) FINESWORD_TEMPLATE_QSNANN_1(16, sign, payload, ! )
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    #define QNAN128(sign, payload) FINESWORD_TEMPLATE_QSNANN_1(128, sign, payload, !!)
    #define SNAN128(sign, payload) FINESWORD_TEMPLATE_QSNANN_1(128, sign, payload, ! )
#endif

#endif /* FINESWORD_LOVECRAFT_H */
