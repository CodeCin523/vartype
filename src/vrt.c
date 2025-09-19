#include <vartype/vrt.h>

#include "vrt_result.c"
#include "vrt_size.c"


// 2-3 main mmap allocator:
//  - general
//      - offset arrays, page arrays
//  - data
//  - names


VRTresult VRT_RegisterPage(
    const VRTpage _parent, const char *const _name,
    VRTpage *page
) {

}
VRTresult VRT_RegisterVar(
    const VRTpage _parent, const char *const _name,
    VRTvar *var
) {

}

VRTresult VRT_FindPage(
    const VRTpage _parent, const char *const _name,
    VRTpage *page
) {

}
VRTresult VRT_FindVar(
    const VRTpage _parent, const char *const _name,
    VRTvar *var
) {

}

VRTresult VRT_GetData(
    const VRTvar _var,
    void **data
) {
    
}