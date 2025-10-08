#include <vartype/vrt_mem.h>

#include "vartype/vrt_result.h"
#include "vrt_offset.c"


#define fVRTmem_RETURN(m) {                 \
    if(m == NULL)                           \
        return VRT_RESULT_CALL_NO_PTR;      \
    if(m->pMem == NULL)                     \
        return VRT_RESULT_INVALID_STATE;    \
}


VRTresult VRTmem_Alloc(
    VRTmem *mem,
    const size_t _size,
    void **addr
) {
    
}
VRTresult VRTmem_Free(
    VRTmem *mem,
    void *addr
) {
    
}