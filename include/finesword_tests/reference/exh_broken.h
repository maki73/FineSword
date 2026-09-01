// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_REFERENCE_EXH_BROKEN_H
#define FINESWORD_TESTS_REFERENCE_EXH_BROKEN_H

#include "finesword/rudiments/types.h"
#include "finesword/config.h"


/* Broken (and couple of reference) Functions for Exhaustive Test Tests
 */

/* NOTE: when calling feraiseexcept() with FE_UNDERFLOW/FE_OVERFLOW
 * whether it also raises FE_INEXACT is implementation-defined.
 * [C99 \S J.3.12]
 * NOTE: faulty ranges of the functions below
 * contain 101 bit patterns rather than 100. 
 */

// NOTE: It ain't DRY

/*  --- Binary32 --- */

// correct abs
f32 abs_reference_f32(f32 x);

// abs; spuriously signals FE_UNDERFLOW for x in [0x81234567, 0x812345CB]
f32 abs_spurious_underflow_0x81234567_to_0x812345CB_f32(f32 x);

// abs; return negative value for x in [0x81234567, 0x812345CB]
f32 abs_return_negative_0x81234567_to_0x812345CB_f32(f32 x);

// abs; spuriously signal FE_INVALID if x===0x62697473 and rounding direction FE_DOWNWARD
f32 abs_spurious_invalid_0x62697473_under_downward_f32(f32 x);

// abs; return and signal nonsense for the last bit pattern
f32 abs_return_and_signal_nonsense_for_0xFFFFFFFF_f32(f32 x);

// abs; return and signal nonsense for the first bit pattern
f32 abs_return_and_signal_nonsense_for_0x00000000_f32(f32 x);

// optional
#if FINESWORD_TARGET_16BIT_FLOAT
    /* --- Binary16 --- */

    /* -- Unary -- */

    // correct abs
    f16 abs_reference_f16(f16 x);

    // abs; spuriously signals FE_UNDERFLOW for x in [0x8123, 0x8187]
    f16 abs_spurious_underflow_0x8123_to_0x8187_f16(f16 x);

    // abs; return negative value for x in [0x8123, 0x8187]
    f16 abs_return_negative_0x8123_to_0x8187_f16(f16 x);

    // abs; spuriously signal FE_INVALID if x===0x3031 and rounding direction FE_DOWNWARD
    f16 abs_spurious_invalid_0x3031_under_downward_f16(f16 x);

    // abs; return and signal nonsense for the last bit pattern
    f16 abs_return_and_signal_nonsense_for_0xFFFF_f16(f16 x);

    // abs; return and signal nonsense for the first bit pattern
    f16 abs_return_and_signal_nonsense_for_0x0000_f16(f16 x);

    /* -- Binary -- */

    // correct copysign
    f16 copysign_reference_f16(f16 x, f16 y);

    // - Boundaries

    // copysign; return and signal nonsense for x===0xFFFF y===0xFFFF
    f16 copysign_return_and_signal_nonsense_for_x_0xFFFF_y_0xFFFF_f16(f16 x, f16 y);

    // copysign; return and signal nonsense for x===0x0000 y===0x0000
    f16 copysign_return_and_signal_nonsense_for_x_0x0000_y_0x0000_f16(f16 x, f16 y);

    // copysign; return and signal nonsense for x===0x0000 y===0xFFFF
    f16 copysign_return_and_signal_nonsense_for_x_0x0000_y_0xFFFF_f16(f16 x, f16 y);

    // copysign; return and signal nonsense for x===0xFFFF y===0x0000
    f16 copysign_return_and_signal_nonsense_for_x_0xFFFF_y_0x0000_f16(f16 x, f16 y);

    // - exception & datum catching

    // copysign; spuriously signal FE_UNDERFLOW for x in [0x8123, 0x8187] y in  [0x3031, 0x3095]
    f16 copysign_spurious_underflow_x_0x8123_to_0x8187_y_0x3031_to_0x3095_f16(f16 x, f16 y);

    // copysign; return negative for x in [0x8123, 0x8187] y in [0x3031, 0x3095]
    f16 copysign_return_negative_x_0x8123_to_0x8187_y_0x3031_to_0x3095_f16(f16 x, f16 y);

    // copysign; spuriously signal FE_INEXACT if x===0x3031 y===0x3031 and rounding direction FE_TOWARDZERO
    f16 copysign_spurious_inexact_for_x_0x3031_y_0x3031_under_towardzero_f16(f16 x, f16 y);
#endif

#endif /* FINESWORD_TESTS_REFERENCE_EXH_BROKEN_H */
