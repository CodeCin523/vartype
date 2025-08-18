#include <vartype/var.h>
#include <vartype/strerror.h>

#include "set.h"

#include <stdlib.h>
#include <string.h>


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// MEMORY MANAGER
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
static struct allocdt {
    uint8_t *dataPool;

    uint8_t *offsetPool;

    uint16_t offsetLCount;
    uint16_t offsetPCount;
    uint8_t dataCount;
};

static inline uint8_t FIND_LEFTMOST_BIT(uint64_t x) {
    if (x == 0) return 0; // or handle differently
#ifdef __GNUC__
    return 63 - __builtin_clzll(x); // 0-based indexing
#else
    /* Binary search approach*/
    uint8_t pos = 0;
    if (x & 0xFFFFFFFF00000000ULL) { x >>= 32; pos += 32; }
    if (x & 0xFFFF0000ULL) { x >>= 16; pos += 16; }
    if (x & 0xFF00U) { x >>= 8; pos += 8; }
    if (x & 0xF0U) { x >>= 4; pos += 4; }
    if (x & 0x0CU) { x >>= 2; pos += 2; }
    if (x & 0x02U) { pos += 1; }
    return pos; // 0-based
    
    /* Right-shift while loop (can be change to a for loop if needed)
    int i = 0;
    while(x != 0) {x >>= 1; ++i; }
    return --i;
    */
#endif
}

static inline vtResult allocdt_offcCheck(
    struct allocdt *dt
) {
    if(dt->offsetLCount >= dt->offsetPCount) {
        dt->offsetPCount *= 2;
        uint8_t *temp = NULL;
#if SET_CALLOC_USAGE == VRT_ON
        temp = calloc(dt->offsetPCount, sizeof(*dt->offsetPool));
        if(temp == NULL)
            return VT_RESULT_FAILED;
        if(dt->offsetPool != NULL) {
            memcpy(temp, dt->offsetPool, dt->offsetLCount);
            free(dt->offsetPool);
        }
#else
#endif
        dt->offsetPool = temp;
    }
    return VT_RESULT_SUCCESS;
}
static inline vtResult allocdt_offDivide(
    struct allocdt *dt,
    uint8_t *first
) {
    if(dt->offsetLCount >= dt->offsetPCount)
        return VT_RESULT_ERR_NO_SPACE;

    memcpy(
        &first[1],
        first,
        dt->offsetLCount - (first - dt->offsetPool)
    );
    
    uint8_t offset = *first - 1;
    first[1] = offset;
    *first   = offset;

    ++dt->offsetLCount;
    return VT_RESULT_SUCCESS;
}
static inline vtResult allocdt_offCombine(
    struct allocdt *dt,
    int i
) {
    // if(dt->dataPool[i] != dt-> dataPool[i+1])
    //     return VT_RESULT_FAILED;
    
    memcpy(
        &dt->offsetPool[i],
        &dt->offsetPool[i+1],
        dt->offsetLCount - i - 1
    );

    dt->offsetPool[i] += 1;

    --dt->offsetLCount;
    return VT_RESULT_SUCCESS;
}

