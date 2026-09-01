#!/usr/bin/env python3

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

from pathlib import Path
import sys


def last_byte_matches(filepath, desired):
    with Path(filepath).open("rb") as file:
        try:
            file.seek(-1, 2)
        except OSError:
            return False # empty files do not end with any desired byte #
        return file.read(1) == desired

def main():
    argc = len(sys.argv)
    argv = sys.argv

    if argc < 2:
        print(f"Error parsing arguments!\nUsage: {argv[0]} [directory1/file1] [directory2/file2] ...", file=sys.stderr, flush=True)
        return 1

    ret = 0

    for i in range(1, argc):
        path = argv[i]
        if Path(path).is_file() or Path(path).is_dir():
            if Path(path).is_file():
                if not last_byte_matches(path, b"\n"):
                    ret = 1
                    print(path)
            else:
                for paths in Path(path).rglob("*"):
                    if paths.is_file():
                        if not last_byte_matches(paths, b"\n"):
                            ret = 1
                            print(paths) 
        else:
            print(f"Error parsing path {i}: the path is invalid!\nUsage: {argv[0]} [directory1/file1] [directory2/file2] ...", file=sys.stderr, flush=True)
            return 1

    return ret


if __name__ == "__main__":
    sys.exit(main())
