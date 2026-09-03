// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword/scalar/sign.h"

#include "finesword/rudiments/astype.h"
#include "finesword/rudiments/ieee754.h"
#include "finesword/rudiments/template.h"


#define X(x) FINESWORD_TEMPLATE_SPECIALIZE(x)

#if defined(__clang_major__) && __clang_major__ < 22
    #define Y volatile // optimizing compiler tax
#else
    #define Y
#endif

/* --- negate() --- */

#define TEMPLATE_NEGATE_FN_1(N) \
f##N negate_f##N (f##N x) {     \
    Y u##N xi = asuint_f##N (x);  \
    xi ^= F##N##_SIGN_MASK;     \
    return asfloat_u##N (xi);   \
}
X(TEMPLATE_NEGATE_FN_1)

/* --- abs() --- */

#define TEMPLATE_ABS_FN_1(N)   \
f##N abs_f##N (f##N x) {       \
    Y u##N xi = asuint_f##N (x); \
    xi &= F##N##_ABS_MASK;     \
    return  asfloat_u##N (xi); \
}
X(TEMPLATE_ABS_FN_1)

/* --- copySign() --- */

#define TEMPLATE_COPYSIGN_FN_1(N)     \
f##N copysign_f##N (f##N x, f##N y) { \
    Y u##N xi = asuint_f##N (x);        \
    Y u##N yi = asuint_f##N (y);        \
    xi &= F##N##_ABS_MASK;            \
    yi &= F##N##_SIGN_MASK;           \
    xi |= yi;                         \
    return  asfloat_u##N (xi);        \
}
X(TEMPLATE_COPYSIGN_FN_1)
