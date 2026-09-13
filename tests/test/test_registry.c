// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */


#include "finesword_tests/test/test_registry.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/test_registry_tmpl.h"

/* --- All Headers Begin --- */
#include "finesword/scalar/sign.h"
#include "finesword_tests/reference/sign.h"
#include "finesword_tests/reference/classify.h"
/* --- All Headers End --- */

#include <stdlib.h>
#include <string.h>


static const test_config test_registry_entries[] =
{
    /* -- sign.h -- */
    UNARY_REGISTERX(abs_f,       w___builtin_fabsf16,     w_fabsf,     w_fabs,     w___builtin_fabsf128,     false, true)
    UNARY_REGISTERX(negate_f,    w_fneg_f16,              w_fneg_f32,  w_fneg_f64, w_fneg_f128,              false, true)
    BINARY_REGISTERX(copysign_f, w___builtin_copysignf16, w_copysignf, w_copysign, w___builtin_copysignf128, false, true)

    /* -- classify.h -- */
    UNARY_REGISTERX(w_iszero_f,      w_ref_iszero_f16,         w_ref_iszero_f32,   w_ref_iszero_f64,   w_ref_iszero_f128,         false, true)
    UNARY_REGISTERX(w_issignminus_f, w_m__builtin_signbit_f16, w_m_signbit_f32,    w_m_signbit_f64,    w_m__builtin_signbit_f128, false, true)
    UNARY_REGISTERX(w_isnormal_f,    w_ref_isnormal_f16,       w_ref_isnormal_f32, w_ref_isnormal_f64, w_ref_isnormal_f128,       false, true)
    UNARY_REGISTERX(w_issubnormal_f,
        w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f16, w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f32,
        w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f64, w_ref_isfinite_and_not_isnormal_and_not_iseq_zero_f128,
        false, true)
    UNARY_REGISTERX(w_isfinite_f, w_ref_isfinite_f16, w_ref_isfinite_f32, w_ref_isfinite_f64, w_ref_isfinite_f128, false, true)
    UNARY_REGISTERX(w_isinfinite_f,
        w_ref_not_finite_and_not_nan_f16, w_ref_not_finite_and_not_nan_f32,
        w_ref_not_finite_and_not_nan_f64, w_ref_not_finite_and_not_nan_f128,
        false, true)
    UNARY_REGISTERX(w_isnan_f,       w_ref_isnan_f16,       w_ref_isnan_f32,       w_ref_isnan_f64,       w_ref_isnan_f128,       false, true)
    UNARY_REGISTERX(w_issignaling_f, w_ref_issignaling_f16, w_ref_issignaling_f32, w_ref_issignaling_f64, w_ref_issignaling_f128, false, true)
    UNARY_REGISTERX(w_isquiet_f,     w_ref_isquiet_f16,     w_ref_isquiet_f32,     w_ref_isquiet_f64,     w_ref_isquiet_f128,     false, true)
};

static const size_t test_registry_count =
    sizeof(test_registry_entries) / sizeof(test_registry_entries[0]);


/* -- functions -- */

void test_registry_list_all_test_names(FILE *stream) {
    if (stream == NULL) {
        stream = stdout;
    }
    for (size_t i = 0; i < test_registry_count; i++) {
        if (test_registry_entries[i].test_name == NULL) {
            abort();
        }

        void_fprintf(stream, "    %s\n", test_registry_entries[i].test_name);
    }
}

void test_registry_list_all_fn_names(FILE *stream) {
    if (stream == NULL) {
        stream = stdout;
    }

    void_printf("[test_fn] :: [ref_fn]:\n");
    
    for (size_t i = 0; i < test_registry_count; i++) {
        if ((test_registry_entries[i].test_name == NULL) || (test_registry_entries[i].ref_name == NULL)) {
            abort();
        }

        void_fprintf(stream, "    %s :: %s\n",
            test_registry_entries[i].test_name,
            test_registry_entries[i].ref_name
        );
    }
}

// returns true iff fn_name is "NULL" or "NULLN"
bool fn_name_matches_null_or_nulln(const char *fn_name) {
    if (strncmp(fn_name, "NULL", (size_t)4) == 0) { // something like stupid_name_NULL_f32 is valid
        if ((strcmp(fn_name, "NULL") == 0) ||
            (strcmp(fn_name, "NULL32") == 0) ||
            (strcmp(fn_name, "NULL64") == 0)
#ifdef FINESWORD_TARGET_16BIT_FLOAT
            || (strcmp(fn_name, "NULL16") == 0)
#endif
#ifdef FINESWORD_TARGET_128BIT_FLOAT
            || (strcmp(fn_name, "NULL128") == 0)
#endif
        )
        {
            return true;
        }
    }

    return false;
}


// NULL if failed
const test_config *test_registry_get_config_pointer_by_test_name(const char *test_name) {
    if (test_name == NULL) {
        return NULL;
    }

    if (fn_name_matches_null_or_nulln(test_name)) {
        return NULL;
    }

    for (size_t i = 0; i < test_registry_count; i++) {
        if (test_registry_entries[i].test_name == NULL) {
            abort();
        }

        if (strcmp(test_name, test_registry_entries[i].test_name) == 0) {
            return &test_registry_entries[i];
        }
    }

    return NULL;
}

const test_config *test_registry_get_config_pointer_by_fn_name(const char *fn_name, bool *is_test_fn) {
    if ((fn_name == NULL) || (is_test_fn == NULL)) {
        return NULL;
    }

    if (fn_name_matches_null_or_nulln(fn_name)) {
        return NULL;
    }

    for (size_t i = 0; i < test_registry_count; i++) {
        if ((test_registry_entries[i].test_name == NULL) || (test_registry_entries[i].ref_name == NULL)) {
            abort();
        }

        // identical ref and test names are basically impossible by how the registry is constructed,
        // but if it occurs it will prioritize test_name entry
        if (strcmp(fn_name, test_registry_entries[i].test_name) == 0) {
            *is_test_fn = true;
            return &test_registry_entries[i];
        }
        if (strcmp(fn_name, test_registry_entries[i].ref_name) == 0) {
            *is_test_fn = false;
            return &test_registry_entries[i];
        }
    }

    return NULL;
}
