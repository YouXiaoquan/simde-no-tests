/* SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Copyright:
 *   2020      Evan Nemerson <evan@nemerson.com>
 */

#if !defined(SIMDE_ARM_NEON_MIN_H)
#define SIMDE_ARM_NEON_MIN_H

#include "types.h"
#include "cgt.h"
#include "bsl.h"

HEDLEY_DIAGNOSTIC_PUSH
SIMDE_DISABLE_UNWANTED_DIAGNOSTICS
SIMDE_BEGIN_DECLS_

SIMDE_FUNCTION_ATTRIBUTES
simde_float32x2_t
simde_vmin_f32(simde_float32x2_t a, simde_float32x2_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vmin_f32(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_f32(simde_vcgt_f32(b, a), a, b);
  #else
    simde_float32x2_private
      r_,
      a_ = simde_float32x2_to_private(a),
      b_ = simde_float32x2_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_float32x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vmin_f32
  #define vmin_f32(a, b) simde_vmin_f32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_float64x1_t
simde_vmin_f64(simde_float64x1_t a, simde_float64x1_t b) {
  #if defined(SIMDE_ARM_NEON_A64V8_NATIVE)
    return vmin_f64(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_f64(simde_vcgt_f64(b, a), a, b);
  #else
    simde_float64x1_private
      r_,
      a_ = simde_float64x1_to_private(a),
      b_ = simde_float64x1_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_float64x1_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A64V8_ENABLE_NATIVE_ALIASES)
  #undef vmin_f64
  #define vmin_f64(a, b) simde_vmin_f64((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int8x8_t
simde_vmin_s8(simde_int8x8_t a, simde_int8x8_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vmin_s8(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_s8(simde_vcgt_s8(b, a), a, b);
  #else
    simde_int8x8_private
      r_,
      a_ = simde_int8x8_to_private(a),
      b_ = simde_int8x8_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_int8x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vmin_s8
  #define vmin_s8(a, b) simde_vmin_s8((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int16x4_t
simde_vmin_s16(simde_int16x4_t a, simde_int16x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vmin_s16(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_s16(simde_vcgt_s16(b, a), a, b);
  #else
    simde_int16x4_private
      r_,
      a_ = simde_int16x4_to_private(a),
      b_ = simde_int16x4_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_int16x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vmin_s16
  #define vmin_s16(a, b) simde_vmin_s16((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int32x2_t
simde_vmin_s32(simde_int32x2_t a, simde_int32x2_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vmin_s32(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_s32(simde_vcgt_s32(b, a), a, b);
  #else
    simde_int32x2_private
      r_,
      a_ = simde_int32x2_to_private(a),
      b_ = simde_int32x2_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_int32x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vmin_s32
  #define vmin_s32(a, b) simde_vmin_s32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int64x1_t
simde_x_vmin_s64(simde_int64x1_t a, simde_int64x1_t b) {
  #if SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_s64(simde_vcgt_s64(b, a), a, b);
  #else
    simde_int64x1_private
      r_,
      a_ = simde_int64x1_to_private(a),
      b_ = simde_int64x1_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_int64x1_from_private(r_);
  #endif
}

SIMDE_FUNCTION_ATTRIBUTES
simde_uint8x8_t
simde_vmin_u8(simde_uint8x8_t a, simde_uint8x8_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vmin_u8(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_u8(simde_vcgt_u8(b, a), a, b);
  #else
    simde_uint8x8_private
      r_,
      a_ = simde_uint8x8_to_private(a),
      b_ = simde_uint8x8_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_uint8x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vmin_u8
  #define vmin_u8(a, b) simde_vmin_u8((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint16x4_t
simde_vmin_u16(simde_uint16x4_t a, simde_uint16x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vmin_u16(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_u16(simde_vcgt_u16(b, a), a, b);
  #else
    simde_uint16x4_private
      r_,
      a_ = simde_uint16x4_to_private(a),
      b_ = simde_uint16x4_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_uint16x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vmin_u16
  #define vmin_u16(a, b) simde_vmin_u16((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint32x2_t
simde_vmin_u32(simde_uint32x2_t a, simde_uint32x2_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vmin_u32(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_u32(simde_vcgt_u32(b, a), a, b);
  #else
    simde_uint32x2_private
      r_,
      a_ = simde_uint32x2_to_private(a),
      b_ = simde_uint32x2_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_uint32x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vmin_u32
  #define vmin_u32(a, b) simde_vmin_u32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint64x1_t
simde_x_vmin_u64(simde_uint64x1_t a, simde_uint64x1_t b) {
  #if SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbsl_u64(simde_vcgt_u64(b, a), a, b);
  #else
    simde_uint64x1_private
      r_,
      a_ = simde_uint64x1_to_private(a),
      b_ = simde_uint64x1_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_uint64x1_from_private(r_);
  #endif
}

SIMDE_FUNCTION_ATTRIBUTES
simde_float32x4_t
simde_vminq_f32(simde_float32x4_t a, simde_float32x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vminq_f32(a, b);
  #elif defined(SIMDE_X86_SSE_NATIVE)
    return _mm_min_ps(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    return vec_min(a, b);
  #elif defined(SIMDE_WASM_SIMD128_NATIVE)
    return wasm_f32x4_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_f32(simde_vcgtq_f32(b, a), a, b);
  #else
    simde_float32x4_private
      r_,
      a_ = simde_float32x4_to_private(a),
      b_ = simde_float32x4_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_float32x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vminq_f32
  #define vminq_f32(a, b) simde_vminq_f32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_float64x2_t
simde_vminq_f64(simde_float64x2_t a, simde_float64x2_t b) {
  #if defined(SIMDE_ARM_NEON_A64V8_NATIVE)
    return vminq_f64(a, b);
  #elif defined(SIMDE_X86_SSE2_NATIVE)
    return _mm_min_pd(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P7_NATIVE)
    return vec_min(a, b);
  #elif defined(SIMDE_WASM_SIMD128_NATIVE)
    return wasm_f64x2_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_f64(simde_vcgtq_f64(b, a), a, b);
  #else
    simde_float64x2_private
      r_,
      a_ = simde_float64x2_to_private(a),
      b_ = simde_float64x2_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_float64x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A64V8_ENABLE_NATIVE_ALIASES)
  #undef vminq_f64
  #define vminq_f64(a, b) simde_vminq_f64((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int8x16_t
simde_vminq_s8(simde_int8x16_t a, simde_int8x16_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vminq_s8(a, b);
  #elif defined(SIMDE_X86_SSE4_1_NATIVE)
    return _mm_min_epi8(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    return vec_min(a, b);
  #elif defined(SIMDE_WASM_SIMD128_NATIVE)
    return wasm_i8x16_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_s8(simde_vcgtq_s8(b, a), a, b);
  #else
    simde_int8x16_private
      r_,
      a_ = simde_int8x16_to_private(a),
      b_ = simde_int8x16_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_int8x16_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vminq_s8
  #define vminq_s8(a, b) simde_vminq_s8((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int16x8_t
simde_vminq_s16(simde_int16x8_t a, simde_int16x8_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vminq_s16(a, b);
  #elif defined(SIMDE_X86_SSE2_NATIVE)
    return _mm_min_epi16(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    return vec_min(a, b);
  #elif defined(SIMDE_WASM_SIMD128_NATIVE)
    return wasm_i16x8_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_s16(simde_vcgtq_s16(b, a), a, b);
  #else
    simde_int16x8_private
      r_,
      a_ = simde_int16x8_to_private(a),
      b_ = simde_int16x8_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_int16x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vminq_s16
  #define vminq_s16(a, b) simde_vminq_s16((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int32x4_t
simde_vminq_s32(simde_int32x4_t a, simde_int32x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vminq_s32(a, b);
  #elif defined(SIMDE_X86_SSE4_1_NATIVE)
    return _mm_min_epi32(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    return vec_min(a, b);
  #elif defined(SIMDE_WASM_SIMD128_NATIVE)
    return wasm_i32x4_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_s32(simde_vcgtq_s32(b, a), a, b);
  #else
    simde_int32x4_private
      r_,
      a_ = simde_int32x4_to_private(a),
      b_ = simde_int32x4_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_int32x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vminq_s32
  #define vminq_s32(a, b) simde_vminq_s32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int64x2_t
simde_x_vminq_s64(simde_int64x2_t a, simde_int64x2_t b) {
  #if defined(SIMDE_X86_AVX512VL_NATIVE)
    return _mm_min_epi64(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P7_NATIVE)
    return vec_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_s64(simde_vcgtq_s64(b, a), a, b);
  #else
    simde_int64x2_private
      r_,
      a_ = simde_int64x2_to_private(a),
      b_ = simde_int64x2_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_int64x2_from_private(r_);
  #endif
}

SIMDE_FUNCTION_ATTRIBUTES
simde_uint8x16_t
simde_vminq_u8(simde_uint8x16_t a, simde_uint8x16_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vminq_u8(a, b);
  #elif defined(SIMDE_X86_SSE2_NATIVE)
    return _mm_min_epu8(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    return vec_min(a, b);
  #elif defined(SIMDE_WASM_SIMD128_NATIVE)
    return wasm_u8x16_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_u8(simde_vcgtq_u8(b, a), a, b);
  #else
    simde_uint8x16_private
      r_,
      a_ = simde_uint8x16_to_private(a),
      b_ = simde_uint8x16_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_uint8x16_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vminq_u8
  #define vminq_u8(a, b) simde_vminq_u8((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint16x8_t
simde_vminq_u16(simde_uint16x8_t a, simde_uint16x8_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vminq_u16(a, b);
  #elif defined(SIMDE_X86_SSE4_1_NATIVE)
    return _mm_min_epu16(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    return vec_min(a, b);
  #elif defined(SIMDE_WASM_SIMD128_NATIVE)
    return wasm_u16x8_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_u16(simde_vcgtq_u16(b, a), a, b);
  #else
    simde_uint16x8_private
      r_,
      a_ = simde_uint16x8_to_private(a),
      b_ = simde_uint16x8_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_uint16x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vminq_u16
  #define vminq_u16(a, b) simde_vminq_u16((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint32x4_t
simde_vminq_u32(simde_uint32x4_t a, simde_uint32x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vminq_u32(a, b);
  #elif defined(SIMDE_X86_SSE4_1_NATIVE)
    return _mm_min_epu32(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    return vec_min(a, b);
  #elif defined(SIMDE_WASM_SIMD128_NATIVE)
    return wasm_u32x4_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_u32(simde_vcgtq_u32(b, a), a, b);
  #else
    simde_uint32x4_private
      r_,
      a_ = simde_uint32x4_to_private(a),
      b_ = simde_uint32x4_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_uint32x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vminq_u32
  #define vminq_u32(a, b) simde_vminq_u32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint64x2_t
simde_x_vminq_u64(simde_uint64x2_t a, simde_uint64x2_t b) {
  #if defined(SIMDE_POWER_ALTIVEC_P7_NATIVE)
    return vec_min(a, b);
  #elif SIMDE_NATURAL_VECTOR_SIZE > 0
    return simde_vbslq_u64(simde_vcgtq_u64(b, a), a, b);
  #else
    simde_uint64x2_private
      r_,
      a_ = simde_uint64x2_to_private(a),
      b_ = simde_uint64x2_to_private(b);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = (a_.values[i] < b_.values[i]) ? a_.values[i] : b_.values[i];
    }

    return simde_uint64x2_from_private(r_);
  #endif
}

SIMDE_END_DECLS_
HEDLEY_DIAGNOSTIC_POP

#endif /* !defined(SIMDE_ARM_NEON_MIN_H) */
