// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/test/correctness/dispatch.h"

#include "finesword/rudiments/types.h"
#include "finesword_tests/test/correctness/exhaustive.h"
#include "finesword_tests/test/correctness/list.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/utilities.h"


/* --- Dispatch --- */

bool passes_dispatched_correctness_test(const test_config *cfg, bool quiet) {
    if (!is_test_config_valid(cfg)) {
        return false; /* fail */
    }

    if (!quiet) {
        void_printf("test:\n");
        info_printf("    ref_fn:  %s\n",  cfg->ref_name);
        info_printf("    test_fn: %s\n", cfg->test_name);
        info_printf("    are_nans_special:      ");
        void_printf("%s\n", cfg->are_nans_special
            ? TESTS_ANSI_GREEN "true" TESTS_ANSI_RESET : TESTS_ANSI_RED "false" TESTS_ANSI_RESET);
        info_printf("    is_quietcomputational: ");
        void_printf("%s\n", cfg->is_quietcomputational
            ? TESTS_ANSI_GREEN "true" TESTS_ANSI_RESET : TESTS_ANSI_RED "false" TESTS_ANSI_RESET);
        // later write the dispatch choice
    }

    // - currently unimplemented -
    if (cfg->is_decimal) {
        error_printf("failed to dispatch! decimal functions are currently unsupported.\n");
        return false; /* fail */       
    }

    /* -- ARITY=1 -- */

    if (cfg->arity == 1) {
        if (cfg->width == 16) {
#if FINESWORD_TARGET_16BIT_FLOAT
            if (!quiet) {
                info_printf("    dispatched to: passes_exhaustive_unary_binary16\n"); 
            }
            void_printf("testing %s...\n", cfg->test_name);
            bool r;
            for (size_t i = 0; i < all_directions_count; i++) {
                info_printf("%s... ", all_directions[i].name); // + space
                void_fflush(stdout);
                r = passes_exhaustive_unary_binary16(
                    cfg->ref_fn.unary16_fn,
                    cfg->test_fn.unary16_fn,
                    cfg->are_nans_special,
                    cfg->is_quietcomputational,
                    all_directions[i].attribute
                );
                if (r) {
                    success_printf("ok\n");
                }
                else {
                    return false; /* fail */
                }
            }
            return true; /* success */
#else
            error_printf("project was compiled without 16-bit float support!\n");
            return false;
#endif
        }

        if (cfg->width == 32) {
            if (!quiet) {
                info_printf("    dispatched to: passes_exhaustive_unary_binary32\n"); 
            }
            void_printf("testing %s...\n", cfg->test_name);
            bool r;
            for (size_t i = 0; i < all_directions_count; i++) {
                info_printf("%s... ", all_directions[i].name); // + space
                void_fflush(stdout);
                r = passes_exhaustive_unary_binary32(
                    cfg->ref_fn.unary32_fn,
                    cfg->test_fn.unary32_fn,
                    cfg->are_nans_special,
                    cfg->is_quietcomputational,
                    all_directions[i].attribute
                );
                if (r) {
                    success_printf("ok\n");
                }
                else {
                    return false; /* fail */
                }
            }
            return true; /* success */
        }

        if (cfg->width == 64) {
            if (!quiet) {
                info_printf("    dispatched to: passes_list_unary_binary64\n"); 
            }
            void_printf("testing %s...\n", cfg->test_name);
            bool r;
            for (size_t i = 0; i < all_directions_count; i++) {
                info_printf("%s... ", all_directions[i].name); // + space
                void_fflush(stdout);
                r = passes_list_unary_binary64(
                    cfg->ref_fn.unary64_fn,
                    cfg->test_fn.unary64_fn,
                    cfg->are_nans_special,
                    all_directions[i].attribute
                );
                if (r) {
                    success_printf("ok\n");
                }
                else {
                    return false; /* fail */
                }
            }
            return true; /* success */
        }

        if (cfg->width == 128) {
#if FINESWORD_TARGET_128BIT_FLOAT
            if (!quiet) {
                info_printf("    dispatched to: passes_list_unary_binary128\n"); 
            }
            void_printf("testing %s...\n", cfg->test_name);
            bool r;
            for (size_t i = 0; i < all_directions_count; i++) {
                info_printf("%s... ", all_directions[i].name);
                void_fflush(stdout);
                r = passes_list_unary_binary128(
                    cfg->ref_fn.unary128_fn,
                    cfg->test_fn.unary128_fn,
                    cfg->are_nans_special,
                    all_directions[i].attribute
                );
                if (r) {
                    success_printf("ok\n");
                }
                else {
                    return false; /* fail */
                }
            }
            return true; /* success */
#else
            error_printf("project was compiled without 128-bit float support!\n");
            return false;
#endif
        }
    }

    /* -- ARITY=2 -- */

    if (cfg->arity == 2) {
        if (cfg->width == 16) {
#if FINESWORD_TARGET_16BIT_FLOAT
            if (!quiet) {
                info_printf("    dispatched to: passes_exhaustive_binary_binary16\n"); 
            }
            void_printf("testing %s...\n", cfg->test_name);
            bool r;
            for (size_t i = 0; i < all_directions_count; i++) {
                info_printf("%s... ", all_directions[i].name); // + space
                void_fflush(stdout);
                r = passes_exhaustive_binary_binary16(
                    cfg->ref_fn.binary16_fn,
                    cfg->test_fn.binary16_fn,
                    cfg->are_nans_special,
                    cfg->is_quietcomputational,
                    all_directions[i].attribute
                );
                if (r) {
                    success_printf("ok\n");
                }
                else {
                    return false; /* fail */
                }
            }
            return true; /* success */
#else
            error_printf("project was compiled without 16-bit float support!\n");
            return false;       
#endif
        }

        if (cfg->width == 32) {
            if (!quiet) {
                info_printf("    dispatched to: passes_list_binary_binary32\n"); 
            }
            void_printf("testing %s...\n", cfg->test_name);
            bool r;
            for (size_t i = 0; i < all_directions_count; i++) {
                info_printf("%s... ", all_directions[i].name); // + space
                void_fflush(stdout);
                r = passes_list_binary_binary32(
                    cfg->ref_fn.binary32_fn,
                    cfg->test_fn.binary32_fn,
                    cfg->are_nans_special,
                    all_directions[i].attribute
                );
                if (r) {
                    success_printf("ok\n");
                }
                else {
                    return false; /* fail */
                }
            }
            return true; /* success */
        }

        if (cfg->width == 64) {
            if (!quiet) {
                info_printf("    dispatched to: passes_list_binary_binary64\n"); 
            }
            void_printf("testing %s...\n", cfg->test_name);
            bool r;
            for (size_t i = 0; i < all_directions_count; i++) {
                info_printf("%s... ", all_directions[i].name); // + space
                void_fflush(stdout);
                r = passes_list_binary_binary64(
                    cfg->ref_fn.binary64_fn,
                    cfg->test_fn.binary64_fn,
                    cfg->are_nans_special,
                    all_directions[i].attribute
                );
                if (r) {
                    success_printf("ok\n");
                }
                else {
                    return false; /* fail */
                }
            }
            return true; /* success */
        }

        if (cfg->width == 128) {
#if FINESWORD_TARGET_128BIT_FLOAT
            if (!quiet) {
                info_printf("    dispatched to: passes_list_binary_binary128\n"); 
            }
            void_printf("testing %s...\n", cfg->test_name);
            bool r;
            for (size_t i = 0; i < all_directions_count; i++) {
                info_printf("%s... ", all_directions[i].name); // + space
                void_fflush(stdout);
                r = passes_list_binary_binary128(
                    cfg->ref_fn.binary128_fn,
                    cfg->test_fn.binary128_fn,
                    cfg->are_nans_special,
                    all_directions[i].attribute
                );
                if (r) {
                    success_printf("ok\n");
                }
                else {
                    return false; /* fail */
                }
            }
            return true; /* success */
#else
            error_printf("project was compiled without 128-bit float support!\n");
            return false;       
#endif
        }
    }

    // error path (otherwise returned earlier)
    error_printf("failed to dispatch!\n");
    return false; /* fail */
}
