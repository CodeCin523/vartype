#ifndef __VRT_MEM_H__
#define __VRT_MEM_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_OFFSET_NOFUNC
#include "vrt_offset.h"
#undef VRT_OFFSET_NOFUNC


typedef struct VRTmem {
    VRToffset offset;
    void *pMem;
    VRToffsetAddr length;
} VRTmem;


#ifndef VRT_MEM_NOFUNC

#endif


#ifdef __cplusplus
};
#endif
#endif