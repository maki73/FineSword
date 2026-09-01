#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

set -euo pipefail

BUILD_DIR="build"
TESTS="ON"

echo "[~] Configuring CMake (build dir: $BUILD_DIR; tests: $TESTS)"
cmake -S . -B "$BUILD_DIR" -DENABLE_TESTS="$TESTS" CMAKE_BUILD_TYPE=Release

echo "[~] Building the project..."
cmake --build "$BUILD_DIR" --config Release --parallel $(nproc)

echo "[+] Done."
