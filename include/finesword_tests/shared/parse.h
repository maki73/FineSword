// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#ifndef FINESWORD_TESTS_SHARED_PARSE_H
#define FINESWORD_TESTS_SHARED_PARSE_H

#include "finesword/rudiments/preprocessor.h"
#include "finesword/rudiments/types.h"

#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


FINESWORD_STATIC_ASSERT(sizeof(unsigned long long) >= sizeof(size_t), sizeof_ull_is_assumed_to_be_ge_sizeof_size_t_);


static inline bool parse_base16_u64(const char *str, u64 *r) {
    if ((str == NULL) || (r == NULL)) {
        return false;
    }
    
    while (isspace((int)((unsigned char)*str)) != 0) {
        str++;
    }

    errno     = 0;
    char *end = NULL;
    // unsigned long long is at least 64-bits [C99 \S 5.2.4.2.1]
    unsigned long long val = strtoull(str, &end, 16);
    if (end == str) {
        return false;
    }
    
    // if unsigned long long is > 64-bits val can be UINT64_MAX 
    if ((errno == ERANGE) || (val > UINT64_MAX)) {
        return false; 
    }

    while (isspace((int)((unsigned char)*end)) != 0) {
        end++;
    }

    if (*end != '\0') {
        return false;
    }

    *r = (u64)val; // safe
    return true;
}

static inline bool parse_base10_size_t(const char *str, size_t *r) {
    if ((str == NULL) || (r == NULL)) {
        return false;
    }
    
    while (isspace((int)((unsigned char)*str)) != 0) {
        str++;
    }

    errno     = 0;
    char *end = NULL;
    unsigned long long val = strtoull(str, &end, 10);
    if (end == str) {
        return false;
    }
    
    if ((errno == ERANGE) || (val > SIZE_MAX)) {
        return false; 
    }

    while (isspace((int)((unsigned char)*end)) != 0) {
        end++;
    }

    if (*end != '\0') {
        return false;
    }

    *r = (size_t)val;
    return true;
}

/* B - 1e9; M - 1e6; K - 1e3. 
 */
static inline bool parse_base10_prefixed_size_t(const char *str, size_t *r) {
    if ((str == NULL) || (r == NULL)) {
        return false;
    }
    
    while (isspace((int)((unsigned char)*str)) != 0) {
        str++;
    }

    errno     = 0;
    char *end = NULL;
    unsigned long long val = strtoull(str, &end, 10);
    if (end == str) {
        return false;
    }
    
    if ((errno == ERANGE) || (val > SIZE_MAX)) {
        return false; 
    }

    size_t tmp = (size_t)val;
    size_t mul = 1;
    switch (*end) {
        case 'B':
        case 'b':
            mul = (size_t)1000000000;
            end++;
            break;
        case 'M':
        case 'm':
            mul = (size_t)1000000;
            end++;
            break;
        case 'K':
        case 'k':
            mul = (size_t)1000;
            end++;
            break;
        default:
            break;
    }

    if (val > (SIZE_MAX/mul)) {
        return false;
    }
    tmp *= mul;

    while (isspace((int)((unsigned char)*end)) != 0) {
        end++;
    }

    if (*end != '\0') {
        return false;
    }

    *r = tmp;
    return true;
}

#endif /* FINESWORD_TESTS_SHARED_PARSE_H */
