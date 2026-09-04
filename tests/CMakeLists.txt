# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#

project(finesword_tests
    VERSION ${FINESWORD_VERSION}
    LANGUAGES C
)

# --------------------------------------------
# FINESWORD TESTS SUBORDINATE CMAKE DIRECTORY 
# --------------------------------------------


# ---- COMPILE FINESWORD TESTS EXECUTABLE ----

file(GLOB FINESWORD_TESTS_FILES CONFIGURE_DEPENDS
    ${FINESWORD_TESTS_SRC}/main.c
    ${FINESWORD_TESTS_SRC}/comptime/*.c
    ${FINESWORD_TESTS_SRC}/shared/*.c
    ${FINESWORD_TESTS_SRC}/module/*.c
    ${FINESWORD_TESTS_SRC}/module/exh/*.c
    ${FINESWORD_TESTS_SRC}/module/list/*.c
    ${FINESWORD_TESTS_SRC}/test/*.c
    ${FINESWORD_TESTS_SRC}/test/correctness/*.c
    ${FINESWORD_TESTS_SRC}/test/performance/*.c
    ${FINESWORD_TESTS_SRC}/reference/*.c
)

add_executable(finesword_tests
    ${FINESWORD_TESTS_FILES}
)

target_include_directories(finesword_tests PUBLIC
    ${FINESWORD_INCLUDE}
)

set_target_properties(finesword_tests PROPERTIES
    C_STANDARD  99            # C99
    C_STANDARD_REQUIRED TRUE  # require
    C_EXTENSIONS        TRUE  # Extensions ALLOWED (e.g., OpenMP, POSIX)
)

if(ENABLE_OMP)
    # if(FINESWORD_IS_LLVM)
    #     message(WARNING "NOTE: LLVM OpenMP implementation (libomp) might raise floating-point \
    #     exceptions interfering and invalidating many tests (especially with non-static schedule()). \
    #     It's highly recommend to use GCC (with libgomp) if possible.")
    # endif()

    find_package(OpenMP 2.0 REQUIRED COMPONENTS C) # OpenMP 2.0+

    if (MSVC)
        # MSVC doesn't have separate libm
        target_link_libraries(finesword_tests PRIVATE   OpenMP::OpenMP_C mincore finesword) # FIX: Add mincore for QueryUnbiasedInterruptTimePrecise
    elseif(FINESWORD_IS_INTELLLVM)
         # let ICX link against their own appropriate math library
        target_link_libraries(finesword_tests PRIVATE   OpenMP::OpenMP_C finesword)
    else()
        target_link_libraries(finesword_tests PRIVATE m OpenMP::OpenMP_C finesword)
    endif()
else()
    message(WARNING "NOTE: compiling without OpenMP; tests will take significantly longer to complete.")

    if (MSVC)
        target_link_libraries(finesword_tests PRIVATE            mincore finesword) # FIX: Add mincore for QueryUnbiasedInterruptTimePrecise
    elseif(FINESWORD_IS_INTELLLVM)
        target_link_libraries(finesword_tests PRIVATE                    finesword)
    else()
        target_link_libraries(finesword_tests PRIVATE m                  finesword)
    endif()
endif()


# --- COPIED FROM TOP-LEVEL ---
# Only freestanding flags removed 
#
if(MSVC)
    message(STATUS "Tests: using MSVC target compile options")
    target_compile_options(finesword_tests PRIVATE
        /O2
        /Wall
        /permissive- /Zc:preprocessor
        /w14242 /w14244 /w14254 /w14267 /w14365
        /w14545 /w14546 /w14547 /w14549 /w14555
        /DFINESWORD_VERSION="${FINESWORD_VERSION}"
    )
elseif(FINESWORD_IS_LLVM)
    message(STATUS "Tests: using LLVM target compile options")
    target_compile_options(finesword_tests PRIVATE
        -O2 -march=native
        -Wall -Wextra -Werror -Wpedantic 
        -Wshadow -Wmissing-prototypes
        -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -Wformat=2
        -Wundef -Wwrite-strings -Warray-bounds -Wswitch-enum
        -Wcast-qual -Wvla -Wunused-variable -Wunused-value
        -Wno-unused-function
        -ffp-model=strict -ffp-exception-behavior=strict

        # try fixing doc/ci.txt note 2 with this
        # ARM does not expose -fexcess-precision=16 option (it handles 16-bit float natively) 
        $<$<NOT:$<BOOL:${FINESWORD_IS_ARM}>>:-fexcess-precision=16>

        -DFINESWORD_VERSION="${FINESWORD_VERSION}"
    )
else()
    message(STATUS "Tests: using generic GCC-style target compile options")
    target_compile_options(finesword_tests PRIVATE
        -O2 -march=native
        -Wall -Wextra -Werror -Wpedantic
        -Wshadow -Wstrict-prototypes -Wcast-align -Wconversion -Wsign-conversion
        -Wnull-dereference -Wformat=2 -Wundef -Wwrite-strings -Warray-bounds
        -Wswitch-enum -Wcast-qual -Wvla -Wunused-variable -Wunused-value
        -Wno-unused-function
        -Wno-unknown-pragmas
        -fsignaling-nans -frounding-math # new
        $<$<BOOL:${FINESWORD_IS_TINYCC}>:-w>
        
        # Add these here
        -fsignaling-nans -frounding-math

        -DFINESWORD_VERSION="${FINESWORD_VERSION}"
    )
endif()

set_target_properties(finesword_tests PROPERTIES
    OUTPUT_NAME "main"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
