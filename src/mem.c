#include <vartype/mem.h>
#include <stdio.h>

#if defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/mman.h>
#elif defined(_WIN64)
#include <windows.h>
#endif


static inline uint8_t VtMem_find_leftmost_bit(uint64_t x) {
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

static inline void VtMem_divide(struct vtAlloc *alloc, int i) {
    // If no more place in offsets
    if(alloc->offsetLCount >= alloc->offsetPCount) {
        alloc->offsetPCount *= 2;
        uint8_t *temp = calloc(alloc->offsetPCount, sizeof(*alloc->offsets));
        memcpy(temp, alloc->offsets, alloc->offsetLCount);
        free(alloc->offsets);
        alloc->offsets = temp;
    }

    // move everything right
    uint8_t offset = alloc->offsets[i] - 1;
    memcpy(&alloc->offsets[i+1], &alloc->offsets[i], alloc->offsetLCount - i);
    // place division
    alloc->offsets[i+1] = offset;
    alloc->offsets[i] = offset;

    ++alloc->offsetLCount;
}
static inline int VtMem_combine(struct vtAlloc *alloc, int i) {
    if(alloc->offsets[i] != alloc->offsets[i+1]) return 0;
    
    uint8_t offset = alloc->offsets[i] + 1;
    
    memcpy(&alloc->offsets[i], &alloc->offsets[i+1], alloc->offsetLCount - i - 1);
    
    alloc->offsets[i] = offset;
    --alloc->offsetLCount;
    return 1;
}

vtResult VtMemInit(struct vtAlloc *_p, uint64_t _s, struct vtAlloc *alloc) {
    int offset = VtMem_find_leftmost_bit(_s);
    _s = 1 << offset;
    
    // ALLOCATE POOL
    if(_p == NULL) {
#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(_WIN64)
        alloc->_data = VirtualAlloc(NULL, _s, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#endif
    } else {
        alloc->_data = VtMemAlloc(_p, _s);
    }
    if(alloc->_data == NULL) return VT_RESULT_MEM_FAILED_MAP;

    // ALLOCATE OFFSETS
    alloc->offsetPCount = 128;
    alloc->offsets = calloc(alloc->offsetPCount, sizeof(*alloc->offsets));
    if(alloc->offsets == NULL) {
        VtMemExit(_p, alloc);
        return VT_RESULT_MEM_FAILED_CALLOC;
    }
    alloc->offsetLCount = 1;
    alloc->offsets[0] = offset;
    
    return VT_RESULT_SUCCESS;
}
void VtMemExit(struct vtAlloc *_p, struct vtAlloc *alloc) {
    // ALLOCATE OFFSETS
    if(alloc->offsets != NULL) free(alloc->offsets);

    if(alloc->_data == NULL) return;

    // ALLOCATE POOL
    if(_p == NULL) {
#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(_WIN64)
         VirtualFree(alloc->_data, 0, MEM_RELEASE);
#endif
    } else {
        VtMemFree(_p, alloc->_data);
    }
}

void *VtMemAlloc(struct vtAlloc *alloc, uint64_t _s) {
    int offset = VtMem_find_leftmost_bit(_s);
    
    // IF OFFSETS IS HAS REACH ITS LIMIT
    if(alloc->offsetLCount >= alloc->offsetPCount) {
        alloc->offsetPCount *= 2;
        uint8_t *temp = calloc(alloc->offsetPCount, sizeof(*alloc->offsets));
        if(alloc->offsets != NULL) {
            memcpy(temp, alloc->offsets, alloc->offsetLCount);
            free(alloc->offsets);
        }
        alloc->offsets = temp;
    }

    // Find position in pool.
    uint8_t *ptr = alloc->_data;
    for(int i = 0; i < alloc->offsetLCount; ++i) {
        int coff = alloc->offsets[i] & 0b01111111;

        if(!alloc->offsets[i] & 0b10000000) {
            ptr += coff;
            continue;
        }
        if(coff == offset) {        // If perfect size
            break;
        } else if(coff > offset) {  // If too big
            // Divide until perfect size
            while(coff != offset) {
                VtMem_divide(alloc, i);
                coff = alloc->offsets[i] & 0b01111111;
            }
            // Lock it
            alloc->offsets[i] |= 0b1000000;
            break;
        }
    }

    return ptr;
}
vtResult VtMemFree(struct vtAlloc *alloc, void *addr) {
    // Find address
    uint8_t *ptr = alloc->_data;
    for(int i = 0; i < alloc->offsetLCount; ++i) {
        int coff = alloc->offsets[i] & 0b01111111;

        if(ptr != addr) {
            ptr += coff;
            continue;
        }

        // problem if i - 1 is the same size and unlock, it will never be combined
        alloc->offsets[i] ^= 0b1000000;
        while(VtMem_combine(alloc, i));
        return VT_RESULT_SUCCESS;
    }
    return VT_RESULT_MEM_ADDR_NOT_FOUND;
}