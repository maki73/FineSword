// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_REFERENCE_SIGN_H
#define FINESWORD_TESTS_REFERENCE_SIGN_H

#include "finesword/rudiments/types.h"
#include "finesword/config.h"


/*  --- Abs() --- */

f32 w_fabsf(f32 x);
f64 w_fabs(f64 x);
#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w__builtin_fabsf16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w__builtin_fabsf128(f128 x);
#endif


/*  --- Negate() --- */

f32 w_fneg_f32(f32 x);
f64 w_fneg_f64(f64 x);
#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_fneg_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_fneg_f128(f128 x);
#endif


/*  --- CopySign() --- */

f32 w_copysignf(f32 x, f32 y);
f64 w_copysign(f64 x, f64 y);
#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w__builtin_copysignf16(f16 x, f16 y);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w__builtin_copysignf128(f128 x, f128 y);
#endif

#endif /* FINESWORD_TESTS_REFERENCE_SIGN_H */
