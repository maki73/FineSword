// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/test/correctness/list.h"

#include "finesword/rudiments/types.h"
#include "finesword/rudiments/astype.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/utilities.h"
#include "finesword/rudiments/template.h"
#include "finesword/rudiments/ieee754.h"
#include "finesword_tests/shared/plus_constants.h"

#include <inttypes.h>
#include <fenv.h>
#pragma STDC FENV_ACCESS ON /* required */


/* -- Table(s) -- */

#define PM(m, N) m, m | F##N##_SIGN_MASK,

#define TEMPLATE_LIST_TABLE_FN_1(N)    \
static const u##N table_list_binary##N[] = { \
    /* NaNs */                         \
    PM(F##N##_MINPAY_POS_SNAN, N)      \
    PM(F##N##_MINPAY_POS_QNAN, N)      \
    PM((F##N##_EXP_MASK | F##N##_MANT_MASK), N) \
    PM(((F##N##_EXP_MASK | F##N##_MANT_MASK) ^ F##N##_D1_BIT_MASK), N) \
    /* Infs */                         \
    PM(F##N##_POS_INF, N)              \
    /* Normal Extremes */              \
    PM(F##N##_POS_MIN_NORMAL, N)       \
    PM(F##N##_MAX_NORMAL, N)           \
    /* Subnormal Extremes */           \
    PM(F##N##_POS_MIN_SUBNORMAL, N)    \
    PM(F##N##_MAX_SUBNORMAL, N)        \
    /* Zeros */                        \
    PM(U##N##_C(0x0), N)               \
    /* Integral Boundaries */          \
    PM(F##N##_NO_FRAC_FROM, N)         \
    PM(F##N##_IS_EVEN_FROM, N)         \
    /* Additional Constants */         \
    PM(F##N##_EPS_AT_ONE, N)           \
    PM(F##N##_BITS_ONE, N)             \
    PM(F##N##_NEXTFLOAT_ONE, N)        \
    PM(F##N##_PREVFLOAT_ONE, N)        \
    PM(F##N##_BITS_PI, N)              \
    PM(F##N##_BITS_TWO_PI, N)          \
    PM(F##N##_BITS_PI_TWO, N)          \
    PM(F##N##_BITS_EULER, N)           \
    PM(F##N##_BITS_INV_EULER, N)       \
    PM(F##N##_BITS_SQRT_TWO, N)        \
    PM(F##N##_BITS_PHI, N)             \
};                                     \
static const size_t                    \
list_binary##N##_count =               \
    sizeof(table_list_binary##N) / sizeof(table_list_binary##N[0]);

TEMPLATE_LIST_TABLE_FN_1(32)
TEMPLATE_LIST_TABLE_FN_1(64)
#if FINESWORD_TARGET_128BIT_FLOAT
    TEMPLATE_LIST_TABLE_FN_1(128)
#endif


/* -- BOILERPLATE -- */

#define BOILER                                               \
    if ((ref_fn == NULL) || (test_fn == NULL)) {             \
        error_printf("invalid function pointer(s)!\n");      \
        return false; /* fail */                             \
    }                                                        \
    if (!is_rounding_direction_valid(rounding_direction)) {  \
        error_printf("invalid rounding direction!\n");       \
        return false; /* fail */                             \
    }                                                        \
                                                             \
    /* setup */                                              \
    fenv_t to_restore;                                       \
    if (feholdexcept(&to_restore) != 0) {                    \
        error_printf("failed to set floating-point environment!\n"); \
        return false; /* fail */                             \
    }                                                        \
    if (fesetround(rounding_direction) != 0) {               \
        error_printf("failed to set rounding direction!\n"); \
        goto fail;                                           \
    }                                                        \
    if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {         \
        error_printf("failed to clear floating-point exceptions!\n"); \
        goto fail;                                           \
    }

#define PLATE                         \
    if (fesetenv(&to_restore) != 0) { \
        error_printf("failed to restore floating-point environment!\n"); \
        return false; /* fail */      \
    }                                 \
                                      \
    return true; /* pass */           \
fail:                                 \
    if (fesetenv(&to_restore) != 0) { \
        error_printf("failed to restore floating-point environment in error path!\n"); \
        return false; /* fail */      \
    }                                 \
    return false; /* fail */


/* -- Unary -- */

bool passes_list_unary_binary64(
    f64 (*ref_fn)(f64),
    f64 (*test_fn)(f64),
    const bool are_nans_special,
    const int rounding_direction
)
{
    BOILER

    for (size_t i = 0; i < list_binary64_count; i++) {
        u64 entry = table_list_binary64[i];
        f64 x = asfloat_u64(entry);

        /* - REFERENCE - */
        if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
            error_printf("failed to clear floating-point exceptions mid-test!\n");
            goto fail;
        }
        f64 ref_r = ref_fn(x);
        int ref_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

        /* - TEST - */
        if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
            error_printf("failed to clear floating-point exceptions mid-test!\n");
            goto fail;
        }
        f64 test_r = test_fn(x);
        int test_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

        bool pass_val   = is_equal_bitwise_ruled_f64(ref_r, test_r, are_nans_special);
        bool pass_flags = ref_e == test_e;
        bool pass       = pass_val && pass_flags;
        if (!pass) {
            error_printf("FAILED\n");
            error_printf("rounding direction: ");
            report_rounding_direction(rounding_direction, stderr);
            error_printf("at x = %f    %a    0x%016" PRIX64 "\n",
                (double)x, (double)x, (uint64_t)asuint_f64(x));

            error_printf("ref(x):\n    %f    %a    0x%016" PRIX64 "\n",
                (double)ref_r, (double)ref_r, (uint64_t)asuint_f64(ref_r));
            report_except_flag(ref_e, stderr);
    
            error_printf("test(x):\n    %f    %a    0x%016" PRIX64 "\n",
                (double)test_r, (double)test_r, (uint64_t)asuint_f64(test_r));
            report_except_flag(test_e, stderr);

            if (pass_val) {
                error_printf("exception(s) mismatch!\n");
            } else {
                error_printf("incorrect floating-point datum!\n");
            }
            void_fflush(stderr);

            goto fail;
        } // failure
    } // for

    PLATE
}

// optional
#if FINESWORD_TARGET_128BIT_FLOAT
    bool passes_list_unary_binary128(
        f128 (*ref_fn)(f128),
        f128 (*test_fn)(f128),
        const bool are_nans_special,
        const int rounding_direction
    )
    {
        BOILER

        for (size_t i = 0; i < list_binary128_count; i++) {
            u128 entry = table_list_binary128[i];
            f128 x = asfloat_u128(entry);

            /* - REFERENCE - */
            if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                error_printf("failed to clear floating-point exceptions mid-test!\n");
                goto fail;
            }
            f128 ref_r = ref_fn(x);
            int ref_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

            /* - TEST - */
            if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                error_printf("failed to clear floating-point exceptions mid-test!\n");
                goto fail;
            }
            f128 test_r = test_fn(x);
            int test_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

            bool pass_val   = is_equal_bitwise_ruled_f128(ref_r, test_r, are_nans_special);
            bool pass_flags = ref_e == test_e;
            bool pass       = pass_val && pass_flags;
            if (!pass) {
                error_printf("FAILED\n");
                error_printf("rounding direction: ");
                report_rounding_direction(rounding_direction, stderr);
                error_printf("at x = %f    ", (double)x);
                print_u128_hex(asuint_f128(x), stderr);

                error_printf("\nref(x):\n    %f    ", (double)ref_r); // we can't (and I won't) print %a, bit pattern's enough
                print_u128_hex(asuint_f128(ref_r), stderr);
                error_printf("\n");
                report_except_flag(ref_e, stderr);

                error_printf("test(x):\n    %f    ", (double)test_r);
                print_u128_hex(asuint_f128(test_r), stderr);
                error_printf("\n");
                report_except_flag(test_e, stderr);

                if (pass_val) {
                    error_printf("exception(s) mismatch!\n");
                } else {
                    error_printf("incorrect floating-point datum!\n");
                }
                void_fflush(stderr);

                goto fail;
            } // failure
        } // for

        PLATE
    }
#endif


/* -- Binary -- */

bool passes_list_binary_binary32(
    f32 (*ref_fn)(f32, f32),
    f32 (*test_fn)(f32, f32),
    const bool are_nans_special,
    const int rounding_direction
)
{
    BOILER

    for (size_t i = 0; i < list_binary32_count; i++) {
        for (size_t j = 0; j < list_binary32_count; j++) {
            u32 entry1 = table_list_binary32[i];
            u32 entry2 = table_list_binary32[j];
            f32 x = asfloat_u32(entry1);
            f32 y = asfloat_u32(entry2);

            /* - REFERENCE - */
            if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                error_printf("failed to clear floating-point exceptions mid-test!\n");
                goto fail;
            }
            f32 ref_r = ref_fn(x, y);
            int ref_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

            /* - TEST - */
            if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                error_printf("failed to clear floating-point exceptions mid-test!\n");
                goto fail;
            }
            f32 test_r = test_fn(x, y);
            int test_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

            bool pass_val   = is_equal_bitwise_ruled_f32(ref_r, test_r, are_nans_special);
            bool pass_flags = ref_e == test_e;
            bool pass       = pass_val && pass_flags;
            if (!pass) {
                error_printf("FAILED\n");
                error_printf("rounding direction: ");
                report_rounding_direction(rounding_direction, stderr);
                error_printf("at x = %f    %a    0x%08" PRIX32 "\n",
                    (double)x, (double)x, (uint32_t)asuint_f32(x));
                error_printf("at y = %f    %a    0x%08" PRIX32 "\n",
                    (double)y, (double)y, (uint32_t)asuint_f32(y));

                error_printf("ref(x, y):\n    %f    %a    0x%08" PRIX32 "\n",
                    (double)ref_r, (double)ref_r, (uint32_t)asuint_f32(ref_r));
                report_except_flag(ref_e, stderr);
        
                error_printf("test(x, y):\n    %f    %a    0x%08" PRIX32 "\n",
                    (double)test_r, (double)test_r, (uint32_t)asuint_f32(test_r));
                report_except_flag(test_e, stderr);

                if (pass_val) {
                    error_printf("exception(s) mismatch!\n");
                } else {
                    error_printf("incorrect floating-point datum!\n");
                }
                void_fflush(stderr);

                goto fail;
            } // failure
        } // inner for
    } // outer for

    PLATE
}


bool passes_list_binary_binary64(
    f64 (*ref_fn)(f64, f64),
    f64 (*test_fn)(f64, f64),
    const bool are_nans_special,
    const int rounding_direction
)
{
    BOILER

    for (size_t i = 0; i < list_binary64_count; i++) {
        for (size_t j = 0; j < list_binary64_count; j++) {
            u64 entry1 = table_list_binary64[i];
            u64 entry2 = table_list_binary64[j];
            f64 x = asfloat_u64(entry1);
            f64 y = asfloat_u64(entry2);

            /* - REFERENCE - */
            if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                error_printf("failed to clear floating-point exceptions mid-test!\n");
                goto fail;
            }
            f64 ref_r = ref_fn(x, y);
            int ref_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

            /* - TEST - */
            if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                error_printf("failed to clear floating-point exceptions mid-test!\n");
                goto fail;
            }
            f64 test_r = test_fn(x, y);
            int test_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

            bool pass_val   = is_equal_bitwise_ruled_f64(ref_r, test_r, are_nans_special);
            bool pass_flags = ref_e == test_e;
            bool pass       = pass_val && pass_flags;
            if (!pass) {
                error_printf("FAILED\n");
                error_printf("rounding direction: ");
                report_rounding_direction(rounding_direction, stderr);
                error_printf("at x = %f    %a    0x%016" PRIX64 "\n",
                    (double)x, (double)x, (uint64_t)asuint_f64(x));
                error_printf("at y = %f    %a    0x%016" PRIX64 "\n",
                    (double)y, (double)y, (uint64_t)asuint_f64(y));

                error_printf("ref(x, y):\n    %f    %a    0x%016" PRIX64 "\n",
                    (double)ref_r, (double)ref_r, (uint64_t)asuint_f64(ref_r));
                report_except_flag(ref_e, stderr);
        
                error_printf("test(x, y):\n    %f    %a    0x%016" PRIX64 "\n",
                    (double)test_r, (double)test_r, (uint64_t)asuint_f64(test_r));
                report_except_flag(test_e, stderr);

                if (pass_val) {
                    error_printf("exception(s) mismatch!\n");
                } else {
                    error_printf("incorrect floating-point datum!\n");
                }
                void_fflush(stderr);

                goto fail;
            } // failure
        } // inner for
    } // outer for

    PLATE
}

#if FINESWORD_TARGET_128BIT_FLOAT
    bool passes_list_binary_binary128(
        f128 (*ref_fn)(f128, f128),
        f128 (*test_fn)(f128, f128),
        const bool are_nans_special,
        const int rounding_direction
    )
    {
        BOILER

        for (size_t i = 0; i < list_binary128_count; i++) {
            for (size_t j = 0; j < list_binary128_count; j++) {
                u128 entry1 = table_list_binary128[i];
                u128 entry2 = table_list_binary128[j];
                f128 x = asfloat_u128(entry1);
                f128 y = asfloat_u128(entry2);

                /* - REFERENCE - */
                if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                    error_printf("failed to clear floating-point exceptions mid-test!\n");
                    goto fail;
                }
                f128 ref_r = ref_fn(x, y);
                int ref_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

                /* - TEST - */
                if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                    error_printf("failed to clear floating-point exceptions mid-test!\n");
                    goto fail;
                }
                f128 test_r = test_fn(x, y);
                int test_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);

                bool pass_val   = is_equal_bitwise_ruled_f128(ref_r, test_r, are_nans_special);
                bool pass_flags = ref_e == test_e;
                bool pass       = pass_val && pass_flags;
                if (!pass) {
                    error_printf("FAILED\n");
                    error_printf("rounding direction: ");
                    report_rounding_direction(rounding_direction, stderr);
                    error_printf("at x = %f    ", (double)x);
                    print_u128_hex(asuint_f128(x), stderr);
                    error_printf("\nat y = %f    ", (double)y);
                    print_u128_hex(asuint_f128(y), stderr);

                    error_printf("\nref(x, y):\n    %f    ", (double)ref_r);
                    print_u128_hex(asuint_f128(ref_r), stderr);
                    error_printf("\n");
                    report_except_flag(ref_e, stderr);

                    error_printf("test(x, y):\n    %f    ", (double)test_r);
                    print_u128_hex(asuint_f128(test_r), stderr);
                    error_printf("\n");
                    report_except_flag(test_e, stderr);

                    if (pass_val) {
                        error_printf("exception(s) mismatch!\n");
                    } else {
                        error_printf("incorrect floating-point datum!\n");
                    }
                    void_fflush(stderr);

                    goto fail;
                } // failure
            } // inner for
        } // outer for

        PLATE
    }
#endif
