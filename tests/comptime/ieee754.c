// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2026 Mark H. (aka maki73)
 *
 * This file is part of FineSword Math Library's Testing Infrastructure.
 * See COPYING and COPYING.LESSER for the full license text.
 */

#include "finesword/rudiments/preprocessor.h"
#include "finesword/rudiments/ieee754.h"
#include "finesword/rudiments/types.h"
#include "finesword/config.h"


#define X(x, y) FINESWORD_STATIC_ASSERT(x, y)

/* -- Layout -- */

X(F32_RADIX_B == 2,     binary_radix_must_be_2_by_definition);
X(F64_RADIX_B == 2,     binary_radix_must_be_2_by_definition);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_RADIX_B == 2, binary_radix_must_be_2_by_definition);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_RADIX_B == 2, binary_radix_must_be_2_by_definition);
#endif

// 1 byte SHALL be 8 bits by the contract
X(F32_TYPE_WIDTH_K == (sizeof(f32)*8),     type_width_must_be_types_sizeof_mul_8);
X(F64_TYPE_WIDTH_K == (sizeof(f64)*8),     type_width_must_be_types_sizeof_mul_8);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_TYPE_WIDTH_K == (sizeof(f16)*8), type_width_must_be_types_sizeof_mul_8);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_TYPE_WIDTH_K ==    (sizeof(f128)*8), type_width_must_be_types_sizeof_mul_8);
#endif

X(F32_PRECISION_P == (F32_MANT_WIDTH_T+1),       p_must_be_t_add_one);
X(F64_PRECISION_P == (F64_MANT_WIDTH_T+1),       p_must_be_t_add_one);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_PRECISION_P ==  (F16_MANT_WIDTH_T+1),  p_must_be_t_add_one);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_PRECISION_P == (F128_MANT_WIDTH_T+1), p_must_be_t_add_one);
#endif

X(F32_EMAX == ((U32_C(1) << (F32_EXP_WIDTH_W-1)) - U32_C(1)),           emax_must_be_1_shl_by_w_sub_1_then_sub_1);
X(F64_EMAX == ((U64_C(1) << (F64_EXP_WIDTH_W-1)) - U64_C(1)),           emax_must_be_1_shl_by_w_sub_1_then_sub_1);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_EMAX ==  (u16)((U16_C(1) << (F16_EXP_WIDTH_W-1)) - U16_C(1)), emax_must_be_1_shl_by_w_sub_1_then_sub_1);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_EMAX ==     (U128_C(1) << (F128_EXP_WIDTH_W-1)) - U128_C(1), emax_must_be_1_shl_by_w_sub_1_then_sub_1);
#endif

X(F32_EMIN == (1-F32_EMAX),          emin_must_be_1_sub_emax);
X(F64_EMIN == (1-F64_EMAX),          emin_must_be_1_sub_emax);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_EMIN ==      (1-F16_EMAX), emin_must_be_1_sub_emax);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_EMIN ==    (1-F128_EMAX), emin_must_be_1_sub_emax);
#endif

X(F32_BIAS == F32_EMAX,          bias_must_be_emax);
X(F64_BIAS == F64_EMAX,          bias_must_be_emax);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_BIAS == F16_EMAX,      bias_must_be_emax);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_BIAS ==    F128_EMAX, bias_must_be_emax);
#endif

X(F32_SIGN_WIDTH == 1,      sign_width_must_be_1);
X(F64_SIGN_WIDTH == 1,      sign_width_must_be_1);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_SIGN_WIDTH == 1,  sign_width_must_be_1);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_SIGN_WIDTH == 1, sign_width_must_be_1);
#endif

X(F32_EXP_WIDTH_W == (F32_TYPE_WIDTH_K-F32_PRECISION_P),        w_must_be_k_sub_p);
X(F64_EXP_WIDTH_W == (F64_TYPE_WIDTH_K-F64_PRECISION_P),        w_must_be_k_sub_p);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_EXP_WIDTH_W == (F16_TYPE_WIDTH_K-F16_PRECISION_P),    w_must_be_k_sub_p);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_EXP_WIDTH_W == (F128_TYPE_WIDTH_K-F128_PRECISION_P), w_must_be_k_sub_p);
#endif

X(F32_MANT_WIDTH_T == (F32_TYPE_WIDTH_K-(F32_EXP_WIDTH_W+1)),        t_must_be_k_sub_from_w_add_1);
X(F64_MANT_WIDTH_T == (F64_TYPE_WIDTH_K-(F64_EXP_WIDTH_W+1)),        t_must_be_k_sub_from_w_add_1);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_MANT_WIDTH_T == (F16_TYPE_WIDTH_K-(F16_EXP_WIDTH_W+1)),    t_must_be_k_sub_from_w_add_1);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_MANT_WIDTH_T == (F128_TYPE_WIDTH_K-(F128_EXP_WIDTH_W+1)), t_must_be_k_sub_from_w_add_1);
#endif

