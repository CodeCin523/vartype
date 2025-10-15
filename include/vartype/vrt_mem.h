#ifndef __VRT_MEM_H__
#define __VRT_MEM_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_OFFSET_NOFUNC
#include "vrt_offset.h"


// Virtual Memory Page Manager
typedef struct VRTmem {
    VRToffset offset;       // 16
    void **pMem;            // 8
    // uint32_t count;         // 4
    uint32_t length;        // 4
} VRTmem;


#ifndef VRT_MEM_NOFUNC
#include <vartype/export_vartype.h>

VRTresult VARTYPE_EXPORT VRTmem_Alloc(
    VRTmem *mem,
    const size_t _size,
    void **addr
);
VRTresult VARTYPE_EXPORT VRTmem_Free(
    VRTmem *mem,
    void *addr
);

#undef VRT_MEM_NOFUNC
#endif


#ifdef __cplusplus
};
#endif
#endif