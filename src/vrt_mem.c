#include <vartype/vrt_mem.h>

#include "vartype/vrt_result.h"
#include "vrt_offset.c"

#include <stdlib.h>

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
    
    if(res == VRT_RESULT_NO_SPACE_IARRAY) { // Size up mem->offset.pool
        if(mem->offset.count < mem->offset.length)
            return VRT_RESULT_INVALID_STATE;
        
        mem->offset.length *= 2;
        VRTsize *temp = calloc(mem->offset.length, sizeof(VRTsize));
        if(temp == NULL)
            return VRT_RESULT_NO_SPACE_CALLOC;
        
        // if(mem->offset.pool != NULL) 
        memcpy(temp, mem->offset.pool, mem->offset.count);
        free(mem->offset.pool);

        mem->offset.pool = temp;

        res = VRToffset_Alloc(&mem->offset, _size, &offsetAddr);
    } else if(res == VRT_RESULT_NO_SPACE_IALLOC) { // Reserve More Space
        // If no space in pMem. 
        void **temp = (void **) calloc(mem->length+1, sizeof(void *));
        if(temp == NULL)
            return VRT_RESULT_NO_SPACE_CALLOC;
        if(mem->pMem != NULL) {
            memcpy(temp, mem->pMem, mem->length);
            free(mem->pMem);
        }
        mem->pMem = temp;

        // Reserve new space.
        void *nPage = NULL;
#ifdef VRT_OS_WIN
        nPage = VirtualAlloc(NULL, mVRTmem_MAP_BYTE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#endif
        if(nPage == NULL)
            return VRT_RESULT_NO_SPACE_VIRMEM;
        mem->pMem[mem->length++] = nPage;

        // Retry alloc.
        VRToffset_Grow(&mem->offset, mVRTmem_MAP_SIZE);
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
    if(addr == NULL)
        return VRT_RESULT_INVALID_STATE;
    fVRTmem_RETURN(mem);

    // IMPLEMENTATION
    size_t offsetAddr = 0xFFFFFFFF;
    for(int i = 0; i < mem->length; ++i) {
        size_t off = (size_t)mem->pMem[i] - (size_t)addr;
        if(off >= 0 && off < mVRTmem_MAP_BYTE) {
            offsetAddr = off + i * mVRTmem_MAP_BYTE;
            break;
        }
    }
    if(offsetAddr == 0xFFFFFFFF)
        return VRT_RESULT_FAILED;
    
    return VRToffset_Free(&mem->offset, offsetAddr);
}