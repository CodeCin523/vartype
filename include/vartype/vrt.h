#ifndef __VRT_H__
#define __VRT_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_NOFUNC
#include "vrt_size.h"
#define VRT_NOFUNC
#include "vrt_result.h"
#undef VRT_NOFUNC

typedef uint16_t VRTpage;

typedef struct VRTvar {
    uint32_t    addr;
    VRTpage     page;
    VRTsize     size;
    uint8_t     NaN;
} VRTvar;

#ifndef VRT_NOFUNC
// VRTresult VRT_Init();

VRTresult VRT_RegisterPage(
    const VRTpage _parent, const char *const _name,
    VRTpage *page
);
VRTresult VRT_RegisterVar(
    const VRTpage _parent, const char *const _name,
    VRTvar *var
);

// VRTresult VRT_GetPParent(
//     const VRTpage _child,
//     VRTpage *page
// );
// VRTresult VRT_GetPChild(
//     const VRTpage _parent, const uint64_t _offset,
//     VRTpage *page
// );

// VRTresult VRT_GetVParent(
//     const VRTvar _child,
//     VRTpage *page
// );
// VRTresult VRT_GetVChild(
//     const VRTpage _parent, const uint64_t _offset,
//     VRTvar *var
// );

VRTresult VRT_FindPage(
    const VRTpage _parent, const char *const _name,
    VRTpage *page
);
VRTresult VRT_FindVar(
    const VRTpage _parent, const char *const _name,
    VRTvar *var
);

VRTresult VRT_GetData(
    const VRTvar _var,
    void **data
);
#else
#undef VRT_NOFUNC
#endif

#ifdef __cplusplus
};
#endif
#endif