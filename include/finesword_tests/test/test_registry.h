// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_TEST_TEST_REGISTRY_H
#define FINESWORD_TESTS_TEST_TEST_REGISTRY_H

#include "finesword/rudiments/types.h"
#include "finesword/rudiments/template.h"

#include <stdio.h>

// void (*generic_fn)(void) method would violate the required Rule 11.1

#define FORALL(x) FINESWORD_TEMPLATE_SPECIALIZE(x) // undef later

// create typedefs
#define FINESWORD_TESTS_UNARY_AND_BINARY_TYPEDEF_FN(N) \
typedef f##N (*test_unary##N##_fn)(f##N);              \
typedef f##N (*test_binary##N##_fn)(f##N, f##N);

FORALL(FINESWORD_TESTS_UNARY_AND_BINARY_TYPEDEF_FN)

#undef FINESWORD_TESTS_UNARY_AND_BINARY_TYPEDEF_FN

// create union
#define FINESWORD_TESTS_UNARY_AND_BINARY_CREATE_UNION_MEMBERS(N) \
    test_unary##N##_fn  unary##N##_fn;                           \
    test_binary##N##_fn binary##N##_fn;

typedef union {
    FORALL(FINESWORD_TESTS_UNARY_AND_BINARY_CREATE_UNION_MEMBERS)
} test_function_signature;

#undef FINESWORD_TESTS_UNARY_AND_BINARY_CREATE_UNION_MEMBERS
#undef FORALL


typedef struct {
    test_function_signature ref_fn;
    const char *ref_name;
    test_function_signature test_fn;
    const char *test_name;
    bool is_decimal; // decimal or binary
    int arity;       // functions arity (e.g., 1 or 2)
    int width;       // floating-point formats width (e.g., 16, 32, 64, 128)
    bool are_nans_special;
    bool is_quietcomputational;
} test_config;


/* NOTE: name of an absent function (function whose pointer is NULL) SHALL be "NULL"
 * or "NULLN" where N is the width (e.g., "NULL32" for f32).
 * NULL **name strings** are invalid and the program will immediately abort upon encoutering one.
 */

// if steam == NULL -> stream = stdout 
void test_registry_list_all_test_names(FILE *stream);
void test_registry_list_all_fn_names  (FILE *stream);

// NULL if failed
// For "NULL" or "NULLN" test_name function will return NULL (i.e., fail)
const test_config *test_registry_get_config_pointer_by_test_name(const char *test_name);

/* NULL if failed; searches both test_name and ref_name; prioritizes test_name
 * access is_test_fn iff the function returned non-NULL, otherwise it's UB
 * if it returned is_test_fn=false, then the function found by name is ref_fn rather than test_fn.
 * If fn_name argument is "NULL" or "NULLN" (where N is the 16, 32, 64, or 128), then
 * this function will return NULL (e.i., fail).
 */
const test_config *test_registry_get_config_pointer_by_fn_name(const char *fn_name, bool *is_test_fn);

// returns true iff fn_name is "NULL" or "NULLN"
bool fn_name_matches_null_or_nulln(const char *fn_name);

#endif /* FINESWORD_TESTS_TEST_TEST_REGISTRY_H */
