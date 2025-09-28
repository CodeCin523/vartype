#include <vartype/vrt.h>

#include "vrt_result.c"
// #include "vrt_mem.c"
#include "vrt_offset.c"

#include <stdlib.h>


// static VRTmem memUtility;
// static VRTmem memData;


VRTresult VRT_Init() {
    // vPM_utility.offset.pool = malloc(VRT_VPM_POOL_JUMP);
    // vPM_utility.offset.size = VRT_VPM_POOL_JUMP;
    // vPM_utility.offset.pool[0] = STINL_ByteToSize(VRT_VPM_PAGE_SIZE);
    // vPM_utility.offset.count = 1;
// 
    // vPM_data.offset.pool = malloc(VRT_VPM_POOL_JUMP);
    // vPM_data.offset.size = VRT_VPM_POOL_JUMP;
    // vPM_data.offset.pool[0] = STINL_ByteToSize(VRT_VPM_PAGE_SIZE);
    // vPM_data.offset.count = 1;
}

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