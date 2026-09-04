// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/module/all.h"

#include "finesword_tests/shared/print.h"
#include "finesword_tests/test/performance/tester.h"
#include "finesword_tests/test/test_registry.h"
#include "finesword_tests/shared/parse.h"

#include <stddef.h>
#include <string.h>


/* [0::name][1::inputs_count][2::repeats][3::random/sweep][4::seed (mandatory if random, else ignored)]
 */
bool perf(const int argc, const char *const *const argv) {
    if (argc < 4) {
        error_printf("perf module expected at least four arguments!\n");
        error_printf("arguments: [name][inputs_count][repeats][\"random\"/\"sweep\"][hex seed]\n");
        error_printf("seed is mandatory if \"random\" else it's ignored\n");
        error_printf("available name(s):\n");
        test_registry_list_all_fn_names(stderr);
        return false; /* fail */
    }

    // [name]
    if (fn_name_matches_null_or_nulln(argv[0])) {
        error_printf("invalid name!\n");
        error_printf("'NULL' and 'NULLN' represent absent functions\n");
        error_printf("available name(s):\n");
        test_registry_list_all_fn_names(stderr);
        return false; /* fail */
    }
    bool is_test_fn;
    const test_config *cfg = test_registry_get_config_pointer_by_fn_name(argv[0], &is_test_fn);
    if (cfg == NULL) {
        error_printf("retrieving config pointer failed!\n");
        error_printf("available name(s):\n");
        test_registry_list_all_fn_names(stderr);
        return false; /* fail */
    }

    // [inputs_count] & [repeats]
    size_t inputs_count;
    if (!parse_base10_prefixed_size_t(argv[1], &inputs_count)) {
        error_printf("failed parsing 'inputs_count'!\n");
        return false; /* fail */
    }
    size_t repeats;
    if (!parse_base10_prefixed_size_t(argv[2], &repeats)) {
        error_printf("failed parsing 'repeats'!\n");
        return false; /* fail */
    }

    // sweep/random
    bool is_random;
    u64 seed = U64_C(0);
    if (strcmp(argv[3], "sweep") == 0) {
        is_random = false;

        if (argc > 4) {
            warning_printf("ignoring additional argument(s)\n");
        }
    }
    else if (strcmp(argv[3], "random") == 0) {
        is_random = true;

        if (argc < 5) {
            error_printf("seed (in hex) is mandatory in \"random\"!\n");
            return false; /* fail */
        }

        if (!parse_base16_u64(argv[4], &seed)) {
            error_printf("failed parsing seed (in hex) argument!\n");
            return false; /* fail */
        }

        if (argc > 5) {
            warning_printf("ignoring additional argument(s)\n");
        }
    } else {
        error_printf("failed parsing/validating \"random\"/\"sweep\" string!\n");
        return false; /* fail */
    }

    void_printf("--- Performance Test Module ---\n\n");

    return performed_dispatched_performance_test(
        cfg,
        is_test_fn,
        inputs_count,
        repeats,
        is_random,
        seed
    );
}
