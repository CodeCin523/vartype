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

#define fVRTsize_S2B(s, b) b = 1 << (s & mVRTsize_DATA)
#ifdef __GNUC__
// 0 based
#define fVRTsize_B2S(s, b) s = b == 0? 0 : 63 - __builtin_clzll(byte)
#else
// 0 based
#define fVRTsize_B2S(s, b) {                                \
    if (b & 0xFFFFFFFF00000000ULL) { b >>= 32; s += 32; }   \
    if (b & 0xFFFF0000ULL) { b >>= 16; s += 16; }           \
    if (b & 0xFF00U) { b >>= 8; s += 8; }                   \
    if (b & 0xF0U) { b >>= 4; s += 4; }                     \
    if (b & 0x0CU) { b >>= 2; s += 2; }                     \
    if (b & 0x02U) { b += 1; }                              \
}
#endif


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