// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/test/performance/pool.h"
#include "finesword_tests/shared/prng.h"
#include "finesword_tests/shared/sys_helpers.h"

#include <stdint.h>


// SHALL be greater than 0
#define STEP ((size_t)4096)

void prefault_zeros_pool(void *pool, size_t N) {
    if (pool == NULL) {
        return;
    }

    if (N > (SIZE_MAX-STEP)) {
        N -= STEP;
    }
    
    for (size_t i = 0; i < N; i += STEP) {
        ((volatile char *)pool)[i] = '\0';
    }
}

bool fill_random_bytes_xoshiro256pp_splitmix64_pool_be(void *pool, size_t N, u64 seed) {
    if ((N == (size_t)0) || (pool == NULL)) {
        return false;;
    }

    xoshiro256_state state;
    xoshiro256_seed_splitmix64(seed, &state);
    if (!is_xoshiro256_state_valid(&state)) {
        return false;
    }

    const size_t w = N / sizeof(u64);
    const size_t r = N % sizeof(u64);
    unsigned char *bpool = (unsigned char *)pool; 

    for (size_t i = 0; i < w; i++) {
        const u64 x = store_be_u64(xoshiro256pp_next(&state));
        (void) memcpy(&bpool[i * sizeof(u64)], &x, sizeof(x));
    }
    if (r != (size_t)0) {
        const u64 last = store_be_u64(xoshiro256pp_next(&state));
        (void) memcpy(&bpool[w * sizeof(u64)], &last, r);
    }

    return true;
}
