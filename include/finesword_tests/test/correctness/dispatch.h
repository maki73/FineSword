// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_TEST_CORRECTNESS_DISPATCH_H
#define FINESWORD_TESTS_TEST_CORRECTNESS_DISPATCH_H

#include "finesword/rudiments/types.h"
#include "finesword_tests/test/test_registry.h"


/* --- dispatch --- */

bool passes_dispatched_correctness_test(
    const test_config *cfg,
    bool quiet
);

#endif /* FINESWORD_TESTS_TEST_CORRECTNESS_DISPATCH_H */
