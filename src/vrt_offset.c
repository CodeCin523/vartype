#include <vartype/vrt_offset.h>

#include "vrt_size.c"

#include <string.h>


#define fVRToffset_CHECK_SPACE(o) (o.count < o.length)

#define fVRToffset_RETURN(o) {              \
    if(o == NULL)                           \
        return VRT_RESULT_CALL_NO_PTR;      \
    if(o->pool == NULL)                     \
        return VRT_RESULT_INVALID_STATE;    \
}

#define fVRToffset_DIVIDE(o, i, _res) {     \
    if(fVRToffset_CHECK_SPACE((*o))) {      \
        VRTsize *first = &offset->pool[i];  \
                                            \
        memmove(                            \
            &first[1],                      \
            &first[0],                      \
            offset->count - i               \
        );                                  \
                                            \
        VRTsize newSize = *first - 1;       \
        first[0] = newSize;                 \
        first[1] = newSize;                 \
        ++offset->count;                    \
                                            \
        _res = VRT_RESULT_SUCCESS;          \
    } else _res = VRT_RESULT_MEM_NO_SPACE;  \
}

#define fVRToffset_COMBINE(o, i, _res) {    \
    if(o->pool[i] == o->pool[i+1]) {        \
        memcpy(                             \
            &offset->pool[i],               \
            &offset->pool[i+1],             \
            offset->count - i - 1           \
        );                                  \
                                            \
        ++offset->pool[i];                  \
        --offset->count;                    \
                                            \
        _res = VRT_RESULT_SUCCESS;          \
    } else _res = VRT_RESULT_INVALID_STATE; \
}


VRTresult VRToffset_Alloc(
    VRToffset *offset,
    const VRTsize _size,
    size_t *addr
) {
    // EARLY RETURNS
    if(addr == NULL)
        return VRT_RESULT_CALL_NO_PTR;
    fVRToffset_RETURN(offset);
    fVRTsize_RETURN(_size);

    // IMPLEMENTATION
    size_t current = 0;
    for(int i = 0; i < offset->count; ++i) {
        VRTsize size = offset->pool[i];
        if(size & mVRTsize_LOCK || size < _size) {
            fVRTsize_S2B(size, size_t temp);
            current += temp;
            continue;
        }
        while(size > _size) {
            VRTresult _res;
            fVRToffset_DIVIDE(offset, i, _res);
            if(_res != VRT_RESULT_SUCCESS)
                return _res;
            size = offset->pool[i];
        }
        offset->pool[i] |= mVRTsize_LOCK;
        *addr = current;
        return VRT_RESULT_SUCCESS;
    }

    return VRT_RESULT_MEM_NO_SPACE;
}
VRTresult VRToffset_Free(
    VRToffset *offset,
    size_t addr
) {
    // EARLY RETURNS
    fVRToffset_RETURN(offset);

    // IMPLEMENTATION
    size_t current = 0;
    for(int i = 0; i < offset->count; ++i) {
        VRTsize size = offset->pool[i] & mVRTsize_DATA;
        if(current != addr) {
            fVRTsize_S2B(size, size_t temp);
            current += temp;
            continue;
        }
        offset->pool[i] &= mVRTsize_DATA;
        // Compression
        while(1) {
            fVRTsize_S2B(size, size_t addrModified);
            fVRTsize_S2B(size + 1, size_t alignmentModified);
            alignmentModified = current % alignmentModified;
            VRTresult _res;

            if(i < offset->count && alignmentModified == 0) {
                if(offset->pool[i] != offset->pool[i+1]) break;
                fVRToffset_COMBINE(offset, i, _res);
            }
            else if(i > 0 && alignmentModified == addrModified) {
                if(offset->pool[i] != offset->pool[i-1]) break;
                --i;
                current -= addrModified;
                fVRToffset_COMBINE(offset, i, _res);
            } else break;

            if(_res != VRT_RESULT_SUCCESS) break;

            VRTsize size = offset->pool[i] & mVRTsize_DATA;
        }
        return VRT_RESULT_SUCCESS;
    }
    return VRT_RESULT_FAILED; //Addr not found
}

VRTresult VRToffset_Grow(
    VRToffset *offset,
    const VRTsize _size
) {
    // EARLY RETURNS
    fVRToffset_RETURN(offset);
    fVRTsize_RETURN(_size);
    if(offset->count >= offset->length)
        return VRT_RESULT_MEM_NO_SPACE;

    // IMPLEMENTATION
    offset->pool[offset->count++] = _size;

    return VRT_RESULT_SUCCESS;
}
VRTresult VRToffset_Shrink(
    VRToffset *offset,
    const VRTsize _size
) {
    // EARLY RETURNS
    fVRToffset_RETURN(offset);
    fVRTsize_RETURN(_size);
    if(offset->pool[offset->count-1] != _size)
        return VRT_RESULT_INVALID_STATE;

    // IMPLEMENTATION
    offset->pool[--offset->count] = 0;

    return VRT_RESULT_SUCCESS;
}