// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_REFERENCE_CLASSIFY_H
#define FINESWORD_TESTS_REFERENCE_CLASSIFY_H

#include "finesword/rudiments/astype.h"
#include "finesword/rudiments/types.h"

#include "finesword/scalar/classify.h"

#include "finesword_tests/shared/wrapper.h"


/* --- isZero --- */

GEN_INLINE_WRAPPER_FORALL_FN(iszero)

f32 w_ref_iszero_f32(f32 x);
f64 w_ref_iszero_f64(f64 x);

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_iszero_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_iszero_f128(f128 x);
#endif


/* --- isSignMinus() --- */

GEN_INLINE_WRAPPER_FORALL_FN(issignminus)

f32 w_m_signbit_f32(f32 x);
f64 w_m_signbit_f64(f64 x);

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_m__builtin_signbit_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_m__builtin_signbit_f128(f128 x);
#endif


/* --- isNormal() --- */

GEN_INLINE_WRAPPER_FORALL_FN(isnormal)

f32 w_ref_isnormal_f32(f32 x);
f64 w_ref_isnormal_f64(f64 x);

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_isnormal_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_isnormal_f128(f128 x);
#endif


/* --- isSubnormal() --- */

GEN_INLINE_WRAPPER_FORALL_FN(issubnormal)

f32 w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f32(f32 x);
f64 w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f64(f64 x);

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f128(f128 x);
#endif


/* --- isFinite() --- */

GEN_INLINE_WRAPPER_FORALL_FN(isfinite)

f32 w_ref_isfinite_f32(f32 x);
f64 w_ref_isfinite_f64(f64 x);

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_isfinite_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_isfinite_f128(f128 x);
#endif


/* --- isInfinite() --- */

GEN_INLINE_WRAPPER_FORALL_FN(isinfinite)

f32 w_ref_not_finite_and_not_nan_f32(f32 x);
f64 w_ref_not_finite_and_not_nan_f64(f64 x);

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_not_finite_and_not_nan_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_not_finite_and_not_nan_f128(f128 x);
#endif


/* --- isNaN() --- */

GEN_INLINE_WRAPPER_FORALL_FN(isnan)

f32 w_ref_isnan_f32(f32 x);
f64 w_ref_isnan_f64(f64 x);

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_isnan_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_isnan_f128(f128 x);
#endif


/* --- isSignaling() + isquiet_fN() --- */

GEN_INLINE_WRAPPER_FORALL_FN(issignaling)
GEN_INLINE_WRAPPER_FORALL_FN(isquiet)

f32 w_ref_issignaling_f32(f32 x);
f64 w_ref_issignaling_f64(f64 x);

f32 w_ref_isquiet_f32(f32 x);
f64 w_ref_isquiet_f64(f64 x);

#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w_ref_issignaling_f16(f16 x);
    f16 w_ref_isquiet_f16(f16 x);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 w_ref_issignaling_f128(f128 x);
    f128 w_ref_isquiet_f128(f128 x);
#endif

#endif /* FINESWORD_TESTS_REFERENCE_CLASSIFY_H */
