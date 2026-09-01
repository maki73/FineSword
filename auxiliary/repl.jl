# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2026 Mark H. (aka maki73)
#
# This file is part of FineSword Math Library's Testing Infrastructure.
# See COPYING and COPYING.LESSER for the full license text.
#



using Quadmath # Float128() (our codebase's f128)
using Printf   # @sprintf


# fast fun cast
u16(x)  = if x <= typemax(UInt16) UInt16(x) else UInt16(x %  UInt32(2)^16) end
u32(x)  = if x <= typemax(UInt32) UInt32(x) else UInt32(x %  UInt64(2)^32) end
u64(x)  = if x <= typemax(UInt64) UInt64(x) else UInt64(x % UInt128(2)^64) end
u128(x) = UInt128(x) # no UInt256 type for 2^128

f16(x)  = Float16(x)
f32(x)  = Float32(x)
f64(x)  = Float64(x)
f128(x) = Float128(x)

# FloatN <-> UintN bit casts
as(x::UInt16)  = reinterpret(Float16,  x)
as(x::UInt32)  = reinterpret(Float32,  x)
as(x::UInt64)  = reinterpret(Float64,  x)
as(x::UInt128) = reinterpret(Float128, x)

as(x::Float16)  = reinterpret(UInt16,  x)
as(x::Float32)  = reinterpret(UInt32,  x)
as(x::Float64)  = reinterpret(UInt64,  x)
as(x::Float128) = reinterpret(UInt128, x)

# layout constants
const B32 = 2
const K32 = 32
const P32 = 24
const EMAX32 =  127
const EMIN32 = -126
const BIAS32 =  127
const SIGN_WIDTH32 = 1
const W32 = 8
const T32 = 23
const SIGN_BIT_INDEX32 = 31
const PMIN32 = 9

const B64 = 2
const K64 = 64
const P64 = 53
const EMAX64 =  1023
const EMIN64 = -1022
const BIAS64 =  1023
const SIGN_WIDTH64 = 1
const W64 = 11
const T64 = 52
const SIGN_BIT_INDEX64 = 63
const PMIN64 = 17

const B16 = 2
const K16 = 16
const P16 = 11
const EMAX16 =  15
const EMIN16 = -14
const BIAS16 =  15
const SIGN_WIDTH16 = 1
const W16 = 5
const T16 = 10
const SIGN_BIT_INDEX16 = 15
const PMIN16 = 5

const B128 = 2
const K128 = 128
const P128 = 113
const EMAX128 =  16383
const EMIN128 = -16382
const BIAS128 =  16383
const SIGN_WIDTH128 = 1
const W128 = 15
const T128 = 112
const SIGN_BIT_INDEX128 = 127
const PMIN128 = 36

# bitize
bit(x)  = bitstring(x)
function bitf(x)
    table = (
        (UInt16,  W16),
        (UInt32,  W32),
        (UInt64,  W64),
        (UInt128, Int((W128+1)/4)) # special hex rule (+ sign bit)
    )

    for (u, w) in table
        if x isa u
            if u != UInt128
                chars  = collect(bitstring(x))
                println(
                    "// ",
                    chars[1],
                    " ",
                    join(chars[2:(w+1)]),
                    " ",
                    join(chars[(w+2):end]),
                )
                return
            else
                chars  = @sprintf("%032X", x)
                println(
                    "// 0x",
                    chars[1:w], # sign amd exp
                    " ",
                    chars[(w+1):end]  # mantissa
                )
                println("HI: 0x", chars[1:16])
                println("LO: 0x", chars[17:end])
                return
            end
        end
    end

    throw(ArgumentError("incorrect type passed to as()!"))
end
