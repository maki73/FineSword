#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#


set -euo pipefail

echo "[~] Removing the build directory"
rm -rf ./build/

echo "[~] Compiling with GCC"
export CC=gcc
./cmake_compile.sh
rm -rf ./build/

echo "[~] Compiling with Clang"
export CC=clang
./cmake_compile.sh
rm -rf ./build/

echo "[~] Compiling with ICX"
export CC=icx
./cmake_compile.sh
rm -rf ./build/

echo "[~] Compiling with TinyCC"
export CC=tcc
./cmake_compile.sh

echo "[+] All done."
exit 0

