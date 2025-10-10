#ifndef __VRT_SIZE_H__
#define __VRT_SIZE_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_RESULT_NOFUNC
#include "vrt_result.h"

#include <stddef.h>


#define VRT_KB(x) (x * 1024)        /* 1024 */
#define VRT_MB(x) (x * 1048576)     /* 1024 * 1024 */
#define VRT_GB(x) (x * 1073741824)  /* 1024 * 1024 * 1024 */

typedef uint8_t VRTsize;


#ifndef VRT_SIZE_NOFUNC
#include <vartype/export_vartype.h>

VRTresult VARTYPE_EXPORT VRT_ByteToSize(
    size_t byte,
    VRTsize *size
);
VRTresult VARTYPE_EXPORT VRT_SizeToByte(
    VRTsize size,
    size_t *byte
);

#undef VRT_SIZE_NOFUNC
#endif


#ifdef __cplusplus
};
#endif
#endif