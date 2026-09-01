// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_RUDIMENTS_PREPROCESSOR_H
#define FINESWORD_RUDIMENTS_PREPROCESSOR_H

/* Preprocessor Related Macros
 */

/* C99 trick for static assert
 * arguments: `cond` SHALL be an integer constant expression and `message` SHALL be a valid macro identifier
 * guarantee: SHALL show at least one diagnostic message at compile-time if the condition is false
 * [C99 \S\S 3.8, 6.7.5.2 \P 1, and 5.1.1.3]
 */
#define FINESWORD_STATIC_ASSERT_2(cond, line, message) typedef char finesword_static_assert_##line##_##message[(cond) ? 1 : -1]
#define FINESWORD_STATIC_ASSERT_1(cond, line, message) FINESWORD_STATIC_ASSERT_2(cond, line, message)
#define FINESWORD_STATIC_ASSERT(cond, message)         FINESWORD_STATIC_ASSERT_1(cond, __LINE__, message)

// example usage:
// FINESWORD_STATIC_ASSERT(sizeof(float) == 4, float_assumed_to_be_4_bytes);

#endif /* FINESWORD_RUDIMENTS_PREPROCESSOR_H */
