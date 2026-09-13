// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_SHARED_WRAPPER_H
#define FINESWORD_TESTS_SHARED_WRAPPER_H

#include "finesword/rudiments/types.h"
#include "finesword/rudiments/template.h"


/* Metaprogramming helper(s) to generate wrappers 
 * for predicates
 * 
 * use GEN_INLINE_WRAPPER_FORALL_FN(name)
 * where name is (as an example) 'isnan', not 'isnan_f'
 */

#define DEFINE_INLINE_WRAPPER_SINGLE_FN(N, name)       \
static inline f##N w_##name##_f##N (f##N x) {          \
    return asfloat_u##N((u##N)((bool)name##_f##N(x))); \
} 


#if !FINESWORD_TARGET_16BIT_FLOAT && !FINESWORD_TARGET_128BIT_FLOAT
    #define GEN_INLINE_WRAPPER_FORALL_FN(name)    \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(32, name) \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(64, name)
#elif FINESWORD_TARGET_16BIT_FLOAT && FINESWORD_TARGET_128BIT_FLOAT
    #define GEN_INLINE_WRAPPER_FORALL_FN(name)    \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(32, name) \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(64, name) \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(16, name) \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(128, name)
#elif FINESWORD_TARGET_16BIT_FLOAT && !FINESWORD_TARGET_128BIT_FLOAT
    #define GEN_INLINE_WRAPPER_FORALL_FN(name)    \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(32, name) \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(64, name) \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(16, name)
#elif !FINESWORD_TARGET_16BIT_FLOAT && FINESWORD_TARGET_128BIT_FLOAT
    #define GEN_INLINE_WRAPPER_FORALL_FN(name)    \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(32, name) \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(64, name) \
        DEFINE_INLINE_WRAPPER_SINGLE_FN(128, name)
#endif

#endif /* FINESWORD_TESTS_SHARED_WRAPPER_H */
