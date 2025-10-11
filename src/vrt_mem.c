#include <vartype/vrt_mem.h>

#include "vartype/vrt_result.h"
#include "vrt_offset.c"

#ifdef _WIN64
#define VRT_OS_WIN
#include <memoryapi.h>
#endif

#define mVRTmem_MAP_BYTE 1024
#define mVRTmem_MAP_SIZE 10

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
    // EARLY RETURNS
    if(addr == NULL)
        return VRT_RESULT_CALL_NO_PTR;
    fVRTmem_RETURN(mem);
    
    // IMPLEMENTATION
    size_t offsetAddr = 0; 
    VRTresult res = VRToffset_Alloc(&mem->offset, _size, &offsetAddr);
    if(res == VRT_RESULT_MEM_NO_SPACE) {

        res = VRToffset_Alloc(&mem->offset, _size, &offsetAddr);
    }
    if(res != VRT_RESULT_SUCCESS)
        return res;

    
}
VRTresult VRTmem_Free(
    VRTmem *mem,
    void *addr
) {
    // EARLY RETURNS
    // IMPLEMENTATION
}