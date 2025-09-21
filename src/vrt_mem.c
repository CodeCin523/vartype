#include <vartype/vrt_mem.h>

#include "vrt_offset.c"


static FORCE_INLINE STINLmem_CHECK(
    VRTmem *mem
) {
    if(mem == NULL)
        return VRT_RESULT_CALL_NO_PTR;
    if(mem->pMem == NULL)
        return VRT_RESULT_INVALID_STATE;

    {   // STINLoffset_CHECK
        const VRTresult res = STINLoffset_CHECK(&mem->offset);
        if(res != VRT_RESULT_SUCCESS)
            return res;
    }

    return VRT_RESULT_SUCCESS;
}

static FORCE_INLINE void *STINLmem_Alloc(
    VRTmem *mem,
    const size_t _size
) {
    VRToffsetAddr addr = STINLoffset_Alloc(&mem->offset, _size);
    if(addr == VRT_OFFSET_INVALID) {
        // GROW
    }
    return addr;
}
static FORCE_INLINE VRTresult STINLmem_Free(
    VRTmem *mem,
    void *addr
) {

}


VRTresult VRTmem_Alloc(
    VRTmem *mem,
    const size_t _size,
    void **addr
) {
    {   // STINLmem_CHECK
        const VRTresult res = STINLmem_CHECK(&mem);
        if(res != VRT_RESULT_SUCCESS)
            return res;
    }

    void *temp = STINLmem_Alloc(mem, _size);
    if(temp == NULL)
        return VRT_RESULT_FAILED;
    *addr = temp;
    
    return VRT_RESULT_SUCCESS;
}
VRTresult VRTmem_Free(
    VRTmem *mem,
    void *addr
) {
    {   // STINLmem_CHECK
        const VRTresult res = STINLmem_CHECK(&mem);
        if(res != VRT_RESULT_SUCCESS)
            return res;
    }

    return STINLmem_Free(mem, addr);
}