X((F32_SIGN_BIT_INDEX == (F32_TYPE_WIDTH_K-1)) && (F32_SIGN_BIT_INDEX == (F32_EXP_WIDTH_W+F32_MANT_WIDTH_T)),          sign_index_must_be_k_sub_1_and_w_add_t_);
X((F64_SIGN_BIT_INDEX == (F64_TYPE_WIDTH_K-1)) && (F64_SIGN_BIT_INDEX == (F64_EXP_WIDTH_W+F64_MANT_WIDTH_T)),          sign_index_must_be_k_sub_1_and_w_add_t_);
#if FINESWORD_TARGET_16BIT_FLOAT
    X((F16_SIGN_BIT_INDEX == (F16_TYPE_WIDTH_K-1)) && (F16_SIGN_BIT_INDEX == (F16_EXP_WIDTH_W+F16_MANT_WIDTH_T)),      sign_index_must_be_k_sub_1_and_w_add_t_);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X((F128_SIGN_BIT_INDEX == (F128_TYPE_WIDTH_K-1)) && (F128_SIGN_BIT_INDEX == (F128_EXP_WIDTH_W+F128_MANT_WIDTH_T)), sign_index_must_be_k_sub_1_and_w_add_t_);
#endif

X(F32_D1_BIT_INDEX == (F32_MANT_WIDTH_T-1),       d1_bit_index_must_be_t_sub_1);
X(F64_D1_BIT_INDEX == (F64_MANT_WIDTH_T-1),       d1_bit_index_must_be_t_sub_1);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_D1_BIT_INDEX  ==  (F16_MANT_WIDTH_T-1), d1_bit_index_must_be_t_sub_1);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_D1_BIT_INDEX == (F128_MANT_WIDTH_T-1), d1_bit_index_must_be_t_sub_1);
#endif


// no PMIN check (would likely be possible in C++, or runtime C)


/* -- Integer Boundaries -- */

// no NO_FRAC_FROM and IS_EVEN_FROM checks (those need asuint)


/* -- Masks -- */

X(F32_D1_BIT_MASK == (U32_C(0x1) << (F32_MANT_WIDTH_T-1)),          d1_bit_mask_must_be_1_shl_by_t_sub_1);
X(F64_D1_BIT_MASK == (U64_C(0x1) << (F64_MANT_WIDTH_T-1)),          d1_bit_mask_must_be_1_shl_by_t_sub_1);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_D1_BIT_MASK == (u16)(U16_C(0x1) << (F16_MANT_WIDTH_T-1)), d1_bit_mask_must_be_1_shl_by_t_sub_1);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_D1_BIT_MASK == (U128_C(0x1) << (F128_MANT_WIDTH_T-1)),   d1_bit_mask_must_be_1_shl_by_t_sub_1);
#endif

X(F32_SIGN_MASK == (U32_C(0x1) << (F32_MANT_WIDTH_T+F32_EXP_WIDTH_W)),          sign_mask_must_be_1_shl_by_t_add_w);
X(F64_SIGN_MASK == (U64_C(0x1) << (F64_MANT_WIDTH_T+F64_EXP_WIDTH_W)),          sign_mask_must_be_1_shl_by_t_add_w);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_SIGN_MASK == (u16)(U16_C(0x1) << (F16_MANT_WIDTH_T+F16_EXP_WIDTH_W)), sign_mask_must_be_1_shl_by_t_add_w);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_SIGN_MASK == (U128_C(0x1) << (F128_MANT_WIDTH_T+F128_EXP_WIDTH_W)),  sign_mask_must_be_1_shl_by_t_add_w);
#endif

X(F32_EXP_MASK == (((F32_EMAX << 1) | U32_C(0x1)) << F32_MANT_WIDTH_T),          exp_mask_must_be_emax_shl_1_and_by_1_shl_by_t_);
X(F64_EXP_MASK == (((F64_EMAX << 1) | U64_C(0x1)) << F64_MANT_WIDTH_T),          exp_mask_must_be_emax_shl_1_and_by_1_shl_by_t_);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_EXP_MASK == (u16)(((F16_EMAX << 1) | U16_C(0x1)) << F16_MANT_WIDTH_T), exp_mask_must_be_emax_shl_1_and_by_1_shl_by_t_);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_EXP_MASK == (((F128_EMAX << 1) | U128_C(0x1)) << F128_MANT_WIDTH_T),  exp_mask_must_be_emax_shl_1_and_by_1_shl_by_t_);
#endif

