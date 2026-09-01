// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_SCALAR_SIGN_H
#define FINESWORD_SCALAR_SIGN_H

#include "finesword/rudiments/types.h"
#include "finesword/config.h"
#include "finesword/rudiments/template.h"

/* Sign Bit Operations
 *
 * requirement: SHALL
 * class: homogeneous quiet-computational
 * reference: [IEEE 754 \S 5.5.1]
 */


/* copy()
 Basically useless in C.
 To y = copy(x) just do fN y = x.
*/

/* --- negate() --- */
FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_UNARY(negate, f, x)

/* --- abs() --- */
FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_UNARY(abs, f, x)

/* --- copySign() --- */
FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_BINARY(copysign, f, x, y)

#endif /* FINESWORD_SCALAR_SIGN_H */
