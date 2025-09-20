#include <vartype/vrt_size.h>

#if defined(_MSC_VER)
  #define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
  #define FORCE_INLINE inline __attribute__((always_inline))
#else
  /* Fallback: request inline, but not forced */
  #define FORCE_INLINE inline
#endif

static FORCE_INLINE VRTsize STINL_ByteToSize(uint64_t b) {
    // FIND_LEFTMOST_BIT
    
    if (b == 0) return 0; // or handle differently
#ifdef __GNUC__
    return 63 - __builtin_clzll(b); // 0-based indexing
#else
    /* Binary search approach*/
    uint8_t pos = 0;
    if (b & 0xFFFFFFFF00000000ULL) { b >>= 32; pos += 32; }
    if (b & 0xFFFF0000ULL) { b >>= 16; pos += 16; }
    if (b & 0xFF00U) { b >>= 8; pos += 8; }
    if (b & 0xF0U) { b >>= 4; pos += 4; }
    if (b & 0x0CU) { b >>= 2; pos += 2; }
    if (b & 0x02U) { pos += 1; }
    return pos; // 0-based
#endif
}
static FORCE_INLINE uint64_t STINL_SizeToByte(VRTsize s) {
    return 1 << s;
}

VRTsize VRT_ByteToSize(uint64_t b) {
    return STINL_ByteToSize(b);
}
uint64_t VRT_SizeToByte(VRTsize s) {
    return STINL_SizeToByte(s);
}