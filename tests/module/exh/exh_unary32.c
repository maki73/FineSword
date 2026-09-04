// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/module/all.h"

#include "finesword_tests/test/correctness/exhaustive.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/sys_helpers.h"
#include "finesword_tests/shared/utilities.h"
#include "finesword_tests/reference/exh_broken.h"

#include <fenv.h>
#pragma STDC FENV_ACCESS ON


/* -- Wrapper --- */

static bool test_passes_exhaustive_unary_binary32(
    f32 (*ref_fn)(f32),
    f32 (*test_fn)(f32),
    const bool are_nans_special,
    const bool is_quietcomputational,
    const int rounding_direction,
    //
    bool expected_ret,
    const char *ref_fn_name,
    const char *test_fn_name
)
{    
    // ref_fn/test_fn not checked for NULL (this is a pass-through function)
    if ((ref_fn_name == NULL) || (test_fn_name == NULL)) {
        error_printf("invalid argument(s) passed to test's wrapper!\n");
        return false;    
    }

    void_printf("arguments:\n");
    info_printf("    ref_fn:  %s\n",  ref_fn_name);
    info_printf("    test_fn: %s\n", test_fn_name);
    info_printf("    are_nans_special:      ");
    void_printf("%s\n", are_nans_special
        ? TESTS_ANSI_GREEN "true" TESTS_ANSI_RESET : TESTS_ANSI_RED "false" TESTS_ANSI_RESET);
    info_printf("    is_quietcomputational: ");
    void_printf("%s\n", is_quietcomputational
        ? TESTS_ANSI_GREEN "true" TESTS_ANSI_RESET : TESTS_ANSI_RED "false" TESTS_ANSI_RESET);
    info_printf("    rounding_direction:    ");
    report_rounding_direction(rounding_direction, stdout);

    void_printf("expected: %s\n", expected_ret
        ? TESTS_ANSI_GREEN "SUCCESS" TESTS_ANSI_RESET : TESTS_ANSI_RED "FAILURE" TESTS_ANSI_RESET);

    i64 t1 = ns_now_helper();
    bool r = passes_exhaustive_unary_binary32(
        ref_fn,
        test_fn,
        are_nans_special,
        is_quietcomputational,
        rounding_direction
    );
    i64 t2 = ns_now_helper();

    if (r != expected_ret) {
        if (expected_ret) {
            error_printf("the function failed unexpectedly!\n");
        } else {
            error_printf("the function should have failed!\n");
        }
        return false;
    }

    info_print_time_took(t1, t2);

    return true;
}


/* --- Main --- */

#define DOIT(ref, test, are_nans_special, is_quietcomputational, rounding_direction, expected_ret) \
    if (!test_passes_exhaustive_unary_binary32( \
        ref,                                    \
        test,                                   \
        are_nans_special,                       \
        is_quietcomputational,                  \
        rounding_direction,                     \
        expected_ret,                           \
        #ref,                                   \
        #test                                   \
    )) {                                        \
        goto fail;                              \
    } else {                                    \
        success_printf("ok\n\n");               \
    }


bool exh_unary32(const int argc, const char *const *const argv) {
    if (argc > 0) {
        warning_printf("ignoring argument(s)\n");
    }
    (void)argv;

    void_printf("--- Exhaustive Checker's Test ---\n\n");
    void_printf("function: passes_exhaustive_unary_binary32\n");
    
    fenv_t to_restore;
    if (feholdexcept(&to_restore) != 0) {
        error_printf("failed to set floating-point environment!\n");
        return false; /* fail */
    }
    if (feclearexcept(FE_ALL_EXCEPT) != 0) {
        error_printf("failed to clear floating-point exceptions!\n");
        goto fail;
    }


    info_printf("\n- basic sanity checks -\n\n");
    DOIT(abs_reference_f32, abs_reference_f32, false, true, FE_TONEAREST, true)
    DOIT(abs_reference_f32, abs_reference_f32, true, false, FE_TONEAREST, true)
    DOIT(abs_reference_f32, abs_reference_f32, true, true,  FE_DOWNWARD,  true)

    info_printf("-- functions with injected failures --\n");
    info_printf("NOTE: is_quietcomputational=false tests with exceptions mismatch should also give an exact offender\n");

    info_printf("\n- incorrect exceptions -\n\n");
    DOIT(abs_reference_f32,
     abs_spurious_underflow_0x81234567_to_0x812345CB_f32, false, true,  FE_TONEAREST, false)
    DOIT(abs_reference_f32,
     abs_spurious_underflow_0x81234567_to_0x812345CB_f32, false, false, FE_TONEAREST, false)    

    info_printf("\n- incorrect datum -\n\n");
    DOIT(abs_reference_f32,
     abs_return_negative_0x81234567_to_0x812345CB_f32, true, true,  FE_TONEAREST, false)
    DOIT(abs_reference_f32,
     abs_return_negative_0x81234567_to_0x812345CB_f32, true, false, FE_TONEAREST, false)  
    
    info_printf("\n- rounding direction sensitive exception failure -\n\n");
    DOIT(abs_reference_f32,
     abs_spurious_invalid_0x62697473_under_downward_f32,  true, true,  FE_TONEAREST, true)
    DOIT(abs_reference_f32,
     abs_spurious_invalid_0x62697473_under_downward_f32,  false,false, FE_TONEAREST, true)
    DOIT(abs_reference_f32,
     abs_spurious_invalid_0x62697473_under_downward_f32,  true, true,  FE_DOWNWARD,  false)
    DOIT(abs_reference_f32,
     abs_spurious_invalid_0x62697473_under_downward_f32,  false,false, FE_DOWNWARD,  false)

    info_printf("\n- both exceptions and data failure at the first and the last bit patterns -\n\n");    
    DOIT(abs_reference_f32,
     abs_return_and_signal_nonsense_for_0xFFFFFFFF_f32,  false, false, FE_TOWARDZERO, false)
    DOIT(abs_reference_f32,
     abs_return_and_signal_nonsense_for_0x00000000_f32,  false, false, FE_TOWARDZERO, false)


    if (fesetenv(&to_restore) != 0) {
        error_printf("failed to restore floating-point environment!\n");
        return false; /* fail */ 
    }

    return true;

fail:
    if (fesetenv(&to_restore) != 0) {
        error_printf("failed to restore floating-point environment in error path!\n");
        return false; /* fail */
    }
    return false; /* fail */
}
