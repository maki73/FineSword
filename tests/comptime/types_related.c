// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include <limits.h>
#include <float.h>

#if CHAR_BIT != 8
    #error "this project assumes 1 byte = 8 bits!"
#endif

#if FLT_RADIX != 2
    #error "by default types.h assumes floating-point formats are binary! \
    if you already substituted float/double types in types.h for types appropriate \
    for your platform, remove this assertion."
#endif

extern void nothing(void);
