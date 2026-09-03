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
    elif len(exec_matches) > 1:
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

def test_dispatch_dispatch_runtime_exit(test_name):
    lib = ctypes.CDLL(LIBS_PATH)
    if finesword_supports_16bit_float(lib):
        name = test_name + "f16"
        run_main_args_exit(["test", name])
    if finesword_supports_32bit_float(lib):
        name = test_name + "f32"
        run_main_args_exit(["test", name])
    if finesword_supports_64bit_float(lib):
        name = test_name + "f64"
        run_main_args_exit(["test", name])
    if finesword_supports_128bit_float(lib):
        name = test_name + "f128"
        run_main_args_exit(["test", name])
    return 0


# -- Main

def main():
    test_dispatch_dispatch_runtime_exit("abs_")
    test_dispatch_dispatch_runtime_exit("negate_")
    test_dispatch_dispatch_runtime_exit("copysign_")
    return 0

if __name__ == "__main__":
    sys.exit(main())

# print("LIBS_PATH: ", LIBS_PATH)
# lib = ctypes.CDLL(LIBS_PATH)
# print("finesword_supports_16bit_float:  ", "true" if finesword_supports_16bit_float(lib)  else "false")
# print("finesword_supports_32bit_float:  ", "true" if finesword_supports_32bit_float(lib)  else "false")
# print("finesword_supports_64bit_float:  ", "true" if finesword_supports_64bit_float(lib)  else "false")
# print("finesword_supports_128bit_float: ", "true" if finesword_supports_128bit_float(lib) else "false")
