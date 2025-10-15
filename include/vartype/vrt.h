#ifndef __VRT_H__
#define __VRT_H__
#ifdef __cplusplus
extern "C" {
#endif

#define VRT_RESULT_NOFUNC
#include "vrt_result.h"
#define VRT_SIZE_NOFUNC
#include "vrt_size.h"


typedef uint16_t VRTpage;

typedef struct VRTvar {
    uint32_t    addr;
    VRTpage     page;
    VRTsize     size;
    uint8_t     NaN;
} VRTvar;


#ifdef VRT_NOFUNC
#include <vartype/export_vartype.h>

VRTresult VARTYPE_EXPORT VRT_Init();

VRTresult VARTYPE_EXPORT VRT_RegisterPage(
    const VRTpage _parent, const char *const _name,
    VRTpage *page
);
VRTresult VARTYPE_EXPORT VRT_RegisterVar(
    const VRTpage _parent, const char *const _name,
    VRTvar *var
);

VRTresult VARTYPE_EXPORT VRT_FindPage(
    const VRTpage _parent, const char *const _name,
    VRTpage *page
);
VRTresult VARTYPE_EXPORT VRT_FindVar(
    const VRTpage _parent, const char *const _name,
    VRTvar *var
);

VRTresult VARTYPE_EXPORT VRT_GetData(
    const VRTvar _var,
    void **data
);

#undef VRT_NOFUNC
#endif


#ifdef __cplusplus
};
#endif
#endif