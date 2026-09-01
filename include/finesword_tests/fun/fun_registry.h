// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_FUN_FUN_REGISTRY_H
#define FINESWORD_TESTS_FUN_FUN_REGISTRY_H

#include "finesword/rudiments/types.h"
#include <stddef.h>
#include <stdio.h>


/* true -> success; false -> failure
 * if function intends to use its arguments, it SHALL check that argc != 0;
 * first (e.i., argv[0]) and subsequent arguments are intended for the module
 * and are NOT literal passed-through argv and argc from the main (they are shifted).
 */
typedef bool (*fun_handler_fn)(const int argc, const char *const *const argv);

struct fun_handler_entry {
    const char *name;
    fun_handler_fn fn;
};


/* -- access fun -- */

size_t fun_registry_get_count  (void);

enum fun_registry_search_status {
    REGISTRY_OK        =  0,
    REGISTRY_NOT_FOUND =  1,
    REGISTRY_INVAL_ARG = -1
};

/* accessing *ret when registry_search_status != REGISTRY_OK is UNDEFINED BEHAVIOR
 is argv are absent, argc SHALL be 0 */
enum fun_registry_search_status
fun_registry_call_by_name(
    const char *name,
    const int argc,
    const char *const *const argv,
    bool *ret
);

/* if stream == NULL -> stdout */
void fun_registry_list_all_names(FILE *stream);

#endif /* FINESWORD_TESTS_FUN_FUN_REGISTRY_H */
