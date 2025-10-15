#include <vartype/vrt_size.h>


// #if defined(_MSC_VER)
//   #define FORCE_INLINE __forceinline
// #elif defined(__GNUC__) || defined(__clang__)
//   #define FORCE_INLINE inline __attribute__((always_inline))
// #else
//   /* Fallback: request inline, but not forced */
//   #define FORCE_INLINE inline
// #endif

#define mVRTsize_LOCK 0b10000000
#define mVRTsize_DATA 0b01111111

#define fVRTsize_RETURN(s) {                \
    if(s & mVRTsize_LOCK )                  \
        return VRT_RESULT_INVALID_STATE;    \
}

#ifdef _WIN64
#define CONSTEXPR_
static CONSTEXPR_ __forceinline int __builtin_clzll(unsigned long long x)
{
#ifdef CONSTEVAL_
    if CONSTEVAL_
    {
        for (int i = 0; i < sizeof(x) * CHAR_BIT; ++i)
        {
            if (x >> (sizeof(x) * CHAR_BIT - 1 - i))
                return i;
        }
        return sizeof(x) * CHAR_BIT;
    }
#endif
#if defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC)
    return (int)_CountLeadingZeros64(x);
#elif defined(_WIN64)
#if defined(__AVX2__) || defined(__LZCNT__)
    return (int)_lzcnt_u64(x);
#else
    unsigned long r;
    _BitScanReverse64(&r, x);
    return (int)(r ^ 63);
#endif
#else
    int l = __builtin_clz((unsigned)x) + 32;
    int h = __builtin_clz((unsigned)(x >> 32));
    return !!((unsigned)(x >> 32)) ? h : l;
#endif
}
#endif

#define fVRTsize_S2B(s, b) b = 1 << (s & mVRTsize_DATA)
// 0 based
#define fVRTsize_B2S(s, b) s = b == 0? 0 : 63 - __builtin_clzll(b)


VRTresult VRT_ByteToSize(
  uint64_t byte,
  VRTsize *size
) {
    if(size == NULL)
        return VRT_RESULT_CALL_NO_PTR;
    if(byte == 0)
        return VRT_RESULT_INVALID_STATE;
    
    fVRTsize_B2S(*size, byte);
    return VRT_RESULT_SUCCESS;
}
VRTresult VRT_SizeToByte(
    VRTsize size,
    uint64_t *byte
) {
    if(byte == NULL)
        return VRT_RESULT_CALL_NO_PTR;

    *byte = fVRTsize_S2B(size, *byte);

    fVRTsize_RETURN(size);
    return VRT_RESULT_SUCCESS;
}