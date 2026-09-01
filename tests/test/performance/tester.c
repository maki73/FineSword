// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/test/performance/tester.h"
#include "finesword/rudiments/astype.h"
#include "finesword_tests/shared/print.h"
#include "finesword_tests/shared/sys_helpers.h"
#include "finesword_tests/shared/utilities.h"
#include "finesword_tests/test/performance/pool.h"
#include "finesword/rudiments/template.h"

#include <stdint.h>
#include <inttypes.h>


#define FORALL FINESWORD_TEMPLATE_SPECIALIZE 

/* -- helpers -- */

/* assumes 1 byte is 8 bits
 */
static bool ckd_total_bytes(size_t inputs_count, size_t width, size_t arity, size_t *total, size_t *bytes_width) {
    const size_t zero = (size_t)0;
    if ((total == NULL) || (bytes_width == NULL) || (inputs_count == zero) || (width == zero) || (arity == zero)) {
        return false;
    }

    if ((width % (size_t)8) != zero) {
        return false;
    }
    *bytes_width = width/((size_t)8); 

    if (arity > (SIZE_MAX/(*bytes_width))) {
        return false;
    }
    size_t single_call_bytes = (*bytes_width) * arity;

    if (inputs_count > (SIZE_MAX/single_call_bytes)) {
        return false;
    }
    *total = inputs_count * single_call_bytes;
    return true;
}


/* --- Test --- */

