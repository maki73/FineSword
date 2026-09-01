// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_SHARED_PRNG_H
#define FINESWORD_TESTS_SHARED_PRNG_H

#include "finesword/rudiments/types.h"

/* We will use xoshiro256++ (here written as 'xoshiro256pp')  
 * and SplitMix64 for seeding
 * 
 * See *Scrambled Linear Pseudorandom Number Generators* by David Blackman and Sebastiano Vigna
 * along with https://prng.di.unimi.it/ for more information.
 */


/* xoshiro256 linear generator's internal state */
typedef struct xoshiro256_state {
    u64 s[4]; // 256-bits   
} xoshiro256_state;

/* s SHALL be a valid non-NULL pointer if you want your PRNG actually seeded
 */
void xoshiro256_seed_splitmix64(u64 seed, xoshiro256_state *s);

/* Again, s SHALL be a valid non-NULL pointer
 */
u64 xoshiro256pp_next(xoshiro256_state *s);

/* Just checks for zero vector state
 */
bool is_xoshiro256_state_valid(const xoshiro256_state *s);

#endif /* FINESWORD_TESTS_SHARED_PRNG_H */
