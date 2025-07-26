#ifndef __VAR_H__
#define __VAR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// TYPES
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
typedef uint16_t vtPage_t;

typedef struct vtVar {
    uint32_t   addr;
    vtPage_t   page;
    uint16_t uCount;
} vtVar_t;

typedef uint32_t vtResult;
#define VT_RESULT_FAILED                    0
#define VT_RESULT_SUCCESS                   1
#define VT_RESULT_ERR_REGISTERED_IN_PARENT  2
#define VT_RESULT_ERR_REGISTERED_LOCALLY    3
#define VT_RESULT_ERR_REGISTERED_IN_CHILD   4
#define VT_RESULT_MEM_FAILED_MAP            5
#define VT_RESULT_MEM_FAILED_CALLOC         6
#define VT_RESULT_MEM_ADDR_NOT_FOUND        7
#define VT_RESULT_ERR_NO_SPACE              8
#define VT_RESULT_ERR_NO_REF                9
#define VT_RESULT_ERR_NO_NAME               10


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// FUNCTIONS
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
vtResult VtInit(
    
);

vtResult VtRegisterPage(
    const vtPage_t _page, const char *const _name,
    vtPage_t *p
);
vtResult VtRegisterVar(
    const vtPage_t _page, const char *const _name, const uint16_t uCount,
    vtVar_t *v
);

vtResult VtFindPage(
    const vtPage_t _page, const char *const _name,
    vtPage_t *p
);
vtResult VtFindVar(
    const vtPage_t _page, const char *const _name,
    vtVar_t *v
);

vtResult VtGetPParent(
    const vtPage_t _child,
    vtPage_t *page
);
vtResult VtGetVParent(
    const vtVar_t _child,
    vtPage_t *page
);

vtResult VtGetPChild(
    const vtPage_t _parent, const uint64_t _offset,
    vtPage_t *page
);

vtResult VtGetData(
    const vtVar_t _var,
    void **data
);


#ifdef __cplusplus
};
#endif

#endif