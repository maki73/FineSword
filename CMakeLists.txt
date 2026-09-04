# SPDX-License-Identifier: LGPL-2.1-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library.
# See COPYING and COPYING.LESSER for the full license text.
#
cmake_minimum_required(VERSION 3.16)
set(FINESWORD_VERSION "0.0.1")
project(finesword
    VERSION   ${FINESWORD_VERSION}
    LANGUAGES C
)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # for linter

# --------------------------------------------
# FINESWORD TOP-LEVEL CMAKE DIRECTORY 
# --------------------------------------------
#
set(FINESWORD_INCLUDE     ${PROJECT_SOURCE_DIR}/include) # for both tests and library
set(FINESWORD_LIBRARY_SRC ${PROJECT_SOURCE_DIR}/src)
set(FINESWORD_TESTS_SRC   ${PROJECT_SOURCE_DIR}/tests)


# --- detect features ---

message(STATUS "Detecting features and options")

# LLVM-based compiler
set(FINESWORD_KNOWN_LLVM_COMPILERS
    Clang
    AppleClang
    IntelLLVM
    ARMClang
)
if(CMAKE_C_COMPILER_ID IN_LIST FINESWORD_KNOWN_LLVM_COMPILERS)
    message(STATUS "IS_LLVM      = TRUE")
    set(FINESWORD_IS_LLVM TRUE)
else()
    message(STATUS "IS_LLVM      = FALSE")
    set(FINESWORD_IS_LLVM FALSE)
endif()

# TinyCC compiler
if(CMAKE_C_COMPILER_ID STREQUAL "TinyCC")
    message(STATUS "IS_TINYCC    = TRUE")
    set(FINESWORD_IS_TINYCC TRUE)
else()
    message(STATUS "IS_TINYCC    = FALSE")
    set(FINESWORD_IS_TINYCC FALSE)
endif()

# Intel LLVM (ICX)
if(CMAKE_C_COMPILER_ID STREQUAL "IntelLLVM")
    message(STATUS "IS_INTELLLVM = TRUE")
    set(FINESWORD_IS_INTELLLVM TRUE)
else()
    message(STATUS "IS_INTELLLVM = FALSE")
    set(FINESWORD_IS_INTELLLVM FALSE)
endif()

# ARM Target Architecture
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|AArch64|arm|ARM|arm64|ARM64)")
    message(STATUS "IS_ARM       = TRUE")
    set(FINESWORD_IS_ARM TRUE)
else()
    message(STATUS "IS_ARM       = FALSE")
    set(FINESWORD_IS_ARM FALSE)
endif()

# --- configure options ---

# -DENABLE_TESTS
if(ENABLE_TESTS)
    message(STATUS "ENABLE_TESTS = TRUE")
    set(FINESWORD_ENABLE_TESTS TRUE)
else()
    message(STATUS "ENABLE_TESTS = FALSE")
    set(FINESWORD_ENABLE_TESTS FALSE)
endif()

# - DENABLE_OMP; TRUE by default except on TinyCC (FALSE by default)
if(FINESWORD_IS_TINYCC)
    option(ENABLE_OMP "TinyCC usually does not support OpenMP" FALSE)
else()
    option(ENABLE_OMP "Enable OpenMP support" TRUE)
endif()
if(ENABLE_OMP)
    message(STATUS "ENABLE_OMP   = TRUE")
    set(FINESWORD_ENABLE_OMP TRUE)
else()
    message(STATUS "ENABLE_OMP   = FALSE")
    set(FINESWORD_ENABLE_OMP FALSE)
endif()

# - DENABLE_LTO; FALSE by default
option(ENABLE_LTO "Enable IPO/LTO" FALSE)
if(ENABLE_LTO)
    message(STATUS "ENABLE_LTO   = TRUE")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    if(MSVC)
        message(WARNING
            "CMAKE_INTERPROCEDURAL_OPTIMIZATION and WINDOWS_EXPORT_ALL_SYMBOLS \
            are often incompatible with MSVC. If the build fails, try disabling \
            IPO/LTO with -DENABLE_LTO=FALSE."
        )
    endif()
else()
    message(STATUS "ENABLE_LTO   = FALSE")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

message(STATUS "Detecting features and options - done")


# ---- COMPILE FINESWORD SHARED LIBRARY ----

