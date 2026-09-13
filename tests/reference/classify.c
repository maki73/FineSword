// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */


#include "finesword_tests/reference/classify.h"
#include "finesword/rudiments/astype.h"
#include "finesword/rudiments/ieee754.h"
#include "finesword/rudiments/types.h"
#include "finesword/config.h"
#include "finesword/rudiments/template.h"

#include <math.h>


#define FORALL(tmpl) FINESWORD_TEMPLATE_SPECIALIZE(tmpl)

/* HPPA's GCC is wrong in its issubnormal optimization */
#ifdef __hppa__
    #define v volatile 
#else
    #define v
#endif


/* --- Hand-Rolled References --- */

#define TEMPLATE_HELPER_REF_ISNAN_FN_1(N) \
static inline                             \
bool helper_ref_isnan_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);            \
    return                                \
    ((xi & F##N##_EXP_MASK)  == F##N##_EXP_MASK) \
    &&                                    \
    ((xi & F##N##_MANT_MASK) != U##N##_C(0)); \
}
FORALL(TEMPLATE_HELPER_REF_ISNAN_FN_1)

/* GCC's 16/128-bit __builtin_isfinite() is wrong
 * isinf() is wrong too
 */
#define TEMPLATE_HELPER_REF_ISFINITE_FN_1(N) \
static inline                                \
bool helper_ref_isfinite_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);               \
    xi &= F##N##_ABS_MASK;                   \
    return !(xi >= F##N##_POS_INF);          \
}
FORALL(TEMPLATE_HELPER_REF_ISFINITE_FN_1)

/* GCC's 16/128-bit __builtin_isnormal() is wrong
 */
#define TEMPLATE_HELPER_REF_ISNORMAL_FN_1(N) \
static inline                                \
bool helper_ref_isnormal_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);               \
    xi &= F##N##_ABS_MASK;                   \
    return                                   \
    (xi <= F##N##_MAX_NORMAL) && (xi > F##N##_MAX_SUBNORMAL); \
}
FORALL(TEMPLATE_HELPER_REF_ISNORMAL_FN_1)

/* - isZero() - */

/* fpclassify() with FP_ZERO is wrong (compiles to vucomiss shenanigans)
 */
#define TEMPLATE_W_REF_ISZERO_FN_1(N) \
f##N w_ref_iszero_f##N (f##N x) {     \
    u##N xi = asuint_f##N (x);        \
    xi &= F##N##_ABS_MASK;            \
    u##N z = U##N##_C(0x0);           \
    u##N o = U##N##_C(0x1);           \
    u##N r = (xi == z) ? o : z;       \
    return asfloat_u##N (r);          \
}
FORALL(TEMPLATE_W_REF_ISZERO_FN_1)


/* --- isSignMinus() --- */

f32 w_m_signbit_f32(f32 x) {
    u32 r = (u32)((bool)signbit((float)x));
    return asfloat_u32(r);
}

