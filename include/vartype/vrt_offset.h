#ifndef __VRT_OFFSET_H__
#define __VRT_OFFSET_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_SIZE_NOFUNC
#include "vrt_size.h"
#undef VRT_SIZE_NOFUNC


// Maybe rename it to VRToffsetBuffer / VRToffsetInfo / VRToffsetData
typedef struct VRToffset {
    VRTsize *pool;
    uint32_t count;
    uint32_t length;
} VRToffset;


#ifndef VRT_OFFSET_NOFUNC
VRTresult VRToffset_Alloc(
    VRToffset *offset,
    const VRTsize _size,
    size_t *addr
);
VRTresult VRToffset_Free(
    VRToffset *offset,
    size_t addr
);

VRTresult VRToffset_Grow(
    VRToffset *offset,
    const VRTsize _size
);
VRTresult VRToffset_Shrink(
    VRToffset *offset,
    const VRTsize _size
);
#endif


#ifdef __cplusplus
};
#endif
#endif