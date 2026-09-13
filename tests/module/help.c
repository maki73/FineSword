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


#define FINESWORD_TESTING_INFRA_LICENSE "GPL-2.0-or-later"

bool help(const int argc, const char *const *const argv) {
    if (argc > 0) {
        warning_printf("ignoring argument(s)\n");
    }
    (void)argc;
    (void)argv;

    void_printf("--- FineSword Math Library's Testing Infrastructure ---\n\n");

    info_printf("-- General\n");
    void_printf("version: %s\n", FINESWORD_VERSION); // defined by CMake/manual_compile.sh
    void_printf("license: " FINESWORD_TESTING_INFRA_LICENSE "\n");

    info_printf("\n-- Usage\n");
    void_printf("/path/to/main [module registry name] [additional pass-through arg(s)] [...]\n");
    void_printf("where module registry is one of the following names:\n");
    mod_registry_list_all_names(stdout);
    void_printf("Some modules need additional pass-through arg(s)\n");
    void_printf("such as: /path/to/main test [required test registry test_fn name]\n");
    void_printf("where available test registry entries are:\n");
    test_registry_list_all_fn_names(stdout);

    return true;
}
