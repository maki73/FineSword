#!/usr/bin/env bash

# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#


# -- Config

FILES="
    src/*.c src/*/*.c
    include/*/*.h include/*/*/*.h
    tests/*.c tests/*/*.c tests/*/*/*.c
"

MISRA_RULES=(
    "1.1" # don't violate C and constraints
    "1.3" # no UB or crit unspec
    "2.1" "2.2" # no unrechable or dead code
    "2.6" "2.7" # no ununsed labels and parameters in functions
    "4.2" # no trigraphs
    "5.1" "5.2" # always distinct identifiers
    "5.3" # don't shadow outer identifiers
    "5.4" "5.5" "5.6" "5.7" "5.8" "5.9" # distinct identifiers things
    "6.1" # no plain int, either signed int or unsigned int
    "6.2" # no single bit fields
    "7.1" # no octals
    "7.2" # 'u'/'U' applies only to unsigned types
    "7.3" # don't use small 'l', only 'L'
    "7.4" # string literals and const char* instead of char *
    "8.1" # explicit types only
    "8.2" # name your parameters
    "8.3" # keep declarations same
    "8.4" # before external linkage have visible compatible declaration
    "8.6" # identifier with external linkage -> only one declaration
    # skipping 8.x for now
    "9.1"
    "9.2" # The initializer for an aggregate or union shall be enclosed in braces
    "9.3" # no partial array initilizations
    "9.4" # never init more than once
    "9.5" # specify size
    "10.1" "10.2" "10.3" "10.4" "10.6" "10.7" "10.8" # ESSENTIAL TYPES
    # "10.5" allow casts to inappropriate essential types (NOTE: it's 'should' and 'advisory')
    
    "11.1" # NOTE: use a generic dispatch with union (not erasing the signatures) 
    
    "11.2" "11.3" "11.4" "11.6" "11.7" "11.8" "11.9" # pointer casts
    "12.1" # explicit operator precedence
    "12.2" # x <</>> y y shall be in [0, sizeof(x)*8 - 1]
    "12.3" # no comma operators
    "12.4" # no unsigned constants expressions wrap arounds
    "13.1" "13.2" "13.3" "13.4" "13.5" "13.6" # persistent side effects stuff
    "14.1" # no float loop counters
    "14.2" # well formed for loops
    "14.3" # no invariant controlling expressions
    "14.4" # essential boolean type for if and iteration statements controllers
    "15.2" # goto shall jump to a later declaration
    "15.3" # goto label in the same block
    "15.6" # "The body of an iteration-statement or a selection-statement shall be a compound-statement"
    "15.7" # "All if … else if constructs shall be terminated with an else statement"
    "16.1" "16.2" "16.3" "16.4" "16.5" "16.6" "16.7" # switch statements
    "17.2" # no recursion
    # REMOVED; rationale: linter constanly misidentifies binary16 functions usage as a violation
    # and the compiler (e.g., Clang) will yell at you for actually doing implicit declaration with current flags anyway
    # "17.3" # no implicit function declration
    "17.4" # all non-void functions -> explicit return statement
    "17.5" # appropriate number of elements
    "17.6" # no static keyword between the []
    "17.7" #  "The value returned by a function having non-void return type shall be used"
    # to be continued...
)

MISRA_PREFIX="misra-c2012-"


# -- Check

GREP_PATTERN=()
for rule in "${MISRA_RULES[@]}"; do
    GREP_PATTERN+=(-e "[$MISRA_PREFIX$rule]")
done
shopt -s nullglob
FILES_EXPANDED=($FILES)
shopt -u nullglob

if ((${#FILES_EXPANDED[@]} == 0)); then
    echo "[!] No files found!" >&2
    exit 1
fi

echo "[~] Checking..."

{
    cppcheck \
        -Iinclude \
        --enable=all \
        --check-level=exhaustive \
        --inline-suppr \
        --suppress=unusedFunction --suppress=missingIncludeSystem \
        --addon=misra.py \
        "${FILES_EXPANDED[@]}" \
        >/dev/null
} 2>&1 | grep -F -A 2 "${GREP_PATTERN[@]}"


PIPE_STATUS=("${PIPESTATUS[@]}")
LINTER_STATUS=${PIPE_STATUS[0]}
GREP_STATUS=${PIPE_STATUS[1]}

if ((LINTER_STATUS != 0)); then
    echo "[!] Linter failed with status $LINTER_STATUS"
    exit "$LINTER_STATUS"
fi

case "$GREP_STATUS" in
    0)
        echo "[+] Done. Violation(s) found!"
        exit 1
        ;;
    1)
        echo "[+] Done. Seems alright."
        exit 0
        ;;
    *)  
        echo "[+] Done. Unknown GREP failure!"
        exit "$GREP_STATUS"
esac
