// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_TEST_CORRECTNESS_EXHAUSTIVE_H
#define FINESWORD_TESTS_TEST_CORRECTNESS_EXHAUSTIVE_H

#include "finesword/rudiments/types.h"

/* Exhaustive Binary Provers
 * 
 * Scheme:
 * - ref_fn  - reference function that is assumed to be correct by definition
 * - test_fn - possibly incorrect function under the test
 *
 * the correctness is evaluated according to two things:
 *   input -> (output, status flags)
 * that is, the input value, the output value,
 * and status flags raised by the function, with respect
 * to `rounding_direction`.
 * 
 * test_fn and ref_fn SHALL NOT yield (i.e., not mess with threads)
 * and SHALL NOT change the environment's settings, such as changing 
 * rounding directions, setting traps, etc.
 */

/* Exhaustively provable (within general feasibility limits) functions are:
 * f: Binary32 -> Binary32
 * f: Binary16 -> Binary16
 * f: Binary16 x Binary16 -> Binary16.
 */


/* f: Binary32 -> Binary32
 */
bool passes_exhaustive_unary_binary32(
    f32 (*ref_fn)(f32),
    f32 (*test_fn)(f32),
    const bool are_nans_special,
    const bool is_quietcomputational,
    const int rounding_direction
);

// optional
#if FINESWORD_TARGET_16BIT_FLOAT
    // the linter doesn't know what _Float16 is and mistakens
    // ref_fn/test_fn for an implicit function declaration
    // cppcheck-suppress-begin misra-c2012-17.3 

    /* f: Binary16 -> Binary16
    */
    bool passes_exhaustive_unary_binary16(
        f16 (*ref_fn)(f16),
        f16 (*test_fn)(f16),
        const bool are_nans_special,
        const bool is_quietcomputational,
        const int rounding_direction
    );

    /* f: Binary16 x Binary16 -> Binary16
    */
    bool passes_exhaustive_binary_binary16(
        f16 (*ref_fn)(f16, f16),
        f16 (*test_fn)(f16, f16),
        const bool are_nans_special,
        const bool is_quietcomputational,
        const int rounding_direction
    );
    // cppcheck-suppress-end misra-c2012-17.3 
#endif

#endif /* FINESWORD_TESTS_TEST_CORRECTNESS_EXHAUSTIVE_H */
