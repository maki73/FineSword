// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_SCALAR_CLASSIFY_H
#define FINESWORD_SCALAR_CLASSIFY_H

#include "finesword/rudiments/types.h"
#include "finesword/rudiments/template.h"

/* Classification Predicates
 *
 * requirement: SHALL
 * class: non-computational
 * reference: [IEEE 754 \S 5.7.2]
 *
 * additions:
 * - isquiet_fN()
 * Rationale:
 *  defining both isSignaling() and 'isQuiet()'
 *  makes the interface more symmetric.
 */


#define NQ1(name) \
    FINESWORD_PUSH_SIGNATURES_PREDICATE_UNARY (name, f, x)
#define NQ2(name) \
    FINESWORD_PUSH_SIGNATURES_PREDICATE_BINARY(name, f, x, y)

/* --- isZero() --- */
NQ1(iszero)

/* --- isSignMinus() --- */
NQ1(issignminus)

/* --- isNormal() --- */
NQ1(isnormal)

/* --- isSubnormal() --- */
NQ1(issubnormal)

/* --- isFinite() --- */
NQ1(isfinite)

/* --- isInfinite() --- */
NQ1(isinfinite)

/* --- isNaN() --- */
NQ1(isnan)

/* --- isSignaling() --- */
NQ1(issignaling)

/* --- + isQuiet() --- */
NQ1(isquiet)

/* --- isCanonical() & radix() --- */
/* 1. [IEEE 754 \S 3.4]
 *  "In binary interchange formats, all number and NaN encodings are canonical."
 * 2. fN types are binaryN by definition (i.e., their radix is 2)
 */

// /* --- totalOrder() --- */
// TODO: NQ2(totalorder, f, x, y)

// /* --- totalOrderMag() --- */
// TODO: NQ2(totalordermag, f, x, y)

// /* --- class() --- */
// #define FP_DATUMS_CLASS_SNAN
// #define FP_DATUMS_CLASS_QNAN
// #define FP_DATUMS_CLASS_NEG_INF
// #define FP_DATUMS_CLASS_NEG_NORMAL
// #define FP_DATUMS_CLASS_NEG_SUBNORMAL
// #define FP_DATUMS_CLASS_NEG_ZERO
// #define FP_DATUMS_CLASS_POS_ZERO
// #define FP_DATUMS_CLASS_POS_SUBNORMAL
// #define FP_DATUMS_CLASS_POS_NORMAL
// #define FP_DATUMS_CLASS_POS_INF


#undef NQ1
#undef NQ2

#endif /* FINESWORD_SCALAR_CLASSIFY_H */
