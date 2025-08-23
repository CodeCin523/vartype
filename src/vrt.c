#include <vartype/vrt.h>
#include <vartype/strresult.h>

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

static inline VRTresult_t allocdt_offcCheck(
    struct allocdt *dt
) {
    if(dt->offsetLCount >= dt->offsetPCount) {
        dt->offsetPCount *= 2;
        uint8_t *temp = NULL;
#if 0==0
        temp = calloc(dt->offsetPCount, sizeof(*dt->offsetPool));
        if(temp == NULL)
            return VRT_RESULT_FAILED;
        if(dt->offsetPool != NULL) {
            memcpy(temp, dt->offsetPool, dt->offsetLCount);
            free(dt->offsetPool);
        }
#else
#endif
        dt->offsetPool = temp;
    }
    return VRT_RESULT_SUCCESS;
}
static inline VRTresult_t allocdt_offDivide(
    struct allocdt *dt,
    uint8_t *first
) {
    if(dt->offsetLCount >= dt->offsetPCount)
        return VRT_RESULT_LACK_SPACE;

    memcpy(
        &first[1],
        first,
        dt->offsetLCount - (first - dt->offsetPool)
    );
    
    uint8_t offset = *first - 1;
    first[1] = offset;
    *first   = offset;

    ++dt->offsetLCount;
    return VRT_RESULT_SUCCESS;
}
static inline VRTresult_t allocdt_offCombine(
    struct allocdt *dt,
    int i
) {
    if(dt->dataPool[i] != dt-> dataPool[i+1])
        return VRT_RESULT_FAILED;
    
    memcpy(
        &dt->offsetPool[i],
        &dt->offsetPool[i+1],
        dt->offsetLCount - i - 1
    );

    dt->offsetPool[i] += 1;

    --dt->offsetLCount;
    return VRT_RESULT_SUCCESS;
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
                    if (allocdt_offcCheck(dt) != VRT_RESULT_SUCCESS)
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
static inline VRTresult_t allocdt_Free(
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

        return VRT_RESULT_SUCCESS;
    }
    return VRT_RESULT_ADDR_NOT_FOUND;
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

#if VRT_SET_STRCMP == VRT_SET_STRCMP_CHAR
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

static inline VRTresult_t namedt_Push(
    struct namedt *dt,
    const char *name, void *ref
) {
    if(dt->nameLCount == dt->namePCount)
        return VRT_RESULT_LACK_SPACE;

    // push back
    dt->pRef[dt->nameLCount].n = name;
    dt->pRef[dt->nameLCount].ref = ref;
    ++dt->nameLCount;

#if VRT_SET_LOOKUP == VRT_SET_LOOKUP_BINTREE
    // I give up
    qsort(dt->pRef, dt->nameLCount, sizeof(struct nameref), nameref_comp);
#endif


    return VRT_RESULT_SUCCESS;
}
static inline VRTresult_t namedt_Pull(
    struct namedt *dt,
    nameid_t id
) {
#if VRT_SET_LOOKUP == VRT_SET_LOOKUP_BINTREE
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

    return VRT_RESULT_SUCCESS;
}

static inline VRTresult_t namedt_Find(
    struct namedt *dt, 
    const char *name,
    nameid_t *id
) {
    struct nameref new = {
        .n = name,
        .ref = NULL
    };
#if VRT_SET_LOOKUP == VRT_SET_LOOKUP_BINTREE
    nameid_t left = 0, right = dt->nameLCount, mid = 0;
    while(left < right) {
        mid = left + (right - left) / 2;
        int r = nameref_comp((const void *)&dt->pRef[mid], (const void *)&new);

        if(r == 0) {
            *id = mid;
            return VRT_RESULT_SUCCESS;
        }

        if(r < 0)
            left = ++mid;
        else
            right = --mid;
    }
    mid = mid < 0 ? 0 : mid;
    if(nameref_comp((const void *)&dt->pRef[mid], (const void *)&new) == 0) {
        *id = mid;
        return VRT_RESULT_SUCCESS;
    }
#else
    for(nameid_t i = 0; i < dt->nameLCount; ++i) {
        if(nameref_comp((const void *)&dt->pRef[i], (const void *)&new) != 0) continue;
        *id = i;
        return VRT_RESULT_SUCCESS;
    }
#endif
    
    // would be fun if it returned the position that it should be at
    return VRT_RESULT_FAILED;
}

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// PAGE DATA
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
struct pagedt {
    struct allocdt  alloc;
    struct namedt   name;
};

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// INTERFACE
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
#ifdef _WIN64
#include <sysinfoapi.h>
#include <memoryapi.h>

#define VRT_OS_WIN
#elif defined(__linux__)
#include <mmap.h>

#define VRT_OS_LIN
#endif


static VRTsetMemory_t setMemory;

static struct namedt pageNames;
static struct pagedt *pPageData;

static struct pageHierachy {
    VRTpage_t i;     // index
    VRTpage_t cc;    // child count
} *pPageHierarchy;
static VRTpage_t pageHierarchyLCount;
static VRTpage_t pageHierarchyPCount;


VRTresult_t VRT_Init(
    const VRTsetMemory_t _mem
) {
    setMemory = _mem;
#ifdef VRT_OS_WIN
    SYSTEM_INFO info = {0};
    GetSystemInfo(&info);

    // REALLY TEMPORARY, NOTHING HERE SHOULD BE FINAL
    setMemory.mmapSize = _mem.mmapSize - _mem.mmapSize % info.dwPageSize;
    setMemory.pageSize = 1<<FIND_LEFTMOST_BIT(_mem.pageSize);
#elif defined(VRT_OS_LIN)
#endif

    pageNames.namePCount = setMemory.pageCount;
    pageNames.nameLCount = 0;
    pageNames.pRef = (struct nameref *) calloc(setMemory.pageCount, sizeof(struct nameref));
    if(pageNames.pRef == NULL)
        return VRT_RESULT_LACK_SPACE;

    pageHierarchyPCount = setMemory.pageCount;
    pageHierarchyLCount = 0;
    pPageHierarchy = (struct pageHierachy *) calloc(setMemory.pageCount, sizeof(struct pageHierachy));
    if(pPageHierarchy == NULL)
        return VRT_RESULT_LACK_SPACE;

    return VRT_RESULT_SUCCESS;
}

VRTresult_t VRT_RegisterPage(
    const VRTpage_t _page, const char *const _name,
    VRTpage_t *p
) {
    if(p==NULL) return VRT_RESULT_NO_PTR;
    if(_name==NULL) return VRT_RESULT_NO_NAME;

    // Does the name already exist?
    // Add 

    return VRT_RESULT_SUCCESS;
}
VRTresult_t VRT_RegisterVar(
    const VRTpage_t _page, const char *const _name, const uint16_t uCount,
    VRTvar_t *v
) {
    if(v==NULL) return VRT_RESULT_NO_PTR;
    if(_name==NULL) return VRT_RESULT_NO_NAME;

    return VRT_RESULT_SUCCESS;
}

VRTresult_t VRT_FindPage(
    const VRTpage_t _page, const char *const _name,
    VRTpage_t *p
) {
    if(p==NULL) return VRT_RESULT_NO_PTR;
    if(_name==NULL) return VRT_RESULT_NO_NAME;

    return VRT_RESULT_SUCCESS;
}
VRTresult_t VRT_FindVar(
    const VRTpage_t _page, const char *const _name,
    VRTvar_t *v
) {
    if(v==NULL) return VRT_RESULT_NO_PTR;
    if(_name==NULL) return VRT_RESULT_NO_NAME;
    return VRT_RESULT_SUCCESS;
}

VRTresult_t VRT_GetPParent(
    const VRTpage_t _child,
    VRTpage_t *page
) {
    return VRT_RESULT_SUCCESS;
}
VRTresult_t VRT_GetPChild(
    const VRTpage_t _parent, const uint64_t _offset,
    VRTpage_t *page
) {
    return VRT_RESULT_SUCCESS;
}

VRTresult_t VRT_GetVParent(
    const VRTvar_t _child,
    VRTpage_t *page
) {
    return VRT_RESULT_SUCCESS;
}
VRTresult_t VRT_GetVChild(
    const VRTpage_t _parent, const uint64_t _offset,
    VRTvar_t *var
) {
    return VRT_RESULT_SUCCESS;
}

VRTresult_t VRT_GetData(
    const VRTvar_t _var,
    void **data
) {
    return VRT_RESULT_SUCCESS;
}

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// STRING ERROR
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
static const char * const errname[] = {
    "Success",                             // VRT_RESULT_SUCCESS
    "Failed",                              // VRT_RESULT_FAILED
    "No pointer available",                // VRT_RESULT_NO_PTR
    "No name provided",                    // VRT_RESULT_NO_NAME
    "Lack of space",                       // VRT_RESULT_LACK_SPACE
    "Address not found",                   // VRT_RESULT_ADDR_NOT_FOUND
    "Registered locally",                  // VRT_RESULT_REGISTERED_LOCALLY
    "Registered in parent",                // VRT_RESULT_REGISTERED_IN_PARENT
    "Registered in child"                  // VRT_RESULT_REGISTERED_IN_CHILD
};
const char *const VRT_StrResult(VRTresult_t _r) {
    if(_r > 9) return NULL;
    return errname[_r];
}