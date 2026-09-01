// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_FUN_ALL_H
#define FINESWORD_TESTS_FUN_ALL_H

#include "finesword/rudiments/types.h"


#define FINESWORD_TESTS_FUN_REGISTRY_REGISTER_ALL \
    REGISTERX(d1_nan)                             \
    REGISTERX(show_supported)                     \
    REGISTERX(exh_unary32)                        \
    REGISTERX(exh_unary16)                        \
    REGISTERX(exh_binary16)                       \
    REGISTERX(list_binary32)                      \
    REGISTERX(list_unary64)                       \
    REGISTERX(list_binary64)                      \
    REGISTERX(list_unary128)                      \
    REGISTERX(list_binary128)                     \
    REGISTERX(test)                               \
    REGISTERX(xoshiro256pp)                       \
    REGISTERX(perf)


#define CV const int argc, const char *const *const argv

// Functions that use str are marked with '// +'

bool d1_nan(CV);

bool show_supported(CV);

bool exh_unary32(CV);
bool exh_unary16(CV);
bool exh_binary16(CV);

bool list_unary64(CV);
bool list_unary128(CV);
bool list_binary32(CV);
bool list_binary64(CV);
bool list_binary128(CV);

bool test(CV); // +

/* MAY pollute fp environment
 */
bool xoshiro256pp(CV); // +

/* MAY pollute fp environment
 */
bool perf(CV); // +

#undef CV

#endif /* FINESWORD_TESTS_FUN_ALL_H */
