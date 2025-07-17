#include <vartype/mem.h>

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

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
        uint8_t *temp = calloc(alloc->offsetPCount, sizeof(*alloc->pOffset));
        memcpy(temp, alloc->pOffset, alloc->offsetLCount);
        free(alloc->pOffset);
        alloc->pOffset = temp;
    }

    // move everything right
    uint8_t offset = alloc->pOffset[i] - 1;
    memcpy(&alloc->pOffset[i+1], &alloc->pOffset[i], alloc->offsetLCount - i);
    // place division
    alloc->pOffset[i+1] = offset;
    alloc->pOffset[i] = offset;

    ++alloc->offsetLCount;
}
static inline int VtMem_combine(struct vtAlloc *alloc, int i) {
    if(alloc->pOffset[i] != alloc->pOffset[i+1]) return 0;
    
    uint8_t offset = alloc->pOffset[i] + 1;
    
    memcpy(&alloc->pOffset[i], &alloc->pOffset[i+1], alloc->offsetLCount - i - 1);
    
    alloc->pOffset[i] = offset;
    --alloc->offsetLCount;
    return 1;
}

vtResult VtMemInit(struct vtAlloc *_p, uint64_t _s, struct vtAlloc *alloc) {
    int offset = VtMem_find_leftmost_bit(_s);
    _s = 1 << offset;
    
    // ALLOCATE POOL
    if(_p == NULL) {
#if defined(unix) || defined(__unix) || defined(__unix__)
        alloc->pData = mmap(NULL, _s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#elif defined(_WIN64)
        alloc->pData = VirtualAlloc(NULL, _s, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#endif
    } else {
        alloc->pData = VtMemAlloc(_p, _s);
    }
    if(alloc->pData == NULL) return VT_RESULT_MEM_FAILED_MAP;

    // ALLOCATE OFFSETS
    alloc->offsetPCount = 128;
    alloc->pOffset = calloc(alloc->offsetPCount, sizeof(*alloc->pOffset));
    if(alloc->pOffset == NULL) {
        VtMemExit(_p, alloc);
        return VT_RESULT_MEM_FAILED_CALLOC;
    }
    alloc->offsetLCount = 1;
    alloc->pOffset[0] = offset;

    alloc->dataSize = _s;
    
    return VT_RESULT_SUCCESS;
}
void VtMemExit(struct vtAlloc *_p, struct vtAlloc *alloc) {
    // ALLOCATE OFFSETS
    if(alloc->pOffset != NULL) free(alloc->pOffset);

    if(alloc->pData == NULL) return;

    // ALLOCATE POOL
    if(_p == NULL) {
#if defined(unix) || defined(__unix) || defined(__unix__)
        munmap(alloc->pData, alloc->dataSize);
#elif defined(_WIN64)
        VirtualFree(alloc->pData, 0, MEM_RELEASE);
#endif
    } else {
        VtMemFree(_p, alloc->pData);
    }
}

void *VtMemAlloc(struct vtAlloc *alloc, uint64_t _s) {
    int offset = VtMem_find_leftmost_bit(_s);
    
    // IF OFFSETS IS HAS REACH ITS LIMIT
    if(alloc->offsetLCount >= alloc->offsetPCount) {
        alloc->offsetPCount *= 2;
        uint8_t *temp = calloc(alloc->offsetPCount, sizeof(*alloc->pOffset));
        if(alloc->pOffset != NULL) {
            memcpy(temp, alloc->pOffset, alloc->offsetLCount);
            free(alloc->pOffset);
        }
        alloc->pOffset = temp;
    }

    // Find position in pool.
    uint8_t *ptr = alloc->pData;
    for(int i = 0; i < alloc->offsetLCount; ++i) {
        int coff = alloc->pOffset[i] & 0b01111111;

        if(!(alloc->pOffset[i] & 0b10000000)) {
            ptr += coff;
            continue;
        }
        if(coff == offset) {        // If perfect size
            break;
        } else if(coff > offset) {  // If too big
            // Divide until perfect size
            while(coff != offset) {
                VtMem_divide(alloc, i);
                coff = alloc->pOffset[i] & 0b01111111;
            }
            // Lock it
            alloc->pOffset[i] |= 0b1000000;
            break;
        }
    }

    return ptr;
}
vtResult VtMemFree(struct vtAlloc *alloc, void *addr) {
    // Find address
    uint8_t *ptr = alloc->pData;
    for(int i = 0; i < alloc->offsetLCount; ++i) {
        int coff = alloc->pOffset[i] & 0b01111111;

        if(ptr != addr) {
            ptr += 1 << coff;
            continue;
        }

        // problem if i - 1 is the same size and unlock, it will never be combined
        alloc->pOffset[i] ^= 0b1000000;
        while(VtMem_combine(alloc, i));
        return VT_RESULT_SUCCESS;
    }
    return VT_RESULT_MEM_ADDR_NOT_FOUND;
}