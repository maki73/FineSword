// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_SHARED_TEST_REGISTRY_TMPL_H
#define FINESWORD_TESTS_SHARED_TEST_REGISTRY_TMPL_H

#include "finesword/rudiments/types.h"
#include "finesword/rudiments/template.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/test/test_registry.h"
#include "finesword/config.h"

#include <stddef.h>


/* -- metaprogramming registration helpers -- */

/* NOTE: passing NULL macro as the function SHALL expand to a NULL-pointer for the function
 * pointer (implementation-defined, e.g., to ((void *)0)) and to a "NULL" or "NULLN"
 * (not "((void *)0)") string for the name. We will define multiple NULLN macros
 * to achieve this behavior.
 */

#define REGISTER_SINGLE_UNARY_BINARYN_1(ref, refname, test, testname, N, nans, q) \
    {                                                          \
        .ref_fn.unary##N##_fn = ref,                           \
        .ref_name = refname,                                   \
        .test_fn.unary##N##_fn = test,                         \
        .test_name = testname,                                 \
        .is_decimal = false,                                   \
        .arity = 1,                                            \
        .width = N,                                            \
        .are_nans_special = nans,                              \
        .is_quietcomputational = q                             \
    },

#define REGISTER_SINGLE_BINARY_BINARYN_1(ref, refname, test, testname, N, nans, q) \
    {                                                           \
        .ref_fn.binary##N##_fn = ref,                           \
        .ref_name = refname,                                    \
        .test_fn.binary##N##_fn = test,                         \
        .test_name = testname,                                  \
        .is_decimal = false,                                    \
        .arity = 2,                                             \
        .width = N,                                             \
        .are_nans_special = nans,                               \
        .is_quietcomputational = q                              \
    },


#define NULL32 NULL
#define NULL64 NULL
#if FINESWORD_TARGET_16BIT_FLOAT
    #define NULL16 NULL
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    #define NULL128 NULL
#endif

#if !FINESWORD_TARGET_16BIT_FLOAT && !FINESWORD_TARGET_128BIT_FLOAT
    #define UNARY_REGISTERX(test_name, ref16, ref32, ref64, ref128, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref32, #ref32, test_name##32, #test_name "32", 32, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref64, #ref64, test_name##64, #test_name "64", 64, are_nans_special, is_quietcomputational)

    #define BINARY_REGISTERX(test_name, ref16, ref32, ref64, ref128, are_nans_special, is_quietcomputational) \
       REGISTER_SINGLE_BINARY_BINARYN_1(ref32, #ref32, test_name##32, #test_name "32", 32, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref64, #ref64, test_name##64, #test_name "64", 64, are_nans_special, is_quietcomputational)
#elif FINESWORD_TARGET_16BIT_FLOAT && FINESWORD_TARGET_128BIT_FLOAT
    #define UNARY_REGISTERX(test_name, ref16, ref32, ref64, ref128, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref32, #ref32, test_name##32, #test_name "32", 32, are_nans_special, is_quietcomputational)  \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref64, #ref64, test_name##64, #test_name "64", 64, are_nans_special, is_quietcomputational)   \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref16, #ref16, test_name##16, #test_name "16", 16, are_nans_special, is_quietcomputational)   \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref128, #ref128, test_name##128, #test_name "128", 128, are_nans_special, is_quietcomputational)

    #define BINARY_REGISTERX(test_name, ref16, ref32, ref64, ref128, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref32, #ref32, test_name##32, #test_name "32", 32, are_nans_special, is_quietcomputational)  \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref64, #ref64, test_name##64, #test_name "64", 64, are_nans_special, is_quietcomputational)   \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref16, #ref16, test_name##16, #test_name "16", 16, are_nans_special, is_quietcomputational)   \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref128, #ref128, test_name##128, #test_name "128", 128, are_nans_special, is_quietcomputational)
#elif FINESWORD_TARGET_16BIT_FLOAT && !FINESWORD_TARGET_128BIT_FLOAT
    #define UNARY_REGISTERX(test_name, ref16, ref32, ref64, ref128, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref32, #ref32, test_name##32, #test_name "32", 32, are_nans_special, is_quietcomputational)  \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref64, #ref64, test_name##64, #test_name "64", 64, are_nans_special, is_quietcomputational)   \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref16, #ref16, test_name##16, #test_name "16", 16, are_nans_special, is_quietcomputational)

    #define BINARY_REGISTERX(test_name, ref16, ref32, ref64, ref128, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref32, #ref32, test_name##32, #test_name "32", 32, are_nans_special, is_quietcomputational)  \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref64, #ref64, test_name##64, #test_name "64", 64, are_nans_special, is_quietcomputational)   \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref16, #ref16, test_name##16, #test_name "16", 16, are_nans_special, is_quietcomputational)
#elif !FINESWORD_TARGET_16BIT_FLOAT && FINESWORD_TARGET_128BIT_FLOAT
    #define UNARY_REGISTERX(test_name, ref16, ref32, ref64, ref128, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref32, #ref32, test_name##32, #test_name "32", 32, are_nans_special, is_quietcomputational)  \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref64, #ref64, test_name##64, #test_name "64", 64, are_nans_special, is_quietcomputational)   \
        REGISTER_SINGLE_UNARY_BINARYN_1(ref128, #ref128, test_name##128, #test_name "128", 128, are_nans_special, is_quietcomputational)

    #define BINARY_REGISTERX(test_name, ref16, ref32, ref64, ref128, are_nans_special, is_quietcomputational) \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref32, #ref32, test_name##32, #test_name "32", 32, are_nans_special, is_quietcomputational)  \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref64, #ref64, test_name##64, #test_name "64", 64, are_nans_special, is_quietcomputational)   \
        REGISTER_SINGLE_BINARY_BINARYN_1(ref128, #ref128, test_name##128, #test_name "128", 128, are_nans_special, is_quietcomputational)
#endif

#endif /* FINESWORD_TESTS_SHARED_TEST_REGISTRY_TMPL_H */
