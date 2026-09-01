// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/fun/all.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/prng.h"
#include "finesword_tests/shared/sys_helpers.h"
#include "finesword_tests/test/performance/pool.h"
#include "finesword_tests/shared/parse.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


/* in pixels; SHALL be resonable so don't touch arbitrarily */
#define WIDTH  ((size_t)4096)
#define HEIGHT ((size_t)4096)
// #define WIDTH  ((size_t)104)
// #define HEIGHT ((size_t)104)
#define PATH "xoshiro256pp_dump_be.ppm"

/* --- helper --- */

static bool ckd_and_nonzero_3xy(size_t *r, size_t x, size_t y) {
    const size_t zero = (size_t)0;
    if ((r == NULL) || (x == zero) || (y == zero)) {
        return false;
    }
    
    if (x > (SIZE_MAX/y)) {
        return false;
    }

    size_t xy = x * y;

    if (xy > (SIZE_MAX/3)) {
        return false;
    }

    *r = xy * (size_t)3;
    return true;
}


/* --- main --- */

bool xoshiro256pp(const int argc, const char *const *const argv) {
    if (argc < 1) {
        error_printf("xoshiro256pp module expected an argument!\n");
        error_printf("supply a 64-bit hex value (required for seeding)\n");
        return false; /* fail */
    }
    if (argc > 1) {
        warning_printf("ignoring additional arguments beyond the supplied seed\n");
    }
    const char *str = argv[0];

    u64 seed;
    if (!parse_base16_u64(str, &seed)) {
        error_printf("parsing argument as 64-bit hex seed failed!\n");
        return false; /* fail */
    }

    void_printf("--- Xoshiro256++: Performance Assessment and Serialized PPM Dump ---\n\n");
    /* NOTE: PPM serialization (or more precisely pool's serialization before writing it as a PPM image)
     * is done for dumps to be bit-identical across big and little-endian architectures. 
     */

    info_printf("seed: 0x%" PRIX64 "\n", (uint64_t)seed);

    size_t total;
    if (!ckd_and_nonzero_3xy(&total, WIDTH, HEIGHT)) {
        error_printf("WIDTH/HEIGHT constants check failed!\n");
        return false; /* fail */
    }
    if ((total % sizeof(u64)) != (size_t)0) {
        error_printf("WIDTH*HEIGHT*3 is not divisible by %zu!\n", sizeof(u64));
        return false; /* fail */
    }

    info_printf("PPM:\n");
    void_printf("    width:  %zu\n", WIDTH);
    void_printf("    height: %zu\n", HEIGHT);
    void_printf("    path: " PATH "\n");


    void_printf("creating and seeding xoshiro256 state with SplitMix64\n");
    xoshiro256_state state;
    xoshiro256_seed_splitmix64(seed, &state);
    if (!is_xoshiro256_state_valid(&state)) {
        error_printf("unexpected failure: state is invalid!\n");
        return false; /* fail */
    }

    {
        f64 ftotal = (f64)total;
        f64 gib = ftotal / F64_C(1073741824.0);
        f64 mib = ftotal / F64_C(1048576.0);
        info_printf("pool's size: %f GiB    %f MiB    %zu bytes\n", gib, mib, total);
    }
    void_printf("allocating pool\n");
    void *pool = allocate_aligned(total, sizeof(u64));
    if (pool == NULL) {
        error_printf("pool's allocation failed!\n");
        return false; /* fail */
    }
    
    i64 t1 = I64_C(0);
    i64 t2 = I64_C(0);

    void_printf("prefaulting pool with zeros\n");
    t1 = ns_now_helper();
    prefault_zeros_pool(pool, total);
    t2 = ns_now_helper();
    info_print_time_took(t1, t2);

    // (total % sizeof(u64)) == 0 and proper alignment are guaranteed by the earlier code
    u64 *u64_pool = pool;

    void_printf("filling with random 64-bit words using xoshiro256++\n");
    size_t calls_count = total / sizeof(u64);
    t1 = ns_now_helper();
    for (size_t i = 0; i < calls_count; i++) {
        u64_pool[i] = xoshiro256pp_next(&state);
    }
    t2 = ns_now_helper();
    info_print_time_took(t1, t2);
    info_printf("stats ~:\n");
    {
        f64 calls_done = (f64)calls_count;
        f64 Delta_ns   = (f64)t2 - (f64)t1;
        f64 npc   = Delta_ns / calls_done;
        f64 cps   = calls_done / (Delta_ns / F64_C(1e9));
        const f64 sizeofu64 = (f64)(sizeof(u64));
        f64 kibps = (cps / F64_C(1024.0))       * sizeofu64;
        f64 mibps = (cps / F64_C(1048576.0))    * sizeofu64;
        f64 gibps = (cps / F64_C(1073741824.0)) * sizeofu64;

        void_printf("    ns per call:    %.6f ns\n", (double)npc);

        void_printf("    calls / s:      %.3f B      %.3f M      %.3f K\n",
            (double)(cps/F64_C(1e9)),
            (double)(cps/F64_C(1e6)),
            (double)(cps/F64_C(1e3))
        );

        void_printf("    throughput / s: %.3f GiB    %.3f MiB    %.3f KiB\n",
            (double)gibps,
            (double)mibps,
            (double)kibps
        );
    }

    /* Rationale: most of the targeted architectures are little-endian,
     * but the ones that are big-endian are usually far more computationally constrained,
     * thus we would pay the reordering price only on (presumably) modern machines. 
     */
    void_printf("serializing pool by reordering byte order to big-endian\n");
    for (size_t i = 0; i < (total/sizeof(u64)); i++) {
        u64_pool[i] = store_be_u64(u64_pool[i]);
    }

    
    void_printf("writing PPM image\n");
    FILE *image = fopen(PATH, "wb");
    if (image == NULL) {
        error_printf("opening " PATH " failed!\n");
        deallocate_aligned(pool);
        return false; /* fail */
    }
    if (fprintf(image, "P6 %zu %zu 255\n", WIDTH, HEIGHT) < 0) {
        error_printf("fprintf error occurred!\n");
        goto fail_fclose;
    }
    if (fwrite(pool, (size_t)1, total, image) != total) {
        error_printf("fwrite error occurred!\n");
        goto fail_fclose;
    }
    if (fclose(image) != 0) {
        error_printf("fclose error occurred!\n");
        deallocate_aligned(pool);
        return false; /* fail */
    }

    void_printf("deallocating pool\n");
    deallocate_aligned(pool);

    return true; /* success */

fail_fclose:
    deallocate_aligned(pool);
    if (fclose(image) != 0) {
        error_printf("fclose error occurred in error path!\n");
        return false; /* fail */
    }
    return false; /* fail */
}
