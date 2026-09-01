// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_SHARED_PRINT_H
#define FINESWORD_TESTS_SHARED_PRINT_H

#include "finesword/rudiments/types.h"
#include "finesword/config.h"

#include <stdio.h>


/* NOTE: I/O failures (such as fprintf() returning value less than zero)
 * are considered fatal erros and will abort immediately. 
 */

#if defined(__GNUC__) || defined(__clang__) // Clang also defines __GNUC__, but don't rely on it
    #define PRINTF_ATTRIBUTE(b, c) __attribute__((format(printf, b, c))) 
#else
    #define PRINTF_ATTRIBUTE(b, c)
#endif

/* -> stdout */
void void_printf   (const char *restrict format, ...) PRINTF_ATTRIBUTE(1, 2) ;
/* info -> stdout */
void info_printf   (const char *restrict format, ...) PRINTF_ATTRIBUTE(1, 2) ;
/* success -> stdout */
void success_printf(const char *restrict format, ...) PRINTF_ATTRIBUTE(1, 2) ;
/* warnings -> stderr */
void warning_printf(const char *restrict format, ...) PRINTF_ATTRIBUTE(1, 2) ;
/* errors -> stderr */
void error_printf  (const char *restrict format, ...) PRINTF_ATTRIBUTE(1, 2) ;
/* fprintf wrapper */
void void_fprintf(FILE *stream, const char *restrict format, ...) PRINTF_ATTRIBUTE(2, 3) ;

#undef PRINTF_ATTRIBUTE

void void_fflush(FILE *stream);

#if FINESWORD_TARGET_128BIT_FLOAT
    /* if stream==NULL -> stream=stdout */
    void print_u128_hex(u128 x, FILE *stream);
#endif

/* MAY signal floating-point exceptions
 * t2 SHOULD be greater than t1 (if you the output to make sense)
 */
void info_print_time_took(i64 t1, i64 t2);

#endif /* FINESWORD_TESTS_SHARED_PRINT_H */
