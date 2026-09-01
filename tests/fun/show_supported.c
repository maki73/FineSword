// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword_tests/fun/all.h"

#include "finesword/rudiments/types.h"
#include "finesword_tests/shared/utilities.h"
#include "finesword_tests/shared/print.h"


static const char *status_ansi(bool boolean) {
    return boolean
        ?
        TESTS_ANSI_GREEN "YES" TESTS_ANSI_RESET
        :
        TESTS_ANSI_RED   "NO"  TESTS_ANSI_RESET;
}

bool show_supported(const int argc, const char *const *const argv) {
    if (argc > 0) {
        warning_printf("ignoring argument(s)\n");
    }
    (void)argv;

    void_printf("--- Runtime Support Query ---\n\n");

    info_printf("radices:\n");
    void_printf("binary : %s\n", status_ansi(finesword_supports_binary()));
    void_printf("decimal: %s\n", status_ansi(finesword_supports_decimal()));

    info_printf("\nwidths:\n");
    void_printf("16-bit:  %s\n", status_ansi(finesword_supports_16bit_float()));
    void_printf("32-bit:  %s\n", status_ansi(finesword_supports_32bit_float()));
    void_printf("64-bit:  %s\n", status_ansi(finesword_supports_64bit_float()));
    void_printf("128-bit: %s\n", status_ansi(finesword_supports_128bit_float()));

    return true; /* success */
}
