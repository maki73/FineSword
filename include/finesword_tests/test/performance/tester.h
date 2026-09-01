// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_TEST_PERFORMANCE_TESTER_H
#define FINESWORD_TESTS_TEST_PERFORMANCE_TESTER_H

#include "finesword/rudiments/types.h"
#include "finesword_tests/test/test_registry.h"


/* --- dispatch --- */

bool performed_dispatched_performance_test(
    const test_config *cfg,
    const bool is_test_fn,
    const size_t inputs_count,
    const size_t repeats,
    const bool is_random,
    const u64 seed
);

#endif /* FINESWORD_TESTS_TEST_PERFORMANCE_TESTER_H */
