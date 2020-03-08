/* Permission is hereby granted, free of charge, to any person
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
 *   2018      Evan Nemerson <evan@nemerson.com>
 */

#include "sse.h"
#include "sse2.h"
#if !defined(SIMDE__AVX_H)
#  if !defined(SIMDE__AVX_H)
#    define SIMDE__AVX_H
#  endif
#  include "sse4.1.h"

HEDLEY_DIAGNOSTIC_PUSH
SIMDE_DISABLE_UNWANTED_DIAGNOSTICS

#  if defined(SIMDE_AVX_NATIVE)
#    undef SIMDE_AVX_NATIVE
#  endif
#  if defined(SIMDE_ARCH_X86_AVX) && !defined(SIMDE_AVX_NO_NATIVE) && !defined(SIMDE_NO_NATIVE)
#    define SIMDE_AVX_NATIVE
#  elif defined(SIMDE_ARCH_ARM_NEON) && !defined(SIMDE_AVX_NO_NEON) && !defined(SIMDE_NO_NEON)
#    define SIMDE_AVX_NEON
#  elif defined(SIMDE_ARCH_POWER_ALTIVEC)
#    define SIMDE_AVX_POWER_ALTIVEC
#  endif

#  if defined(SIMDE_AVX_NATIVE)
#    include <immintrin.h>
#  endif

#  if defined(SIMDE_AVX_POWER_ALTIVEC)
#    include <altivec.h>
#  endif

#  include <stdint.h>
#  include <limits.h>

SIMDE__BEGIN_DECLS

typedef union {
#if defined(SIMDE_VECTOR_SUBSCRIPT)
  SIMDE_ALIGN(32) int8_t          i8 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int16_t        i16 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int32_t        i32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int64_t        i64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint8_t         u8 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint16_t       u16 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint32_t       u32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint64_t       u64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  #if defined(SIMDE__HAVE_INT128)
  SIMDE_ALIGN(32) simde_int128  i128 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) simde_uint128 u128 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  #endif
  SIMDE_ALIGN(32) simde_float32  f32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) simde_float64  f64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int_fast32_t  i32f SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint_fast32_t u32f SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
#else
  SIMDE_ALIGN(32) int8_t          i8[32];
  SIMDE_ALIGN(32) int16_t        i16[16];
  SIMDE_ALIGN(32) int32_t        i32[8];
  SIMDE_ALIGN(32) int64_t        i64[4];
  SIMDE_ALIGN(32) uint8_t         u8[32];
  SIMDE_ALIGN(32) uint16_t       u16[16];
  SIMDE_ALIGN(32) uint32_t       u32[8];
  SIMDE_ALIGN(32) uint64_t       u64[4];
  SIMDE_ALIGN(32) int_fast32_t  i32f[32 / sizeof(int_fast32_t)];
  SIMDE_ALIGN(32) uint_fast32_t u32f[32 / sizeof(uint_fast32_t)];
  #if defined(SIMDE__HAVE_INT128)
  SIMDE_ALIGN(32) simde_int128  i128[2];
  SIMDE_ALIGN(32) simde_uint128 u128[2];
  #endif
  SIMDE_ALIGN(32) simde_float32  f32[8];
  SIMDE_ALIGN(32) simde_float64  f64[4];
#endif

  SIMDE_ALIGN(32) simde__m128_private m128_private[2];
  SIMDE_ALIGN(32) simde__m128         m128[2];

#if defined(SIMDE_AVX_NATIVE)
  SIMDE_ALIGN(32) __m256         n;
#elif defined(SIMDE_ARCH_POWER_ALTIVEC)
  SIMDE_ALIGN(16) vector unsigned char      altivec_u8[2];
  SIMDE_ALIGN(16) vector unsigned short     altivec_u16[2];
  SIMDE_ALIGN(16) vector unsigned int       altivec_u32[2];
  SIMDE_ALIGN(16) vector unsigned long long altivec_u64[2];
  SIMDE_ALIGN(16) vector signed char        altivec_i8[2];
  SIMDE_ALIGN(16) vector signed short       altivec_i16[2];
  SIMDE_ALIGN(16) vector signed int         altivec_i32[2];
  SIMDE_ALIGN(16) vector signed long long   altivec_i64[2];
  SIMDE_ALIGN(16) vector float              altivec_f32[2];
  SIMDE_ALIGN(16) vector double             altivec_f64[2];
#endif
} simde__m256_private;

typedef union {
#if defined(SIMDE_VECTOR_SUBSCRIPT)
  SIMDE_ALIGN(32) int8_t          i8 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int16_t        i16 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int32_t        i32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int64_t        i64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint8_t         u8 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint16_t       u16 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint32_t       u32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint64_t       u64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  #if defined(SIMDE__HAVE_INT128)
  SIMDE_ALIGN(32) simde_int128  i128 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) simde_uint128 u128 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  #endif
  SIMDE_ALIGN(32) simde_float32  f32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) simde_float64  f64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int_fast32_t  i32f SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint_fast32_t u32f SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
#else
  SIMDE_ALIGN(32) int8_t          i8[32];
  SIMDE_ALIGN(32) int16_t        i16[16];
  SIMDE_ALIGN(32) int32_t        i32[8];
  SIMDE_ALIGN(32) int64_t        i64[4];
  SIMDE_ALIGN(32) uint8_t         u8[32];
  SIMDE_ALIGN(32) uint16_t       u16[16];
  SIMDE_ALIGN(32) uint32_t       u32[8];
  SIMDE_ALIGN(32) uint64_t       u64[4];
  #if defined(SIMDE__HAVE_INT128)
  SIMDE_ALIGN(32) simde_int128  i128[2];
  SIMDE_ALIGN(32) simde_uint128 u128[2];
  #endif
  SIMDE_ALIGN(32) simde_float32  f32[8];
  SIMDE_ALIGN(32) simde_float64  f64[4];
  SIMDE_ALIGN(32) int_fast32_t  i32f[32 / sizeof(int_fast32_t)];
  SIMDE_ALIGN(32) uint_fast32_t u32f[32 / sizeof(uint_fast32_t)];
#endif

  SIMDE_ALIGN(32) simde__m128d_private m128d_private[2];
  SIMDE_ALIGN(32) simde__m128d         m128d[2];

#if defined(SIMDE_AVX_NATIVE)
  SIMDE_ALIGN(32) __m256d        n;
#elif defined(SIMDE_ARCH_POWER_ALTIVEC)
  SIMDE_ALIGN(16) vector unsigned char      altivec_u8[2];
  SIMDE_ALIGN(16) vector unsigned short     altivec_u16[2];
  SIMDE_ALIGN(16) vector unsigned int       altivec_u32[2];
  SIMDE_ALIGN(16) vector unsigned long long altivec_u64[2];
  SIMDE_ALIGN(16) vector signed char        altivec_i8[2];
  SIMDE_ALIGN(16) vector signed short       altivec_i16[2];
  SIMDE_ALIGN(16) vector signed int         altivec_i32[2];
  SIMDE_ALIGN(16) vector signed long long   altivec_i64[2];
  SIMDE_ALIGN(16) vector float              altivec_f32[2];
  SIMDE_ALIGN(16) vector double             altivec_f64[2];
#endif
} simde__m256d_private;

typedef union {
#if defined(SIMDE_VECTOR_SUBSCRIPT)
  SIMDE_ALIGN(32) int8_t          i8 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int16_t        i16 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int32_t        i32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int64_t        i64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint8_t         u8 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint16_t       u16 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint32_t       u32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint64_t       u64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  #if defined(SIMDE__HAVE_INT128)
  SIMDE_ALIGN(32) simde_int128  i128 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) simde_uint128 u128 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  #endif
  SIMDE_ALIGN(32) simde_float32  f32 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) simde_float64  f64 SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) int_fast32_t  i32f SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  SIMDE_ALIGN(32) uint_fast32_t u32f SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
#else
  SIMDE_ALIGN(32) int8_t          i8[32];
  SIMDE_ALIGN(32) int16_t        i16[16];
  SIMDE_ALIGN(32) int32_t        i32[8];
  SIMDE_ALIGN(32) int64_t        i64[4];
  SIMDE_ALIGN(32) uint8_t         u8[32];
  SIMDE_ALIGN(32) uint16_t       u16[16];
  SIMDE_ALIGN(32) uint32_t       u32[8];
  SIMDE_ALIGN(32) uint64_t       u64[4];
  SIMDE_ALIGN(32) int_fast32_t  i32f[32 / sizeof(int_fast32_t)];
  SIMDE_ALIGN(32) uint_fast32_t u32f[32 / sizeof(uint_fast32_t)];
  #if defined(SIMDE__HAVE_INT128)
  SIMDE_ALIGN(32) simde_int128  i128[2];
  SIMDE_ALIGN(32) simde_uint128 u128[2];
  #endif
  SIMDE_ALIGN(32) simde_float32  f32[8];
  SIMDE_ALIGN(32) simde_float64  f64[4];
#endif

  SIMDE_ALIGN(32) simde__m128i_private m128i_private[2];
  SIMDE_ALIGN(32) simde__m128i         m128i[2];

#if defined(SIMDE_AVX_NATIVE)
  SIMDE_ALIGN(32) __m256i        n;
#elif defined(SIMDE_ARCH_POWER_ALTIVEC)
  SIMDE_ALIGN(16) vector unsigned char      altivec_u8[2];
  SIMDE_ALIGN(16) vector unsigned short     altivec_u16[2];
  SIMDE_ALIGN(16) vector unsigned int       altivec_u32[2];
  SIMDE_ALIGN(16) vector unsigned long long altivec_u64[2];
  SIMDE_ALIGN(16) vector signed char        altivec_i8[2];
  SIMDE_ALIGN(16) vector signed short       altivec_i16[2];
  SIMDE_ALIGN(16) vector signed int         altivec_i32[2];
  SIMDE_ALIGN(16) vector signed long long   altivec_i64[2];
  SIMDE_ALIGN(16) vector float              altivec_f32[2];
  SIMDE_ALIGN(16) vector double             altivec_f64[2];
#endif
} simde__m256i_private;

#if defined(SIMDE_AVX_NATIVE)
  typedef __m256 simde__m256;
  typedef __m256i simde__m256i;
  typedef __m256d simde__m256d;
#elif defined(SIMDE_VECTOR_SUBSCRIPT)
  typedef simde_float32 simde__m256  SIMDE_ALIGN(32) SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  typedef int_fast32_t  simde__m256i SIMDE_ALIGN(32) SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
  typedef simde_float64 simde__m256d SIMDE_ALIGN(32) SIMDE_VECTOR(32) SIMDE_MAY_ALIAS;
#else
  typedef simde__m256_private  simde__m256;
  typedef simde__m256i_private simde__m256i;
  typedef simde__m256d_private simde__m256d;
#endif

#if !defined(SIMDE_AVX_NATIVE) && defined(SIMDE_ENABLE_NATIVE_ALIASES)
  #define SIMDE_AVX_ENABLE_NATIVE_ALIASES
  typedef simde__m256 __m256;
  typedef simde__m256i __m256i;
  typedef simde__m256d __m256d;
#endif

