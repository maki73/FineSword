// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/fun/fun_registry.h"
#include "finesword_tests/shared/print.h"

/* --- All Headers Begin --- */
#include "finesword_tests/fun/all.h"
/* --- All Headers End --- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define REGISTERX(x) { .name = #x, .fn = (x)  },

static const struct fun_handler_entry fun_registry_entries[] =
{
    FINESWORD_TESTS_FUN_REGISTRY_REGISTER_ALL
};

static const size_t fun_registry_count =
    sizeof(fun_registry_entries) / sizeof(fun_registry_entries[0]);


/* -- access fun -- */

size_t fun_registry_get_count(void) {
    return fun_registry_count;
}

enum fun_registry_search_status
fun_registry_call_by_name(
    const char *name,
    const int argc,
    const char *const *const argv,
    bool *ret
)
{
    // arg is pass-through
    if ((name == NULL) || (ret == NULL)) {
        return REGISTRY_INVAL_ARG;
    }
    for (size_t i = 0; i < fun_registry_count; i++) {
        if (fun_registry_entries[i].name == NULL) {
            abort();
        }
        if (strcmp(name, fun_registry_entries[i].name) == 0) {
            *ret = fun_registry_entries[i].fn(argc, argv);
            return REGISTRY_OK;
        }
    }

    return REGISTRY_NOT_FOUND;
}

void fun_registry_list_all_names(FILE *stream) {
    if (stream == NULL) {
        stream = stdout; // default to stdout
    }
    for (size_t i = 0; i < fun_registry_count; i++) {
        if (fun_registry_entries[i].name == NULL) {
            abort();
        }

        void_fprintf(stream, "    %s\n", fun_registry_entries[i].name);
    }
}

