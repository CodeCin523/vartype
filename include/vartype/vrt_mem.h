#ifndef __VRT_MEM_H__
#define __VRT_MEM_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_OFFSET_NOFUNC
#include "vrt_offset.h"


// Virtual Memory Page Manager
typedef struct VRTmem {
    VRToffset offset;
    void **pMem;
    size_t length;
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