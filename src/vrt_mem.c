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
    
    if(res == VRT_RESULT_NO_SPACE_IARRAY) {
        // need to size-up mem->offset.pool
    } else if(res == VRT_RESULT_NO_SPACE_IALLOC) {
        // need to reserve more space
        // If no space in pMem. 
        if(mem->count >= mem->length) {

        }

        // Reserve new space.
        void *nPage = NULL;
#ifdef VRT_OS_WIN
        nPage = VirtualAlloc(NULL, mVRTmem_MAP_BYTE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#endif
        if(nPage == NULL)
            return VRT_RESULT_NO_SPACE_VIRMEM;

        VRToffset_Grow(&mem->offset, mVRTmem_MAP_SIZE);
        mem->pMem[mem->count++] = nPage;

        // Retry alloc;
        res = VRToffset_Alloc(&mem->offset, _size, &offsetAddr);
    }
    if(res != VRT_RESULT_SUCCESS)
        return res;

    *addr = offsetAddr % mVRTmem_MAP_BYTE + (size_t)mem->pMem[offsetAddr / mVRTmem_MAP_BYTE];
    return VRT_RESULT_SUCCESS;
}
VRTresult VRTmem_Free(
    VRTmem *mem,
    void *addr
) {
    // EARLY RETURNS
    // IMPLEMENTATION
}