static inline uint32_t allocdt_Alloc(
    struct allocdt *dt,
    size_t size
) {
    int offset = FIND_LEFTMOST_BIT(size);

    // Find position in pool.
    uint32_t ptr = 0;
    int i = 0;
    for(; i < dt->offsetLCount; ++i) {
        uint32_t coff = dt->offsetPool[i] & 0b01111111;

        if ((dt->offsetPool[i] & 0b10000000) == 0) {
            if (coff == offset) {    // Perfect Size
                break;
            }
            else if (coff > offset) {  // Too Big
                // Divide until perfect size
                while (coff != offset) {
                    if (allocdt_offcCheck(dt) == VT_RESULT_FAILED)
                        return UINT32_MAX;
                    allocdt_offDivide(dt, &dt->offsetPool[i]);
                    coff = dt->offsetPool[i];
                }
                break;
            }
        }
        ptr += 1 << coff;
    }
    if(i==dt->offsetLCount)
        return UINT32_MAX;
    // LOCK the offset position.
    dt->offsetPool[i] |= 0b10000000;
    return ptr;
}
static inline vtResult allocdt_Free(
    struct allocdt *dt,
    uint32_t addr
) {
    uint32_t ptr = 0;
    for(int i = 0; i < dt->offsetLCount; ++i) {
        uint32_t coff = dt->offsetPool[i] & 0b01111111;

        if(ptr != addr) {
            ptr += 1 << coff;
            continue;
        }

        // UNLOCK the offset position.
        dt->offsetPool[i] ^= 0b10000000;

        // OFFSET COMPRESSION
        while (1/*(dt->offsetPool[i] & 0b10000000) == 0*/) {
            uint32_t coffMod = (1 << coff);
            uint32_t alignCoffMod = ptr % (1 << (coff+1));

            if(i < dt->offsetLCount     && alignCoffMod == 0) {     // RIGHT
                if(dt->offsetPool[i+1] != dt->offsetPool[i]) break;
                allocdt_offCombine(dt, i);
            }else if(i > 0              && alignCoffMod-coffMod == 0) {   // LEFT
                if(dt->offsetPool[i-1] != dt->offsetPool[i]) break;
                allocdt_offCombine(dt, --i);
                ptr -= coffMod;
            } else break;

            coff = dt->offsetPool[i] & 0b01111111;
        }

        return VT_RESULT_SUCCESS;
    }
    return VT_RESULT_MEM_ADDR_NOT_FOUND;
}

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// NAME REGISTER
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
typedef int64_t nameid_t;

static struct nameref {
    const char *n;
    void *ref;
};

static int nameref_comp(const void *a, const void *b) {
    struct nameref *na = (struct nameref *)a, *nb = (struct nameref *)b;

#if SET_STRCMP_CHAR == VRT_ON
    return strcmp(na->n, nb->n);
#else
    int i = na->n - nb->n;
    return i == 0? 0 : i < 0? 1 : -1;
#endif
}


static struct namedt {
    struct nameref *pRef;
    nameid_t namePCount;
    nameid_t nameLCount;
};

static inline vtResult namedt_Push(
    struct namedt *dt,
    const char *name, void *ref
) {
    if(dt->nameLCount == dt->namePCount)
        return VT_RESULT_ERR_NO_SPACE;

    // push back
    dt->pRef[dt->nameLCount].n = name;
    dt->pRef[dt->nameLCount].ref = ref;
    ++dt->nameLCount;

#if SET_LOOKUP_BINTREE == VRT_ON
    // I give up
    qsort(dt->pRef, dt->nameLCount, sizeof(struct nameref), nameref_comp);
#endif


    return VT_RESULT_SUCCESS;
}
static inline vtResult namedt_Pull(
    struct namedt *dt,
    nameid_t id
) {
#if SET_LOOKUP_BINTREE == VRT_ON
    // keep order
    for(id += 1; id < dt->nameLCount; ++id) {
        dt->pRef[id-1] = dt->pRef[id];
    }
    dt->pRef[id-1].n = 0;
    dt->pRef[id-1].ref = 0;
    --dt->nameLCount;
#else
    // place last to i
    --dt->nameLCount;
    dt->pRef[id] = dt->pRef[dt->nameLCount];
    dt->pRef[dt->nameLCount].n = 0;
    dt->pRef[dt->nameLCount].ref = 0;
#endif

    return VT_RESULT_SUCCESS;
}

