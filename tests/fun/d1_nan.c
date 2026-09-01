// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/fun/all.h"

#include "finesword/config.h"
#include "finesword/rudiments/ieee754.h"
#include "finesword/rudiments/types.h"
#include "finesword/rudiments/astype.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/utilities.h"

#include <inttypes.h>
#include <fenv.h>
#pragma STDC FENV_ACCESS ON


bool d1_nan(const int argc, const char *const *const argv) {
    if (argc > 0) {
        warning_printf("ignoring argument(s)\n");
    }
    (void)argv;

    void_printf("--- sNaN/qNaN Polarity Runtime Test ---\n\n");

    info_printf("rounding direction: ");
    report_rounding_direction(fegetround(), stdout); // FIXED: made the function itself check for < 0
    void_printf("\n");

    fenv_t to_restore;
    if (feholdexcept(&to_restore) != 0) {
        error_printf("failed to install non-stop mode!\n");
        return false; /* failed to set */
    }
    /* https://gnats.netbsd.org/60514
     * broken on NetBSD/hppa, FreeBSD/RISC-V, and FreeBSD/AArch64.
     */
    if (feclearexcept(FE_ALL_EXCEPT) != 0) {
        error_printf("failed to clear all floating-point status flags!\n");
        goto fail;
    }

    info_printf("test binary32 0.0/0.0\n");

    volatile f32 zero = +0.0f;
    volatile f32 r    = zero / zero;

    int e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
    info_printf("excepts after:\n");
    report_except_flag(e, stdout);
    
    info_printf("check FE_INVALID\n");
    if ((e & FE_INVALID) == 0) {
    // if (e != FE_INVALID) {
        error_printf("FE_INVALID was not raised!\n");
        goto fail;
    }
    success_printf("ok\n");

    u32 bits = asuint_f32(r);
    u32 sign = (bits & F32_SIGN_MASK)   >> F32_SIGN_BIT_INDEX;
    u32 exp  = (bits & F32_EXP_MASK)    >> F32_MANT_WIDTH_T;
    u32 mant = (bits & F32_MANT_MASK);
    u32 d1   = (bits & F32_D1_BIT_MASK) >> ((u32)F32_MANT_WIDTH_T-U32_C(1));

    info_printf("result (qNaN) dissected:\n");
    void_printf("    bits: 0x%08" PRIX32 "\n", (uint32_t)bits);
    void_printf("    sign: 0x%08" PRIX32 "\n", (uint32_t)sign);
    void_printf("    exp : 0x%08" PRIX32 "\n", (uint32_t)exp);
    void_printf("    mant: 0x%08" PRIX32 "\n", (uint32_t)mant);
    void_printf("    d1  : 0x%08" PRIX32 "\n", (uint32_t)d1);

    info_printf("verify basic NaN properties\n");
    if (((bits & F32_EXP_MASK) != F32_EXP_MASK) || /* exp MUST be all ones */
        (mant == U32_C(0))) /* mantissa MUST be non-zero */
    {
        error_printf("NaN is inconsistent with IEEE 754!\n");
        goto fail;
    }
    if (d1 == U32_C(0)) { /* d1=qNaN is false */
        warning_printf("warning: the platform does not follow the recommended IEEE 754 qNaN convention\n");
        /* not a failure */
    }
    success_printf("ok\n");

    info_printf("validating the following compile-time macro against runtime result\n");
    void_printf("FINESWORD_BINARY_FP_D1_SET_IS_QUIET = %d\n", (int)FINESWORD_BINARY_FP_D1_SET_IS_QUIET);
    if (d1 != (uint32_t)FINESWORD_BINARY_FP_D1_SET_IS_QUIET) {
        error_printf("macro is inconsistent!\n");
        goto fail;
    }
    success_printf("ok\n");

    info_printf("checking binary32 qNaN/sNaN bit pattern ieee754.h macros\n");
    {
        volatile f32 qNaN = asfloat_u32(F32_MINPAY_POS_QNAN);
        volatile f32 sNaN = asfloat_u32(F32_MINPAY_POS_SNAN);

        if (feclearexcept(FE_ALL_EXCEPT) != 0) {
            error_printf("failed to clear all floating-point status flags!\n");
            goto fail;
        }

        volatile f32 r2 = qNaN + qNaN; /* addition(qNaN, qNaN) */
        e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
        if (e != 0) {
            error_printf("qNaN + qNaN raised floating-point exception(s)!\n");
            report_except_flag(e, stderr);
            goto fail;
        }
        (void)r2;

        volatile f32 r3 = sNaN + sNaN; /* addition(sNaN, sNaN) */
        e = fetestexcept(FE_ALL_IEEE754_EXCEPT);
        // if ((e & FE_INVALID) == 0) {
        if (((e & FE_INVALID) == 0) || ((e & ~FE_INVALID) != 0)) {
            // error_printf("sNaN + sNaN did not raise FE_INVALID!\n");
            error_printf("sNaN + sNaN did not raise only FE_INVALID!\n");
            report_except_flag(e, stderr);
            goto fail;
        }
        (void)r3;
    }
    success_printf("ok\n");

    /* restore */
    if (fesetenv(&to_restore) != 0) {
        error_printf("failed to restore floating-point environment!\n");
        return false; /* failed to restore */
    }
    
    return true; /* success */

fail:
    /* fail and restore env */
    if (fesetenv(&to_restore) != 0) {
        error_printf("failed to restore floating-point environment in error path!\n");
        return false; /* fail */
    }
    return false; /* fail */
}
