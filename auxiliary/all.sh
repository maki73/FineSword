#!/usr/bin/env bash

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#


set -euo pipefail

echo "[~] ./auxiliary/newline.py..."
./auxiliary/newline.py src/ include/ tests/ auxiliary/ CMakeLists.txt REUSE.toml README.md

echo "[~] ./auxiliary/guard.py..."
./auxiliary/guard.py

echo "[~] ./auxiliary/license.py..."
./auxiliary/license.py

echo "[~] ./auxiliary/rules.sh..."
./auxiliary/rules.sh

echo "[~] ./auxiliary/compile.sh..."
./auxiliary/compile.sh

echo "[~] check d1_nan"
export CC=tcc # assuming the last compile.sh compiler is TinyCC
./auxiliary/run_main.py "d1_nan"

echo "[~] check Makefile examples"
rm -rf build/
export CC=clang
./cmake_compile.sh > /dev/null 2>&1
cd doc/
make clean && make -j$(nproc)
./example
cd ..

echo "[~] Check manual_compile.sh"
./build/main help
./manual_compile.sh
./main help

echo "[~] Cleanup"
rm -rf build/ xoshiro256pp_dump_be.ppm main libfinesword.so doc/example example

exit 0
