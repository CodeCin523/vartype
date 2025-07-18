#include <vartype/var.h>
#include <vartype/strerror.h>

#include <vartype/mem.h>

#include "set.h"

#include <string.h>


// here to facilitate future implementation
int inline __attribute__((always_inline)) inl_strcmp(const char *a, const char *b) {
#if SET_STR_COMPARE == char
    return strcmp(a,b);
#elif SET_STR_COMPARE == addr
    int i = a - b;
    // there is probably better than this that exists, but for the moment, a state machine will work.
    return i == 0? 0 : i < 0? 1 : -1;
#endif
}

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// DATA HANDLER
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
struct pagedt {
    uint8_t *pData;
    uint64_t dataSize;

    uint8_t *pOffset;
    uint64_t offsetPCount;
    uint64_t offsetLCount;

    const char **ppName;
    void **ppNameRef;
    uint64_t namePCount;
    uint64_t nameLCount;
};


static inline vtResult pagedt_FindName(
    struct pagedt *dt, const char *name, int *id) {
    for(uint64_t i = 0; i < dt->nameLCount; ++i) {
        if(inl_strcmp(dt->ppName[i], name) != 0) continue;
        *id = i;
        return VT_RESULT_SUCCESS;
    }
    return VT_RESULT_FAILED;
}
static inline vtResult pagedt_PushName(
    struct pagedt *dt, const char *name) {
    if(dt->nameLCount == dt->namePCount) {
        // reallocate space and copy dt->name over
    }
    dt->ppName[dt->nameLCount++] = name;
    return VT_RESULT_SUCCESS;
}
static inline vtResult pagedt_Pull(
    struct pagedt *dt, int i) {
    // remove
}


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// INTERFACE
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
vtResult VtInit(
    
) {

}

vtResult VtRegisterPage(
    const vtPage_t _page, const char *const _name,
    vtPage_t *p
) {
    return VT_RESULT_SUCCESS;
}
vtResult VtRegisterVar(
    const vtPage_t _page, const char *const _name, const uint16_t uCount,
    vtVar_t *v
) {
    return VT_RESULT_SUCCESS;
}

vtResult VtFindPage(
    const vtPage_t _page, const char *const _name,
    vtPage_t *p
) {
    return VT_RESULT_SUCCESS;
}
vtResult VtFindVar(
    const vtPage_t _page, const char *const _name,
    vtVar_t *v
) {
    return VT_RESULT_SUCCESS;
}

vtResult VtGetPParent(
    const vtPage_t _child,
    vtPage_t *page
) {
    return VT_RESULT_SUCCESS;
}
vtResult VtGetVParent(
    const vtVar_t _child,
    vtPage_t *page
) {
    return VT_RESULT_SUCCESS;
}

vtResult VtGetPChild(
    const vtPage_t _parent, const uint64_t _offset,
    vtPage_t *page
) {
    return VT_RESULT_SUCCESS;
}

vtResult VtGetData(
    const vtVar_t _var,
    void **data
) {
    return VT_RESULT_SUCCESS;
}

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// STRING ERROR
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
static const char * const errname[] = {
    "Generic fail",
    "Generic success",
    "This name is already registered in one of the parents of the requested page",
    "This name is already registered in the requested page",
    "This name is already registered in on of the childrens of the requested page",
};
const char *const VtStrError(vtResult _r) {
    if(_r > 2) return 0;
    return errname[_r];
}