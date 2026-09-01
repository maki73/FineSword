// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_SHARED_SYS_HELPERS_H
#define FINESWORD_TESTS_SHARED_SYS_HELPERS_H

// #define _POSIX_C_SOURCE 200112L // defined by CMake
#include "finesword/rudiments/types.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>


static inline void atabort_handler(int sig) {
    (void)sig;

    (void) fputs( // *try* writing fatal error message
        "---------------\n"
        "FATAL ERROR\n"
        "---------------\n",
        stderr);
    (void) fflush(NULL);

    _Exit(EXIT_FAILURE); // atexit() will not run
}

/* Please, abort only with abort() or raise(SIGABRT)
 */
static inline void setup_atabort_handler(void) {
    (void) signal(SIGABRT, atabort_handler);
}


/* 1 byte SHALL be 8 bits
 */

/* swap byte order */
static inline u64 byteswap_u64(u64 x) {
    u64 top  = U64_C(0xFF) << 56U;
    u64 null = U64_C(0x0);
    null |= (x & (top >>  0U)) >> 56U;
    null |= (x & (top >>  8U)) >> 40U;
    null |= (x & (top >> 16U)) >> 24U;
    null |= (x & (top >> 24U)) >>  8U;
    null |= (x & (top >> 32U)) <<  8U;
    null |= (x & (top >> 40U)) <<  24U;
    null |= (x & (top >> 48U)) <<  40U;
    null |= (x & (top >> 56U)) <<  56U;
    return null; 
}

/* little-endian byte order */
static inline u64 store_le_u64(u64 x) {
    u8 c[8];
    for (size_t i = 0; i < sizeof(u64); i++) {
        u64 shift = i * U64_C(8);
        c[i] = (u8)(x >> shift); 
    }
    (void) memcpy(&x, c, sizeof(x));
    return x;
}

/* big-endian byte order */
static inline u64 store_be_u64(u64 x) {
    const size_t one  = (size_t)1;
    const size_t s    = sizeof(u64) - one;
    
    u8 c[8];
    for (size_t i = s; i < sizeof(u64); i--) {
        u64 shift = (s - i) * U64_C(8);
        c[i] = (u8)(x >> shift); 
    }
    (void) memcpy(&x, c, sizeof(x));
    return x;
}


#ifdef _WIN32
    #ifndef _WIN32_WINNT
    #   define _WIN32_WINNT 0x0601
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
    #   define WIN32_LEAN_AND_MEAN
    #endif

    // #include <realtimeapiset.h>
    #include <windows.h>
    #include <malloc.h>

    /* https://learn.microsoft.com/en-us/windows/win32/api/realtimeapiset/nf-realtimeapiset-queryunbiasedinterrupttimeprecise
    */
    
    /* Only 100 ns precision
     * Will not function on older Window versions
     * MAY overflow
     */
    static inline i64 ns_now_helper(void) {
        ULONGLONG t;
        QueryUnbiasedInterruptTimePrecise(&t);
        i64 ns = (i64)t * I64_C(100);
        return ns;
        // return (i64)0;
    }

    /* https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/aligned-malloc?view=msvc-170
     */

    /* N - size in bytes;
     returns NULL on failure */
    static inline void *allocate_aligned(size_t N, size_t alignment) {
        return _aligned_malloc(N, alignment); 
    }

    static inline void deallocate_aligned(void *p) {
        _aligned_free(p);
    }
#else
    extern int clock_gettime(clockid_t clock_id, struct timespec *tp);

    /* POSIX <time.h> timespec structure
     long tv_nsec Nanoseconds.
     [C99 \S 5.2.4.2.1] long is at least 32-bits */
    
    /* monotonic clock
     MAY overflow after approximately ~292.3 years */
    static inline i64 ns_now_helper(void) {
        struct timespec t = {0};
        if (clock_gettime(CLOCK_MONOTONIC, &t) != 0) {
            perror("clock_gettime");
            abort();
        }
        i64 ns = ((i64)t.tv_sec * I64_C(1000000000)) + (i64)t.tv_nsec;
        return ns;
    }

    /* https://pubs.opengroup.org/onlinepubs/009604499/functions/posix_memalign.html
     * POSIX posix_memalign()
     * fails with EINVAL if alignment is not 'a power of two multiple of sizeof(void *)'
     * free() to deallocate
     */

    /* N - size in bytes;
     returns NULL on failure */
    static inline void *allocate_aligned(size_t N, size_t alignment) {
        if (N == (size_t)0) {
            return NULL;
        }

        void *p = NULL;
        if (posix_memalign(&p, alignment, N) != 0) {
            return NULL;
        }

        return p;
    }

    static inline void deallocate_aligned(void *p) {
        free(p);
    }
#endif

#endif /* FINESWORD_TESTS_SHARED_SYS_HELPERS_H */
