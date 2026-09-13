#!/usr/bin/env python3

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

import ctypes
import platform
import sys
import subprocess
from pathlib import Path

LIBS_PATH="build/"
PYTHON="python3"
FUN_LIST=(
    # sign.h 
    "negate", "abs", "copysign",
    # classify.h
    "w_iszero", "w_issignminus", "w_isnormal", "w_issubnormal",
    "w_isfinite", "w_isinfinite", "w_isnan", "w_issignaling",
    "w_isquiet"
)

def get_library_name_exit():
    s = platform.system() 
    if   s == "Windows":
        name = "finesword.dll"
    elif s == "Darwin":
        name = "libfinesword.dylib"
    elif s == "Linux":
        name = "libfinesword.so"
    else:
        sys.exit(1)
    
    lib_matches = list(Path(LIBS_PATH).rglob(name))

    if len(lib_matches) == 1:
        return str(lib_matches[0])
    elif len(lib_matches) > 1:
        print("Ambiguous! Found more than one match!", file=sys.stderr, flush=True)
        sys.exit(1)
    else:
        print("No matching file found!",               file=sys.stderr, flush=True)
        sys.exit(1)

LIBS_PATH = get_library_name_exit()


# -- types.h: "Runtime Support Query" Wrappers

# f: void -> _Bool

def finesword_supports_16bit_float(lib):
    lib.finesword_supports_16bit_float.argtypes = []
    lib.finesword_supports_16bit_float.restype  = ctypes.c_bool
    return lib.finesword_supports_16bit_float()

def finesword_supports_32bit_float(lib):
    return True # /* always true in the current configuration */

def finesword_supports_64bit_float(lib):
    return True # /* always true in the current configuration */

def finesword_supports_128bit_float(lib):
    f = finesword_supports_128bit_float
    lib.finesword_supports_128bit_float.argtypes = []
    lib.finesword_supports_128bit_float.restype  = ctypes.c_bool
    return lib.finesword_supports_128bit_float()


# -- Dispatch thy Dispatch!

# copied from comprehensive_self.py
def run_main_args_exit(args):
    o = subprocess.run([PYTHON, "./auxiliary/run_main.py", *args])
    if o.returncode != 0:
        sys.exit(1)
    return

def test_dispatch_dispatch_runtime_exit(test_name, dis16, dis32, dis64, dis128):
    lib = ctypes.CDLL(LIBS_PATH)
    if finesword_supports_16bit_float(lib) and not dis16:
        name = test_name + "_f16"
        run_main_args_exit(["test", name])
    if finesword_supports_32bit_float(lib) and not dis32:
        name = test_name + "_f32"
        run_main_args_exit(["test", name])
    if finesword_supports_64bit_float(lib) and not dis64:
        name = test_name + "_f64"
        run_main_args_exit(["test", name])
    if finesword_supports_128bit_float(lib) and not dis128:
        name = test_name + "_f128"
        run_main_args_exit(["test", name])
    return 0


# -- Main

def main():
    for fun in FUN_LIST:
        test_dispatch_dispatch_runtime_exit(fun, False, False, False, False)

    return 0

if __name__ == "__main__":
    sys.exit(main())
