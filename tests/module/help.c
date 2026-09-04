// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/module/all.h"

#include "finesword/rudiments/types.h"
#include "finesword_tests/module/mod_registry.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/test/test_registry.h"


bool help(const int argc, const char *const *const argv) {
    // if (argc > 0) {
    //     warning_printf("ignoring argument(s)\n");
    // }
    (void)argc;
    (void)argv;

    void_printf("--- FineSword Math Library's Testing Infrastructure ---\n\n");

    info_printf("-- General\n");
    void_printf("version: %s\n", FINESWORD_VERSION); // defined by CMake
    void_printf("license: GPL-2.0-or-later\n");


    info_printf("\n-- Usage\n");
    void_printf("/path/to/main [module registry name] [additional pass-through arg(s)] [...]\n");
    void_printf("where module registry is one of the following names:\n");
    mod_registry_list_all_names(stdout);
    void_printf("Some module registry modules need additional pass-through arg(s)\n");
    void_printf("e.g., /path/to/main test [test registry test_fn name]\n");
    void_printf("available test registry entries:\n");
    test_registry_list_all_fn_names(stdout);

    return true;
}
