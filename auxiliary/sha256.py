#!/usr/bin/env python3

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

import hashlib
from pathlib import Path 
import sys


def main():
    args = sys.argv[1:]

    if len(args) == 0:
        print(f"Error!\nUsage: {argv[0]} [file1] [file2] ...", file=sys.stderr, flush=True)
        return 1

    for arg in args:
        path = Path(arg)

        if not path.is_file():
            return 1
        
        h = hashlib.sha256()
        with path.open("rb") as f:
            for chunk in iter(lambda: f.read(1024 ** 2), b""):
                h.update(chunk)
            
        print(path, ":    ", h.hexdigest(), file=sys.stdout, flush=True)

    return 0

if __name__ == "__main__":
    sys.exit(main())
