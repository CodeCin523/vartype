#ifndef __VRT_SIZE_H__
#define __VRT_SIZE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define VRT_KB(x) (x * 1024)        /* 1024 */
#define VRT_MB(x) (x * 1048576)     /* 1024 * 1024 */
#define VRT_GB(x) (x * 1073741824)  /* 1024 * 1024 * 1024 */

typedef uint8_t VRTsize;

#ifndef VRT_NOFUNC
VRTsize VRT_ByteToSize(uint64_t b);
uint64_t VRT_SizeToByte(VRTsize s);
#else
#undef VRT_NOFUNC
#endif

#ifdef __cplusplus
};
#endif
#endif