# Finesword Math Libary

FineSword Math Library is a self-contained math library written primarily in ISO C99
and aimed at delivering a subset of IEEE Std 754-2019 (henceforth IEEE 754) -defined operations
respecting the compliance contract, as far as portable C within stated assumptions allows.

The correctness contract includes, but is not limited to, the following aspects:
correct handling of signaling NaNs, signed zeros, and the 5 IEEE 754-defined floating-point
exception status flags, results that are correctly rounded (where applicable) and
appropriate for their rounding direction, along with *reasonable*
performance for 32 and 64-bit width variants.
When enabled, 16 and 128-bit variants behave correctly according to their semantics, but are not
necessarily *reasonable* in terms of the performance.

## Assumptions

Cornerstone assumptions this project relies on are:
- 1 byte is 8 bits
- the underlying system (e.g., CPU) or the implementation provided by the compiler
is conformant to IEEE 754. Any non-conformance (such as addition(x, sNaN)
not signaling the invalid operation exception) may, and likely will, result
in incorrect behavior. This does not necessarily concern the *software emulated mode* (planned in the future)
- compiler being faithful to ISO/IEC 9899:1999 or its 2024 year revision when compiled with 16/128-bit floating-point
support (also referred to as simply C99 and C24/C23 respectively)

Many additional assumptions regarding type widths can be found in the
[types.h](include/finesword/rudiments/types.h) and [config.h](include/finesword/config.h) files.
They are not considered *cornerstone* because they are localized and most importantly - can be easily
changed. For instance, if your platform's `float` is not Binary32, you can easily swap it by changing
the definition of `f32` in [types.h](include/finesword/rudiments/types.h) and (possibly) changing
its compile-time test at [types_related.h](tests/comptime/types_related.c), unless you don't need
FineSword Math Library's Testing Infrastructure at all and therefore won't compile anything at tests/ .

## Licensing

Two separate licenses are used for this monorepo:
LGPL-2.1-or-later and GPL-2.0-or-later.

The Finesword Math Libary is licensed under the GNU Lesser General Public License version 2.1 or later.
The FineSword Math Library's Testing Infrastructure is licensed under the GNU General Public License
version 2.0 or later.

See the [COPYING](COPYING) and [COPYING.LESSER](COPYING.LESSER) files for the full licenses' texts.

NOTE: SPDX License Identifiers are used throughout the project along with Copyright
banners. The [LICENSES/](LICENSES/) directory is kept for compatiblity with [REUSE](https://reuse.software/),
which we use for linting.

## Build

This project uses CMake, to build it first clone the repository:
```bash
$ git clone <PROJECT URL>
```
move to the cloned directory (e.g., with 'cd finesword/')

generate the projects build system and compile:
```bash
$ cmake -S . -B build -DENABLE_TESTS=TRUE CMAKE_BUILD_TYPE=Release
$ cmake --build build --config Release
```
that will compile the `finesword` shared library (Finesword Math Libary)
and `main` executable (FineSword Math Library's Testing Infrastructure).

### CMake flags:

`ENABLE_TESTS` is used to control whether the FineSword Math Library's Testing Infrastructure
will be build too, generating the `main` executable with comprehensive tests.
The default value is "FALSE", which will only generate the shared library. 

`ENABLE_OMP` enables or disables [OpenMP](https://www.openmp.org/) used mainly to parallize exhaustive tests
(see: [exhaustive.h](include/finesword_tests/test/correctness/exhaustive.h) and [exhaustive.c](tests/test/correctness/exhaustive.c)).
The default value is "TRUE".

`ENABLE_LTO` enables or disabled interprocedural optimizations (via CMAKE_INTERPROCEDURAL_OPTIMIZATION).
The default value is "FALSE" and this option is often incompatible with MSVC.

### Project's Configuration

To enable/disable 16 and 128-bit floating-point support, either uncomment and change the
`FINESWORD_TARGET_16BIT_FLOAT` and `FINESWORD_TARGET_128BIT_FLOAT` in the [config.h](include/finesword/config.h) file
or define them yourself with `-DCMAKE_C_FLAGS="-DFINESWORD_TARGET_16BIT_FLOAT=<0/1> -DFINESWORD_TARGET_128BIT_FLOAT=<0/1>"`.

To use memcpy (C++ compatible) rather than union-based type punning for the [astype.h](include/finesword/rudiments/astype.h) utilities define
`FINESWORD_ASTYPE_USE_MEMCPY`
as 1. Note that value of 1 might be incompatible with the default CMake configurtion (because of '-nostdlib' flags). 

To enable/disable ANSI do the equivalent as above but for
`FINESWORD_TESTS_USE_ANSI`, this is especially relevant if you plan to pipe the `main`'s output.

`FINESWORD_BINARY_FP_D1_SET_IS_QUIET` is used to control whether the project will be compiled assuming
that bit *d1* (i.e., the trailing significant's MSB) of binary floating-point datum set to 1 will be used for quiet or singal NaNs.
If it's 1, the recommend IEEE 754 encoding will be assumed.
See the "Quieter" part of [exception.h](include/finesword/rudiments/exception.h) for more implementation specific information. 

Generally, Heuristics pick the config's options at compile-time.

### Targets

Targets: <!-- TODO: check that it's up to date -->
| Arch     | OS       | Compiler   | Types            | Status     |
|:--------:|:--------:|:----------:|:----------------:|:----------:|
| x86_64   | Linux    | GCC        | F16,F32,F64,F128 | 1          |
| x86_64   | Linux    | Clang      | F16,F32,F64,F128 | 2          |
| x86_64   | Linux    | ICX        | F16,F32,F64,F128 | 2          |
| x86_64   | Linux    | TinyCC     | F32,F64          | 3&dagger;  |
| x86_64   | Windows  | MSVC       | F32,F64          | 3          |
| AArch64  | Linux    | GCC        | F16,F32,F64      | 2          |
| AArch64  | Linux    | Clang      | F16,F32,F64      | 2          |
| AArch64  | FreeBSD  | Clang      | F16,F32,F64      | 3\*&dagger;|
| AArch64  | FreeBSD  | GCC        | F16,F32,F64      | 3\*&dagger;|
| arm64    | macOS    | AppleClang | F16,F32,F64      | 3          |
| s390x    | Linux    | GCC        | F16,F32,F64,F128 | 3\*        |
| PA-RISC  | NetBSD   | GCC        | F32,F64          | 3\*&dagger;|

1 - works best;
2 - works great but some rough edges exist;
3 - functional

\*emulated;
&dagger; tested without exhaustive tests

In general, the main library should work on any target supporting C,
including freestanding environments (but only with changes to the CMake configuration).

NOTE: PA-RISC was compiled with [manual_compile.sh](manual_compile.sh) rather than CMake
(which embeds FineSword Math Libary in the `main` executable), because I wasn't able to get `cmake` running on it.

## Usage & Examples

NOTE: for performance tests, link against the FineSword Math Libary **statically** (e.g., with [./manual_compile.sh](./manual_compile.sh))

<!-- TODO: Add examples -->
