#include <vartype/vrt_size.h>

#if defined(_MSC_VER)
  #define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
  #define FORCE_INLINE inline __attribute__((always_inline))
#else
  /* Fallback: request inline, but not forced */
  #define FORCE_INLINE inline
#endif

#define VRT_SIZE_LOCK 0b10000000
#define VRT_SIZE_DATA 0b01111111


static FORCE_INLINE VRTresult STINLsize_CHECK(
    VRTsize size
) {
    if(size & VRT_SIZE_LOCK)
        return VRT_RESULT_INVALID_STATE;
    
    return VRT_RESULT_SUCCESS;
}

static FORCE_INLINE VRTsize STINL_ByteToSize(
    uint64_t byte
) {
    // FIND_LEFTMOST_BIT
    
    if (byte == 0) return 0; // or handle differently
#ifdef __GNUC__
    return 63 - __builtin_clzll(byte); // 0-based indexing
#else
    /* Binary search approach*/
    uint8_t pos = 0;
    if (byte & 0xFFFFFFFF00000000ULL) { byte >>= 32; pos += 32; }
    if (byte & 0xFFFF0000ULL) { byte >>= 16; pos += 16; }
    if (byte & 0xFF00U) { byte >>= 8; pos += 8; }
    if (byte & 0xF0U) { byte >>= 4; pos += 4; }
    if (byte & 0x0CU) { byte >>= 2; pos += 2; }
    if (byte & 0x02U) { pos += 1; }
    return pos; // 0-based
#endif
}
static FORCE_INLINE uint64_t STINL_SizeToByte(
    VRTsize size
) {
    return 1 << (size & VRT_SIZE_DATA);
}


VRTresult VRT_ByteToSize(
  uint64_t byte,
  VRTsize *size
) {
    if(size == NULL)
        return VRT_RESULT_CALL_NO_PTR;
    
    *size = STINL_ByteToSize(byte);
    return VRT_RESULT_SUCCESS;
}
VRTresult VRT_SizeToByte(
    VRTsize size,
    uint64_t *byte
) {
    if(byte == NULL)
        return VRT_RESULT_CALL_NO_PTR;
    {   // STINLsize_CHECK
        const VRTresult _res = STINLsize_CHECK(size);
        if(_res != VRT_RESULT_SUCCESS)
            return _res;
    }

    *byte = STINL_SizeToByte(size);
    return VRT_RESULT_SUCCESS;
}