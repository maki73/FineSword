#!/usr/bin/env python3

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

import subprocess
import sys


# NOTE: be very accurate with paths especially
# around adjacent strings concat, spaces, and
# using "./PATH" rather than simply "PATH" 
LGPL_PATHS=[
    "./include/finesword/",
    "./src/"
]

GPL_PATHS=[
    "./include/finesword_tests/",
    "./tests/",
    "./auxiliary/",
    "./.github/"
]

def is_in_paths(filename, paths):
    for path in paths:
        if filename == path or filename.startswith(path.rstrip("/") + "/"):
            return True
    
    return False 

def reuse_lint():
    r = subprocess.run(["reuse", "lint"])
    if r.returncode != 0:
        return False
    else:
        return True


def main():
    if not reuse_lint():
        return 1 # immediately ret 1

    print("\nChecking GPL/LGPL paths...")
    o = subprocess.run(
        ["reuse", "spdx"],
        capture_output=True,
        text=True,
    )

    if o.returncode != 0:
        return 1 # immediately ret 1
    
    lines = o.stdout.splitlines()

    ret = 0

    for i, line in enumerate(lines):
        filename_prefix = "FileName:"
        if not line.startswith(filename_prefix):
            continue
        
        filename = line[len(filename_prefix):].strip()

        if is_in_paths(filename, LGPL_PATHS):
            expected = "LicenseInfoInFile: LGPL-2.1-or-later"
        elif is_in_paths(filename, GPL_PATHS):
            expected = "LicenseInfoInFile: GPL-2.0-or-later"
        else:
            continue

        magic_lines_below = 4
        if i+magic_lines_below >= len(lines) or lines[i+magic_lines_below].strip() != expected:
            print(filename)
            ret = 1

    if ret == 0:
        print("ok")

    return ret

if __name__ == "__main__":
    sys.exit(main())
