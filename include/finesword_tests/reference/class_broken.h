// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_REFERENCE_CLASS_BROKEN_H
#define FINESWORD_TESTS_REFERENCE_CLASS_BROKEN_H

#include "finesword/rudiments/types.h"
#include "finesword/config.h"


/* Functions Broken for a Whole Class of Inputs (for List Test Tests)
 */

// references
f32 copysign_reference_f32(f32 x, f32 y);

f64 abs_reference_f64(f64 x);
f64 copysign_reference_f64(f64 x, f64 y);
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 abs_reference_f128(f128 x);
    f128 copysign_reference_f128(f128 x, f128 y);
#endif

// broken unary
f64 abs_spurious_inexact_forall_nans_f64(f64 x);
f64 abs_return_negative_forall_subnormals_f64(f64 x);
f64 abs_return_negative_forall_infs_under_towardzero_f64(f64 x);
f64 abs_return_and_signal_nonsense_for_pos_zero_f64(f64 x);
f64 abs_return_and_signal_nonsense_for_neg_nan_maxpay_f64(f64 x);
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 abs_spurious_inexact_forall_nans_f128(f128 x);
    f128 abs_return_negative_forall_subnormals_f128(f128 x);
    f128 abs_return_negative_forall_infs_under_towardzero_f128(f128 x);
    f128 abs_return_and_signal_nonsense_for_pos_zero_f128(f128 x);
    f128 abs_return_and_signal_nonsense_for_neg_nan_maxpay_f128(f128 x);
#endif

// broken binary
// NOTE: both conditions are meant to be true
// (e.g., the function below spuriously raises inexact iff x is nan and y is nan) 
f32 copysign_spurious_inexact_forall_x_nans_y_nans_f32(f32 x, f32 y);
f32 copysign_return_negative_forall_x_normals_y_pos_zero_f32(f32 x, f32 y);
f32 copysign_return_positive_forall_x_neg_y_neg_under_upward_f32(f32 x, f32 y);

f64 copysign_spurious_inexact_forall_x_nans_y_nans_f64(f64 x, f64 y);
f64 copysign_return_negative_forall_x_normals_y_pos_zero_f64(f64 x, f64 y);
f64 copysign_return_positive_forall_x_neg_y_neg_under_upward_f64(f64 x, f64 y);
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 copysign_spurious_inexact_forall_x_nans_y_nans_f128(f128 x, f128 y);
    f128 copysign_return_negative_forall_x_normals_y_pos_zero_f128(f128 x, f128 y);
    f128 copysign_return_positive_forall_x_neg_y_neg_under_upward_f128(f128 x, f128 y);
#endif

// NOTE: We *can* also check boundaries as we do for exhaustive tests,
// but it's far less useful for list tests especially considering that
// we already checked that they are in the table (with neg_maxpay and pos_zero,
// since tables are templated)

#endif /* FINESWORD_TESTS_REFERENCE_CLASS_BROKEN_H */
