#!/usr/bin/env python3

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

from pathlib import Path
import subprocess
import hashlib
import sys

PYTHON="python3"

def run_main_args_exit(args):
    o = subprocess.run([PYTHON, "./auxiliary/run_main.py", *args])
    if o.returncode != 0:
        sys.exit(1)
    return

def get_sha256_exit(file1):
    path = Path(file1)

    if not path.is_file():
        sys.exit(1)
        
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 ** 2), b""):
            h.update(chunk)

    return h

def main():
    print("\n\n- - - - Prerequisites - - - -\n")
    run_main_args_exit(["show_supported"])
    run_main_args_exit(["d1_nan"])

    print("\n\n- - - - Xoshiro256++ - - - -\n")
    run_main_args_exit(["xoshiro256pp", "0x49"])
    print("\nchecking hash...")
    h_a = "160859374dd86abf7ef975f66940e88a2e1f5cc432a0c98aadb9b8e27cb9b1d5"
    h_b = get_sha256_exit("./xoshiro256pp_dump_be.ppm").hexdigest()
    assert h_b == h_a, (
        f"Hash mismatch!\nexpected: {h_a}\ngot: {h_b}\nassuming 4096x4096 0x49 be serialized"
    )
    print("Hash matches")

    print("\n\n- - - - List Tests - - - -\n")
    run_main_args_exit(["list_binary32"])
    run_main_args_exit(["list_unary64"])
    run_main_args_exit(["list_binary64"])
    run_main_args_exit(["list_unary128"])
    run_main_args_exit(["list_binary128"])

    print("\n\n- - - - Exhaustive Tests - - - -\n")
    run_main_args_exit(["exh_unary32"])
    run_main_args_exit(["exh_unary16"])
    run_main_args_exit(["exh_binary16"])

    return 0

if __name__ == "__main__":
    sys.exit(main())
