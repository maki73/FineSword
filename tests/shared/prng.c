// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/shared/prng.h"
#include "finesword/rudiments/types.h"

#include <stddef.h>


/* See: https://prng.di.unimi.it/xoshiro256plusplus.c and
 * https://prng.di.unimi.it/splitmix64.c for primary reference implementations.
 */

/* UB for k==0 and k>=64 */
static inline u64 rot_u64(const u64 x, const u64 k) {
    return (x << k) | (x >> (U64_C(64) - k));
}

static inline u64 splitmix64_next(u64 *x) {
	u64
    z =   (*x +=             U64_C(0x9E3779B97F4A7C15));
	z =   (z ^ (z >> 30U)) * U64_C(0xBF58476D1CE4E5B9);
	z =   (z ^ (z >> 27U)) * U64_C(0x94D049BB133111EB);
	return z ^ (z >> 31U);
}

/* s SHALL be non-NULL */
u64 xoshiro256pp_next(xoshiro256_state *s) {
	const u64 r = rot_u64(s->s[0] + s->s[3], U64_C(23)) + s->s[0]; // add-rotate-add
    const u64 t = s->s[1] << 17U;
    
    s->s[2] ^= s->s[0];
    s->s[3] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[0] ^= s->s[3];
    
    s->s[2] ^= t;
    
    s->s[3] = rot_u64(s->s[3], U64_C(45));

    return r;
}

void xoshiro256_seed_splitmix64(u64 seed, xoshiro256_state *s) {
    if (s == NULL) {
        return;
    }
    u64 x = seed;
    for (size_t i = 0; i < (sizeof(s->s)/sizeof(s->s[0])); i++) {
        s->s[i] = splitmix64_next(&x);
    }
}


bool is_xoshiro256_state_valid(const xoshiro256_state *s) {
    if (s == NULL) {
        return false;
    }
    bool boolean = true;
    for (size_t i = 0; i < (sizeof(s->s)/sizeof(s->s[0])); i++) {
        boolean = boolean && (s->s[i] == (size_t)0);
    }
    return !boolean;
}

