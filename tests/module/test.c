// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/module/all.h"

#include "finesword_tests/shared/print.h"
#include "finesword_tests/test/test_registry.h"
#include "finesword_tests/test/correctness/dispatch.h"

#define ISQUIET false


bool test(const int argc, const char *const *const argv) {
    if (argc < 1) {
        error_printf("test module expected an argument!\n");
        error_printf("available test registry name(s):\n");
        test_registry_list_all_test_names(stderr);
        return false; /* fail */
    }
    if (argc > 1) {
        warning_printf("ignoring additional arguments beyond supplied name\n");
    }

    const char *str = argv[0];

    if (fn_name_matches_null_or_nulln(str)) {
        error_printf("invalid name!\n");
        error_printf("'NULL' and 'NULLN' represent absent functions\n");
        error_printf("available test registry name(s):\n");
        test_registry_list_all_test_names(stderr);
        return false; /* fail */
    }

    // returns NULL on failure
    const test_config *cfg = test_registry_get_config_pointer_by_test_name(str);
    if (cfg == NULL) {
        error_printf("getting config pointer failed!\n");
        error_printf("available test registry name(s):\n");
        test_registry_list_all_test_names(stderr);
        return false; /* fail */
    }

    void_printf("--- Correctness Test Module ---\n\n");

    return passes_dispatched_correctness_test(
        cfg,
        ISQUIET
    );
}
