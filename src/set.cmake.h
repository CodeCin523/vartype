#ifndef __SET_H__
#define __SET_H__

#include <stdint.h>

#define VRT_KB(x) (x * 1024)
#define VRT_MB(x) (x * 1024 * 1024)
#define VRT_GB(x) (x * 1024 * 1024 * 1024)

#define VRT_SET_STRCMP_CHAR     0b00000000
#define VRT_SET_STRCMP_ADDR     0b00000001
#define VRT_SET_LOOKUP_LINEAR   0b00000000
#define VRT_SET_LOOKUP_BINTREE  0b00000010

#define VRT_SET_STRCMP  ${VRT_SET_STRCMP}
#define VRT_SET_LOOKUP  ${VRT_SET_LOOKUP}

typedef struct VRTsetMemory {
    uint64_t mmapSize;  // mmap size
    uint64_t pageSize;  // page size
    uint64_t pageCount; // page count

#if VRT_SET_STRCMP == VRT_SET_STRCMP_CHAR
    uint64_t nameSize;  // name mmap size
#endif
} VRTsetMemory;

#endif