HEDLEY_STATIC_ASSERT(32 == sizeof(simde__m256), "simde__m256 size incorrect");
HEDLEY_STATIC_ASSERT(32 == sizeof(simde__m256_private), "simde__m256_private size incorrect");
HEDLEY_STATIC_ASSERT(32 == sizeof(simde__m256i), "simde__m256i size incorrect");
HEDLEY_STATIC_ASSERT(32 == sizeof(simde__m256i_private), "simde__m256i_private size incorrect");
HEDLEY_STATIC_ASSERT(32 == sizeof(simde__m256d), "simde__m256d size incorrect");
HEDLEY_STATIC_ASSERT(32 == sizeof(simde__m256d_private), "simde__m256d_private size incorrect");
#if defined(SIMDE_CHECK_ALIGNMENT) && defined(SIMDE_ALIGN_OF)
HEDLEY_STATIC_ASSERT(SIMDE_ALIGN_OF(simde__m256) == 32, "simde__m256 is not 32-byte aligned");
HEDLEY_STATIC_ASSERT(SIMDE_ALIGN_OF(simde__m256_private) == 32, "simde__m256_private is not 32-byte aligned");
HEDLEY_STATIC_ASSERT(SIMDE_ALIGN_OF(simde__m256i) == 32, "simde__m256i is not 32-byte aligned");
HEDLEY_STATIC_ASSERT(SIMDE_ALIGN_OF(simde__m256i_private) == 32, "simde__m256i_private is not 32-byte aligned");
HEDLEY_STATIC_ASSERT(SIMDE_ALIGN_OF(simde__m256d) == 32, "simde__m256d is not 32-byte aligned");
HEDLEY_STATIC_ASSERT(SIMDE_ALIGN_OF(simde__m256d_private) == 32, "simde__m256d_private is not 32-byte aligned");
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde__m256_from_private(simde__m256_private v) {
  simde__m256 r;
  simde_memcpy(&r, &v, sizeof(r));
  return r;
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256_private
simde__m256_to_private(simde__m256 v) {
  simde__m256_private r;
  simde_memcpy(&r, &v, sizeof(r));
  return r;
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde__m256i_from_private(simde__m256i_private v) {
  simde__m256i r;
  simde_memcpy(&r, &v, sizeof(r));
  return r;
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i_private
simde__m256i_to_private(simde__m256i v) {
  simde__m256i_private r;
  simde_memcpy(&r, &v, sizeof(r));
  return r;
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde__m256d_from_private(simde__m256d_private v) {
  simde__m256d r;
  simde_memcpy(&r, &v, sizeof(r));
  return r;
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d_private
simde__m256d_to_private(simde__m256d v) {
  simde__m256d_private r;
  simde_memcpy(&r, &v, sizeof(r));
  return r;
}

#define SIMDE_CMP_EQ_OQ     0
#define SIMDE_CMP_LT_OS     1
#define SIMDE_CMP_LE_OS     2
#define SIMDE_CMP_UNORD_Q   3
#define SIMDE_CMP_NEQ_UQ    4
#define SIMDE_CMP_NLT_US    5
#define SIMDE_CMP_NLE_US    6
#define SIMDE_CMP_ORD_Q     7
#define SIMDE_CMP_EQ_UQ     8
#define SIMDE_CMP_NGE_US    9
#define SIMDE_CMP_NGT_US   10
#define SIMDE_CMP_FALSE_OQ 11
#define SIMDE_CMP_NEQ_OQ   12
#define SIMDE_CMP_GE_OS    13
#define SIMDE_CMP_GT_OS    14
#define SIMDE_CMP_TRUE_UQ  15
#define SIMDE_CMP_EQ_OS    16
#define SIMDE_CMP_LT_OQ    17
#define SIMDE_CMP_LE_OQ    18
#define SIMDE_CMP_UNORD_S  19
#define SIMDE_CMP_NEQ_US   20
#define SIMDE_CMP_NLT_UQ   21
#define SIMDE_CMP_NLE_UQ   22
#define SIMDE_CMP_ORD_S    23
#define SIMDE_CMP_EQ_US    24
#define SIMDE_CMP_NGE_UQ   25
#define SIMDE_CMP_NGT_UQ   26
#define SIMDE_CMP_FALSE_OS 27
#define SIMDE_CMP_NEQ_OS   28
#define SIMDE_CMP_GE_OQ    29
#define SIMDE_CMP_GT_OQ    30
#define SIMDE_CMP_TRUE_US  31

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_castps_pd (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castps_pd(a);
#else
  return *HEDLEY_REINTERPRET_CAST(simde__m256d*, &a);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castps_pd(a) simde_mm256_castps_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_castps_si256 (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castps_si256(a);
#else
  return *HEDLEY_REINTERPRET_CAST(simde__m256i*, &a);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castps_si256(a) simde_mm256_castps_si256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_castsi256_pd (simde__m256i a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castsi256_pd(a);
#else
  return *HEDLEY_REINTERPRET_CAST(simde__m256d*, &a);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castsi256_pd(a) simde_mm256_castsi256_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_castsi256_ps (simde__m256i a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castsi256_ps(a);
#else
  return *HEDLEY_REINTERPRET_CAST(simde__m256*, &a);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castsi256_ps(a) simde_mm256_castsi256_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_castpd_ps (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castpd_ps(a);
#else
  return *HEDLEY_REINTERPRET_CAST(simde__m256*, &a);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castpd_ps(a) simde_mm256_castpd_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_castpd_si256 (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castpd_si256(a);
#else
  return *HEDLEY_REINTERPRET_CAST(simde__m256i*, &a);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castpd_si256(a) simde_mm256_castpd_si256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_setzero_si256 (void) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setzero_si256();
#else
  simde__m256i_private r_;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i32f) / sizeof(r_.i32f[0])) ; i++) {
    r_.i32f[i] = 0;
  }

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setzero_si256() simde_mm256_setzero_si256()
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_setzero_ps (void) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setzero_ps();
#else
  return simde_mm256_castsi256_ps(simde_mm256_setzero_si256());
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setzero_ps() simde_mm256_setzero_ps()
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_setzero_pd (void) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setzero_pd();
#else
  return simde_mm256_castsi256_pd(simde_mm256_setzero_si256());
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setzero_pd() simde_mm256_setzero_pd()
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_setone_si256 (void) {
  simde__m256i_private r_;

#if defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = ((__typeof__(r_.i32f)) { 0, });
  r_.i32f = ~(r_.i32f);
#elif defined(SIMDE_AVX2_NATIVE)
  __m256i t = _mm256_setzero_si256();
  r_.n = _mm256_cmpeq_epi32(t, t);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i32f) / sizeof(r_.i32f[0])) ; i++) {
    r_.i32f[i] = ~HEDLEY_STATIC_CAST(int_fast32_t, 0);
  }
#endif

  return simde__m256i_from_private(r_);
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_m256_setone_ps (void) {
  return simde_mm256_castsi256_ps(simde_mm256_setone_si256());
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_m256_setone_pd (void) {
  return simde_mm256_castsi256_pd(simde_mm256_setone_si256());
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set_epi8 (int8_t e31, int8_t e30, int8_t e29, int8_t e28,
                      int8_t e27, int8_t e26, int8_t e25, int8_t e24,
                      int8_t e23, int8_t e22, int8_t e21, int8_t e20,
                      int8_t e19, int8_t e18, int8_t e17, int8_t e16,
                      int8_t e15, int8_t e14, int8_t e13, int8_t e12,
                      int8_t e11, int8_t e10, int8_t  e9, int8_t  e8,
                      int8_t  e7, int8_t  e6, int8_t  e5, int8_t  e4,
                      int8_t  e3, int8_t  e2, int8_t  e1, int8_t  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set_epi8(e31, e30, e29, e28, e27, e26, e25, e24,
                         e23, e22, e21, e20, e19, e18, e17, e16,
                         e15, e14, e13, e12, e11, e10,  e9,  e8,
                          e7,  e6,  e5,  e4,  e3,  e2,  e1,  e0);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set_epi8(
    e15, e14, e13, e12, e11, e10,  e9,  e8,
     e7,  e6,  e5,  e4,  e3,  e2,  e1,  e0);
  r_.m128i[1] = simde_mm_set_epi8(
    e31, e30, e29, e28, e27, e26, e25, e24,
    e23, e22, e21, e20, e19, e18, e17, e16);
#else
  r_.i8[ 0] =  e0;
  r_.i8[ 1] =  e1;
  r_.i8[ 2] =  e2;
  r_.i8[ 3] =  e3;
  r_.i8[ 4] =  e4;
  r_.i8[ 5] =  e5;
  r_.i8[ 6] =  e6;
  r_.i8[ 7] =  e7;
  r_.i8[ 8] =  e8;
  r_.i8[ 9] =  e9;
  r_.i8[10] = e10;
  r_.i8[11] = e11;
  r_.i8[12] = e12;
  r_.i8[13] = e13;
  r_.i8[14] = e14;
  r_.i8[15] = e15;
  r_.i8[16] = e16;
  r_.i8[17] = e17;
  r_.i8[18] = e18;
  r_.i8[19] = e19;
  r_.i8[20] = e20;
  r_.i8[21] = e21;
  r_.i8[22] = e22;
  r_.i8[23] = e23;
  r_.i8[24] = e24;
  r_.i8[25] = e25;
  r_.i8[26] = e26;
  r_.i8[27] = e27;
  r_.i8[28] = e28;
  r_.i8[29] = e29;
  r_.i8[30] = e30;
  r_.i8[31] = e31;
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_epi8(e31, e30, e29, e28, e27, e26, e25, e24, e23, e22, e21, e20, e19, e18, e17, e16, e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0) \
  simde_mm256_set_epi8(e31, e30, e29, e28, e27, e26, e25, e24, e23, e22, e21, e20, e19, e18, e17, e16, e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set_epi16 (int16_t e15, int16_t e14, int16_t e13, int16_t e12,
                       int16_t e11, int16_t e10, int16_t  e9, int16_t  e8,
                       int16_t  e7, int16_t  e6, int16_t  e5, int16_t  e4,
                       int16_t  e3, int16_t  e2, int16_t  e1, int16_t  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set_epi16(e15, e14, e13, e12, e11, e10,  e9,  e8,
                           e7,  e6,  e5,  e4,  e3,  e2,  e1,  e0);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set_epi16( e7,  e6,  e5,  e4,  e3,  e2,  e1,  e0);
  r_.m128i[1] = simde_mm_set_epi16(e15, e14, e13, e12, e11, e10,  e9,  e8);
#else
  r_.i16[ 0] =  e0;
  r_.i16[ 1] =  e1;
  r_.i16[ 2] =  e2;
  r_.i16[ 3] =  e3;
  r_.i16[ 4] =  e4;
  r_.i16[ 5] =  e5;
  r_.i16[ 6] =  e6;
  r_.i16[ 7] =  e7;
  r_.i16[ 8] =  e8;
  r_.i16[ 9] =  e9;
  r_.i16[10] = e10;
  r_.i16[11] = e11;
  r_.i16[12] = e12;
  r_.i16[13] = e13;
  r_.i16[14] = e14;
  r_.i16[15] = e15;
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_epi16(e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0) \
  simde_mm256_set_epi16(e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set_epi32 (int32_t e7, int32_t e6, int32_t e5, int32_t e4,
                       int32_t e3, int32_t e2, int32_t e1, int32_t e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set_epi32(e7, e6, e5, e4, e3, e2, e1, e0);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set_epi32(e3, e2, e1, e0);
  r_.m128i[1] = simde_mm_set_epi32(e7, e6, e5, e4);
#else
  r_.i32[ 0] =  e0;
  r_.i32[ 1] =  e1;
  r_.i32[ 2] =  e2;
  r_.i32[ 3] =  e3;
  r_.i32[ 4] =  e4;
  r_.i32[ 5] =  e5;
  r_.i32[ 6] =  e6;
  r_.i32[ 7] =  e7;
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_epi32(e7, e6, e5, e4, e3, e2, e1, e0) \
  simde_mm256_set_epi32(e7, e6, e5, e4, e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set_epi64x (int64_t  e3, int64_t  e2, int64_t  e1, int64_t  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set_epi64x(e3, e2, e1, e0);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set_epi64x(e1, e0);
  r_.m128i[1] = simde_mm_set_epi64x(e3, e2);
#else
  r_.i64[0] = e0;
  r_.i64[1] = e1;
  r_.i64[2] = e2;
  r_.i64[3] = e3;
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_epi64x(e3, e2, e1, e0) simde_mm256_set_epi64x(e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_x_mm256_set_epu8 (uint8_t e31, uint8_t e30, uint8_t e29, uint8_t e28,
                        uint8_t e27, uint8_t e26, uint8_t e25, uint8_t e24,
                        uint8_t e23, uint8_t e22, uint8_t e21, uint8_t e20,
                        uint8_t e19, uint8_t e18, uint8_t e17, uint8_t e16,
                        uint8_t e15, uint8_t e14, uint8_t e13, uint8_t e12,
                        uint8_t e11, uint8_t e10, uint8_t  e9, uint8_t  e8,
                        uint8_t  e7, uint8_t  e6, uint8_t  e5, uint8_t  e4,
                        uint8_t  e3, uint8_t  e2, uint8_t  e1, uint8_t  e0) {
  simde__m256i_private r_;

  r_.u8[ 0] =  e0;
  r_.u8[ 1] =  e1;
  r_.u8[ 2] =  e2;
  r_.u8[ 3] =  e3;
  r_.u8[ 4] =  e4;
  r_.u8[ 5] =  e5;
  r_.u8[ 6] =  e6;
  r_.u8[ 7] =  e7;
  r_.u8[ 8] =  e8;
  r_.u8[ 9] =  e9;
  r_.u8[10] = e10;
  r_.u8[11] = e11;
  r_.u8[12] = e12;
  r_.u8[13] = e13;
  r_.u8[14] = e14;
  r_.u8[15] = e15;
  r_.u8[16] = e16;
  r_.u8[17] = e17;
  r_.u8[18] = e18;
  r_.u8[19] = e19;
  r_.u8[20] = e20;
  r_.u8[20] = e20;
  r_.u8[21] = e21;
  r_.u8[22] = e22;
  r_.u8[23] = e23;
  r_.u8[24] = e24;
  r_.u8[25] = e25;
  r_.u8[26] = e26;
  r_.u8[27] = e27;
  r_.u8[28] = e28;
  r_.u8[29] = e29;
  r_.u8[30] = e30;
  r_.u8[31] = e31;

  return simde__m256i_from_private(r_);
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_x_mm256_set_epu16 (uint16_t e15, uint16_t e14, uint16_t e13, uint16_t e12,
                       uint16_t e11, uint16_t e10, uint16_t  e9, uint16_t  e8,
                       uint16_t  e7, uint16_t  e6, uint16_t  e5, uint16_t  e4,
                       uint16_t  e3, uint16_t  e2, uint16_t  e1, uint16_t  e0) {
  simde__m256i_private r_;

  r_.u16[ 0] =  e0;
  r_.u16[ 1] =  e1;
  r_.u16[ 2] =  e2;
  r_.u16[ 3] =  e3;
  r_.u16[ 4] =  e4;
  r_.u16[ 5] =  e5;
  r_.u16[ 6] =  e6;
  r_.u16[ 7] =  e7;
  r_.u16[ 8] =  e8;
  r_.u16[ 9] =  e9;
  r_.u16[10] = e10;
  r_.u16[11] = e11;
  r_.u16[12] = e12;
  r_.u16[13] = e13;
  r_.u16[14] = e14;
  r_.u16[15] = e15;

  return simde__m256i_from_private(r_);
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_x_mm256_set_epu32 (uint32_t e7, uint32_t e6, uint32_t e5, uint32_t e4,
                       uint32_t e3, uint32_t e2, uint32_t e1, uint32_t e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set_epi32(e7, e6, e5, e4, e3, e2, e1, e0);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set_epi32(e3, e2, e1, e0);
  r_.m128i[1] = simde_mm_set_epi32(e7, e6, e5, e4);
#else
  r_.u32[ 0] =  e0;
  r_.u32[ 1] =  e1;
  r_.u32[ 2] =  e2;
  r_.u32[ 3] =  e3;
  r_.u32[ 4] =  e4;
  r_.u32[ 5] =  e5;
  r_.u32[ 6] =  e6;
  r_.u32[ 7] =  e7;
#endif

  return simde__m256i_from_private(r_);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_x_mm256_set_epu64x (uint64_t  e3, uint64_t  e2, uint64_t  e1, uint64_t  e0) {
  simde__m256i_private r_;

  r_.u64[0] = e0;
  r_.u64[1] = e1;
  r_.u64[2] = e2;
  r_.u64[3] = e3;

  return simde__m256i_from_private(r_);
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_set_ps (simde_float32 e7, simde_float32 e6, simde_float32 e5, simde_float32 e4,
                    simde_float32 e3, simde_float32 e2, simde_float32 e1, simde_float32 e0) {
  simde__m256_private r_;

#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set_ps(e7, e6, e5, e4, e3, e2, e1, e0);
#elif defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_set_ps(e3, e2, e1, e0);
  r_.m128[1] = simde_mm_set_ps(e7, e6, e5, e4);
#else
  r_.f32[0] = e0;
  r_.f32[1] = e1;
  r_.f32[2] = e2;
  r_.f32[3] = e3;
  r_.f32[4] = e4;
  r_.f32[5] = e5;
  r_.f32[6] = e6;
  r_.f32[7] = e7;
#endif

  return simde__m256_from_private(r_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_ps(e7, e6, e5, e4, e3, e2, e1, e0) \
  simde_mm256_set_ps(e7, e6, e5, e4, e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_set_pd (simde_float64 e3, simde_float64 e2, simde_float64 e1, simde_float64 e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set_pd(e3, e2, e1, e0);
#else
  simde__m256d_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_set_pd(e1, e0);
  r_.m128d[1] = simde_mm_set_pd(e3, e2);
#else
  r_.f64[0] = e0;
  r_.f64[1] = e1;
  r_.f64[2] = e2;
  r_.f64[3] = e3;
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_pd(e3, e2, e1, e0) \
  simde_mm256_set_pd(e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_set_m128 (simde__m128 e1, simde__m128 e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_insertf128_ps(_mm256_castps128_ps256(e0), e1, 1);
#else
  simde__m256_private r_;
  simde__m128_private
    e1_ = simde__m128_to_private(e1),
    e0_ = simde__m128_to_private(e0);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128_private[0] = e0_;
  r_.m128_private[1] = e1_;
#elif defined(SIMDE__HAVE_INT128)
  r_.i128[0] = e0_.i128[0];
  r_.i128[1] = e1_.i128[0];
#else
  r_.i64[0] = e0_.i64[0];
  r_.i64[1] = e0_.i64[1];
  r_.i64[2] = e1_.i64[0];
  r_.i64[3] = e1_.i64[1];
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_m128(e1, e0) simde_mm256_set_m128(e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_set_m128d (simde__m128d e1, simde__m128d e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_insertf128_pd(_mm256_castpd128_pd256(e0), e1, 1);
#else
  simde__m256d_private r_;
  simde__m128d_private
    e1_ = simde__m128d_to_private(e1),
    e0_ = simde__m128d_to_private(e0);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d_private[0] = e0_;
  r_.m128d_private[1] = e1_;
#else
  r_.i64[0] = e0_.i64[0];
  r_.i64[1] = e0_.i64[1];
  r_.i64[2] = e1_.i64[0];
  r_.i64[3] = e1_.i64[1];
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_m128d(e1, e0) simde_mm256_set_m128d(e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set_m128i (simde__m128i e1, simde__m128i e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_insertf128_si256(_mm256_castsi128_si256(e0), e1, 1);
#else
  simde__m256i_private r_;
  simde__m128i_private
    e1_ = simde__m128i_to_private(e1),
    e0_ = simde__m128i_to_private(e0);
  
#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i_private[0] = e0_;
  r_.m128i_private[1] = e1_;
#else
  r_.i64[0] = e0_.i64[0];
  r_.i64[1] = e0_.i64[1];
  r_.i64[2] = e1_.i64[0];
  r_.i64[3] = e1_.i64[1];
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set_m128i(e1, e0) simde_mm256_set_m128i(e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set1_epi8 (int8_t a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set1_epi8(a);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set1_epi8(a);
  r_.m128i[1] = simde_mm_set1_epi8(a);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i8) / sizeof(r_.i8[0])) ; i++) {
    r_.i8[i] = a;
  }
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set1_epi8(a) simde_mm256_set1_epi8(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set1_epi16 (int16_t a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set1_epi16(a);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set1_epi16(a);
  r_.m128i[1] = simde_mm_set1_epi16(a);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i16) / sizeof(r_.i16[0])) ; i++) {
    r_.i16[i] = a;
  }
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set1_epi16(a) simde_mm256_set1_epi16(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set1_epi32 (int32_t a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set1_epi32(a);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set1_epi32(a);
  r_.m128i[1] = simde_mm_set1_epi32(a);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i32) / sizeof(r_.i32[0])) ; i++) {
    r_.i32[i] = a;
  }
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set1_epi32(a) simde_mm256_set1_epi32(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_set1_epi64x (int64_t a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set1_epi64x(a);
#else
  simde__m256i_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128i[0] = simde_mm_set1_epi64x(a);
  r_.m128i[1] = simde_mm_set1_epi64x(a);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i64) / sizeof(r_.i64[0])) ; i++) {
    r_.i64[i] = a;
  }
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set1_epi64x(a) simde_mm256_set1_epi64x(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_set1_ps (simde_float32 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set1_ps(a);
#else
  simde__m256_private r_;

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_set1_ps(a);
  r_.m128[1] = simde_mm_set1_ps(a);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a;
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set1_ps(a) simde_mm256_set1_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_set1_pd (simde_float64 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_set1_pd(a);
#else
  simde__m256d_private r_;

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_set1_pd(a);
  r_.m128d[1] = simde_mm_set1_pd(a);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a;
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_set1_pd(a) simde_mm256_set1_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_add_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_add_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_add_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_add_ps(a_.m128[1], b_.m128[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.f32 = a_.f32 + b_.f32;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a_.f32[i] + b_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_add_ps(a, b) simde_mm256_add_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_add_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_add_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_add_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_add_pd(a_.m128d[1], b_.m128d[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.f64 = a_.f64 + b_.f64;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a_.f64[i] + b_.f64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_add_pd(a, b) simde_mm256_add_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_addsub_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_addsub_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE3_NATIVE)
  r_.m128[0] = simde_mm_addsub_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_addsub_ps(a_.m128[1], b_.m128[1]);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i += 2) {
    r_.f32[  i  ] = a_.f32[  i  ] - b_.f32[  i  ];
    r_.f32[i + 1] = a_.f32[i + 1] + b_.f32[i + 1];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_addsub_ps(a, b) simde_mm256_addsub_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_addsub_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_addsub_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_SSE3_NATIVE)
  r_.m128d[0] = simde_mm_addsub_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_addsub_pd(a_.m128d[1], b_.m128d[1]);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i += 2) {
    r_.f64[  i  ] = a_.f64[  i  ] - b_.f64[  i  ];
    r_.f64[i + 1] = a_.f64[i + 1] + b_.f64[i + 1];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_addsub_pd(a, b) simde_mm256_addsub_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_and_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_and_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_and_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_and_ps(a_.m128[1], b_.m128[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = a_.i32f & b_.i32f;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i32f) / sizeof(r_.i32f[0])) ; i++) {
    r_.i32f[i] = a_.i32f[i] & b_.i32f[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_and_ps(a, b) simde_mm256_and_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_and_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_and_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_and_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_and_pd(a_.m128d[1], b_.m128d[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = a_.i32f & b_.i32f;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i32f) / sizeof(r_.i32f[0])) ; i++) {
    r_.i32f[i] = a_.i32f[i] & b_.i32f[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_and_pd(a, b) simde_mm256_and_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_andnot_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_andnot_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_andnot_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_andnot_ps(a_.m128[1], b_.m128[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = ~a_.i32f & b_.i32f;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i32f) / sizeof(r_.i32f[0])) ; i++) {
    r_.i32f[i] = ~a_.i32f[i] & b_.i32f[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_andnot_ps(a, b) simde_mm256_andnot_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_andnot_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_andnot_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_andnot_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_andnot_pd(a_.m128d[1], b_.m128d[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = ~a_.i32f & b_.i32f;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.i32f) / sizeof(r_.i32f[0])) ; i++) {
    r_.i32f[i] = ~a_.i32f[i] & b_.i32f[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_andnot_pd(a, b) simde_mm256_andnot_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_blend_ps (simde__m256 a, simde__m256 b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xff) == imm8, "imm8 must be in range [0, 255]") {
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = ((imm8 >> i) & 1) ? b_.f32[i] : a_.f32[i];
  }

  return simde__m256_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_blend_ps(a, b, imm8) _mm256_blend_ps(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_blend_ps(a, b, imm8) simde_mm256_blend_ps(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_blend_pd (simde__m256d a, simde__m256d b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xf) == imm8, "imm8 must be in range [0, 15]") {
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = ((imm8 >> i) & 1) ? b_.f64[i] : a_.f64[i];
  }
  return simde__m256d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_blend_pd(a, b, imm8) _mm256_blend_pd(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_blend_pd(a, b, imm8) simde_mm256_blend_pd(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_blendv_ps (simde__m256 a, simde__m256 b, simde__m256 mask) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_blendv_ps(a, b, mask);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b),
    mask_ = simde__m256_to_private(mask);

#if defined(SIMDE_SSE4_1_NATIVE)
  r_.m128[0] = simde_mm_blendv_ps(a_.m128[0], b_.m128[0], mask_.m128[0]);
  r_.m128[1] = simde_mm_blendv_ps(a_.m128[1], b_.m128[1], mask_.m128[1]);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.u32) / sizeof(r_.u32[0])) ; i++) {
    r_.f32[i] = (mask_.u32[i] & (UINT32_C(1) << 31)) ? b_.f32[i] : a_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_blendv_ps(a, b, imm8) simde_mm256_blendv_ps(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_blendv_pd (simde__m256d a, simde__m256d b, simde__m256d mask) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_blendv_pd(a, b, mask);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b),
    mask_ = simde__m256d_to_private(mask);

#if defined(SIMDE_SSE4_1_NATIVE)
  r_.m128d[0] = simde_mm_blendv_pd(a_.m128d[0], b_.m128d[0], mask_.m128d[0]);
  r_.m128d[1] = simde_mm_blendv_pd(a_.m128d[1], b_.m128d[1], mask_.m128d[1]);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.u64) / sizeof(r_.u64[0])) ; i++) {
    r_.f64[i] = (mask_.u64[i] & (UINT64_C(1) << 63)) ? b_.f64[i] : a_.f64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_blendv_pd(a, b, imm8) simde_mm256_blendv_pd(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_broadcast_pd (simde__m128d const * mem_addr) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_broadcast_pd(mem_addr);
#else
  simde__m256d_private r_;

  r_.m128d_private[0] = *HEDLEY_REINTERPRET_CAST(simde__m128d_private const*, mem_addr);
  r_.m128d_private[1] = *HEDLEY_REINTERPRET_CAST(simde__m128d_private const*, mem_addr);

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_broadcast_pd(mem_addr) simde_mm256_broadcast_pd(mem_addr)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_broadcast_ps (simde__m128 const * mem_addr) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_broadcast_ps(mem_addr);
#else
  simde__m256_private r_;

  r_.m128_private[0] = *HEDLEY_REINTERPRET_CAST(simde__m128_private const*, mem_addr);
  r_.m128_private[1] = *HEDLEY_REINTERPRET_CAST(simde__m128_private const*, mem_addr);

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_broadcast_ps(mem_addr) simde_mm256_broadcast_ps(mem_addr)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_broadcast_sd (simde_float64 const * a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_broadcast_sd(a);
#else
  simde__m256d_private r_;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = *a;
  }

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_broadcast_sd(mem_addr) simde_mm256_broadcast_sd(mem_addr)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm_broadcast_ss (simde_float32 const * a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_broadcast_ss(a);
#else
  simde__m128_private r_;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = *a;
  }

  return simde__m128_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_broadcast_ss(mem_addr) simde_mm_broadcast_ss(mem_addr)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_broadcast_ss (simde_float32 const * a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_broadcast_ss(a);
#else
  simde__m256_private r_;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = *a;
  }

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_broadcast_ss(mem_addr) simde_mm256_broadcast_ss(mem_addr)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_castpd128_pd256 (simde__m128d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castpd128_pd256(a);
#else
  simde__m256d_private r_;
  simde__m128d_private a_ = simde__m128d_to_private(a);

  r_.m128d_private[0] = a_;

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castpd128_pd256(a) simde_mm256_castpd128_pd256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128d
simde_mm256_castpd256_pd128 (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castpd256_pd128(a);
#else
  return simde__m256d_to_private(a).m128d[0];
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castpd256_pd128(a) simde_mm256_castpd256_pd128(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_castps128_ps256 (simde__m128 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castps128_ps256(a);
#else
  simde__m256_private r_;
  simde__m128_private a_ = simde__m128_to_private(a);

  r_.m128_private[0] = a_;

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castps128_si256(a) simde_mm256_castps128_si256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm256_castps256_ps128 (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castps256_ps128(a);
#else
  return simde__m256_to_private(a).m128[0];
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castps256_ps128(a) simde_mm256_castps256_ps128(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_castsi128_si256 (simde__m128i a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castsi128_si256(a);
#else
  simde__m256i_private r_;
  simde__m128i_private a_ = simde__m128i_to_private(a);

  r_.m128i_private[0] = a_;

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castsi128_si256(a) simde_mm256_castsi128_si256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128i
simde_mm256_castsi256_si128 (simde__m256i a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_castsi256_si128(a);
#else
  return simde__m256i_to_private(a).m128i[0];
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_castsi256_si128(a) simde_mm256_castsi256_si128(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_ceil_pd (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_ceil_pd(a);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = ceil(a_.f64[i]);
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_ceil_pd(a) simde_mm256_ceil_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_ceil_ps (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_ceil_ps(a);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = ceilf(a_.f32[i]);
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_ceil_ps(a) simde_mm256_ceil_ps(a)
#endif

HEDLEY_DIAGNOSTIC_PUSH
SIMDE_DIAGNOSTIC_DISABLE_FLOAT_EQUAL

/* This implementation does not support signaling NaNs (yet?) */
SIMDE__FUNCTION_ATTRIBUTES
simde__m128d
simde_mm_cmp_pd (simde__m128d a, simde__m128d b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 31) == imm8, "imm8 must one of the SIMDE_CMP_* macros (values: [0, 31])") {
  simde__m128d_private
    r_,
    a_ = simde__m128d_to_private(a),
    b_ = simde__m128d_to_private(b);

#if defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  switch (imm8) {
    case SIMDE_CMP_EQ_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 == b_.f64);
      break;
    case SIMDE_CMP_LT_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 < b_.f64);
      break;
    case SIMDE_CMP_LE_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 <= b_.f64);
      break;
    case SIMDE_CMP_UNORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
      }
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 != b_.f64);
      break;
    case SIMDE_CMP_NLT_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 >= b_.f64);
      break;
    case SIMDE_CMP_NLE_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 > b_.f64);
      break;
    case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
        r_.u64[i] = (!isnan(a_.f64[i]) && !isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
      }
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 == b_.f64);
      break;
    case SIMDE_CMP_NGE_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 < b_.f64);
      break;
    case SIMDE_CMP_NGT_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 <= b_.f64);
      break;
    case SIMDE_CMP_FALSE_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) SIMDE_TAUTOLOGICAL_COMPARE_(a_.f64 != a_.f64);
      break;
    case SIMDE_CMP_NEQ_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 != b_.f64);
      break;
    case SIMDE_CMP_GE_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 >= b_.f64);
      break;
    case SIMDE_CMP_GT_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 > b_.f64);
      break;
    case SIMDE_CMP_TRUE_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) SIMDE_TAUTOLOGICAL_COMPARE_(a_.i64 == a_.i64);
      break;
    case SIMDE_CMP_EQ_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 == b_.f64);
      break;
    case SIMDE_CMP_LT_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 < b_.f64);
      break;
    case SIMDE_CMP_LE_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 <= b_.f64);
      break;
    case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
      }
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
     case SIMDE_CMP_NEQ_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 != b_.f64);
      break;
    case SIMDE_CMP_NLT_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 >= b_.f64);
      break;
    case SIMDE_CMP_NLE_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 > b_.f64);
      break;
    case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? UINT64_C(0) : ~UINT64_C(0);
      }
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 == b_.f64);
      break;
    case SIMDE_CMP_NGE_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 < b_.f64);
      break;
    case SIMDE_CMP_NGT_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 <= b_.f64);
      break;
    case SIMDE_CMP_FALSE_OS:
      r_.i32f = (a_.i32f ^ a_.i32f);
      break;
    case SIMDE_CMP_NEQ_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 != b_.f64);
      break;
    case SIMDE_CMP_GE_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 >= b_.f64);
      break;
    case SIMDE_CMP_GT_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 > b_.f64);
      break;
    case SIMDE_CMP_TRUE_US:
      r_.i32f = ~(r_.i32f ^ r_.i32f);
      break;
  }
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    switch (imm8) {
      case SIMDE_CMP_EQ_OQ:
        r_.u64[i] = (a_.f64[i] == b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_LT_OS:
        r_.u64[i] = (a_.f64[i] < b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_LE_OS:
        r_.u64[i] = (a_.f64[i] <= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_UNORD_Q:
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NEQ_UQ:
        r_.u64[i] = (a_.f64[i] != b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NLT_US:
        r_.u64[i] = (a_.f64[i] >= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NLE_US:
        r_.u64[i] = (a_.f64[i] > b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_ORD_Q:
        r_.u64[i] = (!isnan(a_.f64[i]) && !isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_EQ_UQ:
        r_.u64[i] = (a_.f64[i] == b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NGE_US:
        r_.u64[i] = (a_.f64[i] < b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NGT_US:
        r_.u64[i] = (a_.f64[i] <= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_FALSE_OQ:
        r_.u64[i] = UINT64_C(0);
        break;
      case SIMDE_CMP_NEQ_OQ:
        r_.u64[i] = (a_.f64[i] != b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_GE_OS:
        r_.u64[i] = (a_.f64[i] >= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_GT_OS:
        r_.u64[i] = (a_.f64[i] > b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_TRUE_UQ:
        r_.u64[i] = ~UINT64_C(0);
        break;
      case SIMDE_CMP_EQ_OS:
        r_.u64[i] = (a_.f64[i] == b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_LT_OQ:
        r_.u64[i] = (a_.f64[i] < b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_LE_OQ:
        r_.u64[i] = (a_.f64[i] <= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_NEQ_US:
        r_.u64[i] = (a_.f64[i] != b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NLT_UQ:
        r_.u64[i] = (a_.f64[i] >= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NLE_UQ:
        r_.u64[i] = (a_.f64[i] > b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_ORD_S:
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? UINT64_C(0) : ~UINT64_C(0);
        break;
      case SIMDE_CMP_EQ_US:
        r_.u64[i] = (a_.f64[i] == b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NGE_UQ:
        r_.u64[i] = (a_.f64[i] < b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NGT_UQ:
        r_.u64[i] = (a_.f64[i] <= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_FALSE_OS:
        r_.u64[i] = UINT64_C(0);
        break;
      case SIMDE_CMP_NEQ_OS:
        r_.u64[i] = (a_.f64[i] != b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_GE_OQ:
        r_.u64[i] = (a_.f64[i] >= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_GT_OQ:
        r_.u64[i] = (a_.f64[i] > b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_TRUE_US:
        r_.u64[i] = ~UINT64_C(0);
        break;
    }
  }
#endif

  return simde__m128d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE) && (!defined(__clang__) || !defined(__AVX512F__))
#  define simde_mm_cmp_pd(a, b, imm8) _mm_cmp_pd(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_cmp_pd(a, b, imm8) simde_mm_cmp_pd(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm_cmp_ps (simde__m128 a, simde__m128 b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 31) == imm8, "imm8 must one of the SIMDE_CMP_* macros (values: [0, 31])") {
  simde__m128_private
    r_,
    a_ = simde__m128_to_private(a),
    b_ = simde__m128_to_private(b);

#if defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  switch (imm8) {
    case SIMDE_CMP_EQ_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 == b_.f32);
      break;
    case SIMDE_CMP_LT_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 < b_.f32);
      break;
    case SIMDE_CMP_LE_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 <= b_.f32);
      break;
    case SIMDE_CMP_UNORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
      }
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 != b_.f32);
      break;
    case SIMDE_CMP_NLT_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 >= b_.f32);
      break;
    case SIMDE_CMP_NLE_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 > b_.f32);
      break;
    case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
        r_.u32[i] = (!isnan(a_.f32[i]) && !isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
      }
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 == b_.f32);
      break;
    case SIMDE_CMP_NGE_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 < b_.f32);
      break;
    case SIMDE_CMP_NGT_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 <= b_.f32);
      break;
    case SIMDE_CMP_FALSE_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) SIMDE_TAUTOLOGICAL_COMPARE_(a_.i32 != a_.i32);
      break;
    case SIMDE_CMP_NEQ_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 != b_.f32);
      break;
    case SIMDE_CMP_GE_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 >= b_.f32);
      break;
    case SIMDE_CMP_GT_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 > b_.f32);
      break;
    case SIMDE_CMP_TRUE_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) SIMDE_TAUTOLOGICAL_COMPARE_(a_.i32 == a_.i32);
      break;
    case SIMDE_CMP_EQ_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 == b_.f32);
      break;
    case SIMDE_CMP_LT_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 < b_.f32);
      break;
    case SIMDE_CMP_LE_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 <= b_.f32);
      break;
    case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
      }
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 != b_.f32);
      break;
    case SIMDE_CMP_NLT_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 >= b_.f32);
      break;
    case SIMDE_CMP_NLE_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 > b_.f32);
      break;
    case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? UINT32_C(0) : ~UINT32_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 == b_.f32);
      break;
    case SIMDE_CMP_NGE_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 < b_.f32);
      break;
    case SIMDE_CMP_NGT_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 <= b_.f32);
      break;
    case SIMDE_CMP_FALSE_OS:
      r_.i32 = (a_.i32 ^ a_.i32);
      break;
    case SIMDE_CMP_NEQ_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 != b_.f32);
      break;
    case SIMDE_CMP_GE_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 >= b_.f32);
      break;
    case SIMDE_CMP_GT_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 > b_.f32);
      break;
    case SIMDE_CMP_TRUE_US:
      r_.i32 = ~(a_.i32 ^ a_.i32);
      break;
  }
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    switch (imm8) {
      case SIMDE_CMP_EQ_OQ:
        r_.u32[i] = (a_.f32[i] == b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_LT_OS:
        r_.u32[i] = (a_.f32[i] < b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_LE_OS:
        r_.u32[i] = (a_.f32[i] <= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_UNORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_NEQ_UQ:
        r_.u32[i] = (a_.f32[i] != b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NLT_US:
        r_.u32[i] = (a_.f32[i] >= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NLE_US:
        r_.u32[i] = (a_.f32[i] > b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u32[i] = (!isnan(a_.f32[i]) && !isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_EQ_UQ:
        r_.u32[i] = (a_.f32[i] == b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NGE_US:
        r_.u32[i] = (a_.f32[i] < b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NGT_US:
        r_.u32[i] = (a_.f32[i] <= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_FALSE_OQ:
        r_.u32[i] = UINT32_C(0);
        break;
      case SIMDE_CMP_NEQ_OQ:
        r_.u32[i] = (a_.f32[i] != b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_GE_OS:
        r_.u32[i] = (a_.f32[i] >= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_GT_OS:
        r_.u32[i] = (a_.f32[i] > b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_TRUE_UQ:
        r_.u32[i] = ~UINT32_C(0);
        break;
      case SIMDE_CMP_EQ_OS:
        r_.u32[i] = (a_.f32[i] == b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_LT_OQ:
        r_.u32[i] = (a_.f32[i] < b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_LE_OQ:
        r_.u32[i] = (a_.f32[i] <= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_NEQ_US:
        r_.u32[i] = (a_.f32[i] != b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NLT_UQ:
        r_.u32[i] = (a_.f32[i] >= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NLE_UQ:
        r_.u32[i] = (a_.f32[i] > b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? UINT32_C(0) : ~UINT32_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_EQ_US:
        r_.u32[i] = (a_.f32[i] == b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NGE_UQ:
        r_.u32[i] = (a_.f32[i] < b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NGT_UQ:
        r_.u32[i] = (a_.f32[i] <= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_FALSE_OS:
        r_.u32[i] = UINT32_C(0);
        break;
      case SIMDE_CMP_NEQ_OS:
        r_.u32[i] = (a_.f32[i] != b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_GE_OQ:
        r_.u32[i] = (a_.f32[i] >= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_GT_OQ:
        r_.u32[i] = (a_.f32[i] > b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_TRUE_US:
        r_.u32[i] = ~UINT32_C(0);
        break;
    }
  }
#endif

  return simde__m128_from_private(r_);
}
/* Prior to 9.0 clang has problems with _mm{,256}_cmp_{ps,pd} for all four of the true/false
   comparisons, but only when AVX-512 is enabled.  __FILE_NAME__ was added in 9.0, so that's
   what we use to check for clang 9 since the version macros are unreliable. */
#if defined(SIMDE_AVX_NATIVE) && (!defined(__clang__) || !defined(__AVX512F__))
#  define simde_mm_cmp_ps(a, b, imm8) _mm_cmp_ps(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_cmp_ps(a, b, imm8) simde_mm_cmp_ps(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128d
simde_mm_cmp_sd (simde__m128d a, simde__m128d b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 31) == imm8, "imm8 must one of the SIMDE_CMP_* macros (values: [0, 31])") {
  simde__m128d_private
    r_,
    a_ = simde__m128d_to_private(a),
    b_ = simde__m128d_to_private(b);

  switch (imm8) {
    case SIMDE_CMP_EQ_OQ:
      r_.u64[0] = (a_.f64[0] == b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_LT_OS:
      r_.u64[0] = (a_.f64[0] < b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_LE_OS:
      r_.u64[0] = (a_.f64[0] <= b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_UNORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      r_.u64[0] = (isnan(a_.f64[0]) || isnan(b_.f64[0])) ? ~UINT64_C(0) : UINT64_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_UQ:
      r_.u64[0] = (a_.f64[0] != b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_NLT_US:
      r_.u64[0] = (a_.f64[0] >= b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_NLE_US:
      r_.u64[0] = (a_.f64[0] > b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      r_.u64[0] = (!isnan(a_.f64[0]) && !isnan(b_.f64[0])) ? ~UINT64_C(0) : UINT64_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_UQ:
      r_.u64[0] = (a_.f64[0] == b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_NGE_US:
      r_.u64[0] = (a_.f64[0] < b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_NGT_US:
      r_.u64[0] = (a_.f64[0] <= b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_FALSE_OQ:
      r_.u64[0] = UINT64_C(0);
      break;
    case SIMDE_CMP_NEQ_OQ:
      r_.u64[0] = (a_.f64[0] != b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_GE_OS:
      r_.u64[0] = (a_.f64[0] >= b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_GT_OS:
      r_.u64[0] = (a_.f64[0] > b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_TRUE_UQ:
      r_.u64[0] = ~UINT64_C(0);
      break;
    case SIMDE_CMP_EQ_OS:
      r_.u64[0] = (a_.f64[0] == b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_LT_OQ:
      r_.u64[0] = (a_.f64[0] < b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_LE_OQ:
      r_.u64[0] = (a_.f64[0] <= b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      r_.u64[0] = (isnan(a_.f64[0]) || isnan(b_.f64[0])) ? ~UINT64_C(0) : UINT64_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_US:
      r_.u64[0] = (a_.f64[0] != b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_NLT_UQ:
      r_.u64[0] = (a_.f64[0] >= b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_NLE_UQ:
      r_.u64[0] = (a_.f64[0] > b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      r_.u64[0] = (isnan(a_.f64[0]) || isnan(b_.f64[0])) ? UINT64_C(0) : ~UINT64_C(0);
#else
  HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_US:
      r_.u64[0] = (a_.f64[0] == b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_NGE_UQ:
      r_.u64[0] = (a_.f64[0] < b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_NGT_UQ:
      r_.u64[0] = (a_.f64[0] <= b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_FALSE_OS:
      r_.u64[0] = UINT64_C(0);
      break;
    case SIMDE_CMP_NEQ_OS:
      r_.u64[0] = (a_.f64[0] != b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_GE_OQ:
      r_.u64[0] = (a_.f64[0] >= b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_GT_OQ:
      r_.u64[0] = (a_.f64[0] > b_.f64[0]) ? ~UINT64_C(0) : UINT64_C(0);
      break;
    case SIMDE_CMP_TRUE_US:
      r_.u64[0] = ~UINT64_C(0);
      break;
  }
  r_.u64[1] = a_.u64[1];

  return simde__m128d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm_cmp_sd(a, b, imm8) _mm_cmp_sd(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_cmp_sd(a, b, imm8) simde_mm_cmp_sd(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm_cmp_ss (simde__m128 a, simde__m128 b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 31) == imm8, "imm8 must one of the SIMDE_CMP_* macros (values: [0, 31])") {
  simde__m128_private
    r_,
    a_ = simde__m128_to_private(a),
    b_ = simde__m128_to_private(b);

  switch (imm8) {
    case SIMDE_CMP_EQ_OQ:
      r_.u32[0] = (a_.f32[0] == b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_LT_OS:
      r_.u32[0] = (a_.f32[0] < b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_LE_OS:
      r_.u32[0] = (a_.f32[0] <= b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_UNORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      r_.u32[0] = (isnan(a_.f32[0]) || isnan(b_.f32[0])) ? ~UINT32_C(0) : UINT32_C(0);
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_UQ:
      r_.u32[0] = (a_.f32[0] != b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_NLT_US:
      r_.u32[0] = (a_.f32[0] >= b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_NLE_US:
      r_.u32[0] = (a_.f32[0] > b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      r_.u32[0] = (!isnan(a_.f32[0]) && !isnan(b_.f32[0])) ? ~UINT32_C(0) : UINT32_C(0);
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_UQ:
      r_.u32[0] = (a_.f32[0] == b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_NGE_US:
      r_.u32[0] = (a_.f32[0] < b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_NGT_US:
      r_.u32[0] = (a_.f32[0] <= b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_FALSE_OQ:
      r_.u32[0] = UINT32_C(0);
      break;
    case SIMDE_CMP_NEQ_OQ:
      r_.u32[0] = (a_.f32[0] != b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_GE_OS:
      r_.u32[0] = (a_.f32[0] >= b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_GT_OS:
      r_.u32[0] = (a_.f32[0] > b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_TRUE_UQ:
      r_.u32[0] = ~UINT32_C(0);
      break;
    case SIMDE_CMP_EQ_OS:
      r_.u32[0] = (a_.f32[0] == b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_LT_OQ:
      r_.u32[0] = (a_.f32[0] < b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_LE_OQ:
      r_.u32[0] = (a_.f32[0] <= b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      r_.u32[0] = (isnan(a_.f32[0]) || isnan(b_.f32[0])) ? ~UINT32_C(0) : UINT32_C(0);
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_US:
      r_.u32[0] = (a_.f32[0] != b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_NLT_UQ:
      r_.u32[0] = (a_.f32[0] >= b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_NLE_UQ:
      r_.u32[0] = (a_.f32[0] > b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      r_.u32[0] = (isnan(a_.f32[0]) || isnan(b_.f32[0])) ? UINT32_C(0) : ~UINT32_C(0);
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_US:
      r_.u32[0] = (a_.f32[0] == b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_NGE_UQ:
      r_.u32[0] = (a_.f32[0] < b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_NGT_UQ:
      r_.u32[0] = (a_.f32[0] <= b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_FALSE_OS:
      r_.u32[0] = UINT32_C(0);
      break;
    case SIMDE_CMP_NEQ_OS:
      r_.u32[0] = (a_.f32[0] != b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_GE_OQ:
      r_.u32[0] = (a_.f32[0] >= b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_GT_OQ:
      r_.u32[0] = (a_.f32[0] > b_.f32[0]) ? ~UINT32_C(0) : UINT32_C(0);
      break;
    case SIMDE_CMP_TRUE_US:
      r_.u32[0] = ~UINT32_C(0);
      break;
  }
  r_.u32[1] = a_.u32[1];
  r_.u32[2] = a_.u32[2];
  r_.u32[3] = a_.u32[3];

  return simde__m128_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm_cmp_ss(a, b, imm8) _mm_cmp_ss(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_cmp_ss(a, b, imm8) simde_mm_cmp_ss(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_cmp_pd (simde__m256d a, simde__m256d b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 31) == imm8, "imm8 must one of the SIMDE_CMP_* macros (values: [0, 31])") {
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);


#if defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  switch (imm8) {
    case SIMDE_CMP_EQ_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 == b_.f64);
      break;
    case SIMDE_CMP_LT_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 < b_.f64);
      break;
    case SIMDE_CMP_LE_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 <= b_.f64);
      break;
    case SIMDE_CMP_UNORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 != b_.f64);
      break;
    case SIMDE_CMP_NLT_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 >= b_.f64);
      break;
    case SIMDE_CMP_NLE_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 > b_.f64);
      break;
    case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
        r_.u64[i] = (!isnan(a_.f64[i]) && !isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 == b_.f64);
      break;
    case SIMDE_CMP_NGE_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 < b_.f64);
      break;
    case SIMDE_CMP_NGT_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 <= b_.f64);
      break;
    case SIMDE_CMP_FALSE_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) SIMDE_TAUTOLOGICAL_COMPARE_(a_.i64 != a_.i64);
      break;
    case SIMDE_CMP_NEQ_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 != b_.f64);
      break;
    case SIMDE_CMP_GE_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 >= b_.f64);
      break;
    case SIMDE_CMP_GT_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 > b_.f64);
      break;
    case SIMDE_CMP_TRUE_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) SIMDE_TAUTOLOGICAL_COMPARE_(a_.i64 == a_.i64);
      break;
    case SIMDE_CMP_EQ_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 == b_.f64);
      break;
    case SIMDE_CMP_LT_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 < b_.f64);
      break;
    case SIMDE_CMP_LE_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 <= b_.f64);
      break;
    case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
     case SIMDE_CMP_NEQ_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 != b_.f64);
      break;
    case SIMDE_CMP_NLT_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 >= b_.f64);
      break;
    case SIMDE_CMP_NLE_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 > b_.f64);
      break;
    case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? UINT64_C(0) : ~UINT64_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_US:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 == b_.f64);
      break;
    case SIMDE_CMP_NGE_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 < b_.f64);
      break;
    case SIMDE_CMP_NGT_UQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 <= b_.f64);
      break;
    case SIMDE_CMP_FALSE_OS:
      r_.i32f = (a_.i32f ^ a_.i32f);
      break;
    case SIMDE_CMP_NEQ_OS:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 != b_.f64);
      break;
    case SIMDE_CMP_GE_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 >= b_.f64);
      break;
    case SIMDE_CMP_GT_OQ:
      r_.i64 = (int64_t SIMDE_VECTOR(sizeof(r_))) (a_.f64 > b_.f64);
      break;
    case SIMDE_CMP_TRUE_US:
      r_.i32f = ~(r_.i32f ^ r_.i32f);
      break;
  }
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    switch (imm8) {
      case SIMDE_CMP_EQ_OQ:
        r_.u64[i] = (a_.f64[i] == b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_LT_OS:
        r_.u64[i] = (a_.f64[i] < b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_LE_OS:
        r_.u64[i] = (a_.f64[i] <= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_UNORD_Q:
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NEQ_UQ:
        r_.u64[i] = (a_.f64[i] != b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NLT_US:
        r_.u64[i] = (a_.f64[i] >= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NLE_US:
        r_.u64[i] = (a_.f64[i] > b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u64[i] = (!isnan(a_.f64[i]) && !isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
#else
        HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_EQ_UQ:
        r_.u64[i] = (a_.f64[i] == b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NGE_US:
        r_.u64[i] = (a_.f64[i] < b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NGT_US:
        r_.u64[i] = (a_.f64[i] <= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_FALSE_OQ:
        r_.u64[i] = UINT64_C(0);
        break;
      case SIMDE_CMP_NEQ_OQ:
        r_.u64[i] = (a_.f64[i] != b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_GE_OS:
        r_.u64[i] = (a_.f64[i] >= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_GT_OS:
        r_.u64[i] = (a_.f64[i] > b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_TRUE_UQ:
        r_.u64[i] = ~UINT64_C(0);
        break;
      case SIMDE_CMP_EQ_OS:
        r_.u64[i] = (a_.f64[i] == b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_LT_OQ:
        r_.u64[i] = (a_.f64[i] < b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_LE_OQ:
        r_.u64[i] = (a_.f64[i] <= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? ~UINT64_C(0) : UINT64_C(0);
#else
        HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_NEQ_US:
        r_.u64[i] = (a_.f64[i] != b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NLT_UQ:
        r_.u64[i] = (a_.f64[i] >= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NLE_UQ:
        r_.u64[i] = (a_.f64[i] > b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u64[i] = (isnan(a_.f64[i]) || isnan(b_.f64[i])) ? UINT64_C(0) : ~UINT64_C(0);
#else
        HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_EQ_US:
        r_.u64[i] = (a_.f64[i] == b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NGE_UQ:
        r_.u64[i] = (a_.f64[i] < b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_NGT_UQ:
        r_.u64[i] = (a_.f64[i] <= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_FALSE_OS:
        r_.u64[i] = UINT64_C(0);
        break;
      case SIMDE_CMP_NEQ_OS:
        r_.u64[i] = (a_.f64[i] != b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_GE_OQ:
        r_.u64[i] = (a_.f64[i] >= b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_GT_OQ:
        r_.u64[i] = (a_.f64[i] > b_.f64[i]) ? ~UINT64_C(0) : UINT64_C(0);
        break;
      case SIMDE_CMP_TRUE_US:
        r_.u64[i] = ~UINT64_C(0);
        break;
    }
  }
#endif

  return simde__m256d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE) && (!defined(__clang__) || !defined(__AVX512F__))
#  define simde_mm256_cmp_pd(a, b, imm8) _mm256_cmp_pd(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cmp_pd(a, b, imm8) simde_mm256_cmp_pd(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_cmp_ps (simde__m256 a, simde__m256 b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 31) == imm8, "imm8 must one of the SIMDE_CMP_* macros (values: [0, 31])") {
    simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);


#if defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  switch (imm8) {
    case SIMDE_CMP_EQ_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 == b_.f32);
      break;
    case SIMDE_CMP_LT_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 < b_.f32);
      break;
    case SIMDE_CMP_LE_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 <= b_.f32);
      break;
    case SIMDE_CMP_UNORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 != b_.f32);
      break;
    case SIMDE_CMP_NLT_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 >= b_.f32);
      break;
    case SIMDE_CMP_NLE_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 > b_.f32);
      break;
    case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
        r_.u32[i] = (!isnan(a_.f32[i]) && !isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 == b_.f32);
      break;
    case SIMDE_CMP_NGE_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 < b_.f32);
      break;
    case SIMDE_CMP_NGT_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 <= b_.f32);
      break;
    case SIMDE_CMP_FALSE_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) SIMDE_TAUTOLOGICAL_COMPARE_(a_.i32 != a_.i32);
      break;
    case SIMDE_CMP_NEQ_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 != b_.f32);
      break;
    case SIMDE_CMP_GE_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 >= b_.f32);
      break;
    case SIMDE_CMP_GT_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 > b_.f32);
      break;
    case SIMDE_CMP_TRUE_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) SIMDE_TAUTOLOGICAL_COMPARE_(a_.i32 == a_.i32);
      break;
    case SIMDE_CMP_EQ_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 == b_.f32);
      break;
    case SIMDE_CMP_LT_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 < b_.f32);
      break;
    case SIMDE_CMP_LE_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 <= b_.f32);
      break;
    case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_NEQ_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 != b_.f32);
      break;
    case SIMDE_CMP_NLT_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 >= b_.f32);
      break;
    case SIMDE_CMP_NLE_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 > b_.f32);
      break;
    case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
      for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? UINT32_C(0) : ~UINT32_C(0);
      }
#else
      HEDLEY_UNREACHABLE();
#endif
      break;
    case SIMDE_CMP_EQ_US:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 == b_.f32);
      break;
    case SIMDE_CMP_NGE_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 < b_.f32);
      break;
    case SIMDE_CMP_NGT_UQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 <= b_.f32);
      break;
    case SIMDE_CMP_FALSE_OS:
      r_.i32 = (a_.i32 ^ a_.i32);
      break;
    case SIMDE_CMP_NEQ_OS:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 != b_.f32);
      break;
    case SIMDE_CMP_GE_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 >= b_.f32);
      break;
    case SIMDE_CMP_GT_OQ:
      r_.i32 = (int32_t SIMDE_VECTOR(sizeof(r_))) (a_.f32 > b_.f32);
      break;
    case SIMDE_CMP_TRUE_US:
      r_.i32 = ~(a_.i32 ^ a_.i32);
      break;
  }
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    switch (imm8) {
      case SIMDE_CMP_EQ_OQ:
        r_.u32[i] = (a_.f32[i] == b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_LT_OS:
        r_.u32[i] = (a_.f32[i] < b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_LE_OS:
        r_.u32[i] = (a_.f32[i] <= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_UNORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
#else
        HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_NEQ_UQ:
        r_.u32[i] = (a_.f32[i] != b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NLT_US:
        r_.u32[i] = (a_.f32[i] >= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NLE_US:
        r_.u32[i] = (a_.f32[i] > b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_ORD_Q:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u32[i] = (!isnan(a_.f32[i]) && !isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
#else
        HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_EQ_UQ:
        r_.u32[i] = (a_.f32[i] == b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NGE_US:
        r_.u32[i] = (a_.f32[i] < b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NGT_US:
        r_.u32[i] = (a_.f32[i] <= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_FALSE_OQ:
        r_.u32[i] = UINT32_C(0);
        break;
      case SIMDE_CMP_NEQ_OQ:
        r_.u32[i] = (a_.f32[i] != b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_GE_OS:
        r_.u32[i] = (a_.f32[i] >= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_GT_OS:
        r_.u32[i] = (a_.f32[i] > b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_TRUE_UQ:
        r_.u32[i] = ~UINT32_C(0);
        break;
      case SIMDE_CMP_EQ_OS:
        r_.u32[i] = (a_.f32[i] == b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_LT_OQ:
        r_.u32[i] = (a_.f32[i] < b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_LE_OQ:
        r_.u32[i] = (a_.f32[i] <= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_UNORD_S:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? ~UINT32_C(0) : UINT32_C(0);
#else
        HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_NEQ_US:
        r_.u32[i] = (a_.f32[i] != b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NLT_UQ:
        r_.u32[i] = (a_.f32[i] >= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NLE_UQ:
        r_.u32[i] = (a_.f32[i] > b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_ORD_S:
#if defined(SIMDE_HAVE_MATH_H)
        r_.u32[i] = (isnan(a_.f32[i]) || isnan(b_.f32[i])) ? UINT32_C(0) : ~UINT32_C(0);
#else
        HEDLEY_UNREACHABLE();
#endif
        break;
      case SIMDE_CMP_EQ_US:
        r_.u32[i] = (a_.f32[i] == b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NGE_UQ:
        r_.u32[i] = (a_.f32[i] < b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_NGT_UQ:
        r_.u32[i] = (a_.f32[i] <= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_FALSE_OS:
        r_.u32[i] = UINT32_C(0);
        break;
      case SIMDE_CMP_NEQ_OS:
        r_.u32[i] = (a_.f32[i] != b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_GE_OQ:
        r_.u32[i] = (a_.f32[i] >= b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_GT_OQ:
        r_.u32[i] = (a_.f32[i] > b_.f32[i]) ? ~UINT32_C(0) : UINT32_C(0);
        break;
      case SIMDE_CMP_TRUE_US:
        r_.u32[i] = ~UINT32_C(0);
        break;
    }
  }
#endif

  return simde__m256_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE) && (!defined(__clang__) || !defined(__AVX512F__))
#  define simde_mm256_cmp_ps(a, b, imm8) _mm256_cmp_ps(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cmp_ps(a, b, imm8) simde_mm256_cmp_ps(a, b, imm8)
#endif

HEDLEY_DIAGNOSTIC_POP /* -Wfloat-equal */

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_cvtepi32_pd (simde__m128i a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_cvtepi32_pd(a);
#else
  simde__m256d_private r_;
  simde__m128i_private a_ = simde__m128i_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = HEDLEY_STATIC_CAST(simde_float64, a_.i32[i]);
  }

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cvtepi32_pd(a) simde_mm256_cvtepi32_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_cvtepi32_ps (simde__m256i a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_cvtepi32_ps(a);
#else
  simde__m256_private r_;
  simde__m256i_private a_ = simde__m256i_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = HEDLEY_STATIC_CAST(simde_float32, a_.i32[i]);
  }

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cvtepi32_ps(a) simde_mm256_cvtepi32_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128i
simde_mm256_cvtpd_epi32 (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_cvtpd_epi32(a);
#else
  simde__m128i_private r_;
  simde__m256d_private a_ = simde__m256d_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(a_.f64) / sizeof(a_.f64[0])) ; i++) {
    r_.i32[i] = SIMDE_CONVERT_FTOI(int32_t, round(a_.f64[i]));
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m128i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cvtpd_epi32(a) simde_mm256_cvtpd_epi32(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm256_cvtpd_ps (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_cvtpd_ps(a);
#else
  simde__m128_private r_;
  simde__m256d_private a_ = simde__m256d_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = HEDLEY_STATIC_CAST(simde_float32, a_.f64[i]);
  }

  return simde__m128_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cvtpd_ps(a) simde_mm256_cvtpd_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_cvtps_epi32 (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_cvtps_epi32(a);
#else
  simde__m256i_private r_;
  simde__m256_private a_ = simde__m256_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(a_.f32) / sizeof(a_.f32[0])) ; i++) {
    r_.i32[i] = SIMDE_CONVERT_FTOI(int32_t, roundf(a_.f32[i]));
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cvtps_epi32(a) simde_mm256_cvtps_epi32(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_cvtps_pd (simde__m128 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_cvtps_pd(a);
#else
  simde__m256d_private r_;
  simde__m128_private a_ = simde__m128_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(a_.f32) / sizeof(a_.f32[0])) ; i++) {
    r_.f64[i] = a_.f32[i];
  }

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cvtps_pd(a) simde_mm256_cvtps_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128i
simde_mm256_cvttpd_epi32 (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_cvttpd_epi32(a);
#else
  simde__m128i_private r_;
  simde__m256d_private a_ = simde__m256d_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(a_.f64) / sizeof(a_.f64[0])) ; i++) {
    r_.i32[i] = SIMDE_CONVERT_FTOI(int32_t, trunc(a_.f64[i]));
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m128i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cvttpd_epi32(a) simde_mm256_cvttpd_epi32(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_cvttps_epi32 (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_cvttps_epi32(a);
#else
  simde__m256i_private r_;
  simde__m256_private a_ = simde__m256_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(a_.f32) / sizeof(a_.f32[0])) ; i++) {
    r_.i32[i] = SIMDE_CONVERT_FTOI(int32_t, trunc(a_.f32[i]));
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_cvttps_epi32(a) simde_mm256_cvttps_epi32(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_div_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_div_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_div_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_div_ps(a_.m128[1], b_.m128[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.f32 = a_.f32 / b_.f32;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a_.f32[i] / b_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_div_ps(a, b) simde_mm256_div_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_div_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_div_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_div_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_div_pd(a_.m128d[1], b_.m128d[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.f64 = a_.f64 / b_.f64;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a_.f64[i] / b_.f64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_div_pd(a, b) simde_mm256_div_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128d
simde_mm256_extractf128_pd (simde__m256d a, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 1) == imm8, "imm8 must be 0 or 1") {
  return simde__m256d_to_private(a).m128d[imm8];
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_extractf128_pd(a, imm8) _mm256_extractf128_pd(a, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_extractf128_pd(a, imm8) simde_mm256_extractf128_pd(a, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm256_extractf128_ps (simde__m256 a, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 1) == imm8, "imm8 must be 0 or 1") {
  return simde__m256_to_private(a).m128[imm8];
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_extractf128_ps(a, imm8) _mm256_extractf128_ps(a, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_extractf128_ps(a, imm8) simde_mm256_extractf128_ps(a, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128i
simde_mm256_extractf128_si256 (simde__m256i a, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 1) == imm8, "imm8 must be 0 or 1") {
  return simde__m256i_to_private(a).m128i[imm8];
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_extractf128_si256(a, imm8) _mm256_extractf128_si256(a, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_extractf128_si256(a, imm8) simde_mm256_extractf128_si256(a, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_floor_pd (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_floor_pd(a);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = floor(a_.f64[i]);
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_floor_pd(a) simde_mm256_floor_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_floor_ps (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_floor_ps(a);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = floorf(a_.f32[i]);
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_floor_ps(a) simde_mm256_floor_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_hadd_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_hadd_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

  r_.f32[0] = a_.f32[0] + a_.f32[1];
  r_.f32[1] = a_.f32[2] + a_.f32[3];
  r_.f32[2] = b_.f32[0] + b_.f32[1];
  r_.f32[3] = b_.f32[2] + b_.f32[3];
  r_.f32[4] = a_.f32[4] + a_.f32[5];
  r_.f32[5] = a_.f32[6] + a_.f32[7];
  r_.f32[6] = b_.f32[4] + b_.f32[5];
  r_.f32[7] = b_.f32[6] + b_.f32[7];

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_hadd_ps(a, b) simde_mm256_hadd_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_hadd_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_hadd_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

  r_.f64[0] = a_.f64[0] + a_.f64[1];
  r_.f64[1] = b_.f64[0] + b_.f64[1];
  r_.f64[2] = a_.f64[2] + a_.f64[3];
  r_.f64[3] = b_.f64[2] + b_.f64[3];

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_hadd_pd(a, b) simde_mm256_hadd_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_hsub_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_hsub_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

  r_.f32[0] = a_.f32[0] - a_.f32[1];
  r_.f32[1] = a_.f32[2] - a_.f32[3];
  r_.f32[2] = b_.f32[0] - b_.f32[1];
  r_.f32[3] = b_.f32[2] - b_.f32[3];
  r_.f32[4] = a_.f32[4] - a_.f32[5];
  r_.f32[5] = a_.f32[6] - a_.f32[7];
  r_.f32[6] = b_.f32[4] - b_.f32[5];
  r_.f32[7] = b_.f32[6] - b_.f32[7];

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_hsub_ps(a, b) simde_mm256_hsub_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_hsub_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_hsub_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

  r_.f64[0] = a_.f64[0] - a_.f64[1];
  r_.f64[1] = b_.f64[0] - b_.f64[1];
  r_.f64[2] = a_.f64[2] - a_.f64[3];
  r_.f64[3] = b_.f64[2] - b_.f64[3];

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_hsub_pd(a, b) simde_mm256_hsub_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_insert_epi8 (simde__m256i a, int8_t i, const int index)
    HEDLEY_REQUIRE_MSG((index & 0xff) == index, "index must be in [0, 31]") {
  simde__m256i_private a_ = simde__m256i_to_private(a);

  a_.i8[index] = i;

  return simde__m256i_from_private(a_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_insert_epi8(a, i, imm8) simde_mm256_insert_epi8(a, i, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_insert_epi16 (simde__m256i a, int16_t i, const int index)
    HEDLEY_REQUIRE_MSG((index & 0xff) == index, "index must be in [0, 15]")  {
  simde__m256i_private a_ = simde__m256i_to_private(a);

  a_.i16[index] = i;

  return simde__m256i_from_private(a_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_insert_epi16(a, i, imm8) simde_mm256_insert_epi16(a, i, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_insert_epi32 (simde__m256i a, int32_t i, const int index)
    HEDLEY_REQUIRE_MSG((index & 0xff) == index, "index must be in [0, 7]")  {
  simde__m256i_private a_ = simde__m256i_to_private(a);

  a_.i32[index] = i;

  return simde__m256i_from_private(a_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_insert_epi32(a, i, imm8) simde_mm256_insert_epi32(a, i, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_insert_epi64 (simde__m256i a, int64_t i, const int index)
    HEDLEY_REQUIRE_MSG((index & 0xff) == index, "index must be in [0, 3]")  {
  simde__m256i_private a_ = simde__m256i_to_private(a);

  a_.i64[index] = i;

  return simde__m256i_from_private(a_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_insert_epi64(a, i, imm8) simde_mm256_insert_epi64(a, i, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d simde_mm256_insertf128_pd(simde__m256d a, simde__m128d b, int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xff) == imm8, "imm8 must be in [0, 3]") {
  simde__m256d_private a_ = simde__m256d_to_private(a);
  simde__m128d_private b_ = simde__m128d_to_private(b);

  a_.m128d_private[imm8] = b_;

  return simde__m256d_from_private(a_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_insertf128_pd(a, b, imm8) simde_mm256_insertf128_pd(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256 simde_mm256_insertf128_ps(simde__m256 a, simde__m128 b, int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xff) == imm8, "imm8 must be in [0, 7]") {
  simde__m256_private a_ = simde__m256_to_private(a);
  simde__m128_private b_ = simde__m128_to_private(b);

  a_.m128_private[imm8] = b_;

  return simde__m256_from_private(a_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_insertf128_ps(a, b, imm8) simde_mm256_insertf128_ps(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i simde_mm256_insertf128_si256(simde__m256i a, simde__m128i b, int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xff) == imm8, "imm8 must be in [0, 7]") {
  simde__m256i_private a_ = simde__m256i_to_private(a);
  simde__m128i_private b_ = simde__m128i_to_private(b);

  a_.m128i_private[imm8] = b_;

  return simde__m256i_from_private(a_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_insertf128_si256(a, b, imm8) simde_mm256_insertf128_si256(a, b, imm8)
#endif

#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_dp_ps(a, b, imm8) _mm256_dp_ps(a, b, imm8)
#else
#  define simde_mm256_dp_ps(a, b, imm8) \
    simde_mm256_set_m128( \
      simde_mm_dp_ps(simde_mm256_extractf128_ps(a, 1), simde_mm256_extractf128_ps(b, 1), imm8), \
      simde_mm_dp_ps(simde_mm256_extractf128_ps(a, 0), simde_mm256_extractf128_ps(b, 0), imm8))
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_dp_ps(a, b, imm8) simde_mm256_dp_ps(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int32_t
simde_mm256_extract_epi32 (simde__m256i a, const int index) {
  return simde__m256i_to_private(a).i32[index];
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_extract_epi32(a, index) simde_mm256_extract_epi32(a, index)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int64_t
simde_mm256_extract_epi64 (simde__m256i a, const int index) {
  return simde__m256i_to_private(a).i64[index];
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_extract_epi64(a, index) simde_mm256_extract_epi64(a, index)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_lddqu_si256 (simde__m256i const * a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_loadu_si256(a);
#else
  simde__m256i_private r_;

  simde_memcpy(&r_, a, sizeof(r_));

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_lddqu_si256(a) simde_mm256_lddqu_si256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_load_pd (const double a[HEDLEY_ARRAY_PARAM(4)]) {
  simde_assert_aligned(32, a);

#if defined(SIMDE_AVX_NATIVE)
  return _mm256_load_pd(a);
#else
  simde__m256d r;
  r = *SIMDE_CAST_ALIGN(32, simde__m256d const*, a);
  return r;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_load_pd(a) simde_mm256_load_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_load_ps (const float a[HEDLEY_ARRAY_PARAM(8)]) {
  simde_assert_aligned(32, a);

#if defined(SIMDE_AVX_NATIVE)
  return _mm256_load_ps(a);
#else
  simde__m256 r;
  r = *SIMDE_CAST_ALIGN(32, simde__m256 const*, a);
  return r;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_load_ps(a) simde_mm256_load_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_load_si256 (simde__m256i const * mem_addr) {
  simde_assert_aligned(32, mem_addr);

#if defined(SIMDE_AVX_NATIVE)
  return _mm256_load_si256((__m256i const*) mem_addr);
#else
  simde__m256i r;
  r = *mem_addr;
  return r;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_load_si256(a) simde_mm256_load_si256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_loadu_pd (const double a[HEDLEY_ARRAY_PARAM(4)]) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_loadu_pd(a);
#else
  simde__m256d r;
  simde_memcpy(&r, a, sizeof(r));
  return r;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_loadu_pd(a) simde_mm256_loadu_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_loadu_ps (const float a[HEDLEY_ARRAY_PARAM(8)]) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_loadu_ps(a);
#else
  simde__m256 r;
  simde_memcpy(&r, a, sizeof(r));
  return r;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_loadu_ps(a) simde_mm256_loadu_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_loadu_si256 (simde__m256i const * a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_loadu_si256(a);
#else
  simde__m256i r;
  simde_memcpy(&r, a, sizeof(r));
  return r;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_loadu_si256(a) simde_mm256_loadu_si256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_loadu2_m128 (const float hiaddr[HEDLEY_ARRAY_PARAM(4)], const float loaddr[HEDLEY_ARRAY_PARAM(4)]) {
  return
    simde_mm256_insertf128_ps(simde_mm256_castps128_ps256(simde_mm_loadu_ps(loaddr)),
            simde_mm_loadu_ps(hiaddr), 1);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_loadu2_m128(hiaddr, loaddr) simde_mm256_loadu2_m128(hiaddr, loaddr)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_loadu2_m128d (const double hiaddr[HEDLEY_ARRAY_PARAM(2)], const double loaddr[HEDLEY_ARRAY_PARAM(2)]) {
  return
    simde_mm256_insertf128_pd(simde_mm256_castpd128_pd256(simde_mm_loadu_pd(loaddr)),
            simde_mm_loadu_pd(hiaddr), 1);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_loadu2_m128d(hiaddr, loaddr) simde_mm256_loadu2_m128d(hiaddr, loaddr)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_loadu2_m128i (const simde__m128i* hiaddr, const simde__m128i* loaddr) {
  return
    simde_mm256_insertf128_si256(simde_mm256_castsi128_si256(simde_mm_loadu_si128(loaddr)),
         simde_mm_loadu_si128(hiaddr), 1);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_loadu2_m128i(hiaddr, loaddr) simde_mm256_loadu2_m128i(hiaddr, loaddr)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128d
simde_mm_maskload_pd (const simde_float64 mem_addr[HEDLEY_ARRAY_PARAM(4)], simde__m128i mask) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_maskload_pd(mem_addr, mask);
#else
  simde__m128d_private r_;
  simde__m128i_private mask_ = simde__m128i_to_private(mask);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.i64[i] = HEDLEY_REINTERPRET_CAST(int64_t const*, mem_addr)[i] & (mask_.i64[i] >> 63);
  }

  return simde__m128d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_maskload_pd(mem_addr, mask) simde_mm_maskload_pd(mem_addr, mask)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_maskload_pd (const simde_float64 mem_addr[HEDLEY_ARRAY_PARAM(4)], simde__m256i mask) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_maskload_pd(mem_addr, mask);
#else
  simde__m256d_private r_;
  simde__m256i_private mask_ = simde__m256i_to_private(mask);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.i64[i] = HEDLEY_REINTERPRET_CAST(int64_t const*, mem_addr)[i] & (mask_.i64[i] >> 63);
  }

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_maskload_pd(mem_addr, mask) simde_mm256_maskload_pd(mem_addr, mask)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm_maskload_ps (const simde_float32 mem_addr[HEDLEY_ARRAY_PARAM(4)], simde__m128i mask) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_maskload_ps(mem_addr, mask);
#else
  simde__m128_private r_;
  simde__m128i_private mask_ = simde__m128i_to_private(mask);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.i32[i] = HEDLEY_REINTERPRET_CAST(int32_t const*, mem_addr)[i] & (mask_.i32[i] >> 31);
  }

  return simde__m128_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_maskload_ps(mem_addr, mask) simde_mm_maskload_ps(mem_addr, mask)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_maskload_ps (const simde_float32 mem_addr[HEDLEY_ARRAY_PARAM(4)], simde__m256i mask) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_maskload_ps(mem_addr, mask);
#else
  simde__m256_private r_;
  simde__m256i_private mask_ = simde__m256i_to_private(mask);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.i32[i] = HEDLEY_REINTERPRET_CAST(int32_t const*, mem_addr)[i] & (mask_.i32[i] >> 31);
  }

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_maskload_ps(mem_addr, mask) simde_mm256_maskload_ps(mem_addr, mask)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm_maskstore_pd (simde_float64 mem_addr[HEDLEY_ARRAY_PARAM(2)], simde__m128i mask, simde__m128d a) {
#if defined(SIMDE_AVX_NATIVE)
  _mm_maskstore_pd(mem_addr, mask, a);
#else
  simde__m128i_private mask_ = simde__m128i_to_private(mask);
  simde__m128d_private a_ = simde__m128d_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < 2 ; i++) {
    if (mask_.u64[i] >> 63)
      mem_addr[i] = a_.f64[i];
  }
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_maskstore_pd(mem_addr, mask, a) simde_mm_maskstore_pd(mem_addr, mask, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_maskstore_pd (simde_float64 mem_addr[HEDLEY_ARRAY_PARAM(4)], simde__m256i mask, simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  _mm256_maskstore_pd(mem_addr, mask, a);
#else
  simde__m256i_private mask_ = simde__m256i_to_private(mask);
  simde__m256d_private a_ = simde__m256d_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < 4 ; i++) {
    if (mask_.u64[i] & (UINT64_C(1) << 63))
      mem_addr[i] = a_.f64[i];
  }
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_maskstore_pd(mem_addr, mask, a) simde_mm256_maskstore_pd(mem_addr, mask, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm_maskstore_ps (simde_float32 mem_addr[HEDLEY_ARRAY_PARAM(4)], simde__m128i mask, simde__m128 a) {
#if defined(SIMDE_AVX_NATIVE)
  _mm_maskstore_ps(mem_addr, mask, a);
#else
  simde__m128i_private mask_ = simde__m128i_to_private(mask);
  simde__m128_private a_ = simde__m128_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < 4 ; i++) {
    if (mask_.u32[i] & (UINT32_C(1) << 31))
      mem_addr[i] = a_.f32[i];
  }
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_maskstore_ps(mem_addr, mask, a) simde_mm_maskstore_ps(mem_addr, mask, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_maskstore_ps (simde_float32 mem_addr[HEDLEY_ARRAY_PARAM(8)], simde__m256i mask, simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  _mm256_maskstore_ps(mem_addr, mask, a);
#else
  simde__m256i_private mask_ = simde__m256i_to_private(mask);
  simde__m256_private a_ = simde__m256_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < 8 ; i++) {
    if (mask_.u32[i] & (UINT32_C(1) << 31))
      mem_addr[i] = a_.f32[i];
  }
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_maskstore_ps(mem_addr, mask, a) simde_mm256_maskstore_ps(mem_addr, mask, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_min_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_min_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_min_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_min_ps(a_.m128[1], b_.m128[1]);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = (a_.f32[i] < b_.f32[i]) ? a_.f32[i] : b_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_min_ps(a, b) simde_mm256_min_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_min_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_min_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_min_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_min_pd(a_.m128d[1], b_.m128d[1]);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = (a_.f64[i] < b_.f64[i]) ? a_.f64[i] : b_.f64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_min_pd(a, b) simde_mm256_min_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_max_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_max_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_max_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_max_ps(a_.m128[1], b_.m128[1]);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = (a_.f32[i] > b_.f32[i]) ? a_.f32[i] : b_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_max_ps(a, b) simde_mm256_max_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_max_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_max_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_max_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_max_pd(a_.m128d[1], b_.m128d[1]);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = (a_.f64[i] > b_.f64[i]) ? a_.f64[i] : b_.f64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_max_pd(a, b) simde_mm256_max_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_movedup_pd (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_movedup_pd(a);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a);

#if defined(SIMDE__SHUFFLE_VECTOR)
  r_.f64 = SIMDE__SHUFFLE_VECTOR(64, 32, a_.f64, a_.f64, 0, 0, 2, 2);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i += 2) {
    r_.f64[i] = r_.f64[i + 1] = a_.f64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_movedup_pd(a) simde_mm256_movedup_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_movehdup_ps (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_movehdup_ps(a);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

#if defined(SIMDE__SHUFFLE_VECTOR)
  r_.f32 = SIMDE__SHUFFLE_VECTOR(32, 32, a_.f32, a_.f32, 1, 1, 3, 3, 5, 5, 7, 7);
#else
  SIMDE__VECTORIZE
  for (size_t i = 1 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i += 2) {
    r_.f32[i - 1] = r_.f32[i] = a_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_movehdup_ps(a) simde_mm256_movehdup_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_moveldup_ps (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE) && 0
  return _mm256_moveldup_ps(a);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

#if defined(SIMDE__SHUFFLE_VECTOR)
  r_.f32 = SIMDE__SHUFFLE_VECTOR(32, 32, a_.f32, a_.f32, 0, 0, 2, 2, 4, 4, 6, 6);
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i += 2) {
    r_.f32[i] = r_.f32[i + 1] = a_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_moveldup_ps(a) simde_mm256_moveldup_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_movemask_ps (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_movemask_ps(a);
#else
  simde__m256_private a_ = simde__m256_to_private(a);
  int r = 0;

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.f32) / sizeof(a_.f32[0])) ; i++) {
    r |= (a_.u32[i] >> 31) << i;
  }

  return r;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_movemask_ps(a) simde_mm256_movemask_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_movemask_pd (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_movemask_pd(a);
#else
  simde__m256d_private a_ = simde__m256d_to_private(a);
  int r = 0;

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.f64) / sizeof(a_.f64[0])) ; i++) {
    r |= (a_.u64[i] >> 63) << i;
  }

  return r;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_movemask_pd(a) simde_mm256_movemask_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_mul_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_mul_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_mul_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_mul_ps(a_.m128[1], b_.m128[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.f32 = a_.f32 * b_.f32;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a_.f32[i] * b_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_mul_ps(a, b) simde_mm256_mul_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_mul_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_mul_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_mul_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_mul_pd(a_.m128d[1], b_.m128d[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.f64 = a_.f64 * b_.f64;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a_.f64[i] * b_.f64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_mul_pd(a, b) simde_mm256_mul_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_or_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_or_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_or_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_or_ps(a_.m128[1], b_.m128[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = a_.i32f | b_.i32f;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.u32) / sizeof(r_.u32[0])) ; i++) {
    r_.u32[i] = a_.u32[i] | b_.u32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_or_ps(a, b) simde_mm256_or_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_or_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_or_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_or_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_or_pd(a_.m128d[1], b_.m128d[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = a_.i32f | b_.i32f;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.u64) / sizeof(r_.u64[0])) ; i++) {
    r_.u64[i] = a_.u64[i] | b_.u64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_or_pd(a, b) simde_mm256_or_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_permute_ps (simde__m256 a, const int imm8) {
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a_.m128_private[i >> 2].f32[(imm8 >> ((i << 1) & 7)) & 3];
  }

  return simde__m256_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_permute_ps(a, imm8) _mm256_permute_ps(a, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_permute_ps(a, imm8) simde_mm256_permute_ps(a, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_permute_pd (simde__m256d a, const int imm8) {
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a_.f64[((imm8 >> i) & 1) + (i & 2)];
  }

  return simde__m256d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_permute_pd(a, imm8) _mm256_permute_pd(a, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_permute_pd(a, imm8) simde_mm256_permute_pd(a, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm_permute_ps (simde__m128 a, const int imm8) {
  simde__m128_private
    r_,
    a_ = simde__m128_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a_.f32[(imm8 >> ((i << 1) & 7)) & 3];
  }

  return simde__m128_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm_permute_ps(a, imm8) _mm_permute_ps(a, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_permute_ps(a, imm8) simde_mm_permute_ps(a, imm8)
#endif


SIMDE__FUNCTION_ATTRIBUTES
simde__m128d
simde_mm_permute_pd (simde__m128d a, const int imm8) {
  simde__m128d_private
    r_,
    a_ = simde__m128d_to_private(a);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a_.f64[((imm8 >> i) & 1) + (i & 2)];
  }

  return simde__m128d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm_permute_pd(a, imm8) _mm_permute_pd(a, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_permute_pd(a, imm8) simde_mm_permute_pd(a, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128
simde_mm_permutevar_ps (simde__m128 a, simde__m128i b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_permutevar_ps(a, b);
#else
  simde__m128_private
    r_,
    a_ = simde__m128_to_private(a);
  simde__m128i_private b_ = simde__m128i_to_private(b);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a_.f32[b_.i32[i] & 3];
  }

  return simde__m128_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_permutevar_ps(a, b) simde_mm_permutevar_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m128d
simde_mm_permutevar_pd (simde__m128d a, simde__m128i b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_permutevar_pd(a, b);
#else
  simde__m128d_private
    r_,
    a_ = simde__m128d_to_private(a);
  simde__m128i_private b_ = simde__m128i_to_private(b);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a_.f64[(b_.i64[i] & 2) >> 1];
  }

  return simde__m128d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_permutevar_pd(a, b) simde_mm_permutevar_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_permutevar_ps (simde__m256 a, simde__m256i b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_permutevar_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);
  simde__m256i_private b_ = simde__m256i_to_private(b);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a_.f32[(b_.i32[i] & 3) + (i & 4)];
  }

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_permutevar_ps(a, b) simde_mm256_permutevar_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_permutevar_pd (simde__m256d a, simde__m256i b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_permutevar_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a);
  simde__m256i_private b_ = simde__m256i_to_private(b);

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a_.f64[((b_.i64[i] & 2) >> 1) + (i & 2)];
  }

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_permutevar_pd(a, b) simde_mm256_permutevar_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_permute2f128_ps (simde__m256 a, simde__m256 b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xff) == imm8, "imm8 must be in range [0, 15]") {
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

  r_.m128_private[0] = (imm8 & 0x08) ? simde__m128_to_private(simde_mm_setzero_ps()) : ((imm8 & 0x02) ? b_.m128_private[(imm8     ) & 1] : a_.m128_private[(imm8     ) & 1]);
  r_.m128_private[1] = (imm8 & 0x80) ? simde__m128_to_private(simde_mm_setzero_ps()) : ((imm8 & 0x20) ? b_.m128_private[(imm8 >> 4) & 1] : a_.m128_private[(imm8 >> 4) & 1]);

  return simde__m256_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_permute2f128_ps(a, b, imm8) _mm256_permute2f128_ps(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_permute2f128_ps(a, b, imm8) simde_mm256_permute2f128_ps(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_permute2f128_pd (simde__m256d a, simde__m256d b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xf) == imm8, "imm8 must be in range [0, 15]") {
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

  r_.m128d_private[0] = (imm8 & 0x08) ? simde__m128d_to_private(simde_mm_setzero_pd()) : ((imm8 & 0x02) ? b_.m128d_private[(imm8     ) & 1] : a_.m128d_private[(imm8     ) & 1]);
  r_.m128d_private[1] = (imm8 & 0x80) ? simde__m128d_to_private(simde_mm_setzero_pd()) : ((imm8 & 0x20) ? b_.m128d_private[(imm8 >> 4) & 1] : a_.m128d_private[(imm8 >> 4) & 1]);

  return simde__m256d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_permute2f128_pd(a, b, imm8) _mm256_permute2f128_pd(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_permute2f128_pd(a, b, imm8) simde_mm256_permute2f128_pd(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_permute2f128_si256 (simde__m256i a, simde__m256i b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xf) == imm8, "imm8 must be in range [0, 15]") {
  simde__m256i_private
    r_,
    a_ = simde__m256i_to_private(a),
    b_ = simde__m256i_to_private(b);

  r_.m128i_private[0] = (imm8 & 0x08) ? simde__m128i_to_private(simde_mm_setzero_si128()) : ((imm8 & 0x02) ? b_.m128i_private[(imm8     ) & 1] : a_.m128i_private[(imm8     ) & 1]);
  r_.m128i_private[1] = (imm8 & 0x80) ? simde__m128i_to_private(simde_mm_setzero_si128()) : ((imm8 & 0x20) ? b_.m128i_private[(imm8 >> 4) & 1] : a_.m128i_private[(imm8 >> 4) & 1]);

  return simde__m256i_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_permute2f128_si128(a, b, imm8) _mm256_permute2f128_si128(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_permute2f128_si256(a, b, imm8) simde_mm256_permute2f128_si256(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_rcp_ps (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_rcp_ps(a);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

#if defined(SIMDE_SSE_NATIVE)
  for (size_t i = 0 ; i < (sizeof(r_.m128) / sizeof(r_.m128_private[0])) ; i++) {
    r_.m128[i] = simde_mm_rcp_ps(a_.m128[i]);
  }
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = SIMDE_FLOAT32_C(1.0) / a_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_rcp_ps(a) simde_mm256_rcp_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_round_ps (simde__m256 a, const int rounding) {
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    switch (rounding & ~SIMDE_MM_FROUND_NO_EXC) {
      case SIMDE_MM_FROUND_TO_NEAREST_INT:
        r_.f32[i] = nearbyintf(a_.f32[i]);
        break;
      case SIMDE_MM_FROUND_TO_NEG_INF:
        r_.f32[i] = floorf(a_.f32[i]);
        break;
      case SIMDE_MM_FROUND_TO_POS_INF:
        r_.f32[i] = ceilf(a_.f32[i]);
        break;
      case SIMDE_MM_FROUND_TO_ZERO:
        r_.f32[i] = truncf(a_.f32[i]);
        break;
    }
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_round_ps(a, rounding) _mm256_round_ps(a, rounding)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_round_ps(a, rounding) simde_mm256_round_ps(a, rounding)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_round_pd (simde__m256d a, const int rounding) {
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    switch (rounding & ~SIMDE_MM_FROUND_NO_EXC) {
      case SIMDE_MM_FROUND_TO_NEAREST_INT:
        r_.f64[i] = nearbyint(a_.f64[i]);
        break;
      case SIMDE_MM_FROUND_TO_NEG_INF:
        r_.f64[i] = floor(a_.f64[i]);
        break;
      case SIMDE_MM_FROUND_TO_POS_INF:
        r_.f64[i] = ceil(a_.f64[i]);
        break;
      case SIMDE_MM_FROUND_TO_ZERO:
        r_.f64[i] = trunc(a_.f64[i]);
        break;
    }
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_round_pd(a, rounding) _mm256_round_pd(a, rounding)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_round_pd(a, rounding) simde_mm256_round_pd(a, rounding)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_rsqrt_ps (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_rsqrt_ps(a);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

#if defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = 1.0f / sqrtf(a_.f32[i]);
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_rsqrt_ps(a) simde_mm256_rsqrt_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_setr_epi8 (
    int8_t e31, int8_t e30, int8_t e29, int8_t e28, int8_t e27, int8_t e26, int8_t e25, int8_t e24,
    int8_t e23, int8_t e22, int8_t e21, int8_t e20, int8_t e19, int8_t e18, int8_t e17, int8_t e16,
    int8_t e15, int8_t e14, int8_t e13, int8_t e12, int8_t e11, int8_t e10, int8_t  e9, int8_t  e8,
    int8_t  e7, int8_t  e6, int8_t  e5, int8_t  e4, int8_t  e3, int8_t  e2, int8_t  e1, int8_t  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setr_epi8(
      e31, e30, e29, e28, e27, e26, e25, e24,
      e23, e22, e21, e20, e19, e18, e17, e16,
      e15, e14, e13, e12, e11, e10,  e9,  e8,
       e7,  e6,  e5,  e4,  e3,  e2,  e1,  e0);
#else
  return simde_mm256_set_epi8(
       e0,  e1,  e2,  e3,  e4,  e5,  e6,  e7,
       e8,  e9, e10, e11, e12, e13, e14, e15,
      e16, e17, e18, e19, e20, e21, e22, e23,
      e24, e25, e26, e27, e28, e29, e30, e31);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_epi8(e31, e30, e29, e28, e27, e26, e25, e24, e23, e22, e21, e20, e19, e18, e17, e16, e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0) \
    simde_mm256_setr_epi8(e31, e30, e29, e28, e27, e26, e25, e24, e23, e22, e21, e20, e19, e18, e17, e16, e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_setr_epi16 (
    int16_t e15, int16_t e14, int16_t e13, int16_t e12, int16_t e11, int16_t e10, int16_t  e9, int16_t  e8,
    int16_t  e7, int16_t  e6, int16_t  e5, int16_t  e4, int16_t  e3, int16_t  e2, int16_t  e1, int16_t  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setr_epi16(
      e15, e14, e13, e12, e11, e10,  e9,  e8,
       e7,  e6,  e5,  e4,  e3,  e2,  e1,  e0);
#else
  return simde_mm256_set_epi16(
       e0,  e1,  e2,  e3,  e4,  e5,  e6,  e7,
       e8,  e9, e10, e11, e12, e13, e14, e15);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_epi16(e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0) \
    simde_mm256_setr_epi16(e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_setr_epi32 (
    int32_t  e7, int32_t  e6, int32_t  e5, int32_t  e4, int32_t  e3, int32_t  e2, int32_t  e1, int32_t  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setr_epi32(e7, e6, e5, e4, e3, e2, e1, e0);
#else
  return simde_mm256_set_epi32(e0, e1, e2, e3, e4, e5, e6, e7);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_epi32(e7, e6, e5, e4, e3, e2, e1, e0) \
    simde_mm256_setr_epi32(e7, e6, e5, e4, e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_setr_epi64 (int64_t  e3, int64_t  e2, int64_t  e1, int64_t  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setr_epi64x(e3, e2, e1, e0);
#else
  return simde_mm256_set_epi64x(e0, e1, e2, e3);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_epi64(e3, e2, e1, e0) \
    simde_mm256_setr_epi64(e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_setr_ps (
    simde_float32  e7, simde_float32  e6, simde_float32  e5, simde_float32  e4,
    simde_float32  e3, simde_float32  e2, simde_float32  e1, simde_float32  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setr_ps(
       e7,  e6,  e5,  e4,  e3,  e2,  e1,  e0);
#else
  return simde_mm256_set_ps(
       e0,  e1,  e2,  e3,  e4,  e5,  e6,  e7);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_ps(e7, e6, e5, e4, e3, e2, e1, e0) \
    simde_mm256_setr_ps(e7, e6, e5, e4, e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_setr_pd (simde_float64  e3, simde_float64  e2, simde_float64  e1, simde_float64  e0) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_setr_pd(e3, e2, e1, e0);
#else
  return simde_mm256_set_pd(e0, e1, e2, e3);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_pd(e3, e2, e1, e0) \
    simde_mm256_setr_pd(e3, e2, e1, e0)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_setr_m128 (simde__m128 lo, simde__m128 hi) {
  return simde_mm256_set_m128(hi, lo);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_m128(lo, hi) \
    simde_mm256_setr_m128(lo, hi)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_setr_m128d (simde__m128d lo, simde__m128d hi) {
  return simde_mm256_set_m128d(hi, lo);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_m128d(lo, hi) \
    simde_mm256_setr_m128d(lo, hi)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_setr_m128i (simde__m128i lo, simde__m128i hi) {
  return simde_mm256_set_m128i(hi, lo);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_setr_m128i(lo, hi) \
    simde_mm256_setr_m128i(lo, hi)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_shuffle_ps (simde__m256 a, simde__m256 b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xff) == imm8, "imm8 must be in range [0, 255]") {
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

  // SIMDE__VECTORIZE
  // for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
  //   r_.f32[i] = ((i & 2) ? b : a).m128[i >> 2].f32[(imm8 >> ((i & 3) << 1) & 3)];
  // }

  r_.f32[0] = a_.m128_private[0].f32[(imm8 >> 0) & 3];
  r_.f32[1] = a_.m128_private[0].f32[(imm8 >> 2) & 3];
  r_.f32[2] = b_.m128_private[0].f32[(imm8 >> 4) & 3];
  r_.f32[3] = b_.m128_private[0].f32[(imm8 >> 6) & 3];
  r_.f32[4] = a_.m128_private[1].f32[(imm8 >> 0) & 3];
  r_.f32[5] = a_.m128_private[1].f32[(imm8 >> 2) & 3];
  r_.f32[6] = b_.m128_private[1].f32[(imm8 >> 4) & 3];
  r_.f32[7] = b_.m128_private[1].f32[(imm8 >> 6) & 3];

  return simde__m256_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_shuffle_ps(a, b, imm8) _mm256_shuffle_ps(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_shuffle_ps(a, b, imm8) simde_mm256_shuffle_ps(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_shuffle_pd (simde__m256d a, simde__m256d b, const int imm8)
    HEDLEY_REQUIRE_MSG((imm8 & 0xff) == imm8, "imm8 must be in range [0, 15]") {
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

  r_.f64[0] = a_.f64[((imm8     ) & 1)    ];
  r_.f64[1] = b_.f64[((imm8 >> 1) & 1)    ];
  r_.f64[2] = a_.f64[((imm8 >> 2) & 1) | 2];
  r_.f64[3] = b_.f64[((imm8 >> 3) & 1) | 2];

  return simde__m256d_from_private(r_);
}
#if defined(SIMDE_AVX_NATIVE)
#  define simde_mm256_shuffle_pd(a, b, imm8) _mm256_shuffle_pd(a, b, imm8)
#endif
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_shuffle_pd(a, b, imm8) simde_mm256_shuffle_pd(a, b, imm8)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_sqrt_ps (simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_sqrt_ps(a);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_sqrt_ps(a_.m128[0]);
  r_.m128[1] = simde_mm_sqrt_ps(a_.m128[1]);
#elif defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = sqrtf(a_.f32[i]);
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_sqrt_ps(a) simde_mm256_sqrt_ps(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_sqrt_pd (simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_sqrt_pd(a);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_sqrt_pd(a_.m128d[0]);
  r_.m128d[1] = simde_mm_sqrt_pd(a_.m128d[1]);
#elif defined(SIMDE_HAVE_MATH_H)
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = sqrt(a_.f64[i]);
  }
#else
  HEDLEY_UNREACHABLE();
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_sqrt_pd(a) simde_mm256_sqrt_pd(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_store_ps (simde_float32 mem_addr[8], simde__m256 a) {
  simde_assert_aligned(32, mem_addr);

#if defined(SIMDE_AVX_NATIVE)
  _mm256_store_ps(mem_addr, a);
#else
  *SIMDE_CAST_ALIGN(32, simde__m256*, mem_addr) = a;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_store_ps(mem_addr, a) simde_mm256_store_ps(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_store_pd (simde_float64 mem_addr[4], simde__m256d a) {
  simde_assert_aligned(32, mem_addr);

#if defined(SIMDE_AVX_NATIVE)
  _mm256_store_pd(mem_addr, a);
#else
  *SIMDE_CAST_ALIGN(32, simde__m256d*, mem_addr) =  a;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_store_pd(mem_addr, a) simde_mm256_store_pd(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_store_si256 (simde__m256i* mem_addr, simde__m256i a) {
  simde_assert_aligned(32, mem_addr);

#if defined(SIMDE_AVX_NATIVE)
  _mm256_store_si256(mem_addr, a);
#else
  *mem_addr = a;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_store_si256(mem_addr, a) simde_mm256_store_si256(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_storeu_ps (simde_float32 mem_addr[8], simde__m256 a) {
#if defined(SIMDE_AVX_NATIVE)
  _mm256_storeu_ps(mem_addr, a);
#else
  simde_memcpy(mem_addr, &a, sizeof(a));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_storeu_ps(mem_addr, a) simde_mm256_storeu_ps(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_storeu_pd (simde_float64 mem_addr[4], simde__m256d a) {
#if defined(SIMDE_AVX_NATIVE)
  _mm256_storeu_pd(mem_addr, a);
#else
  simde_memcpy(mem_addr, &a, sizeof(a));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_storeu_pd(mem_addr, a) simde_mm256_storeu_pd(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_storeu_si256 (simde__m256i* mem_addr, simde__m256i a) {
#if defined(SIMDE_AVX_NATIVE)
  _mm256_storeu_si256(mem_addr, a);
#else
  simde_memcpy(mem_addr, &a, sizeof(a));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_storeu_si256(mem_addr, a) simde_mm256_storeu_si256(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_storeu2_m128 (simde_float32 hi_addr[4], simde_float32 lo_addr[4], simde__m256 a) {
  simde_mm_storeu_ps(lo_addr, simde_mm256_castps256_ps128(a));
  simde_mm_storeu_ps(hi_addr, simde_mm256_extractf128_ps(a, 1));
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_storeu2_m128(hi_addr, lo_addr, a) simde_mm256_storeu2_m128(hi_addr, lo_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_storeu2_m128d (simde_float64 hi_addr[2], simde_float64 lo_addr[2], simde__m256d a) {
  simde_mm_storeu_pd(lo_addr, simde_mm256_castpd256_pd128(a));
  simde_mm_storeu_pd(hi_addr, simde_mm256_extractf128_pd(a, 1));
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_storeu2_m128d(hi_addr, lo_addr, a) simde_mm256_storeu2_m128d(hi_addr, lo_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_storeu2_m128i (simde__m128i* hi_addr, simde__m128i* lo_addr, simde__m256i a) {
  simde_mm_storeu_si128(lo_addr, simde_mm256_castsi256_si128(a));
  simde_mm_storeu_si128(hi_addr, simde_mm256_extractf128_si256(a, 1));
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_storeu2_m128i(hi_addr, lo_addr, a) simde_mm256_storeu2_m128i(hi_addr, lo_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_stream_ps (simde_float32 mem_addr[8], simde__m256 a) {
  simde_assert_aligned(32, mem_addr);

#if defined(SIMDE_AVX_NATIVE)
  _mm256_stream_ps(mem_addr, a);
#else
  *SIMDE_CAST_ALIGN(32, simde__m256*, mem_addr) = a;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_stream_ps(mem_addr, a) simde_mm256_stream_ps(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_stream_pd (simde_float64 mem_addr[4], simde__m256d a) {
  simde_assert_aligned(32, mem_addr);

#if defined(SIMDE_AVX_NATIVE)
  _mm256_stream_pd(mem_addr, a);
#else
  *SIMDE_CAST_ALIGN(32, simde__m256d*, mem_addr) =  a;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_stream_pd(mem_addr, a) simde_mm256_stream_pd(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm256_stream_si256 (simde__m256i* mem_addr, simde__m256i a) {
  simde_assert_aligned(32, mem_addr);

#if defined(SIMDE_AVX_NATIVE)
  _mm256_stream_si256(mem_addr, a);
#else
  *mem_addr = a;
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_stream_si256(mem_addr, a) simde_mm256_stream_si256(mem_addr, a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_sub_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_sub_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_sub_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_sub_ps(a_.m128[1], b_.m128[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.f32 = a_.f32 - b_.f32;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f32) / sizeof(r_.f32[0])) ; i++) {
    r_.f32[i] = a_.f32[i] - b_.f32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_sub_ps(a, b) simde_mm256_sub_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_sub_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_sub_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_sub_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_sub_pd(a_.m128d[1], b_.m128d[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.f64 = a_.f64 - b_.f64;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.f64) / sizeof(r_.f64[0])) ; i++) {
    r_.f64[i] = a_.f64[i] - b_.f64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_sub_pd(a, b) simde_mm256_sub_pd(a, b)
#endif

#if defined(SIMDE_DIAGNOSTIC_DISABLE_UNINITIALIZED_)
  HEDLEY_DIAGNOSTIC_PUSH
  SIMDE_DIAGNOSTIC_DISABLE_UNINITIALIZED_
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_undefined_ps (void) {
  simde__m256_private r_;

#if \
    defined(SIMDE_AVX_NATIVE) && \
    (!defined(HEDLEY_GCC_VERSION) || HEDLEY_GCC_VERSION_CHECK(5,0,0)) && \
    (!defined(__has_builtin) || HEDLEY_HAS_BUILTIN(__builtin_ia32_undef256))
  r_.n = _mm256_undefined_ps();
#elif !defined(SIMDE_DIAGNOSTIC_DISABLE_UNINITIALIZED_)
  r_ = simde_mm256_setzero_ps();
#endif

  return simde__m256_from_private(r_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_undefined_ps() simde_mm256_undefined_ps()
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_undefined_pd (void) {
  simde__m256d_private r_;

#if \
    defined(SIMDE_AVX_NATIVE) && \
    (!defined(HEDLEY_GCC_VERSION) || HEDLEY_GCC_VERSION_CHECK(5,0,0)) && \
    (!defined(__has_builtin) || HEDLEY_HAS_BUILTIN(__builtin_ia32_undef256))
  r_.n = _mm256_undefined_pd();
#elif !defined(SIMDE_DIAGNOSTIC_DISABLE_UNINITIALIZED_)
  r_ = simde_mm256_setzero_pd();
#endif

  return simde__m256d_from_private(r_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_undefined_pd() simde_mm256_undefined_pd()
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_undefined_si256 (void) {
  simde__m256i_private r_;
#if \
    defined(SIMDE_AVX_NATIVE) && \
    (!defined(HEDLEY_GCC_VERSION) || HEDLEY_GCC_VERSION_CHECK(5,0,0)) && \
    (!defined(__has_builtin) || HEDLEY_HAS_BUILTIN(__builtin_ia32_undef256))
  r_.n = _mm256_undefined_si256();
#elif !defined(SIMDE_DIAGNOSTIC_DISABLE_UNINITIALIZED_)
  r_ = simde_mm256_setzero_si256();
#endif

  return simde__m256i_from_private(r_);
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_undefined_si256() simde_mm256_undefined_si256()
#endif

#if defined(SIMDE_DIAGNOSTIC_DISABLE_UNINITIALIZED_)
  HEDLEY_DIAGNOSTIC_POP
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_xor_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_xor_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE_SSE_NATIVE)
  r_.m128[0] = simde_mm_xor_ps(a_.m128[0], b_.m128[0]);
  r_.m128[1] = simde_mm_xor_ps(a_.m128[1], b_.m128[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = a_.i32f ^ b_.i32f;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.u32) / sizeof(r_.u32[0])) ; i++) {
    r_.u32[i] = a_.u32[i] ^ b_.u32[i];
  }
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_xor_ps(a, b) simde_mm256_xor_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_xor_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_xor_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE_ARCH_X86_SSE2)
  r_.m128d[0] = simde_mm_xor_pd(a_.m128d[0], b_.m128d[0]);
  r_.m128d[1] = simde_mm_xor_pd(a_.m128d[1], b_.m128d[1]);
#elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS)
  r_.i32f = a_.i32f ^ b_.i32f;
#else
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.u64) / sizeof(r_.u64[0])) ; i++) {
    r_.u64[i] = a_.u64[i] ^ b_.u64[i];
  }
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_xor_pd(a, b) simde_mm256_xor_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_unpackhi_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_unpackhi_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE__SHUFFLE_VECTOR)
  r_.f32 = SIMDE__SHUFFLE_VECTOR(32, 32, a_.f32, b_.f32, 2, 10, 3, 11, 6, 14, 7, 15);
#else
  r_.f32[0] = a_.f32[2];
  r_.f32[1] = b_.f32[2];
  r_.f32[2] = a_.f32[3];
  r_.f32[3] = b_.f32[3];
  r_.f32[4] = a_.f32[6];
  r_.f32[5] = b_.f32[6];
  r_.f32[6] = a_.f32[7];
  r_.f32[7] = b_.f32[7];
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_unpackhi_ps(a, b) simde_mm256_unpackhi_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_unpackhi_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_unpackhi_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE__SHUFFLE_VECTOR)
  r_.f64 = SIMDE__SHUFFLE_VECTOR(64, 32, a_.f64, b_.f64, 1, 5, 3, 7);
#else
  r_.f64[0] = a_.f64[1];
  r_.f64[1] = b_.f64[1];
  r_.f64[2] = a_.f64[3];
  r_.f64[3] = b_.f64[3];
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_unpackhi_pd(a, b) simde_mm256_unpackhi_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_unpacklo_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_unpacklo_ps(a, b);
#else
  simde__m256_private
    r_,
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

#if defined(SIMDE__SHUFFLE_VECTOR)
  r_.f32 = SIMDE__SHUFFLE_VECTOR(32, 32, a_.f32, b_.f32, 0, 8, 1, 9, 4, 12, 5, 13);
#else
  r_.f32[0] = a_.f32[0];
  r_.f32[1] = b_.f32[0];
  r_.f32[2] = a_.f32[1];
  r_.f32[3] = b_.f32[1];
  r_.f32[4] = a_.f32[4];
  r_.f32[5] = b_.f32[4];
  r_.f32[6] = a_.f32[5];
  r_.f32[7] = b_.f32[5];
#endif

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_unpacklo_ps(a, b) simde_mm256_unpacklo_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_unpacklo_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_unpacklo_pd(a, b);
#else
  simde__m256d_private
    r_,
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

#if defined(SIMDE__SHUFFLE_VECTOR)
  r_.f64 = SIMDE__SHUFFLE_VECTOR(64, 32, a_.f64, b_.f64, 0, 4, 2, 6);
#else
  r_.f64[0] = a_.f64[0];
  r_.f64[1] = b_.f64[0];
  r_.f64[2] = a_.f64[2];
  r_.f64[3] = b_.f64[2];
#endif

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_unpacklo_pd(a, b) simde_mm256_unpacklo_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256
simde_mm256_zextps128_ps256 (simde__m128 a) {
#if defined(SIMDE_AVX_NATIVE) && !defined(SIMDE_BUG_GCC_REV_274313)
  return _mm256_zextps128_ps256(a);
#else
  simde__m256_private r_;

  r_.m128_private[0] = simde__m128_to_private(a);
  r_.m128_private[1] = simde__m128_to_private(simde_mm_setzero_ps());

  return simde__m256_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_zextps128_ps256(a) simde_mm256_zextps128_ps256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256d
simde_mm256_zextpd128_pd256 (simde__m128d a) {
#if defined(SIMDE_AVX_NATIVE) && !defined(SIMDE_BUG_GCC_REV_274313)
  return _mm256_zextpd128_pd256(a);
#else
  simde__m256d_private r_;

  r_.m128d_private[0] = simde__m128d_to_private(a);
  r_.m128d_private[1] = simde__m128d_to_private(simde_mm_setzero_pd());

  return simde__m256d_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_zextpd128_pd256(a) simde_mm256_zextpd128_pd256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m256i
simde_mm256_zextsi128_si256 (simde__m128i a) {
#if defined(SIMDE_AVX_NATIVE) && !defined(SIMDE_BUG_GCC_REV_274313)
  return _mm256_zextsi128_si256(a);
#else
  simde__m256i_private r_;

  r_.m128i_private[0] = simde__m128i_to_private(a);
  r_.m128i_private[1] = simde__m128i_to_private(simde_mm_setzero_si128());

  return simde__m256i_from_private(r_);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_zextsi128_si256(a) simde_mm256_zextsi128_si256(a)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm_testc_ps (simde__m128 a, simde__m128 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_testc_ps(a, b);
#else
  uint_fast32_t r = 0;
  simde__m128_private
    a_ = simde__m128_to_private(a),
    b_ = simde__m128_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.u32) / sizeof(a_.u32[0])) ; i++) {
    r |= ~a_.u32[i] & b_.u32[i];
  }

  return HEDLEY_STATIC_CAST(int, ((~r >> 31) & 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_testc_ps(a, b) simde_mm_testc_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm_testc_pd (simde__m128d a, simde__m128d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_testc_pd(a, b);
#else
  uint_fast64_t r = 0;
  simde__m128d_private
    a_ = simde__m128d_to_private(a),
    b_ = simde__m128d_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.u64) / sizeof(a_.u64[0])) ; i++) {
    r |= ~a_.u64[i] & b_.u64[i];
  }

  return HEDLEY_STATIC_CAST(int, ((~r >> 63) & 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_testc_pd(a, b) simde_mm_testc_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testc_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testc_ps(a, b);
#else
  uint_fast32_t r = 0;
  simde__m256_private
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.u32) / sizeof(a_.u32[0])) ; i++) {
    r |= ~a_.u32[i] & b_.u32[i];
  }

  return HEDLEY_STATIC_CAST(int, ((~r >> 31) & 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testc_ps(a, b) simde_mm256_testc_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testc_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testc_pd(a, b);
#else
  uint_fast64_t r = 0;
  simde__m256d_private
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.u64) / sizeof(a_.u64[0])) ; i++) {
    r |= ~a_.u64[i] & b_.u64[i];
  }

  return HEDLEY_STATIC_CAST(int, ((~r >> 63) & 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testc_pd(a, b) simde_mm256_testc_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testc_si256 (simde__m256i a, simde__m256i b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testc_si256(a, b);
#else
  int_fast32_t r = 0;
  simde__m256i_private
    a_ = simde__m256i_to_private(a),
    b_ = simde__m256i_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.i32f) / sizeof(a_.i32f[0])) ; i++) {
    r |= ~a_.i32f[i] & b_.i32f[i];
  }

  return HEDLEY_STATIC_CAST(int, !r);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testc_si256(a, b) simde_mm256_testc_si256(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm_testz_ps (simde__m128 a, simde__m128 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_testz_ps(a, b);
#else
  uint_fast32_t r = 0;
  simde__m128_private
    a_ = simde__m128_to_private(a),
    b_ = simde__m128_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.u32) / sizeof(a_.u32[0])) ; i++) {
    r |= a_.u32[i] & b_.u32[i];
  }

  return HEDLEY_STATIC_CAST(int, ((~r >> 31) & 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_testz_ps(a, b) simde_mm_testz_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm_testz_pd (simde__m128d a, simde__m128d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_testz_pd(a, b);
#else
  uint_fast64_t r = 0;
  simde__m128d_private
    a_ = simde__m128d_to_private(a),
    b_ = simde__m128d_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.u64) / sizeof(a_.u64[0])) ; i++) {
    r |= a_.u64[i] & b_.u64[i];
  }

  return HEDLEY_STATIC_CAST(int, ((~r >> 63) & 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_testz_pd(a, b) simde_mm_testz_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testz_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testz_ps(a, b);
#else
  uint_fast32_t r = 0;
  simde__m256_private
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.u32) / sizeof(a_.u32[0])) ; i++) {
    r |= a_.u32[i] & b_.u32[i];
  }

  return HEDLEY_STATIC_CAST(int, ((~r >> 31) & 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testz_ps(a, b) simde_mm256_testz_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testz_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testz_pd(a, b);
#else
  uint_fast64_t r = 0;
  simde__m256d_private
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.u64) / sizeof(a_.u64[0])) ; i++) {
    r |= a_.u64[i] & b_.u64[i];
  }

  return HEDLEY_STATIC_CAST(int, ((~r >> 63) & 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testz_pd(a, b) simde_mm256_testz_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testz_si256 (simde__m256i a, simde__m256i b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testz_si256(a, b);
#else
  int_fast32_t r = 0;
  simde__m256i_private
    a_ = simde__m256i_to_private(a),
    b_ = simde__m256i_to_private(b);

  SIMDE__VECTORIZE_REDUCTION(|:r)
  for (size_t i = 0 ; i < (sizeof(a_.i32f) / sizeof(a_.i32f[0])) ; i++) {
    r |= a_.i32f[i] & b_.i32f[i];
  }

  return HEDLEY_STATIC_CAST(int, !r);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testz_si256(a, b) simde_mm256_testz_si256(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm_testnzc_ps (simde__m128 a, simde__m128 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_testnzc_ps(a, b);
#else
  uint32_t rz = 0, rc = 0;
  simde__m128_private
    a_ = simde__m128_to_private(a),
    b_ = simde__m128_to_private(b);

  for (size_t i = 0 ; i < (sizeof(a_.u32) / sizeof(a_.u32[0])) ; i++) {
    rc |= ~a_.u32[i] & b_.u32[i];
    rz |=  a_.u32[i] & b_.u32[i];
  }

  return
    (rc >> ((sizeof(rc) * CHAR_BIT) - 1)) &
    (rz >> ((sizeof(rz) * CHAR_BIT) - 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_testnzc_ps(a, b) simde_mm_testnzc_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm_testnzc_pd (simde__m128d a, simde__m128d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm_testnzc_pd(a, b);
#else
  uint64_t rc = 0, rz = 0;
  simde__m128d_private
    a_ = simde__m128d_to_private(a),
    b_ = simde__m128d_to_private(b);

  for (size_t i = 0 ; i < (sizeof(a_.u64) / sizeof(a_.u64[0])) ; i++) {
    rc |= ~a_.u64[i] & b_.u64[i];
    rz |=  a_.u64[i] & b_.u64[i];
  }

  return
    (rc >> ((sizeof(rc) * CHAR_BIT) - 1)) &
    (rz >> ((sizeof(rz) * CHAR_BIT) - 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm_testnzc_pd(a, b) simde_mm_testnzc_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testnzc_ps (simde__m256 a, simde__m256 b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testnzc_ps(a, b);
#else
  uint32_t rc = 0, rz = 0;
  simde__m256_private
    a_ = simde__m256_to_private(a),
    b_ = simde__m256_to_private(b);

  for (size_t i = 0 ; i < (sizeof(a_.u32) / sizeof(a_.u32[0])) ; i++) {
    rc |= ~a_.u32[i] & b_.u32[i];
    rz |=  a_.u32[i] & b_.u32[i];
  }

  return
    (rc >> ((sizeof(rc) * CHAR_BIT) - 1)) &
    (rz >> ((sizeof(rz) * CHAR_BIT) - 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testnzc_ps(a, b) simde_mm256_testnzc_ps(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testnzc_pd (simde__m256d a, simde__m256d b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testnzc_pd(a, b);
#else
  uint64_t rc = 0, rz = 0;
  simde__m256d_private
    a_ = simde__m256d_to_private(a),
    b_ = simde__m256d_to_private(b);

  for (size_t i = 0 ; i < (sizeof(a_.u64) / sizeof(a_.u64[0])) ; i++) {
    rc |= ~a_.u64[i] & b_.u64[i];
    rz |=  a_.u64[i] & b_.u64[i];
  }

  return
    (rc >> ((sizeof(rc) * CHAR_BIT) - 1)) &
    (rz >> ((sizeof(rz) * CHAR_BIT) - 1));
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testnzc_pd(a, b) simde_mm256_testnzc_pd(a, b)
#endif

SIMDE__FUNCTION_ATTRIBUTES
int
simde_mm256_testnzc_si256 (simde__m256i a, simde__m256i b) {
#if defined(SIMDE_AVX_NATIVE)
  return _mm256_testnzc_si256(a, b);
#else
  int32_t rc = 0, rz = 0;
  simde__m256i_private
    a_ = simde__m256i_to_private(a),
    b_ = simde__m256i_to_private(b);

  for (size_t i = 0 ; i < (sizeof(a_.i32f) / sizeof(a_.i32f[0])) ; i++) {
    rc |= ~a_.i32f[i] & b_.i32f[i];
    rz |=  a_.i32f[i] & b_.i32f[i];
  }

  return !!(rc & rz);
#endif
}
#if defined(SIMDE_AVX_ENABLE_NATIVE_ALIASES)
#  define _mm256_testnzc_si256(a, b) simde_mm256_testnzc_si256(a, b)
#endif

SIMDE__END_DECLS

HEDLEY_DIAGNOSTIC_POP

#endif /* !defined(SIMDE__AVX_H) */