X(F32_MANT_MASK == ((U32_C(0x1) << F32_MANT_WIDTH_T) - U32_C(1)),          mant_mask_must_be_1_shl_t_then_sub_1);
X(F64_MANT_MASK == ((U64_C(0x1) << F64_MANT_WIDTH_T) - U64_C(1)),          mant_mask_must_be_1_shl_t_then_sub_1);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_MANT_MASK == (u16)((U16_C(0x1) << F16_MANT_WIDTH_T) - U16_C(1)), mant_mask_must_be_1_shl_t_then_sub_1);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_MANT_MASK == ((U128_C(0x1) << F128_MANT_WIDTH_T) - U128_C(1)),  mant_mask_must_be_1_shl_t_then_sub_1);
#endif

X(F32_ABS_MASK == (~F32_SIGN_MASK),          abs_mask_must_be_not_sign_mask);
X(F64_ABS_MASK == (~F64_SIGN_MASK),          abs_mask_must_be_not_sign_mask);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_ABS_MASK == (u16)(~F16_SIGN_MASK), abs_mask_must_be_not_sign_mask);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_ABS_MASK ==    (~F128_SIGN_MASK), abs_mask_must_be_not_sign_mask);
#endif


/* -- Patterns -- */

#if FINESWORD_BINARY_FP_D1_SET_IS_QUIET
    X(F32_MINPAY_POS_QNAN == (F32_EXP_MASK | F32_D1_BIT_MASK),          when_fp_d1_set_is_quiet_1_minpay_pos_qnan_must_be_exp_mask_or_d1_bit_mask);
    X(F64_MINPAY_POS_QNAN == (F64_EXP_MASK | F64_D1_BIT_MASK),          when_fp_d1_set_is_quiet_1_minpay_pos_qnan_must_be_exp_mask_or_d1_bit_mask);
    #if FINESWORD_TARGET_16BIT_FLOAT
        X(F16_MINPAY_POS_QNAN == (u16)(F16_EXP_MASK | F16_D1_BIT_MASK), when_fp_d1_set_is_quiet_1_minpay_pos_qnan_must_be_exp_mask_or_d1_bit_mask);
    #endif
    #if FINESWORD_TARGET_128BIT_FLOAT
        X(F128_MINPAY_POS_QNAN ==   (F128_EXP_MASK | F128_D1_BIT_MASK), when_fp_d1_set_is_quiet_1_minpay_pos_qnan_must_be_exp_mask_or_d1_bit_mask);
    #endif

    X(F32_MINPAY_POS_SNAN == (F32_EXP_MASK | U32_C(0x1)),          when_fp_d1_set_is_quiet_1_minpay_pos_snan_must_be_exp_mask_or_1);
    X(F64_MINPAY_POS_SNAN == (F64_EXP_MASK | U64_C(0x1)),          when_fp_d1_set_is_quiet_1_minpay_pos_snan_must_be_exp_mask_or_1);
    #if FINESWORD_TARGET_16BIT_FLOAT
        X(F16_MINPAY_POS_SNAN == (u16)(F16_EXP_MASK | U16_C(0x1)), when_fp_d1_set_is_quiet_1_minpay_pos_snan_must_be_exp_mask_or_1);
    #endif
    #if FINESWORD_TARGET_128BIT_FLOAT
        X(F128_MINPAY_POS_SNAN ==   (F128_EXP_MASK | U128_C(0x1)), when_fp_d1_set_is_quiet_1_minpay_pos_snan_must_be_exp_mask_or_1);
    #endif
