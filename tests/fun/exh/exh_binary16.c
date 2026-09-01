// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/fun/all.h"

#include "finesword_tests/test/correctness/exhaustive.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/sys_helpers.h"
#include "finesword_tests/shared/utilities.h"
#include "finesword_tests/reference/exh_broken.h"
#include "finesword/rudiments/types.h"

#include <fenv.h>
#pragma STDC FENV_ACCESS ON


#if !FINESWORD_TARGET_16BIT_FLOAT
bool exh_binary16(const int argc, const char *const *const argv) {
    if (argc > 0) {
        warning_printf("ignoring argument(s)\n");
    }
    (void)argv;

    info_printf("the project was compiled without 16-bit floating-point support.\n");
    return true;
}
#else
// NOTE: Again, basically a literal copy of test_exhaustive32 test

/* -- Wrapper -- */

static bool test_passes_exhaustive_binary_binary16(
    f16 (*ref_fn)(f16, f16),
    f16 (*test_fn)(f16, f16),
    const bool are_nans_special,
    const bool is_quietcomputational,
    const int rounding_direction,
    //
    bool expected_ret,
    const char *ref_fn_name,
    const char *test_fn_name
)
{
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
    bool r = passes_exhaustive_binary_binary16(
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
    if (!test_passes_exhaustive_binary_binary16( \
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


bool exh_binary16(const int argc, const char *const *const argv) {
    if (argc > 0) {
        warning_printf("ignoring argument(s)\n");
    }
    (void)argv;

    void_printf("--- Exhaustive Checker's Test ---\n\n");
    void_printf("function: passes_exhaustive_binary_binary16\n");
    
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
    DOIT(copysign_reference_f16, copysign_reference_f16, false, true, FE_TONEAREST, true)
    DOIT(copysign_reference_f16, copysign_reference_f16, true, false, FE_TONEAREST, true)
    DOIT(copysign_reference_f16, copysign_reference_f16, true, true,  FE_DOWNWARD,  true)

    info_printf("-- functions with injected failures --\n");
    info_printf("NOTE: is_quietcomputational=false tests with exceptions mismatch should also give an exact offender\n");

    info_printf("\n- incorrect exceptions -\n\n");
    DOIT(copysign_reference_f16,
     copysign_spurious_underflow_x_0x8123_to_0x8187_y_0x3031_to_0x3095_f16, false, true,  FE_TONEAREST, false)
    DOIT(copysign_reference_f16,
     copysign_spurious_underflow_x_0x8123_to_0x8187_y_0x3031_to_0x3095_f16, false, false, FE_TONEAREST, false)    

    info_printf("\n- incorrect datum -\n\n");
    DOIT(copysign_reference_f16,
     copysign_return_negative_x_0x8123_to_0x8187_y_0x3031_to_0x3095_f16, true, true,  FE_TONEAREST, false)
    DOIT(copysign_reference_f16,
     copysign_return_negative_x_0x8123_to_0x8187_y_0x3031_to_0x3095_f16, true, false, FE_TONEAREST, false)  
    
    info_printf("\n- rounding direction sensitive exception failure -\n\n");
    DOIT(copysign_reference_f16,
     copysign_spurious_inexact_for_x_0x3031_y_0x3031_under_towardzero_f16,  true, true,  FE_TONEAREST, true)
    DOIT(copysign_reference_f16,
     copysign_spurious_inexact_for_x_0x3031_y_0x3031_under_towardzero_f16,  false,false, FE_TONEAREST, true)
    DOIT(copysign_reference_f16,
     copysign_spurious_inexact_for_x_0x3031_y_0x3031_under_towardzero_f16,  true, true,  FE_TOWARDZERO,  false)
    DOIT(copysign_reference_f16,
     copysign_spurious_inexact_for_x_0x3031_y_0x3031_under_towardzero_f16,  false,false, FE_TOWARDZERO,  false)

    info_printf("\n- both exceptions and data failure at the boundaries -\n\n");    
    DOIT(copysign_reference_f16,
     copysign_return_and_signal_nonsense_for_x_0xFFFF_y_0xFFFF_f16,  false, false, FE_UPWARD, false)
    DOIT(copysign_reference_f16,
     copysign_return_and_signal_nonsense_for_x_0x0000_y_0x0000_f16,  false, false, FE_UPWARD, false)
    DOIT(copysign_reference_f16,
     copysign_return_and_signal_nonsense_for_x_0x0000_y_0xFFFF_f16,  false, false, FE_UPWARD, false)
    DOIT(copysign_reference_f16,
     copysign_return_and_signal_nonsense_for_x_0xFFFF_y_0x0000_f16,  false, false, FE_UPWARD, false)


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
#endif /* FINESWORD_TARGET_16BIT_FLOAT */
