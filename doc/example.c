#define FINESWORD_FINESWORD_NO_UNDEF_INTERNAL 1
#include <finesword/finesword.h>
#include <finesword/lovecraft.h>
#include <finesword/config.h>

#include <stdio.h>
#include <inttypes.h>


int main(void) {    
    f32 x  = QNAN32(+,  F32_MANT_MASK/U32_C(2)) // max payload
    f64 y  = SNAN64(-, U64_C(0x49))
#if FINESWORD_TARGET_128BIT_FLOAT
    f128 z = SNAN128(+, FINESWORD_U128_C_HI_LO(U64_C(0x0000737271706968), U64_C(0x6766656463626160)))
#endif
#if FINESWORD_TARGET_16BIT_FLOAT
    f16 w  = QNAN16(-, U16_C(0x111))
#endif

    (void) printf("x (QNAN32): 0x%08" PRIX32 "\n", (uint32_t)asuint_f32(x));
    {
        (void) printf("    issignminus() = %s\n", issignminus_f32(x) ? "true" : "false"); // false
        (void) printf("    isnan()       = %s\n", isnan_f32(x)       ? "true" : "false"); // true
        (void) printf("    issignaling() = %s\n", issignaling_f32(x) ? "true" : "false"); // false
        (void) printf("    isquiet()     = %s\n", isquiet_f32(x)     ? "true" : "false"); // true
    }

    (void) printf("y (SNAN64): 0x%016" PRIX64 "\n", (uint64_t)asuint_f64(y));
    {
        (void) printf("    issignminus() = %s\n", issignminus_f64(y) ? "true" : "false"); // true
        (void) printf("    isnan()       = %s\n", isnan_f64(y)       ? "true" : "false"); // true
        (void) printf("    issignaling() = %s\n", issignaling_f64(y) ? "true" : "false"); // true
        (void) printf("    isquiet()     = %s\n", isquiet_f64(y)     ? "true" : "false"); // false
    }

#if FINESWORD_TARGET_128BIT_FLOAT
    u64 hi = (u64)(asuint_f128(z) >> 64U); u64 lo = (u64)asuint_f128(z);
    (void) printf("z (SNAN128): 0x%016" PRIX64 "%016" PRIX64 "\n", (uint64_t)hi, (uint64_t)lo);
    {
        (void) printf("    issignminus() = %s\n", issignminus_f128(z) ? "true" : "false"); // false
        (void) printf("    isnan()       = %s\n", isnan_f128(z)       ? "true" : "false"); // true
        (void) printf("    issignaling() = %s\n", issignaling_f128(z) ? "true" : "false"); // true
        (void) printf("    isquiet()     = %s\n", isquiet_f128(z)     ? "true" : "false"); // false
    }
#endif

#if FINESWORD_TARGET_16BIT_FLOAT
    (void) printf("w (QNAN16): 0x%04" PRIX16 "\n", (uint16_t)asuint_f16(w));
    {
        (void) printf("    issignminus() = %s\n", issignminus_f16(w) ? "true" : "false"); // true
        (void) printf("    isnan()       = %s\n", isnan_f16(w)       ? "true" : "false"); // true
        (void) printf("    issignaling() = %s\n", issignaling_f16(w) ? "true" : "false"); // false
        (void) printf("    isquiet()     = %s\n", isquiet_f16(w)     ? "true" : "false"); // true
    }
#endif

    return 0;
}
