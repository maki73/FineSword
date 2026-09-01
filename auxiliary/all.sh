#!/bin/bash

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

echo "[~] ./auxiliary/license.sh..."
./auxiliary/license.py

echo "[~] ./auxiliary/rules.sh..."
./auxiliary/rules.sh

echo "[~] ./auxiliary/compile.sh..."
./auxiliary/compile.sh

echo "[~] check d1_nan"
export CC=tcc # assuming the last compile.sh compiler is TinyCC
./auxiliary/run_main.py "d1_nan"


exit 0
