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
import re

INCLUDE=Path("./include")

def get_path_identifier(filepath):
    relative = Path(filepath).resolve().relative_to(
        INCLUDE.resolve()
    )
    return re.sub(
        r"[^A-Z0-9]+",
        "_",
        relative.as_posix().upper(),
    ).strip("_")


def check_guard(filepath):
    identifier   = get_path_identifier(filepath)
    first_clause = (
        "#ifndef "   + identifier + "\n#define " + identifier
    ).encode("ascii")
    last_clause  = (
        "#endif /* " + identifier + " */\n"
    ).encode("ascii")

    with open(filepath, "rb") as file:
        data = file.read()
    
    first_pos = data.find(b"#ifndef")
    if first_pos == -1 or not data[first_pos:].startswith(first_clause):
        print(filepath)
        return 1
    
    last_pos = data.rfind(b"#endif")
    if last_pos == -1 or not data[last_pos:].startswith(last_clause):
        print(filepath)
        return 1
    
    return 0

def main():
    ret = 0

    path = INCLUDE
    if Path(path).is_file() or Path(path).is_dir():
        if Path(path).is_file():
            ret = max(check_guard(Path(path)), ret)
        else:
            for paths in Path(path).rglob("*"):
                if Path(paths).is_file():
                    ret = max(check_guard(Path(paths)), ret)

    return ret


if __name__ == "__main__":
    sys.exit(main())
