#ifndef __VRT_OFFSET_H__
#define __VRT_OFFSET_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_NOFUNC
#include "vrt_size.h"
#undef VRT_NOFUNC

// Maybe rename it, currently offset address
typedef uint32_t VRToffsetAddr; 

// Maybe rename it to VRToffsetBuffer / VRToffsetInfo / VRToffsetData
typedef struct VRToffset {
    VRTsize *pool;
    VRToffsetAddr count;
    VRToffsetAddr length;
} VRToffset;

#ifndef VRT_NOFUNC
VRTresult VRToffset_Alloc(
    VRToffset *offset,
    const VRTsize _size,
    VRToffsetAddr *addr
);
VRTresult VRToffset_Free(
    VRToffset *offset,
    VRToffsetAddr addr
);

VRTresult VRToffset_Grow(
    VRToffset *offset,
    const VRTsize _size
);
VRTresult VRToffset_Shrink(
    VRToffset *offset,
    const VRTsize _size
);
#else
#undef VRT_NOFUNC
#endif

#ifdef __cplusplus
};
#endif
#endif