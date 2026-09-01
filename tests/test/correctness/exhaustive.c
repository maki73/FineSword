// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/test/correctness/exhaustive.h"
#include "finesword_tests/shared/utilities.h"
#include "finesword_tests/shared/print.h"
#include "finesword/rudiments/types.h"
#include "finesword/rudiments/astype.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <fenv.h>
#pragma STDC FENV_ACCESS ON /* required */

/* can work without OpenMP (single-threaded) */
#ifdef _OPENMP
    #if _OPENMP < 200203
        #error "in multithreaded mode this code requires OpenMP 2.0 or later! \
            try compiling in sequential mode when newer OpenMP is unavailable."
    #endif
    // #include <omp.h>
#endif


/* -- Written for OpenMP 2.0+ -- */

/* Exhaustively provable (within general feasibility limits) functions are:
 * f: Binary32 -> Binary32
 * f: Binary16 -> Binary16
 * f: Binary16 x Binary16 -> Binary16.
 */


/* Threads will check the global "did we already fail?" status each N iterations;
 MUST be a power of two minus one, or more precisely
 (i % N == 0 <-> i & MASK == 0)
 */
#define U32_CHECKN_FAILED_MASK U32_C(0x3FF) /* 1024 iterations */
// #define U32_CHECKN_FAILED_MASK U32_C(4095)


/* f: Binary32 -> Binary32; space: 2^32; multithreaded */
bool passes_exhaustive_unary_binary32(
    f32 (*ref_fn)(f32),
    f32 (*test_fn)(f32),
    const bool are_nans_special,
    const bool is_quietcomputational,
    const int rounding_direction
)
{
    if ((ref_fn == NULL) || (test_fn == NULL)) {
        error_printf("invalid function pointer(s)!\n");
        return false; /* fail */
    }
    if (!is_rounding_direction_valid(rounding_direction)) {
        error_printf("invalid rounding direction!\n");
        return false; /* fail */
    }

    // GLOBAL; access critically
    bool test_failed = false;

    // FIX: explicitly make stderr a variable
    FILE *err = stderr; 

    #pragma omp parallel default(none) shared(test_failed, \
        are_nans_special, is_quietcomputational, rounding_direction, ref_fn, test_fn, err)
    {
        /* setup */
        fenv_t to_restore_env;
        bool have_env = false;
        if (feholdexcept(&to_restore_env) == 0) {
            have_env = true;
            if (fesetround(rounding_direction) == 0) {
                if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                    error_printf("thread failed to clear floating-point exceptions!\n");
                    #pragma omp critical
                    {
                        test_failed = true;
                    }
                }
            } else {
                error_printf("thread failed to set rounding direction!\n");
                #pragma omp critical
                {
                    test_failed = true;
                }
            }
        } else {
            error_printf("thread failed to set floating-point environment!\n");
            #pragma omp critical
            {
                test_failed = true;
            }

        }
        /* NOTE: if the setup above failed, threads will still enter the
         main loop running for N iterations before querying test_failed
         and skipping (or cancelling) the remaining iterations.
         */

        // private variables
        bool dirty = false;
        bool stop  = false;
        i64 i; // OpenMP 2.0 \S 2.4.1 "the for loop iteration variable MUST have a signed integer type" 
        #pragma omp for schedule(static)
        for (i = 0; i <= (i64)UINT32_MAX; i++) {
            u32 i_32 = (u32)i; // safe

            // enter critical section only once per N iterations
            if ((i_32 & U32_CHECKN_FAILED_MASK) == U32_C(0)) {
                #pragma omp critical
                {
                    stop = test_failed;
                }
            }
            if (stop) {
                /* if supported, try cancelling (it's a newer OpenMP 4.0+ feature)
                 else, just skip */
                 #if defined(_OPENMP) && _OPENMP >= 201307
                    #pragma omp cancel for
                #endif
                continue;
            }

            f32 x = asfloat_u32(i_32); /* SHAN'T signal exceptions per contract */
            
            /* - REFERENCE - */
            if (dirty) {
                if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                    #pragma omp critical
                    {
                        test_failed = true;
                    }
                    stop = true;
                    continue;
                }
                dirty = false;
            }
            f32 ref_r = ref_fn(x);
            int ref_e = 0;
            if (!is_quietcomputational) {
                ref_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
                if (ref_e != 0) {
                    dirty = true;
                }
            }

            /* - TEST - */
            if (dirty) {
                if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                    #pragma omp critical
                    {
                        test_failed = true;
                    }
                    stop = true;
                    continue;
                }
                dirty = false;
            }
            f32 test_r = test_fn(x);
            int test_e = 0;
            if (!is_quietcomputational) {
                test_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
                if (test_e != 0) {
                    dirty = true;
                }
            }

            /* - COMPARE - */
            bool pass_val   = is_equal_bitwise_ruled_f32(ref_r, test_r, are_nans_special); /* SHALL signal no exceptions per contract */
            bool pass_flags = ref_e    == test_e;
            bool pass       = pass_val && pass_flags;
            if (!pass) {
                bool prev_test_failed;
                
                #pragma omp critical
                { /* CAS-like */
                    prev_test_failed = test_failed;
                    if (!test_failed) {
                        test_failed =  true;
                    }
                }
                stop = true;

                if (!prev_test_failed) { /* only the first failed input is print */
                    error_printf("FAILED\n");
                    error_printf("rounding direction: ");
                    report_rounding_direction(rounding_direction, err);
                    error_printf("at x = %f    %a    0x%08" PRIX32 "\n",
                        (double)x, (double)x, (uint32_t)asuint_f32(x));

                    error_printf("ref(x):\n    %f    %a    0x%08" PRIX32 "\n",
                        (double)ref_r, (double)ref_r, (uint32_t)asuint_f32(ref_r));
                    if (!is_quietcomputational) {  // reporting exceptions is useless otherwise - they are not captured 
                        report_except_flag(ref_e, err);
                    }

                    error_printf("test(x):\n    %f    %a    0x%08" PRIX32 "\n",
                        (double)test_r, (double)test_r, (uint32_t)asuint_f32(test_r));
                    if (!is_quietcomputational) {  
                        report_except_flag(test_e, err);
                    }
                    
                    if (pass_val) {
                        error_printf("exception(s) mismatch!\n");
                    } else {
                        error_printf("incorrect floating-point datum!\n");
                    }
                    // maybe add an explicit fflush(err)?
                    void_fflush(err);
                } // failure report
            } // failure 
        } // for

        /* if quietcomputational=true, since floating-point status flags are sticky we can
         check their status only once at the end rather than querying it after each call
         */
        int end_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
        if (is_quietcomputational && (end_e != 0)) {
            bool prev_test_failed;
                
            #pragma omp critical
            { /* CAS-like */
                prev_test_failed = test_failed;
                if (!test_failed) {
                    test_failed =  true;
                }
            }
            stop = true;

            if (!prev_test_failed) { /* only the first thread */
                error_printf("FAILED\n");
                error_printf("rounding direction: ");
                report_rounding_direction(rounding_direction, err);
                error_printf("cumulative excepts:\n");
                report_except_flag(end_e, err);
                error_printf("spurious exception(s) from a quietcomputational function!\n");
                void_fflush(err);
            } // failure report
        } // failure

        // restore env if feholdexcept() didn't fail
        if (have_env) {
            if (fesetenv(&to_restore_env) != 0) {
                error_printf("thread failed to restore floating-point environment!\n");
                #pragma omp critical
                {
                    test_failed = true;
                }
            }
        }
    } // parallel extent

    return !test_failed;
}

