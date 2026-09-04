// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/module/mod_registry.h"
#include "finesword_tests/shared/print.h"

/* --- All Headers Begin --- */
#include "finesword_tests/module/all.h"
/* --- All Headers End --- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define REGISTERX(x) { .name = #x, .fn = (x)  },

static const struct mod_handler_entry mod_registry_entries[] =
{
    FINESWORD_TESTS_MOD_REGISTRY_REGISTER_ALL
};

static const size_t mod_registry_count =
    sizeof(mod_registry_entries) / sizeof(mod_registry_entries[0]);


/* -- access fun -- */

size_t mod_registry_get_count(void) {
    return mod_registry_count;
}

enum mod_registry_search_status
mod_registry_call_by_name(
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
    for (size_t i = 0; i < mod_registry_count; i++) {
        if (mod_registry_entries[i].name == NULL) {
            abort();
        }
        if (strcmp(name, mod_registry_entries[i].name) == 0) {
            *ret = mod_registry_entries[i].fn(argc, argv);
            return REGISTRY_OK;
        }
    }

    return REGISTRY_NOT_FOUND;
}

void mod_registry_list_all_names(FILE *stream) {
    if (stream == NULL) {
        stream = stdout; // default to stdout
    }
    for (size_t i = 0; i < mod_registry_count; i++) {
        if (mod_registry_entries[i].name == NULL) {
            abort();
        }

        void_fprintf(stream, "    %s\n", mod_registry_entries[i].name);
    }
}
