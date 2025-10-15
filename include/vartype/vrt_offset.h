#ifndef __VRT_OFFSET_H__
#define __VRT_OFFSET_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_SIZE_NOFUNC
#include "vrt_size.h"


// Maybe rename it to VRToffsetBuffer / VRToffsetInfo / VRToffsetData
typedef struct VRToffset {
    VRTsize *pool;      // 8
    uint32_t count;     // 4
    uint32_t length;    // 4
} VRToffset;


#ifdef VRT_OFFSET_NOFUNC
#include <vartype/export_vartype.h>

VRTresult VARTYPE_EXPORT VRToffset_Alloc(
    VRToffset *offset,
    const VRTsize _size,
    size_t *addr
);
VRTresult VARTYPE_EXPORT VRToffset_Free(
    VRToffset *offset,
    size_t addr
);

VRTresult VARTYPE_EXPORT VRToffset_Grow(
    VRToffset *offset,
    const VRTsize _size
);
VRTresult VARTYPE_EXPORT VRToffset_Shrink(
    VRToffset *offset,
    const VRTsize _size
);

// VRTresult VARTYPE_EXPORT VRToffset_Optimize(
//     VRToffset *offset
// );
#undef VRT_OFFSET_NOFUNC
#endif


#ifdef __cplusplus
};
#endif
#endif