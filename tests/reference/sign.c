// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */


#include "finesword_tests/reference/sign.h"
#include "finesword/rudiments/astype.h"
#include "finesword/rudiments/ieee754.h"
#include "finesword/rudiments/types.h"
#include "finesword/config.h"

#include <math.h>

/* See doc/ci.txt note 2
 */
#define     LLVM_USE_EXPLICIT_BIT_W_FNEG_F16 1
#if defined(__i386__) || defined(__x86_64__)
    #define X86_USE_EXPLICIT_BIT_W_FABS_F16  1
#else
    #define X86_USE_EXPLICIT_BIT_W_FABS_F16  0
#endif

/*  --- Abs() --- */


f32 w_fabsf(f32 x) {
    return (f32)fabsf((float)x);
}

f64 w_fabs(f64 x) {
    return (f64)fabs((double)x);
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w__builtin_fabsf16(f16 x) {
        #if X86_USE_EXPLICIT_BIT_W_FABS_F16
            volatile u16 xi = asuint_f16(x);
            xi &= F16_ABS_MASK;
            return asfloat_u16(xi);
        #else
            return (f16)__builtin_fabsf16(x);
        #endif
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w__builtin_fabsf128(f128 x) {
        return (f128)__builtin_fabsf128(x);
    }
#endif


/*  --- Negate() --- */

f32 w_fneg_f32(f32 x) {
    return -x;
}

f64 w_fneg_f64(f64 x) {
    return -x;
}

#if FINESWORD_TARGET_16BIT_FLOAT
    /* See doc/ci.txt note 2
     */
    f16 w_fneg_f16(f16 x) {
        #if LLVM_USE_EXPLICIT_BIT_W_FNEG_F16
            volatile u16 xi = asuint_f16(x);
            xi ^= F16_SIGN_MASK;
            return asfloat_u16(xi);
        #else
            return -(x);
        #endif
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_fneg_f128(f128 x) {
        return -(x);
    }
#endif


/*  --- CopySign() --- */

f32 w_copysignf(f32 x, f32 y) {
    return (f32)copysignf((float)x, (float)y);
}

f64 w_copysign(f64 x, f64 y) {
    return (f64)copysign((double)x, (double)y);
}

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w__builtin_copysignf16(f16 x, f16 y) {
        return (f16)__builtin_copysignf16(x, y);
    }
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w__builtin_copysignf128(f128 x, f128 y) {
        return (f128)__builtin_copysignf128(x, y);
    }
#endif