file(GLOB FINESWORD_LIBRARY_FILES CONFIGURE_DEPENDS
    ${FINESWORD_LIBRARY_SRC}/rudiments/*.c
    ${FINESWORD_LIBRARY_SRC}/scalar/*.c
    # ${FINESWORD_LIBRARY_SRC}/vector/*.c vectorized routines are currently unsupported
)

add_library(finesword SHARED
    ${FINESWORD_LIBRARY_FILES}
)

target_include_directories(finesword PUBLIC
    ${FINESWORD_INCLUDE}
)

set_target_properties(finesword PROPERTIES
    C_STANDARD  99            # C99 (ISO/IEC 9899:1999)
    C_STANDARD_REQUIRED TRUE  # require it
    C_EXTENSIONS        FALSE # disable all compiler specific extensions; only ISO C
)

# set_property(TARGET finesword PROPERTY LINK_LIBRARIES "")

# 3 cases:
# - MSVC
# - LLVM-based compiler
# - Generic GCC/Clang/ICX/Xcode
if(MSVC)
    message(STATUS "Library: using MSVC target compile options")
    target_compile_options(finesword PRIVATE
        /O2

        # /X /Zl

        /Wall # No /WX (MSVC generates too much noise)
        /permissive- /Zc:preprocessor
        /w14242 /w14244 /w14254 /w14267 /w14365
        /w14545 /w14546 /w14547 /w14549 /w14555

        /DFINESWORD_VERSION="${FINESWORD_VERSION}"
    )
    set_target_properties(finesword PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    )
elseif(FINESWORD_IS_LLVM)
    message(STATUS "Library: using LLVM target compile options")
    target_compile_options(finesword PRIVATE
        -O2 -march=native
        -Wno-error=invalid-feature-combination

        -ffreestanding -nostdinc -nostdlib -nodefaultlibs # no -nostartfiles

        -Wall -Wextra -Werror -Wpedantic 
        -Wshadow -Wmissing-prototypes # -Wstrict-prototypes -> -Wmissing-prototypes
        -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -Wformat=2
        -Wundef -Wwrite-strings -Warray-bounds -Wswitch-enum
        -Wcast-qual -Wvla -Wunused-variable -Wunused-value

        -Wno-unused-function
        # No -Wno-unknown-pragmas (LLVM recognizes STDC FENV_ACCESS)

        # directly fixes ICX's complaints about STDC FENV_ACCESS,
        # indirectly sets the right tone for all LLVM-based compilers
        -ffp-model=strict -ffp-exception-behavior=strict # NOTE: GCC does not recognize these

        -DFINESWORD_VERSION="${FINESWORD_VERSION}"
    )
else()
    message(STATUS "Library: using generic GCC-style target compile options")
    target_compile_options(finesword PRIVATE
        -O2 -march=native

        -ffreestanding -nostdinc -nostdlib
        # $<$<NOT:$<BOOL:${FINESWORD_IS_TINYCC}>>:-nodefaultlibs -nostartfiles>
        $<$<NOT:$<BOOL:${FINESWORD_IS_TINYCC}>>:-nodefaultlibs>
        $<$<NOT:$<BOOL:${FINESWORD_IS_TINYCC}>>:-nostartfiles>
        # TinyCC doesn't recognize these two

        -Wall -Wextra -Werror -Wpedantic
        -Wshadow -Wstrict-prototypes -Wcast-align -Wconversion -Wsign-conversion
        -Wnull-dereference -Wformat=2 -Wundef -Wwrite-strings -Warray-bounds
        -Wswitch-enum -Wcast-qual -Wvla -Wunused-variable -Wunused-value

        -Wno-unused-function
        -Wno-unknown-pragmas # GCC doesn't recognize STDC FENV_ACCESS pragma

        # if TinyCC, add -w to suppress STDC warning
        # NOTE: a lot of collateral damage as it disables all warnings 
        $<$<BOOL:${FINESWORD_IS_TINYCC}>:-w>

        # GCC equivalent
        # -fsignaling-nans -frounding-math

        -DFINESWORD_VERSION="${FINESWORD_VERSION}"
    )
endif()


# ---- IF ENABLE_TESTS -> ADD TESTS/ SUBDIRECTORY ----

if(FINESWORD_ENABLE_TESTS)
    add_subdirectory(
        ${FINESWORD_TESTS_SRC}
    )
endif()