#else
    X(F32_MINPAY_POS_QNAN == (F32_EXP_MASK | U32_C(0x1)),          when_fp_d1_set_is_quiet_0_minpay_pos_qnan_must_be_exp_mask_or_1);
    X(F64_MINPAY_POS_QNAN == (F64_EXP_MASK | U64_C(0x1)),          when_fp_d1_set_is_quiet_0_minpay_pos_qnan_must_be_exp_mask_or_1);
    #if FINESWORD_TARGET_16BIT_FLOAT
        X(F16_MINPAY_POS_QNAN == (u16)(F16_EXP_MASK | U16_C(0x1)), when_fp_d1_set_is_quiet_0_minpay_pos_qnan_must_be_exp_mask_or_1);
    #endif
    #if FINESWORD_TARGET_128BIT_FLOAT
        X(F128_MINPAY_POS_QNAN == (F128_EXP_MASK | U128_C(0x1)),   when_fp_d1_set_is_quiet_0_minpay_pos_qnan_must_be_exp_mask_or_1);
    #endif

    X(F32_MINPAY_POS_SNAN == (F32_EXP_MASK | F32_D1_BIT_MASK),          when_fp_d1_set_is_quiet_0_minpay_pos_snan_must_be_exp_mask_or_d1_bit_mask);
    X(F64_MINPAY_POS_SNAN == (F64_EXP_MASK | F64_D1_BIT_MASK),          when_fp_d1_set_is_quiet_0_minpay_pos_snan_must_be_exp_mask_or_d1_bit_mask);
    #if FINESWORD_TARGET_16BIT_FLOAT
        X(F16_MINPAY_POS_SNAN == (u16)(F16_EXP_MASK | F16_D1_BIT_MASK), when_fp_d1_set_is_quiet_0_minpay_pos_snan_must_be_exp_mask_or_d1_bit_mask);
    #endif
    #if FINESWORD_TARGET_128BIT_FLOAT
        X(F128_MINPAY_POS_SNAN ==   (F128_EXP_MASK | F128_D1_BIT_MASK), when_fp_d1_set_is_quiet_0_minpay_pos_snan_must_be_exp_mask_or_d1_bit_mask);
    #endif
#endif


X(F32_POS_INF == F32_EXP_MASK,          pos_inf_must_be_exp_mask);
X(F64_POS_INF == F64_EXP_MASK,          pos_inf_must_be_exp_mask);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_POS_INF == (u16)F16_EXP_MASK, pos_inf_must_be_exp_mask);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_POS_INF == F128_EXP_MASK,    pos_inf_must_be_exp_mask);
#endif

X(F32_NEG_INF == (F32_EXP_MASK | F32_SIGN_MASK),          neg_inf_must_be_exp_mask_or_sign_mask);
X(F64_NEG_INF == (F64_EXP_MASK | F64_SIGN_MASK),          neg_inf_must_be_exp_mask_or_sign_mask);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_NEG_INF == (u16)(F16_EXP_MASK | F16_SIGN_MASK), neg_inf_must_be_exp_mask_or_sign_mask);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_NEG_INF ==   (F128_EXP_MASK | F128_SIGN_MASK), neg_inf_must_be_exp_mask_or_sign_mask);
#endif

X(F32_MAX_NORMAL == (F32_EXP_MASK - U32_C(1)),          max_normal_must_be_exp_mask_sub_1);
X(F64_MAX_NORMAL == (F64_EXP_MASK - U64_C(1)),          max_normal_must_be_exp_mask_sub_1);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_MAX_NORMAL == (u16)(F16_EXP_MASK - U16_C(1)), max_normal_must_be_exp_mask_sub_1);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_MAX_NORMAL ==   (F128_EXP_MASK - U128_C(1)), max_normal_must_be_exp_mask_sub_1);
#endif

X(F32_POS_MIN_NORMAL == (U32_C(0x1) << F32_MANT_WIDTH_T),          pos_min_normal_must_be_1_shl_t_);
X(F64_POS_MIN_NORMAL == (U64_C(0x1) << F64_MANT_WIDTH_T),          pos_min_normal_must_be_1_shl_t_);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_POS_MIN_NORMAL == (u16)(U16_C(0x1) << F16_MANT_WIDTH_T), pos_min_normal_must_be_1_shl_t_);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_POS_MIN_NORMAL ==   (U128_C(0x1) << F128_MANT_WIDTH_T), pos_min_normal_must_be_1_shl_t_);
#endif

X(F32_MAX_SUBNORMAL == F32_MANT_MASK,          max_subnormal_must_be_mant_mask);
X(F64_MAX_SUBNORMAL == F64_MANT_MASK,          max_subnormal_must_be_mant_mask);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_MAX_SUBNORMAL == (u16)F16_MANT_MASK, max_subnormal_must_be_mant_mask);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_MAX_SUBNORMAL == F128_MANT_MASK,    max_subnormal_must_be_mant_mask);
#endif

X(F32_POS_MIN_SUBNORMAL == U32_C(0x1),            pos_min_subnormal_must_be_1);
X(F64_POS_MIN_SUBNORMAL == U64_C(0x1),            pos_min_subnormal_must_be_1);
#if FINESWORD_TARGET_16BIT_FLOAT
    X(F16_POS_MIN_SUBNORMAL == (u16)(U16_C(0x1)), pos_min_subnormal_must_be_1);
#endif
#if FINESWORD_TARGET_128BIT_FLOAT
    X(F128_POS_MIN_SUBNORMAL ==      U128_C(0x1), pos_min_subnormal_must_be_1);
#endif
