// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/reference/class_broken.h"
#include "finesword/rudiments/types.h"
#include "finesword/rudiments/astype.h"
#include "finesword/rudiments/ieee754.h"

#include <fenv.h>
#pragma STDC FENV_ACCESS ON /* required */

// NOTE: VERY, and I mean VERY, NOT DRY
// I really don't want to use templates for references/broken functions
// (plus you kind of get to see what is the other way than macro metaprogramming)  

/* see: doc/ci.txt note #1
 */
#if defined(__clang__) && (__clang_major__ < 22)
    #define VOLATILEX volatile
#else
    #define VOLATILEX
#endif


/* --- references --- */

f32 copysign_reference_f32(f32 x, f32 y) {
    u32 xi = asuint_f32(x);
    u32 yi = asuint_f32(y);
    xi &= F32_ABS_MASK;
    yi &= F32_SIGN_MASK;
    xi |= yi;
    return asfloat_u32(xi);
}


f64 abs_reference_f64(f64 x) {
    u64 xi = asuint_f64(x);
    xi &= F64_ABS_MASK;
    return asfloat_u64(xi);
}

f64 copysign_reference_f64(f64 x, f64 y) {
    u64 xi = asuint_f64(x);
    u64 yi = asuint_f64(y);
    xi &= F64_ABS_MASK;
    yi &= F64_SIGN_MASK;
    xi |= yi;
    return asfloat_u64(xi);
}

// optional
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 abs_reference_f128(f128 x) {
        u128 xi = asuint_f128(x);
        xi &= F128_ABS_MASK;
        return asfloat_u128(xi);
    }

    f128 copysign_reference_f128(f128 x, f128 y) {
        u128 xi = asuint_f128(x);
        u128 yi = asuint_f128(y);
        xi &= F128_ABS_MASK;
        yi &= F128_SIGN_MASK;
        xi |= yi;
        return asfloat_u128(xi);
    }
#endif


/* --- broken --- */

/* -- unary -- */

f64 abs_spurious_inexact_forall_nans_f64(f64 x) {
    VOLATILEX u64 xi = asuint_f64(x);
    xi &= F64_ABS_MASK;
    if (xi > F64_POS_INF) {
        (void) feraiseexcept(FE_INEXACT);
    }
    return abs_reference_f64(x);
}

f64 abs_return_negative_forall_subnormals_f64(f64 x) {
    VOLATILEX u64 xi = asuint_f64(x);
    xi &= F64_ABS_MASK;
    if ((xi <= F64_MAX_SUBNORMAL) && (xi >= F64_POS_MIN_SUBNORMAL)) { // .. != 0
        return asfloat_u64(xi | F64_SIGN_MASK);
    }
    return abs_reference_f64(x);
}

f64 abs_return_negative_forall_infs_under_towardzero_f64(f64 x) {
    VOLATILEX u64 xi = asuint_f64(x);
    xi &= F64_ABS_MASK;
    if (xi == F64_POS_INF) {
        if (fegetround() == FE_TOWARDZERO) {
            return asfloat_u64(xi | F64_SIGN_MASK);
        }
    }
    return abs_reference_f64(x);
}

f64 abs_return_and_signal_nonsense_for_pos_zero_f64(f64 x) {
    VOLATILEX u64 xi = asuint_f64(x);
    if (xi == U64_C(0x0)) {
        (void) feraiseexcept(FE_DIVBYZERO);
        return asfloat_u64(xi | F64_SIGN_MASK);
    }
    return abs_reference_f64(x);
}

f64 abs_return_and_signal_nonsense_for_neg_nan_maxpay_f64(f64 x) {
    VOLATILEX u64 xi = asuint_f64(x);
    if (xi == (F64_SIGN_MASK | F64_EXP_MASK | F64_MANT_MASK)) {
        (void) feraiseexcept(FE_DIVBYZERO);
        return F64_C(77.7);
    }
    return abs_reference_f64(x);
}

// optional
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 abs_spurious_inexact_forall_nans_f128(f128 x) {
        VOLATILEX u128 xi = asuint_f128(x);
        xi &= F128_ABS_MASK;
        if (xi > F128_POS_INF) {
            (void) feraiseexcept(FE_INEXACT);
        }
        return abs_reference_f128(x);
    }

    f128 abs_return_negative_forall_subnormals_f128(f128 x) {
        VOLATILEX u128 xi = asuint_f128(x);
        xi &= F128_ABS_MASK;
        if ((xi <= F128_MAX_SUBNORMAL) && (xi >= F128_POS_MIN_SUBNORMAL)) {
            return asfloat_u128(xi | F128_SIGN_MASK);
        }
        return abs_reference_f128(x);
    }

    f128 abs_return_negative_forall_infs_under_towardzero_f128(f128 x) {
        VOLATILEX u128 xi = asuint_f128(x);
        xi &= F128_ABS_MASK;
        if (xi == F128_POS_INF) {
            if (fegetround() == FE_TOWARDZERO) {
                return asfloat_u128(xi | F128_SIGN_MASK);
            }
        }
        return abs_reference_f128(x);
    }

    f128 abs_return_and_signal_nonsense_for_pos_zero_f128(f128 x) {
        VOLATILEX u128 xi = asuint_f128(x);
        if (xi == U128_C(0x0)) {
            (void) feraiseexcept(FE_DIVBYZERO);
            return asfloat_u128(xi | F128_SIGN_MASK);
        }
        return abs_reference_f128(x);
    }

    f128 abs_return_and_signal_nonsense_for_neg_nan_maxpay_f128(f128 x) {
        VOLATILEX u128 xi = asuint_f128(x);
        if (xi == (F128_SIGN_MASK | F128_EXP_MASK | F128_MANT_MASK)) {
            (void) feraiseexcept(FE_DIVBYZERO);
            return F128_C(77.7);
        }
        return abs_reference_f128(x);
    }
