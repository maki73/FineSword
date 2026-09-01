// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_SHARED_UTILITIES_H
#define FINESWORD_TESTS_SHARED_UTILITIES_H

#include "finesword/config.h"
#include "finesword/rudiments/astype.h"
#include "finesword/rudiments/ieee754.h"
#include "finesword/rudiments/types.h"
#include "finesword/rudiments/template.h"
#include "finesword_tests/test/test_registry.h"

#include <stdio.h>
#include <fenv.h>


/* --- FP --- */

/* NOTE: Don't use FE_ALL_EXCEPT, because some implementations, as the
 * C Standard and IEEE 754 allow, define exceptions beyond the 5
 * IEEE 754-defined ones.
 * A notable example would be ICX with FE_DENORMAL (commonly 0x02).
 */
#if !defined(FE_INVALID) || !defined(FE_DIVBYZERO) || !defined(FE_OVERFLOW) || !defined(FE_UNDERFLOW) || !defined(FE_INEXACT)
    #error "fenv.h does not provide masks for all 5 IEEE 754 exception status flags!"
#else
    #define FE_ALL_IEEE754_EXCEPT (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT)
#endif

// also check rounding directions
#if !defined(FE_TONEAREST) || !defined(FE_TOWARDZERO) || !defined(FE_UPWARD) || !defined(FE_DOWNWARD)
    #error "fenv.h does not provide 4 IEEE 754 binary rounding directions!"
#endif

/* --- ANSI Colors --- */

#if FINESWORD_TESTS_USE_ANSI
    #define TESTS_ANSI_RED     "\x1b[31m"
    #define TESTS_ANSI_GREEN   "\x1b[32m"
    #define TESTS_ANSI_BLUE    "\x1b[34m"
    #define TESTS_ANSI_YELLOW  "\x1b[33m"
    #define TESTS_ANSI_RESET   "\x1b[0m"
    #define TESTS_ANSI_BOLD    "\x1b[1m"
#else
    #define TESTS_ANSI_RED     ""
    #define TESTS_ANSI_GREEN   ""
    #define TESTS_ANSI_BLUE    ""
    #define TESTS_ANSI_YELLOW  ""
    #define TESTS_ANSI_RESET   ""
    #define TESTS_ANSI_BOLD    ""
#endif


/* --- report --- */

struct an_attribute_entry {
    const char *name;
    int attribute;
};

extern const struct an_attribute_entry all_excepts[];
extern const size_t all_excepts_count;
extern const struct an_attribute_entry all_directions[];
extern const size_t all_directions_count;


void report_except_flag(int flags, FILE *stream);
void report_rounding_direction(int direction, FILE *stream);
bool is_rounding_direction_valid(int direction);


/* -- compare --- */

/* behavior:
 * are_nans_special=false -> only binary fp data with identical bit pattern are equal
 * are_nans_special=true  -> same as above, except for NaNs
 *  which get compared by their kind (regardless of the payload and sign).
 *
 * this operation SHALL never signal any exceptions.
 */

#define FINESWORD_TESTS_TEMPLATE_IS_EQUAL_BITWISE_RULED_FN_1(N) \
static inline bool is_equal_bitwise_ruled_f##N (                \
    f##N x,                                                     \
    f##N y,                                                     \
    bool are_nans_special                                       \
) {                                                             \
    u##N xi = asuint_f##N (x);                                  \
    u##N yi = asuint_f##N (y);                                  \
    bool xnan = (xi & F##N##_ABS_MASK) > F##N##_POS_INF;        \
    bool ynan = (yi & F##N##_ABS_MASK) > F##N##_POS_INF;        \
    if (are_nans_special && (xnan && ynan)) {                   \
        return                                                  \
        (xi & F##N##_D1_BIT_MASK) == (yi & F##N##_D1_BIT_MASK); \
    }                                                           \
    return xi == yi;                                            \
}

FINESWORD_TEMPLATE_SPECIALIZE(
    FINESWORD_TESTS_TEMPLATE_IS_EQUAL_BITWISE_RULED_FN_1)
#undef FINESWORD_TESTS_TEMPLATE_IS_EQUAL_BITWISE_RULED_FN_1


/* -- test config validation helper -- */


bool is_test_config_valid(const test_config *cfg);

#endif /* FINESWORD_TESTS_SHARED_UTILITIES_H */
