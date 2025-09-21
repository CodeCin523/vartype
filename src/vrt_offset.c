#include <vartype/vrt_offset.h>

#include "vrt_size.c"

#include <string.h>

#define VRT_OFFSET_INVALID UINT32_MAX


static FORCE_INLINE VRTresult STINLoffset_CHECK(
    VRToffset *offset
) {
    if(offset == NULL)
        return VRT_RESULT_CALL_NO_PTR;
    if(offset->pool == NULL)
        return VRT_RESULT_INVALID_STATE;

    if(offset->count >= offset->size)
        return VRT_RESULT_MEM_NO_SPACE;

    return VRT_RESULT_SUCCESS;
}

static FORCE_INLINE VRTresult STINLoffset_Divide(
    VRToffset *offset,
    VRToffsetAddr i
) {
    if(offset->count >= offset->size)
        return VRT_RESULT_MEM_NO_SPACE;
    
    VRTsize *first = &offset->pool[i];

    memmove(
        &first[1],
        &first[0],
        offset->count - i
    );

    VRTsize newSize = *first - 1;
    first[0] = newSize;
    first[1] = newSize;

    ++offset->count;

    return VRT_RESULT_SUCCESS;
}
static FORCE_INLINE VRTresult STINLoffset_Combine(
    VRToffset *offset,
    VRToffsetAddr i
) {
    if(offset->pool[i] != offset->pool[i+1])
        return VRT_RESULT_INVALID_STATE;
    
    memcpy(
        &offset->pool[i],
        &offset->pool[i+1],
        offset->count - i - 1
    );

    ++offset->pool[i];

    --offset->count;

    return VRT_RESULT_SUCCESS;
}

static FORCE_INLINE VRToffsetAddr STINLoffset_Alloc(
    VRToffset *offset,
    const VRTsize _size
) {
    VRToffsetAddr addr = 0;
    for(int i = 0; i < offset->count; ++i) {
        VRTsize s = offset->pool[i];
        if(s & VRT_SIZE_LOCK || s < _size) {
            addr += STINL_SizeToByte(s);
            continue;
        }
        while(s > _size) {
            if(STINLoffset_Divide(offset, i) != VRT_RESULT_SUCCESS)
                return VRT_OFFSET_INVALID;
            s = offset->pool[i];
        }
        offset->pool[i] |= VRT_SIZE_LOCK;
        return addr;
    }
    return VRT_OFFSET_INVALID;
}
static FORCE_INLINE VRTresult STINLoffset_Free(
    VRToffset *offset,
    VRToffsetAddr addr
) {
    VRToffsetAddr ptr = 0;
    for(int i = 0; i < offset->count; ++i) {
        VRTsize size = offset->pool[i] & VRT_SIZE_DATA;
        if(ptr != addr) {
            ptr += STINL_SizeToByte(size);
            continue;
        }

        offset->pool[i] &= VRT_SIZE_DATA;

        // Compression
        while(1) {
            VRToffsetAddr addrModified
                = STINL_SizeToByte(size);
            VRToffsetAddr alignmentModified
                = ptr % STINL_SizeToByte(size + 1);
            
            if(
                i < offset->count &&
                alignmentModified == 0
            ) {
                if(offset->pool[i] != offset->pool[i+1]) break;
                STINLoffset_Combine(offset, i);
            }
            else if(
                i > 0 &&
                alignmentModified == addrModified
            ) {
                if(offset->pool[i] != offset->pool[i-1]) break;
                STINLoffset_Combine(offset, --i);
                ptr -= addrModified;
            } else break;

            VRTsize size = offset->pool[i] & VRT_SIZE_DATA;
        }

        return VRT_RESULT_SUCCESS;
    }
    return VRT_RESULT_FAILED; //Addr not found
}


VRTresult VRToffset_Alloc(
    VRToffset *offset,
    const VRTsize _size,
    VRToffsetAddr *addr
) {
    if(addr == NULL)
        return VRT_RESULT_CALL_NO_PTR;
    {   // STINLoffset_CHECK
        const VRTresult _res = STINLoffset_CHECK(offset);
        if(_res != VRT_RESULT_SUCCESS && _res != VRT_RESULT_MEM_NO_SPACE)
            return _res;
    }
    {   // STINLsize_CHECK
        const VRTresult _res = STINLsize_CHECK(_size);
        if(_res != VRT_RESULT_SUCCESS)
            return _res;
    }

    VRToffsetAddr temp = STINLoffset_Alloc(offset, _size);
    if(temp == VRT_OFFSET_INVALID)
        return VRT_RESULT_MEM_NO_SPACE;
    *addr = temp;

    return VRT_RESULT_SUCCESS;
}
VRTresult VRToffset_Free(
    VRToffset *offset,
    VRToffsetAddr addr
) {
    {   // STINLoffset_CHECK
        const VRTresult _res = STINLoffset_CHECK(offset);
        if(_res != VRT_RESULT_SUCCESS && _res != VRT_RESULT_MEM_NO_SPACE)
            return _res;
    }

    return STINLoffset_Free(offset, addr);
}

VRTresult VRToffset_Grow(
    VRToffset *offset,
    const VRTsize _size
) {
    {   // STINLoffset_CHECK
        const VRTresult _res = STINLoffset_CHECK(offset);
        if(_res != VRT_RESULT_SUCCESS && _res != VRT_RESULT_MEM_NO_SPACE)
            return _res;
    }
    {   // STINLsize_CHECK
        const VRTresult _res = STINLsize_CHECK(_size);
        if(_res != VRT_RESULT_SUCCESS)
            return _res;
    }

    return VRT_RESULT_FAILED;
}
VRTresult VRToffset_Shrink(
    VRToffset *offset,
    const VRTsize _size
) {
    {   // STINLoffset_CHECK
        const VRTresult _res = STINLoffset_CHECK(offset);
        if(_res != VRT_RESULT_SUCCESS && _res != VRT_RESULT_MEM_NO_SPACE)
            return _res;
    }
    {   // STINLsize_CHECK
        const VRTresult _res = STINLsize_CHECK(_size);
        if(_res != VRT_RESULT_SUCCESS)
            return _res;
    }

    return VRT_RESULT_FAILED;
}