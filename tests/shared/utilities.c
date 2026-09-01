// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/shared/utilities.h"
#include "finesword/rudiments/types.h"
#include "finesword_tests/shared/print.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


/* --- report --- */

const struct an_attribute_entry all_excepts[] = {
    { "FE_INVALID  ", FE_INVALID   }, // invalid operation
    { "FE_DIVBYZERO", FE_DIVBYZERO }, // division by zero 
    { "FE_OVERFLOW ", FE_OVERFLOW  }, // overflow
    { "FE_UNDERFLOW", FE_UNDERFLOW }, // underflow
    { "FE_INEXACT  ", FE_INEXACT   }  // inexact
};

const size_t all_excepts_count =
    sizeof(all_excepts) / sizeof(all_excepts[0]);

const struct an_attribute_entry all_directions[] = {
    { "FE_TONEAREST ", FE_TONEAREST  }, // roundTiesToEven
    { "FE_TOWARDZERO", FE_TOWARDZERO }, // roundTowardZero
    { "FE_UPWARD    ", FE_UPWARD     }, // roundTowardPositive
    { "FE_DOWNWARD  ", FE_DOWNWARD   }, // roundTowardNegative 
}; // [C24/C23 F.3 \P 3]

const size_t all_directions_count =
    sizeof(all_directions) / sizeof(all_directions[0]);

    
// if stream == NULL -> stdout
void report_except_flag(int flags, FILE *stream) {
    if (stream == NULL) {
        stream = stdout;
    }

    for (size_t i = 0; i < all_excepts_count; i++) {
        bool is_on = (flags & all_excepts[i].attribute) != 0;
        
        const char *status = is_on ?
        TESTS_ANSI_GREEN "ON"  TESTS_ANSI_RESET :
        TESTS_ANSI_RED   "OFF" TESTS_ANSI_RESET;
    
        if (all_excepts[i].name == NULL) {
            abort();
        }
            
        void_fprintf(stream, "    %s:   %s\n", all_excepts[i].name, status);
    }
}

// if stream == NULL -> stdout
void report_rounding_direction(int direction, FILE *stream) {
    if (stream == NULL) {
        stream = stdout;
    }

    bool hit = false;
    for (size_t i = 0; i < all_directions_count; i++) {
        bool is_on = direction == all_directions[i].attribute;
        if (!is_on) {
            continue;
        }
       
        if (all_directions[i].name == NULL) {
            abort();
        }

        void_fprintf(stream, TESTS_ANSI_BLUE "%s\n" TESTS_ANSI_RESET, all_directions[i].name);
             
        hit = true;
    }
    if (!hit) {
       error_printf("UNKNOWN\n");
       abort();
    }
}

bool is_rounding_direction_valid(int direction) {
    for (size_t i = 0; i < all_directions_count; i++) {
        if (direction == all_directions[i].attribute) {
            return true;
        }
    }
    return false;
}


/* -- test config validation helper -- */

#define SWITCH_VALIDATE_FUNCTION_POINTERRS_N_1(N)  \
    case N:                                        \
        if (cfg->arity == 1) {                     \
            is_valid =                             \
            (cfg->ref_fn.unary##N##_fn != NULL)    \
            &&                                     \
            (cfg->test_fn.unary##N##_fn != NULL);  \
            break;                                 \
        }                                          \
        if (cfg->arity == 2) {                     \
            is_valid =                             \
            (cfg->ref_fn.binary##N##_fn != NULL)   \
            &&                                     \
            (cfg->test_fn.binary##N##_fn != NULL); \
            break;                                 \
        }                                          \
        is_valid = false;                          \
        break;

bool is_test_config_valid(const test_config *cfg) {
    if (cfg == NULL) {
        error_printf("invalid config pointer!\n");
        return false; /* fail */
    }
    
    if ((cfg->ref_name == NULL) || (cfg->test_name == NULL)) {
        error_printf("invalid config name string(s)!\n");
        return false; /* fail */
    }

    if ((cfg->arity < 0) || (cfg->width < 0)) {
        error_printf("invalid parameter(s) in the passed config!\n");
        return false; /* fail */
    }

    bool is_valid = false;
    switch (cfg->width) {
        FINESWORD_TEMPLATE_SPECIALIZE(SWITCH_VALIDATE_FUNCTION_POINTERRS_N_1)
    default:
        is_valid = false;
        break;
    }
    
    if (!is_valid) {
        error_printf("failed to validate config!\n");
        // dump whole config as diagnostic
        error_printf("config:\n");
        error_printf("    ref_name: %s\n",  cfg->ref_name);
        error_printf("    test_name: %s\n", cfg->test_name);
        error_printf("    is_decimal: %s\n", cfg->is_decimal ? "true" : "false");
        error_printf("    arity: %d\n", cfg->arity);
        error_printf("    width: %d\n", cfg->width);
        error_printf("    are_nans_special: %s\n", cfg->are_nans_special ? "true" : "false");
        error_printf("    is_quietcomputational: %s\n", cfg->is_quietcomputational ? "true" : "false");
    }

    return is_valid;
}
