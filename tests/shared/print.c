// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/shared/print.h"
#include "finesword/rudiments/types.h"
#include "finesword_tests/shared/utilities.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


// define it again
#if defined(__GNUC__) || defined(__clang__) // Clang also defines __GNUC__, but don't rely on it
    #define PRINTF_ATTRIBUTE(b, c) __attribute__((format(printf, b, c))) 
#else
    #define PRINTF_ATTRIBUTE(b, c)
#endif

static void finesword_custom_printf(
    const char *ansi, FILE
    *restrict stream,
    const char *restrict format,
    va_list args
) PRINTF_ATTRIBUTE(3, 0); 

static inline void finesword_custom_printf(
        const char *ansi,
        FILE *restrict stream,
        const char *restrict format,
        va_list args
)
{
    if ((format == NULL) || (ansi == NULL) || (stream == NULL)) {
        abort();
    }

    if (fputs(ansi, stream) == EOF) {
        abort();
    } 
    int r = vfprintf(stream, format, args);
    if (r < 0) {
        abort();
    }
    if (*ansi != '\0') {
        if (fputs(TESTS_ANSI_RESET, stream) == EOF) {
            abort();
        }
    }
}

void void_printf(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    finesword_custom_printf("", stdout, format, args);
    va_end(args);
}

// info to stdout
void info_printf(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    finesword_custom_printf(TESTS_ANSI_BLUE, stdout, format, args);
    va_end(args);
}

// success to stdout
void success_printf(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    finesword_custom_printf(TESTS_ANSI_GREEN, stdout, format, args);
    va_end(args);
}

// warnings to stderr
void warning_printf(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    finesword_custom_printf(TESTS_ANSI_YELLOW, stderr, format, args);
    va_end(args);
}

// errors to stderr
void error_printf(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    finesword_custom_printf(TESTS_ANSI_RED, stderr, format, args);
    va_end(args);
}


void void_fflush(FILE *stream) {
    if (fflush(stream) != 0) {
        abort();
    }
}

void void_fprintf(FILE *stream, const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    finesword_custom_printf("", stream, format, args);
    va_end(args);
}


#if FINESWORD_TARGET_128BIT_FLOAT
    // if stream==NULL -> stream=stdout
    void print_u128_hex(u128 x, FILE *stream) {
        if (stream == NULL) {
            stream = stdout;
        }
        // suboptimal but as straightforward as it gets 
        u128 hi128 = x >> 64U;
        u128 lo128 = x & FINESWORD_U128_C_HI_LO(U64_C(0x0000000000000000), U64_C(0xFFFFFFFFFFFFFFFF));
        uint64_t high = (uint64_t)hi128;
        uint64_t low  = (uint64_t)lo128;
        if (fprintf(stream, "0x%016" PRIX64 "%016" PRIX64, high, low) < 0) {
            abort();
        }
    }
#endif

/* MAY signal floating-point exceptions
 * t2 SHOULD be greater than t1 (if you the output to make sense)
 */
void info_print_time_took(i64 t1, i64 t2) {
    f64 ns = (f64)t2 - (f64)t1;
    f64 s  = ns / F64_C(1e9);
    f64 ms = ns / F64_C(1e6);

    info_printf("took: %.3f s    %.3f ms    %" PRId64 " ns\n",
        (double)s,
        (double)ms,
        (int64_t)ns
    );
}