#endif


/* -- binary -- */

f32 copysign_spurious_inexact_forall_x_nans_y_nans_f32(f32 x, f32 y){
    VOLATILEX u32 xi = asuint_f32(x);
    VOLATILEX u32 yi = asuint_f32(y);
    xi &= F32_ABS_MASK;
    yi &= F32_ABS_MASK;
    if ((xi > F32_POS_INF) && (yi > F32_POS_INF)) {
        (void) feraiseexcept(FE_INEXACT);
    }
    return copysign_reference_f32(x, y);
}

f32 copysign_return_negative_forall_x_normals_y_pos_zero_f32(f32 x, f32 y) {
    VOLATILEX u32 xi = asuint_f32(x);
    VOLATILEX u32 yi = asuint_f32(y);
    xi &= F32_ABS_MASK;
    if ((xi <= F32_MAX_NORMAL) && (xi >= F32_POS_MIN_NORMAL) && (yi == U32_C(0x0))) {
        return asfloat_u32(xi | F32_SIGN_MASK);
    }
    return copysign_reference_f32(x, y);
}

f32 copysign_return_positive_forall_x_neg_y_neg_under_upward_f32(f32 x, f32 y) {
    VOLATILEX u32 xi = asuint_f32(x);
    VOLATILEX u32 yi = asuint_f32(y);
    if ((xi >= F32_SIGN_MASK) && (yi >= F32_SIGN_MASK)) {
        if (fegetround() == FE_UPWARD) {
            return asfloat_u32(xi & F32_ABS_MASK);
        }
    }
    return copysign_reference_f32(x, y);
}


f64 copysign_spurious_inexact_forall_x_nans_y_nans_f64(f64 x, f64 y){
    VOLATILEX u64 xi = asuint_f64(x);
    VOLATILEX u64 yi = asuint_f64(y);
    xi &= F64_ABS_MASK;
    yi &= F64_ABS_MASK;
    if ((xi > F64_POS_INF) && (yi > F64_POS_INF)) {
        (void) feraiseexcept(FE_INEXACT);
    }
    return copysign_reference_f64(x, y);
}

f64 copysign_return_negative_forall_x_normals_y_pos_zero_f64(f64 x, f64 y) {
    VOLATILEX u64 xi = asuint_f64(x);
    VOLATILEX u64 yi = asuint_f64(y);
    xi &= F64_ABS_MASK;
    if ((xi <= F64_MAX_NORMAL) && (xi >= F64_POS_MIN_NORMAL) && (yi == U64_C(0x0))) {
        return asfloat_u64(xi | F64_SIGN_MASK);
    }
    return copysign_reference_f64(x, y);
}

f64 copysign_return_positive_forall_x_neg_y_neg_under_upward_f64(f64 x, f64 y) {
    VOLATILEX u64 xi = asuint_f64(x);
    VOLATILEX u64 yi = asuint_f64(y);
    if ((xi >= F64_SIGN_MASK) && (yi >= F64_SIGN_MASK)) {
        if (fegetround() == FE_UPWARD) {
            return asfloat_u64(xi & F64_ABS_MASK);
        }
    }
    return copysign_reference_f64(x, y);
}

// optional
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 copysign_spurious_inexact_forall_x_nans_y_nans_f128(f128 x, f128 y){
        VOLATILEX u128 xi = asuint_f128(x);
        VOLATILEX u128 yi = asuint_f128(y);
        xi &= F128_ABS_MASK;
        yi &= F128_ABS_MASK;
        if ((xi > F128_POS_INF) && (yi > F128_POS_INF)) {
            (void) feraiseexcept(FE_INEXACT);
        }
        return copysign_reference_f128(x, y);
    }

    f128 copysign_return_negative_forall_x_normals_y_pos_zero_f128(f128 x, f128 y) {
        VOLATILEX u128 xi = asuint_f128(x);
        VOLATILEX u128 yi = asuint_f128(y);
        xi &= F128_ABS_MASK;
        if ((xi <= F128_MAX_NORMAL) && (xi >= F128_POS_MIN_NORMAL) && (yi == U128_C(0x0))) {
            return asfloat_u128(xi | F128_SIGN_MASK);
        }
        return copysign_reference_f128(x, y);
    }

    f128 copysign_return_positive_forall_x_neg_y_neg_under_upward_f128(f128 x, f128 y) {
        VOLATILEX u128 xi = asuint_f128(x);
        VOLATILEX u128 yi = asuint_f128(y);
        if ((xi >= F128_SIGN_MASK) && (yi >= F128_SIGN_MASK)) {
            if (fegetround() == FE_UPWARD) {
                return asfloat_u128(xi & F128_ABS_MASK);
            }
        }
        return copysign_reference_f128(x, y);
    }
#endif
