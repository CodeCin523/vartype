#ifndef __SET_H__
#define __SET_H__

#include <stdint.h>

#define VRT_SET_STRCMP_CHAR     0b00000000
#define VRT_SET_STRCMP_ADDR     0b00000001
#define VRT_SET_LOOKUP_LINEAR   0b00000000
#define VRT_SET_LOOKUP_BINTREE  0b00000010

#define VRT_SET_STRCMP  ${VRT_SET_STRCMP}
#define VRT_SET_LOOKUP  ${VRT_SET_LOOKUP}

typedef struct VRTsetMemorySize {
    uint64_t mmapSize;  // mmap size
    uint64_t pageSize;  // page size

#if VRT_SET_STRCMP == VRT_SET_STRCMP_CHAR
    uint64_t nameSize;  // name mmap size
#endif
} VRTsetMemory_t;

#endif

