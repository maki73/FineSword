// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/fun/fun_registry.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/sys_helpers.h"
#include "finesword_tests/shared/utilities.h"

#include <stdlib.h>


int main(int argc, char **argv) {
    setup_atabort_handler();

    // if ((argc != 2) && (argc != 3)) {
    if (argc < 2) {
        error_printf("invalid number of arguments!\n");
        if (argc > 0) { /* argc=1 */
            error_printf("usage %s [fun registry name] [optional pass-through arg(s)] [...] ..\n", argv[0]);
            error_printf("available fun registry name(s):\n");
            fun_registry_list_all_names(stderr);
        }
        return EXIT_FAILURE;
    }
    

    int    pass_argc = 0;
    char **pass_argv = NULL;
    if (argc > 2) {
        pass_argc = argc - 2;
        pass_argv = argv + 2;
    }

    // /* NULL for nothing
    //  function either (void)' it if it's unused
    //  or checks for != NULL and returns failure
    //  */
    // char *arg = NULL;
    // if (argc == 3) {
    //     arg = argv[2];
    // }

    bool ret; /* read only when REGISTRY_OK */
    enum fun_registry_search_status status =
        fun_registry_call_by_name(argv[1], pass_argc,
            (const char *const *const)pass_argv,  // not exactly clean
            &ret);

    if (status == REGISTRY_NOT_FOUND) {
        error_printf("cannot find '%s' in fun registry!\n", argv[1]);
        error_printf("available fun registry name(s):\n");
        fun_registry_list_all_names(stderr);

        return EXIT_FAILURE;
    }
    
    if (status == REGISTRY_INVAL_ARG) {
        error_printf("fun_registry_call_by_name received invalid argument(s)!\n");
        return EXIT_FAILURE;
    }

    if (status == REGISTRY_OK) {
        if (!ret) {
            // void_printf("\nAll done.\n");
            void_printf("---------------\n");
            void_printf("STATUS: FAILED\n");
            void_printf("---------------\n");
            return EXIT_FAILURE;
        } else {
            void_printf("\nAll done.\n");
            void_printf("---------------\n");
            void_printf("STATUS: SUCCESS\n");
            void_printf("---------------\n");
            return EXIT_SUCCESS;
        }
    }

    error_printf("got unknown return status from fun_registry_call_by_name!\n");
    return EXIT_FAILURE;
}
