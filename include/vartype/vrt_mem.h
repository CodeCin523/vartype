#ifndef __VRT_MEM_H__
#define __VRT_MEM_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_OFFSET_NOFUNC
#include "vrt_offset.h"
#undef VRT_OFFSET_NOFUNC


// Virtual Memory Page Manager
typedef struct VRTmem {
    VRToffset offset;
    void **pMem;
    size_t length;
} VRTmem;


#ifndef VRT_MEM_NOFUNC
VRTresult VRTmem_Alloc(
    VRTmem *mem,
    const size_t _size,
    void **addr
);
VRTresult VRTmem_Free(
    VRTmem *mem,
    void *addr
);
#endif


#ifdef __cplusplus
};
#endif
#endif