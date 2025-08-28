#ifndef __VRT_H__
#define __VRT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "set.h"

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// TYPES
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
typedef uint16_t VRTpage;

typedef struct VRTvar {
    uint32_t   addr;
    VRTpage  page;
    uint16_t uCount;
} VRTvar;

typedef uint32_t VRTresult;
#define VRT_RESULT_SUCCESS              0
#define VRT_RESULT_FAILED               1
#define VRT_RESULT_NO_PTR               2
#define VRT_RESULT_NO_NAME              3
#define VRT_RESULT_LACK_SPACE           4
#define VRT_RESULT_ADDR_NOT_FOUND       5
#define VRT_RESULT_REGISTERED_LOCALLY   6
#define VRT_RESULT_REGISTERED_IN_PARENT 7
#define VRT_RESULT_REGISTERED_IN_CHILD  8
#define VRT_RESULT_INVALID_PARENT       9

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// FUNCTIONS
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
VRTresult VRT_Init(
    const VRTsetMemory _mem
);

VRTresult VRT_RegisterPage(
    const VRTpage _page, const char *const _name,
    VRTpage *p
);
VRTresult VRT_RegisterVar(
    const VRTpage _page, const char *const _name, const uint16_t uCount,
    VRTvar *v
);

VRTresult VRT_FindPage(
    const VRTpage _page, const char *const _name,
    VRTpage *p
);
VRTresult VRT_FindVar(
    const VRTpage _page, const char *const _name,
    VRTvar *v
);

VRTresult VRT_GetPParent(
    const VRTpage _child,
    VRTpage *page
);
VRTresult VRT_GetPChild(
    const VRTpage _parent, const uint64_t _offset,
    VRTpage *page
);

VRTresult VRT_GetVParent(
    const VRTvar _child,
    VRTpage *page
);
VRTresult VRT_GetVChild(
    const VRTpage _parent, const uint64_t _offset,
    VRTvar *var
);

VRTresult VRT_GetData(
    const VRTvar _var,
    void **data
);


#ifdef __cplusplus
};
#endif
#endif