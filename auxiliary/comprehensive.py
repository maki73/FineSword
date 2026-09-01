#!/usr/bin/env python3

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

import subprocess
import sys

PYTHON="python"

def run_main_args_exit(args):
    o = subprocess.run([PYTHON, "./auxiliary/run_main.py", *args])

    if o.returncode != 0:
        sys.exit(1)
    
    return


def main():
    run_main_args_exit(["show_supported"])
    run_main_args_exit(["d1_nan"])
    run_main_args_exit(["xoshiro256pp", "0x49"])
    o = subprocess.run([PYTHON, "./auxiliary/sha256.py", "xoshiro256pp_dump_be.ppm"])
    if o.returncode != 0:
        sys.exit(1)
    run_main_args_exit(["exh_unary32"])
    run_main_args_exit(["exh_unary16"])
    run_main_args_exit(["exh_binary16"])
    run_main_args_exit(["list_binary32"])
    run_main_args_exit(["list_unary64"])
    run_main_args_exit(["list_binary64"])
    run_main_args_exit(["list_unary128"])
    run_main_args_exit(["list_binary128"])

    return 0

if __name__ == "__main__":
    sys.exit(main())
