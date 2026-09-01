// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_TEST_PERFORMANCE_POOL_H
#define FINESWORD_TESTS_TEST_PERFORMANCE_POOL_H

#include "finesword/rudiments/types.h"

#include <stddef.h>


/* N - pool's size in bytes
 MAY not prefault every page */
void prefault_zeros_pool(void *pool, size_t N);

/* N - pool's size in bytes
 * pool SHALL be properly aligned for u64
 * Given same N and seed, pool SHALL be *bit-identical* across little and big-endian architectures
 */
bool fill_random_bytes_xoshiro256pp_splitmix64_pool_be(void *pool, size_t N, u64 seed);

#endif /* FINESWORD_TESTS_TEST_PERFORMANCE_POOL_H */