f64 w_m_signbit_f64(f64 x) {
    u64 r = (u64)((bool)signbit((double)x));
    return asfloat_u64(r);
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_m__builtin_signbit_f16(f16 x) {
        u16 r = (u16)((bool)__builtin_signbit(x));
        return asfloat_u16(r);
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_m__builtin_signbit_f128(f128 x) {
        u128 r = (u128)((bool)__builtin_signbit(x));
        return asfloat_u128(r);
    }
#endif


/* --- isNormal() --- */

f32 w_ref_isnormal_f32(f32 x) {
    return asfloat_u32((u32)helper_ref_isnormal_f32(x));
}

f64 w_ref_isnormal_f64(f64 x) {
    return asfloat_u64((u64)helper_ref_isnormal_f64(x));
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_isnormal_f16(f16 x) {
        return asfloat_u16((u16)helper_ref_isnormal_f16(x));
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_isnormal_f128(f128 x) {
        return asfloat_u128((u128)helper_ref_isnormal_f128(x));
    }
#endif


/* --- isSubnormal() --- */

f32 w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f32(f32 x) {
    // GCC's hppa optimizer's doing nonsense
    v bool a =  helper_ref_isfinite_f32(x);
    v bool b = !helper_ref_isnormal_f32(x);
    if (a && b) {
        v bool c = !(x == F32_C(0.0)); // does not execute if x's a NaN 
        if (c) {
            return asfloat_u32(U32_C(0x1));
        }
    }
    return asfloat_u32(U32_C(0x0));
}

f64 w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f64(f64 x) {
    v bool a =  helper_ref_isfinite_f64(x);
    v bool b = !helper_ref_isnormal_f64(x);
    if (a && b) {
        v bool c = !(x == F64_C(0.0));
        if (c) {
            return asfloat_u64(U64_C(0x1));
        }
    }
    return asfloat_u64(U64_C(0x0));
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f16(f16 x) {
        v bool a =  helper_ref_isfinite_f16(x);
        v bool b = !helper_ref_isnormal_f16(x);
        if (a && b) {
            v bool c = !(x == F16_C(0.0));
            if (c) {
                return asfloat_u16(U16_C(0x1));
            }
        }
        return asfloat_u16(U16_C(0x0));
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f128(f128 x) {
        v bool a =  helper_ref_isfinite_f128(x);
        v bool b = !helper_ref_isnormal_f128(x);
        if (a && b) {
            v bool c = !(x == F128_C(0.0)); // VERY expensive
            if (c) {
                return asfloat_u128(U128_C(0x1));
            }
        }
        return asfloat_u128(U128_C(0x0));
    }
#endif


/* --- isFinite() --- */

f32 w_ref_isfinite_f32(f32 x) {
    return asfloat_u32((u32)helper_ref_isfinite_f32(x));
}

f64 w_ref_isfinite_f64(f64 x) {
    return asfloat_u64((u64)helper_ref_isfinite_f64(x));
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_isfinite_f16(f16 x) {
        return asfloat_u16((u16)helper_ref_isfinite_f16(x));
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_isfinite_f128(f128 x) {
        return asfloat_u128((u128)helper_ref_isfinite_f128(x));
    }
#endif


/* --- isInfinite() --- */

f32 w_ref_not_finite_and_not_nan_f32(f32 x) {
    if (!helper_ref_isfinite_f32(x) && !helper_ref_isnan_f32(x)) {
        return asfloat_u32(U32_C(0x1));
    }
    return asfloat_u32(U32_C(0x0));
}

f64 w_ref_not_finite_and_not_nan_f64(f64 x) {
    if (!helper_ref_isfinite_f64(x) && !helper_ref_isnan_f64(x)) {
        return asfloat_u64(U64_C(0x1));
    }
    return asfloat_u64(U64_C(0x0));
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_not_finite_and_not_nan_f16(f16 x) {
        if (!helper_ref_isfinite_f16(x) && !helper_ref_isnan_f16(x)) {
            return asfloat_u16(U16_C(0x1));
        }
        return asfloat_u16(U16_C(0x0));
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_not_finite_and_not_nan_f128(f128 x) {
        if (!helper_ref_isfinite_f128(x) && !helper_ref_isnan_f128(x)) {
            return asfloat_u128(U128_C(0x1));
        }
        return asfloat_u128(U128_C(0x0));
    }
#endif


/* --- isNaN() --- */

f32 w_ref_isnan_f32(f32 x) {
    return asfloat_u32((u32)helper_ref_isnan_f32(x));
}

f64 w_ref_isnan_f64(f64 x) {
    return asfloat_u64((u64)helper_ref_isnan_f64(x));
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_isnan_f16(f16 x) {
        return asfloat_u16((u16)helper_ref_isnan_f16(x));
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_isnan_f128(f128 x) {
        return asfloat_u128((u128)helper_ref_isnan_f128(x));
    }
#endif


/* --- isSignaling() + isquiet_fN() --- */

#define PASTE_IF_N(N, cmp)          \
        u##N xi = asuint_f##N (x);  \
        xi &= F##N##_D1_BIT_MASK;   \
        xi >>= F##N##_D1_BIT_INDEX; \
        if (xi cmp (u##N)FINESWORD_BINARY_FP_D1_SET_IS_QUIET) { \
            return asfloat_u##N(   \
                U##N##_C(0x1));    \
        }                          \

f32 w_ref_issignaling_f32(f32 x) {
    if (helper_ref_isnan_f32(x)) {
        PASTE_IF_N(32, !=)
    }
    return asfloat_u32(U32_C(0x0));
}

f64 w_ref_issignaling_f64(f64 x) {
    if (helper_ref_isnan_f64(x)) {
        PASTE_IF_N(64, !=)
    }
    return asfloat_u64(U64_C(0x0));
}

f32 w_ref_isquiet_f32(f32 x) {
    if (helper_ref_isnan_f32(x)) {
        PASTE_IF_N(32, ==)
    }
    return asfloat_u32(U32_C(0x0));
}

f64 w_ref_isquiet_f64(f64 x) {
    if (helper_ref_isnan_f64(x)) {
        PASTE_IF_N(64, ==)
    }
    return asfloat_u64(U64_C(0x0));
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_issignaling_f16(f16 x) {
        if ((bool)helper_ref_isnan_f16(x)) {
            PASTE_IF_N(16, !=)
        }
        return asfloat_u16(U16_C(0x0));
    }
    f16 w_ref_isquiet_f16(f16 x) {
        if ((bool)helper_ref_isnan_f16(x)) {
            PASTE_IF_N(16, ==)
        }
        return asfloat_u16(U16_C(0x0));
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_issignaling_f128(f128 x) {
        if ((bool)helper_ref_isnan_f128(x)) {
            PASTE_IF_N(128, !=)
        }
        return asfloat_u128(U128_C(0x0));
    }
    f128 w_ref_isquiet_f128(f128 x) {
        if ((bool)helper_ref_isnan_f128(x)) {
            PASTE_IF_N(128, ==)
        }
        return asfloat_u128(U128_C(0x0));
    }
#endif