// optional
#if FINESWORD_TARGET_16BIT_FLOAT

/* f: Binary16 -> Binary16; space: 2^16; single-threaded */
bool passes_exhaustive_unary_binary16(
    f16 (*ref_fn)(f16),
    f16 (*test_fn)(f16),
    const bool are_nans_special,
    const bool is_quietcomputational,
    const int rounding_direction
)
{
    if ((ref_fn == NULL) || (test_fn == NULL)) {
        error_printf("invalid function pointer(s)!\n");
        return false; /* fail */
    }
    if (!is_rounding_direction_valid(rounding_direction)) {
        error_printf("invalid rounding direction!\n");
        return false; /* fail */
    }

    /* setup */ 
    fenv_t to_restore;
    if (feholdexcept(&to_restore) != 0) {
        error_printf("failed to set floating-point environment!\n");
        return false; /* fail */
    }
    if (fesetround(rounding_direction) != 0) {
        error_printf("failed to set rounding direction!\n");
        goto fail;
    }
    if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
        error_printf("failed to clear floating-point exceptions!\n");
        goto fail;
    }

    bool dirty = false;
    for (u32 i = 0; i <= (u32)UINT16_MAX; i++) {
        u16 i_16 = (u16)i; // safe
        f16 x = asfloat_u16(i_16);

        /* - REFERENCE - */
        if (dirty) {
            if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                error_printf("failed to clear floating-point exceptions mid-test!\n");
                goto fail;
            }
            dirty = false;
        }
        f16 ref_r = ref_fn(x);
        int ref_e = 0;
        if (!is_quietcomputational) {
            ref_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
            if (ref_e != 0) {
                dirty = true;
            }
        }

        /* - TEST - */
        if (dirty) {
            if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                error_printf("failed to clear floating-point exceptions mid-test!\n");
                goto fail;
            }
            dirty = false;
        }
        f16 test_r = test_fn(x);
        int test_e = 0;
        if (!is_quietcomputational) {
            test_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
            if (test_e != 0) {
                dirty = true;
            }
        }

        /* - COMPARE - */
        bool pass_val   = is_equal_bitwise_ruled_f16(ref_r, test_r, are_nans_special); /* SHALL signal no exceptions per contract */
        bool pass_flags = ref_e == test_e;
        bool pass       = pass_val && pass_flags;
        if (!pass) {
            error_printf("FAILED\n");
            error_printf("rounding direction: ");
            report_rounding_direction(rounding_direction, stderr);
            error_printf("at x = %f    %a    0x%04" PRIX16 "\n",
                (double)x, (double)x, (uint16_t)asuint_f16(x));

            error_printf("ref(x):\n    %f    %a    0x%04" PRIX16 "\n",
                (double)ref_r, (double)ref_r, (uint16_t)asuint_f16(ref_r));
            if (!is_quietcomputational) { // reporting exceptions is useless otherwise - they are not captured
                report_except_flag(ref_e, stderr);
            }

            error_printf("test(x):\n    %f    %a    0x%04" PRIX16 "\n",
                (double)test_r, (double)test_r, (uint16_t)asuint_f16(test_r));
            if (!is_quietcomputational) {
                report_except_flag(test_e, stderr);
            }

            if (pass_val) {
                error_printf("exception(s) mismatch!\n");
            } else {
                error_printf("incorrect floating-point datum!\n");
            }
            void_fflush(stderr);

            goto fail;
        } // failure
    } // for

    int end_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
    if (is_quietcomputational && (end_e != 0)) {
        error_printf("FAILED\n");
        error_printf("rounding direction: ");
        report_rounding_direction(rounding_direction, stderr);
        error_printf("cumulative excepts:\n");
        report_except_flag(end_e, stderr);
        error_printf("spurious exception(s) from a quietcomputational function!\n");
        void_fflush(stderr);
        
        goto fail;
    } // failure

    // restore env
    if (fesetenv(&to_restore) != 0) {
        error_printf("failed to restore floating-point environment!\n");
        return false; /* fail */
    }

    return true; /* pass */

