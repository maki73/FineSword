// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword/config.h"
#include "finesword/rudiments/types.h"


/* -- Runtime Support Query --- */

bool finesword_supports_decimal(void) {
#if FINESWORD_SUPPORTS_DECIMAL
    return true;
#else
    return false;
#endif
}

bool finesword_supports_binary (void) {
#if FINESWORD_SUPPORTS_BINARY
    return true;
#else
    return false;
#endif
}


bool finesword_supports_16bit_float (void) {
#if FINESWORD_TARGET_16BIT_FLOAT
    return true;
#else
    return false;
#endif
}

bool finesword_supports_32bit_float (void) {
    return true; /* always true in the current configuration */
}

bool finesword_supports_64bit_float (void) {
    return true; /* always true in the current configuration */
}

bool finesword_supports_128bit_float(void) {
#if FINESWORD_TARGET_128BIT_FLOAT
    return true;
#else
    return false;
#endif 
}
