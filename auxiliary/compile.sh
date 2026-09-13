#!/usr/bin/env bash

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

echo "[~] GCC"
export CC=gcc
./cmake_compile.sh
python auxiliary/functions_extern.py
rm -rf ./build/

echo "[~] Clang"
export CC=clang
./cmake_compile.sh
python auxiliary/functions_extern.py
rm -rf ./build/

echo "[~] ICX"
export CC=icx
./cmake_compile.sh
python auxiliary/functions_extern.py
rm -rf ./build/

echo "[~] TinyCC"
export CC=tcc
./cmake_compile.sh
# TODO: non-exhaustive subset

echo "[+] All done."
exit 0

