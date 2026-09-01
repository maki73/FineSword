// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_TEST_CORRECTNESS_LIST_H
#define FINESWORD_TESTS_TEST_CORRECTNESS_LIST_H

#include "finesword/rudiments/types.h"

/* List Tests
 * 
 * These tests verify returned floating-point datum
 * and exceptions using a static, predefined set of inputs.
 * They prove nothing, and only sanitize against obvious failures
 * (which is surprisingly often enough considering our template scheme
 * and that, for at least one width, a function is always exhaustively testable,
 * assuming 16-bit float is supported).
 */


/* f: Binary32 x Binary32 -> Binary32
 */
bool passes_list_binary_binary32(
    f32 (*ref_fn)(f32, f32),
    f32 (*test_fn)(f32, f32),
    const bool are_nans_special,
    const int rounding_direction
);

/* f: Binary64 -> Binary64
 */
bool passes_list_unary_binary64(
    f64 (*ref_fn)(f64),
    f64 (*test_fn)(f64),
    const bool are_nans_special,
    const int rounding_direction
);
 
/* f: Binary64 x Binary64 -> Binary64
 */
bool passes_list_binary_binary64(
    f64 (*ref_fn)(f64, f64),
    f64 (*test_fn)(f64, f64),
    const bool are_nans_special,
    const int rounding_direction
);

// optional
#if FINESWORD_TARGET_128BIT_FLOAT
    /* f: Binary128 -> Binary128
    */
    bool passes_list_unary_binary128(
        f128 (*ref_fn)(f128),
        f128 (*test_fn)(f128),
        const bool are_nans_special,
        const int rounding_direction
    );
    
    /* f: Binary128 x Binary128 -> Binary128
    */
    bool passes_list_binary_binary128(
        f128 (*ref_fn)(f128, f128),
        f128 (*test_fn)(f128, f128),
        const bool are_nans_special,
        const int rounding_direction
    );
#endif

#endif /* FINESWORD_TESTS_TEST_CORRECTNESS_LIST_H */