#define TEMPLATE_UNARY_PERF_TEST_TEMPLATE(N)         \
        if (cfg->width == N) {                       \
            u##N *arr = pool;                        \
                                                     \
            if (!is_random) {                        \
                void_printf("filling pool with sweeping words\n"); \
                for (size_t i = 0; i < (total_bytes/sizeof(u##N)); i++) { \
                    arr[i] = (u##N)i;                \
                }                                    \
            }                                        \
                                                     \
            f##N (*fn)(f##N);                        \
            if (is_test_fn) {                        \
                fn = cfg->test_fn.unary##N##_fn;     \
            }                                        \
            else {                                   \
                fn = cfg->ref_fn.unary##N##_fn;      \
            }                                        \
                                                     \
            void_printf("entering timed loop...\n"); \
            volatile f##N sink = F##N##_C(0.0);      \
            i64 t1 = ns_now_helper();                \
            for (size_t i = 0; i < repeats; i++) {   \
                (void)i;                             \
                for (size_t j = 0; j < inputs_count; j++) { \
                    f##N x = asfloat_u##N(arr[j]);   \
                    sink = fn(x); /* don't do +=, it's idiotic */ \
                }                                    \
            }                                        \
            i64 t2 = ns_now_helper();                \
            (void)sink;                              \
            void_printf("  ");                       \
            info_print_time_took(t1, t2);            \
            {                                        \
                f64 calls_done = (f64)inputs_count * (f64)repeats; \
                f64 Delta_ns   = (f64)t2 - (f64)t1;  \
                f64 npc   = Delta_ns / calls_done;   \
                info_printf("  ns per call:    %.6f ns\n", (double)npc); \
            }                                        \
            void_printf("  sink (ignore): %.1g\n", (double)sink); \
                                                     \
            ret = true;                              \
            goto finish;                             \
        }

#define TEMPLATE_BINARY_PERF_TEST_TEMPLATE(N)        \
        if (cfg->width == N) {                       \
            u##N *arr = pool;                        \
                                                     \
            if (!is_random) {                        \
                void_printf("filling pool with sweeping words\n"); \
                for (size_t i = 0; i < (total_bytes/sizeof(u##N)); i++) { \
                    arr[i] = (u##N)i;                \
                }                                    \
            }                                        \
                                                     \
            f##N (*fn)(f##N, f##N);                  \
            if (is_test_fn) {                        \
                fn = cfg->test_fn.binary##N##_fn;    \
            }                                        \
            else {                                   \
                fn = cfg->ref_fn.binary##N##_fn;     \
            }                                        \
                                                     \
            void_printf("entering timed loop...\n"); \
            volatile f##N sink = F##N##_C(0.0);      \
            i64 t1 = ns_now_helper();                \
            for (size_t i = 0; i < repeats; i++) {   \
                (void)i;                             \
                for (size_t j = 0; j < inputs_count; j++) { \
                    f##N x = asfloat_u##N(arr[j]);   \
                    f##N y = asfloat_u##N(arr[j + inputs_count]); \
                    sink = fn(x, y);                 \
                }                                    \
            }                                        \
            i64 t2 = ns_now_helper();                \
            (void)sink;                              \
            void_printf("  ");                       \
            info_print_time_took(t1, t2);            \
            {                                        \
                f64 calls_done = (f64)inputs_count * (f64)repeats; \
                f64 Delta_ns   = (f64)t2 - (f64)t1;  \
                f64 npc   = Delta_ns / calls_done;   \
                info_printf("  ns per call:    %.6f ns\n", (double)npc); \
            }                                        \
            void_printf("  sink (ignore): %.1g\n", (double)sink); \
                                                     \
            ret = true;                              \
            goto finish;                             \
        }


bool performed_dispatched_performance_test(
    const test_config *cfg,
    const bool is_test_fn,
    const size_t inputs_count,
    const size_t repeats,
    const bool is_random,
    const u64 seed
)
{
    if (!is_test_config_valid(cfg)) {
        return false; /* fail */
    }
    {
        const size_t zero = (size_t)0;
        if ((inputs_count == zero) || (repeats == zero)) {
            error_printf("inputs_count/repeats cannot be zero!\n");
            return false; /* fail */
        }
    }

    // - currently unimplemented -
    if (cfg->is_decimal) {
        error_printf("decimal functions are currently unsupported!\n");
        return false; /* fail */       
    }

    void_printf("config:\n");
    {
        if (is_test_fn) {
            info_printf("    fn: %s [test_fn]\n", cfg->test_name);
        }
        else {
            info_printf("    fn: %s [ref_fn]\n",  cfg->ref_name);
        }
        info_printf("    width: %d\n", cfg->width);
        info_printf("    arity: %d\n", cfg->arity);     
        f64 inputs_count_m = (f64)inputs_count / F64_C(1e6);
        info_printf("    inputs:  %.3f M (exactly: %zu)\n",
            (double)inputs_count_m,
            inputs_count
        );
        f64 repeats_k = (f64)repeats / F64_C(1e3);
        info_printf("    repeats: %.3f K (exactly: %zu)\n",
            (double)repeats_k,
            repeats
        );
        info_printf("    pool: %s\n", is_random ? "random" : "sweep");
        if (is_random) {
            info_printf("    seed: 0x%" PRIX64 "\n", (uint64_t)seed);
        }
        void_printf("\n");
    }

    const size_t width = (size_t)(cfg->width); 
    const size_t arity = (size_t)(cfg->arity);

    size_t total_bytes;
    size_t bytes_width;
    if (!ckd_total_bytes(inputs_count, width, arity, &total_bytes, &bytes_width)) {
        error_printf("computing total bytes failed!\n");
        return false; /* fail */
    }

    void_printf("allocating pool\n");
    {
        f64 ftotal = (f64)total_bytes;
        f64 gib = ftotal / F64_C(1073741824.0);
        f64 mib = ftotal / F64_C(1048576.0);
        info_printf("    size: %f GiB    %f MiB    %zu bytes\n", gib, mib, total_bytes);
    }
    size_t alignment = bytes_width < sizeof(u64) ? sizeof(u64) : bytes_width; // filler needs u64 alignment
    void *pool = allocate_aligned(total_bytes, alignment);
    if (pool == NULL) {
        error_printf("failed to allocate %zu bytes pool with %zu alignment!\n", total_bytes, alignment);
        return false; /* fail */
    }

    if (is_random) {
        void_printf("using xoshiro256++ with SplitMix64 to fill pool with random bytes (big-endian)\n");
        if (!fill_random_bytes_xoshiro256pp_splitmix64_pool_be(pool, total_bytes, seed)) {
            error_printf("failed to fill pool with random bytes!\n");
            deallocate_aligned(pool);
            return false; /* fail */
        }
    }

    bool ret = false; /* fail by default */

    /* -- ARITY=1 -- */

    if (cfg->arity == 1) {
        FORALL(TEMPLATE_UNARY_PERF_TEST_TEMPLATE)

        // else
        ret = false;
        goto finish;
    }

    /* -- ARITY=2 -- */
    if (cfg->arity == 2) {
        FORALL(TEMPLATE_BINARY_PERF_TEST_TEMPLATE)

        // else
        ret = false;
        goto finish; 
    }

    /* -- tail -- */
    
    goto finish;

finish:
    if (!ret) {
        error_printf("failed!\n");
    }

    void_printf("deallocating...\n");
    deallocate_aligned(pool);
    return ret;
}
