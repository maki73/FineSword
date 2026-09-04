#!/usr/bin/env bash

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

set -euo pipefail

EXEC_NAME="./main"

echo "[~] Compiling single executable '$EXEC_NAME' with cc"
cc -Iinclude -O2 -std=c99 \
    -Wall -Wextra -Wpedantic \
    -Wshadow -Wmissing-prototypes \
    -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -Wformat=2 \
    -Wundef -Wwrite-strings -Warray-bounds -Wswitch-enum \
    -Wcast-qual -Wvla -Wunused-variable -Wunused-value \
    -Wno-unused-function \
    -Wno-unknown-pragmas \
    src/*/*.c \
    tests/*/*/*.c tests/*/*.c tests/*.c \
    -lm -D_POSIX_C_SOURCE=200112L -fopenmp \
    -o "$EXEC_NAME"

echo "[+] Done."