static inline vtResult namedt_Find(
    struct namedt *dt, 
    const char *name,
    nameid_t *id
) {
    struct nameref new = {
        .n = name,
        .ref = NULL
    };
#if SET_LOOKUP_BINTREE == VRT_ON
    nameid_t left = 0, right = dt->nameLCount, mid = 0;
    while(left < right) {
        mid = left + (right - left) / 2;
        int r = nameref_comp((const void *)&dt->pRef[mid], (const void *)&new);

        if(r == 0) {
            *id = mid;
            return VT_RESULT_SUCCESS;
        }

        if(r < 0)
            left = ++mid;
        else
            right = --mid;
    }
    mid = mid < 0 ? 0 : mid;
    if(nameref_comp((const void *)&dt->pRef[mid], (const void *)&new) == 0) {
        *id = mid;
        return VT_RESULT_SUCCESS;
    }
#else
    for(nameid_t i = 0; i < dt->nameLCount; ++i) {
        if(nameref_comp((const void *)&dt->pRef[i], (const void *)&new) != 0) continue;
        *id = i;
        return VT_RESULT_SUCCESS;
    }
#endif
    
    // would be fun if it returned the position that it should be at
    return VT_RESULT_FAILED;
}


// // ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// // PAGE DATA
// // ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
struct pagedt {
    struct namedt n;

    uint8_t *pData;
    uint64_t dataSize;

    uint8_t *pOffset;
    uint64_t offsetPCount;
    uint64_t offsetLCount;
};

static inline vtResult pagedt_Init(struct pagedt *dt) {
    return VT_RESULT_SUCCESS;
}


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// INTERFACE
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
#if SET_STRCMP_CHAR == VRT_ON
#else
#endif

static struct namedt pageNames;

static struct pagedt *pPageData;
static struct pageHierachy {
    vtPage_t i;     // index
    vtPage_t cc;    // child count
} *pPageHierarchy;
static vtPage_t pageHierarchyLCount;
static vtPage_t pageHierarchyPCount;


vtResult VtInit(
    
) {
    return VT_RESULT_SUCCESS;
}

vtResult VtRegisterPage(
    const vtPage_t _page, const char *const _name,
    vtPage_t *p
) {
    if(p==NULL) return VT_RESULT_ERR_NO_REF;
    if(_name==NULL) return VT_RESULT_ERR_NO_NAME;

    // if cannot add more
    if(pageHierarchyLCount==pageHierarchyPCount)
        return VT_RESULT_ERR_NO_SPACE;
    
    if(namedt_Find(&pageNames, _name, NULL) != VT_RESULT_SUCCESS)
        return VT_RESULT_ERR_REGISTERED_LOCALLY;

    // find parent
    struct pageHierachy *pa = pPageHierarchy;
    for(vtPage_t i = 0; i < pageHierarchyLCount; ++i) {
        if(pPageHierarchy[i].i != _page) continue;
        pa = &pPageHierarchy[i];
        break;
    }
    // find place
    for(vtPage_t i = 0; i < pageHierarchyPCount; ++i) {
        if(pPageData[i].pData != NULL) continue;
        *p = i;
        break;
    }
    pagedt_Init(&pPageData[*p]);

    

    
    return VT_RESULT_SUCCESS;
}
vtResult VtRegisterVar(
    const vtPage_t _page, const char *const _name, const uint16_t uCount,
    vtVar_t *v
) {
    if(v==NULL) return VT_RESULT_ERR_NO_REF;
    if(_name==NULL) return VT_RESULT_ERR_NO_NAME;

    return VT_RESULT_SUCCESS;
}

vtResult VtFindPage(
    const vtPage_t _page, const char *const _name,
    vtPage_t *p
) {
    if(p==NULL) return VT_RESULT_ERR_NO_REF;
    if(_name==NULL) return VT_RESULT_ERR_NO_NAME;

    return VT_RESULT_SUCCESS;
}
vtResult VtFindVar(
    const vtPage_t _page, const char *const _name,
    vtVar_t *v
) {
    if(v==NULL) return VT_RESULT_ERR_NO_REF;
    if(_name==NULL) return VT_RESULT_ERR_NO_NAME;
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
    "Failed to request a memory map",
    "Failed to request heap memory",
    "Address wasn't found",
    "No memory was left",
    "No reference was passed to the function",
    "No name was passed to the function"
};
const char *const VtStrError(vtResult _r) {
    if(_r > 10) return NULL;
    return errname[_r];
}