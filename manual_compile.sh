#!/usr/bin/env bash

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

set -eu

# NOTES:
#  Enabling 16/128-bit float(s) will disable -Wpedantic with #pragma directives
#  Thou SHALT update FINESWORD_VERSION (such that it's same as in CMake configuration)

EXEC_NAME='./main'
ADD_FLAGS=
COMPILER=cc # default

case ${ENABLE_LTO-} in
    TRUE|true|ON|on|1)
        echo "[~] LTO - enabled"
        ADD_FLAGS="-flto"
        ;;
    *)
        echo "[~] LTO - disabled"
        ;;
esac

if [ "${CC+x}" = x ]; then
    COMPILER=$CC
fi

echo "[~] Compiling single executable '$EXEC_NAME' with $COMPILER"
"$COMPILER" -Iinclude -O2 -std=c99 \
    -Wall -Wextra -Wpedantic \
    -Wshadow -Wmissing-prototypes \
    -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -Wformat=2 \
    -Wundef -Wwrite-strings -Warray-bounds -Wswitch-enum \
    -Wcast-qual -Wvla -Wunused-variable -Wunused-value \
    -Wno-unused-function \
    -Wno-unknown-pragmas \
    $ADD_FLAGS \
    -DFINESWORD_VERSION='"0.0.1"' \
    src/*/*.c \
    tests/*/*/*.c tests/*/*.c tests/*.c \
    -lm -D_POSIX_C_SOURCE=200112L -fopenmp \
    -o "$EXEC_NAME"

echo "[+] Done."
