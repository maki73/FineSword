// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_RUDIMENTS_TEMPLATE_H
#define FINESWORD_RUDIMENTS_TEMPLATE_H

#include "finesword/config.h"

/* Metaprogramming Helpers for Templates
 */

/* Provides:
 FINESWORD_TEMPLATE_SPECIALIZE
 FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_UNARY
 FINESWORD_PUSH_SIGNATURES_PREDICATE_UNARY
 FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_BINARY
 FINESWORD_PUSH_SIGNATURES_PREDICATE_BINARY
 */

#if !FINESWORD_TARGET_16BIT_FLOAT && !FINESWORD_TARGET_128BIT_FLOAT
    #define FINESWORD_TEMPLATE_SPECIALIZE(tmpl) \
        tmpl(32)                                \
        tmpl(64)

    #define FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_UNARY(name, prefix, x) \
        prefix##32  name##_##prefix##32(prefix##32   x);                 \
        prefix##64  name##_##prefix##64(prefix##64   x);

    #define FINESWORD_PUSH_SIGNATURES_PREDICATE_UNARY(name, prefix, x) \
        bool name##_##prefix##32(prefix##32 x);                        \
        bool name##_##prefix##64(prefix##64 x);

    #define FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_BINARY(name, prefix, x, y) \
        prefix##32 name##_##prefix##32(prefix##32 x, prefix##32 y);          \
        prefix##64 name##_##prefix##64(prefix##64 x, prefix##64 y);

    #define FINESWORD_PUSH_SIGNATURES_PREDICATE_BINARY(name, prefix, x, y) \
        bool name##_##prefix##32(prefix##32 x, prefix##32 y);              \
        bool name##_##prefix##64(prefix##64 x, prefix##64 y);
#elif FINESWORD_TARGET_16BIT_FLOAT && FINESWORD_TARGET_128BIT_FLOAT
    #define FINESWORD_TEMPLATE_SPECIALIZE(tmpl) \
        tmpl(32)                                \
        tmpl(64)                                \
        tmpl(16)                                \
        tmpl(128)

    #define FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_UNARY(name, prefix, x) \
        prefix##32  name##_##prefix##32(prefix##32   x);                 \
        prefix##64  name##_##prefix##64(prefix##64   x);                 \
        prefix##16  name##_##prefix##16(prefix##16   x);                 \
        prefix##128 name##_##prefix##128(prefix##128 x);

    #define FINESWORD_PUSH_SIGNATURES_PREDICATE_UNARY(name, prefix, x) \
        bool name##_##prefix##32(prefix##32   x);                      \
        bool name##_##prefix##64(prefix##64   x);                      \
        bool name##_##prefix##16(prefix##16   x);                      \
        bool name##_##prefix##128(prefix##128 x);

    #define FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_BINARY(name, prefix, x, y) \
        prefix##32  name##_##prefix##32(prefix##32   x, prefix##32  y);      \
        prefix##64  name##_##prefix##64(prefix##64   x, prefix##64  y);      \
        prefix##16  name##_##prefix##16(prefix##16   x, prefix##16  y);      \
        prefix##128 name##_##prefix##128(prefix##128 x, prefix##128 y);

    #define FINESWORD_PUSH_SIGNATURES_PREDICATE_BINARY(name, prefix, x, y) \
        bool name##_##prefix##32(prefix##32   x, prefix##32  y);           \
        bool name##_##prefix##64(prefix##64   x, prefix##64  y);           \
        bool name##_##prefix##16(prefix##16   x, prefix##16  y);           \
        bool name##_##prefix##128(prefix##128 x, prefix##128 y);
#elif FINESWORD_TARGET_16BIT_FLOAT && !FINESWORD_TARGET_128BIT_FLOAT
    #define FINESWORD_TEMPLATE_SPECIALIZE(tmpl) \
        tmpl(32)                                \
        tmpl(64)                                \
        tmpl(16)

    #define FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_UNARY(name, prefix, x) \
        prefix##32  name##_##prefix##32(prefix##32   x);                 \
        prefix##64  name##_##prefix##64(prefix##64   x);                 \
        prefix##16  name##_##prefix##16(prefix##16   x);

    #define FINESWORD_PUSH_SIGNATURES_PREDICATE_BINARY(name, prefix, x, y) \
        bool name##_##prefix##32(prefix##32 x, prefix##32 y);              \
        bool name##_##prefix##64(prefix##64 x, prefix##64 y);              \
        bool name##_##prefix##16(prefix##16 x, prefix##16 y);

    #define FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_BINARY(name, prefix, x, y) \
        prefix##32 name##_##prefix##32(prefix##32 x, prefix##32 y);          \
        prefix##64 name##_##prefix##64(prefix##64 x, prefix##64 y);          \
        prefix##16 name##_##prefix##16(prefix##16 x, prefix##16 y);

    #define FINESWORD_PUSH_SIGNATURES_PREDICATE_UNARY(name, prefix, x) \
        bool name##_##prefix##32(prefix##32 x);                        \
        bool name##_##prefix##64(prefix##64 x);                        \
        bool name##_##prefix##16(prefix##16 x);
#elif !FINESWORD_TARGET_16BIT_FLOAT && FINESWORD_TARGET_128BIT_FLOAT
    #define FINESWORD_TEMPLATE_SPECIALIZE(tmpl) \
        tmpl(32)                                \
        tmpl(64)                                \
        tmpl(128)

    #define FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_UNARY(name, prefix, x) \
        prefix##32  name##_##prefix##32(prefix##32   x);                 \
        prefix##64  name##_##prefix##64(prefix##64   x);                 \
        prefix##128 name##_##prefix##128(prefix##128 x);

    #define FINESWORD_PUSH_SIGNATURES_PREDICATE_UNARY(name, prefix, x) \
        bool name##_##prefix##32(prefix##32   x);                      \
        bool name##_##prefix##64(prefix##64   x);                      \
        bool name##_##prefix##128(prefix##128 x);

    #define FINESWORD_PUSH_SIGNATURES_HOMOGENEOUS_BINARY(name, prefix, x, y) \
        prefix##32  name##_##prefix##32(prefix##32   x, prefix##32  y);      \
        prefix##64  name##_##prefix##64(prefix##64   x, prefix##64  y);      \
        prefix##128 name##_##prefix##128(prefix##128 x, prefix##128 y);
    
    #define FINESWORD_PUSH_SIGNATURES_PREDICATE_BINARY(name, prefix, x, y) \
        bool name##_##prefix##32(prefix##32   x, prefix##32  y);           \
        bool name##_##prefix##64(prefix##64   x, prefix##64  y);           \
        bool name##_##prefix##128(prefix##128 x, prefix##128 y);
#endif

#endif /* FINESWORD_RUDIMENTS_TEMPLATE_H */
