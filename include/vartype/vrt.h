#ifndef __VRT_H__
#define __VRT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "set.h"


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// TYPES
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
typedef uint16_t VRTpage_t;

typedef struct VRTvar {
    uint32_t   addr;
    VRTpage_t  page;
    uint16_t uCount;
} VRTvar_t;

typedef uint32_t VRTresult_t;
#define VRT_RESULT_SUCCESS              0
#define VRT_RESULT_FAILED               1
#define VRT_RESULT_NO_PTR               2
#define VRT_RESULT_NO_NAME              3
#define VRT_RESULT_LACK_SPACE           4
#define VRT_RESULT_ADDR_NOT_FOUND       5
#define VRT_RESULT_REGISTERED_LOCALLY   6
#define VRT_RESULT_REGISTERED_IN_PARENT 7
#define VRT_RESULT_REGISTERED_IN_CHILD  8


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// FUNCTIONS
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
VRTresult_t VRT_Init(
    const VRTsetMemory_t _mem
);

VRTresult_t VRT_RegisterPage(
    const VRTpage_t _page, const char *const _name,
    VRTpage_t *p
);
VRTresult_t VRT_RegisterVar(
    const VRTpage_t _page, const char *const _name, const uint16_t uCount,
    VRTvar_t *v
);

VRTresult_t VRT_FindPage(
    const VRTpage_t _page, const char *const _name,
    VRTpage_t *p
);
VRTresult_t VRT_FindVar(
    const VRTpage_t _page, const char *const _name,
    VRTvar_t *v
);

VRTresult_t VRT_GetPParent(
    const VRTpage_t _child,
    VRTpage_t *page
);
VRTresult_t VRT_GetPChild(
    const VRTpage_t _parent, const uint64_t _offset,
    VRTpage_t *page
);

VRTresult_t VRT_GetVParent(
    const VRTvar_t _child,
    VRTpage_t *page
);
VRTresult_t VRT_GetVChild(
    const VRTpage_t _parent, const uint64_t _offset,
    VRTvar_t *var
);

VRTresult_t VRT_GetData(
    const VRTvar_t _var,
    void **data
);


#ifdef __cplusplus
};
#endif

#endif