fail:
    if (fesetenv(&to_restore) != 0) {
        error_printf("failed to restore floating-point environment in error path!\n");
        return false; /* fail */
    }
    return false; /* fail */
}


// NOTE: not DRY relative to passes_exhaustive_unary_binary32()

/* f: Binary16 x Binary16 -> Binary16; space: 2^32; multithreaded */
bool passes_exhaustive_binary_binary16(
    f16 (*ref_fn)(f16, f16),
    f16 (*test_fn)(f16, f16),
    const bool are_nans_special,
    const bool is_quietcomputational,
    const int rounding_direction
)
{
    if ((ref_fn == NULL) || (test_fn == NULL)) {
        error_printf("invalid function pointer(s)!\n");
        return false; /* fail */
    }
    if (!is_rounding_direction_valid(rounding_direction)) {
        error_printf("invalid rounding direction!\n");
        return false; /* fail */
    }

    // GLOBAL; access critically
    bool test_failed = false;

    FILE *err = stderr;

    #pragma omp parallel default(none) shared(test_failed, \
        are_nans_special, is_quietcomputational, rounding_direction, ref_fn, test_fn, err)
    {
        /* setup */
        fenv_t to_restore_env;
        bool have_env = false;
        if (feholdexcept(&to_restore_env) == 0) {
            have_env = true;
            if (fesetround(rounding_direction) == 0) {
                if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                    error_printf("thread failed to clear floating-point exceptions!\n");
                    #pragma omp critical
                    {
                        test_failed = true;
                    }
                }
            } else {
                error_printf("thread failed to set rounding direction!\n");
                #pragma omp critical
                {
                    test_failed = true;
                }
            }
        } else {
            error_printf("thread failed to set floating-point environment!\n");
            #pragma omp critical
            {
                test_failed = true;
            }

        }

        // private variables
        bool dirty = false;
        bool stop  = false;
        /* 2D -> 1D; S_16 \times S_16 \cong S_32 where S_N is the set of all natural numbers < 2^N */
        i64 i;
        #pragma omp for schedule(static)
        for (i = 0; i <= (i64)UINT32_MAX; i++) {
            u32 i_32 = (u32)i; // safe
            u32 lo = i_32 & U32_C(0x0000FFFF);
            u32 hi = i_32 >> 16U; // top's zeros

            // N iterations of the loop
            if ((i_32 & U32_CHECKN_FAILED_MASK) == U32_C(0)) {
                #pragma omp critical
                {
                    stop = test_failed;
                }
            }
            if (stop) {
                 #if defined(_OPENMP) && _OPENMP >= 201307
                    #pragma omp cancel for
                #endif
                continue;
            }

            u16 lo_16 = (u16)lo;
            u16 hi_16 = (u16)hi;
            f16 x = asfloat_u16(lo_16);
            f16 y = asfloat_u16(hi_16);

            /* - REFERENCE - */
            if (dirty) {
                if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                    #pragma omp critical
                    {
                        test_failed = true;
                    }
                    stop = true;
                    continue;
                }
                dirty = false;
            }
            f16 ref_r = ref_fn(x, y);
            int ref_e = 0;
            if (!is_quietcomputational) {
                ref_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
                if (ref_e != 0) {
                    dirty = true;
                }
            }

            /* - TEST - */
            if (dirty) {
                if (feclearexcept(FE_ALL_IEEE754_EXCEPT) != 0) {
                    #pragma omp critical
                    {
                        test_failed = true;
                    }
                    stop = true;
                    continue;
                }
                dirty = false;
            }
            f16 test_r = test_fn(x, y);
            int test_e = 0;
            if (!is_quietcomputational) {
                test_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
                if (test_e != 0) {
                    dirty = true;
                }
            }

            /* - COMPARE - */
            bool pass_val   = is_equal_bitwise_ruled_f16(ref_r, test_r, are_nans_special);
            bool pass_flags = ref_e    == test_e;
            bool pass       = pass_val && pass_flags;
            if (!pass) {
                bool prev_test_failed;
                
                #pragma omp critical
                {
                    prev_test_failed = test_failed;
                    if (!test_failed) {
                        test_failed =  true;
                    }
                }
                stop = true;

                if (!prev_test_failed) {
                    error_printf("FAILED\n");
                    error_printf("rounding direction: ");
                    report_rounding_direction(rounding_direction, err);
                    error_printf("at x = %f    %a    0x%04" PRIX16 "\n",
                        (double)x, (double)x, (uint16_t)asuint_f16(x));
                    error_printf("at y = %f    %a    0x%04" PRIX16 "\n",
                        (double)y, (double)y, (uint16_t)asuint_f16(y));

                    error_printf("ref(x, y):\n    %f    %a    0x%04" PRIX16 "\n",
                        (double)ref_r, (double)ref_r, (uint16_t)asuint_f16(ref_r));
                    if (!is_quietcomputational) {
                        report_except_flag(ref_e, err);
                    }
                    error_printf("test(x, y):\n    %f    %a    0x%04" PRIX16 "\n",
                        (double)test_r, (double)test_r, (uint16_t)asuint_f16(test_r));
                    if (!is_quietcomputational) {
                        report_except_flag(test_e, err);
                    }
                    
                    if (pass_val) {
                        error_printf("exception(s) mismatch!\n");
                    } else {
                        error_printf("incorrect floating-point datum!\n");
                    }
                    // maybe add an explicit fflush(err)?
                    void_fflush(err);
                } // failure report
            } // failure 
        } // for


        int end_e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
        if (is_quietcomputational && (end_e != 0)) {
            bool prev_test_failed;
                
            #pragma omp critical
            {
                prev_test_failed = test_failed;
                if (!test_failed) {
                    test_failed =  true;
                }
            }
            stop = true;

            if (!prev_test_failed) { /* only the first thread */
                error_printf("FAILED\n");
                error_printf("rounding direction: ");
                report_rounding_direction(rounding_direction, err);
                error_printf("cumulative excepts:\n");
                report_except_flag(end_e, err);
                error_printf("spurious exception(s) from a quietcomputational function!\n");
                void_fflush(err);
            } // failure report
        } // failure

        if (have_env) {
            if (fesetenv(&to_restore_env) != 0) {
                error_printf("thread failed to restore floating-point environment!\n");
                #pragma omp critical
                {
                    test_failed = true;
                }
            }
        }
    } // parallel extent

    return !test_failed;
}

#endif /* FINESWORD_TARGET_16BIT_FLOAT */
