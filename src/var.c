#include <vartype/var.h>
#include <vartype/strerror.h>

#include "set.h"

#include <string.h>


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// UTILS
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
#if defined(_MSC_VER)
    #define FORCE_INLINE __forceinline
#elif defined(__GNUC__)
    #define FORCE_INLINE __attribute__((always_inline)) inline
#else
    #define FORCE_INLINE inline
#endif


int FORCE_INLINE inl_strcmp(const char *a, const char *b) {
#if SET_STRCMP_CHAR == VRT_ON
    return strcmp(a,b);
#else
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
    struct pagedt *dt,
    const char *name, uint64_t *id
) {
#if SET_LOOKUP_BINTREE == VRT_ON
    uint64_t left = 0, right = dt->nameLCount, mid;
    while(left != right) {
        mid = left + (right - left) / 2;
        int r = inl_strcmp(dt->ppName[mid], name);

        if(r == 0) {
            *id = mid;
            return VT_RESULT_SUCCESS;
        }

        if(r < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }
    if(inl_strcmp(dt->ppName[left], name) == 0) {
        *id = mid;
        return VT_RESULT_SUCCESS;
    }
#else
    for(uint64_t i = 0; i < dt->nameLCount; ++i) {
        if(inl_strcmp(dt->ppName[i], name) != 0) continue;
        *id = i;
        return VT_RESULT_SUCCESS;
    }
#endif
     // would be fun if it returned the position that it should be at
    return VT_RESULT_FAILED;
}
static inline vtResult pagedt_PushName(
    struct pagedt *dt,
    const char *name, void *ref
) {
    if(dt->nameLCount == dt->namePCount) {
        // reallocate space and copy dt->name over
        return VT_RESULT_ERR_NO_SPACE;
    }

#if SET_LOOKUP_BINTREE == VRT_ON
    // find where it needs to go
    uint64_t left = 0, right = dt->nameLCount, mid;
    while(left != right) {
        mid = left + (right - left) / 2;
        int r = inl_strcmp(dt->ppName[mid], name);

        // should not happen if interface implementation is good
        // if(r == 0) {
        //     return VT_RESULT_ERR_REGISTERED_LOCALLY;
        // }

        if(r < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }
    // push everything after it one right
    for(right = dt->nameLCount; right >= mid; --right) {
        dt->ppName      [right] = dt->ppName    [right-1];
        dt->ppNameRef   [right] = dt->ppNameRef [right-1];
    }
    // place the value
    dt->ppName      [mid] = name;
    dt->ppNameRef   [mid] = ref;
#else
    // push back
    dt->ppName      [dt->nameLCount] = name;
    dt->ppNameRef   [dt->nameLCount] = ref;
    ++dt->nameLCount;
#endif
    return VT_RESULT_SUCCESS;
}
static inline vtResult pagedt_PullName(
    struct pagedt *dt,
    uint64_t i
) {
#if SET_LOOKUP_BINTREE == VRT_ON
    // keep order
    for(i += 1; i < dt->nameLCount; ++i) {
        dt->ppName      [i-1] = dt->ppName      [i];
        dt->ppNameRef   [i-1] = dt->ppNameRef   [i];
    }
    dt->ppName          [i-1] = 0;
    dt->ppNameRef       [i-1] = 0;
    --dt->nameLCount;
#else
    // place last to i
    --dt->nameLCount;
    dt->ppName      [i] = dt->ppName    [dt->nameLCount];
    dt->ppNameRef   [i] = dt->ppNameRef [dt->nameLCount];
    dt->ppName    [dt->nameLCount] = 0;
    dt->ppNameRef [dt->nameLCount] = 0;
#endif
    return VT_RESULT_SUCCESS;
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