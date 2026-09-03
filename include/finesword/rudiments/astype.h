// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_RUDIMENTS_ASTYPE_H
#define FINESWORD_RUDIMENTS_ASTYPE_H

#include "finesword/config.h"
#include "finesword/rudiments/types.h"
#include "finesword/rudiments/template.h"

/* Reinterpretation Utilities
 *
 * All operations here SHALL signal no exceptions per contract.
 * NOTE: (nonnormative) [C99 \S 6.5.2.3(82)]
 */

#if defined(__clang_major__) && __clang_major__ < 22
    #define X volatile // optimizing compiler tax
#else
    #define X
#endif


#define FINESWORD_TEMPLATE_UNION_ASUINT_FN_1(N) \
static inline u##N asuint_f##N (f##N x) {       \
    union { X f##N f; X u##N i; } u;            \
    u.f = x;                                    \
    return u.i;                                 \
}

#define FINESWORD_TEMPLATE_UNION_ASFLOAT_UN_1(N) \
static inline f##N asfloat_u##N (u##N x) {       \
    union { X f##N f; X u##N i; } u;             \
    u.i = x;                                     \
    return u.f;                                  \
}

#define FINESWORD_TEMPLATE_MEMCPY_ASUINT_FN_1(N) \
static inline u##N asuint_f##N (f##N x) {        \
    X u##N i;                                    \
    (void) memcpy(&i, &x, sizeof(i));            \
    return i;                                    \
}

#define FINESWORD_TEMPLATE_MEMCPY_ASFLOAT_UN_1(N) \
static inline f##N asfloat_u##N (u##N x) {        \
    X f##N f;                                     \
    (void) memcpy(&f, &x, sizeof(f));             \
    return f;                                     \
}

#define FORALL(x) FINESWORD_TEMPLATE_SPECIALIZE(x)

#if FINESWORD_ASTYPE_USE_MEMCPY
    #include <string.h>
    FORALL(FINESWORD_TEMPLATE_MEMCPY_ASUINT_FN_1)
    FORALL(FINESWORD_TEMPLATE_MEMCPY_ASFLOAT_UN_1)
#else
    FORALL(FINESWORD_TEMPLATE_UNION_ASUINT_FN_1)
    FORALL(FINESWORD_TEMPLATE_UNION_ASFLOAT_UN_1)
#endif

#undef FORALL
#undef FINESWORD_TEMPLATE_UNION_ASUINT_FN_1
#undef FINESWORD_TEMPLATE_UNION_ASFLOAT_UN_1
#undef FINESWORD_TEMPLATE_MEMCPY_ASUINT_FN_1
#undef FINESWORD_TEMPLATE_MEMCPY_ASFLOAT_UN_1

#undef X

#endif /* FINESWORD_RUDIMENTS_ASTYPE_H */
