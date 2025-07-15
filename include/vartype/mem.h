#ifndef __MEM_H__
#define __MEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "var.h"


struct vtAlloc{
    uint8_t *_data;
    uint8_t *offsets;
    uint16_t offsetLCount;
    uint16_t offsetPCount;
};


vtResult VtMemInit(struct vtAlloc *_p, uint64_t _s, struct vtAlloc *alloc);
void VtMemExit(struct vtAlloc *_p, struct vtAlloc *alloc);
void *VtMemAlloc(struct vtAlloc *alloc, uint64_t _s);
vtResult VtMemFree(struct vtAlloc *alloc, void *addr);


#ifdef __cplusplus
};
#endif

#endif