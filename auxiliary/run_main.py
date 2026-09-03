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
import sys
import os
import time as t

BUILD_DIR="./build"

def main():
    if os.name == "nt": # maybe change to platform.system()? but it works so don't touch it
        exec_matches = list(Path(BUILD_DIR).rglob("main.exe"))
    else:
        exec_matches = list(Path(BUILD_DIR).rglob("main"))

    ret = 1 # fail #

    if len(exec_matches) == 1:
        print("path: ", exec_matches[0])

        env = os.environ.copy()
        print("env: OMP_CANCELLATION=TRUE", end=" ")
        env["OMP_CANCELLATION"] = "TRUE"
        if os.environ.get("CC") == 'tcc':
            print("LD_LIBRARY_PATH=", BUILD_DIR, end="")
            env["LD_LIBRARY_PATH"]  = BUILD_DIR
        print()

        print("args: pass-through")
        args = sys.argv[1:]

        print("running...", flush=True)
        t1 = t.monotonic()
        ret = subprocess.run(
            [str(exec_matches[0]), *args],
            env=env
        ).returncode
        t1 = t.monotonic() - t1
        print(f"\n'{exec_matches[0]}", end=" ")
        print(*args, end="' ")
        print(f"took: {t1}s")
    
    elif len(exec_matches) > 1:
        print("Ambiguous! Found more than one match!", file=sys.stderr, flush=True)
    else:
        print("No matching file found!",               file=sys.stderr, flush=True)

    return ret
    

if __name__ == "__main__":
    sys.exit(main())
