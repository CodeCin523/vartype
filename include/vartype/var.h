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

typedef uint8_t vtType_t;
#define VT_TYPE_b1  0
#define VT_TYPE_b32 1
#define VT_TYPE_b64 2

typedef struct vtVar {
    uint32_t addr;
    vtPage_t page;
    vtType_t type;
    uint8_t  _NaN;
} vtVar_t;

typedef uint32_t vtResult;
#define VT_RESULT_NULL      0
// Boolean
#define VT_RESULT_FAIL      0
#define VT_RESULT_SUCCESS   1
// Equality
#define VT_RESULT_GREATER   2
#define VT_RESULT_EQUAL     3
#define VT_RESULT_NOT_EQUAL 0
#define VT_RESULT_SMALLER   4


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// FUNCTIONS
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
vtResult VtInit(uint64_t defMem, uint64_t allocMem);

vtResult VtRegisterPage(
    const char *_n, vtPage_t _p,
    vtPage_t *_r);
vtResult VtRegisterVar(
    const char *_n, vtPage_t _p,
     vtVar_t *_r);

vtResult VtComparePage(
    vtPage_t _p1, vtPage_t _p2);
vtResult VtCompareVar(
    vtVar_t _v1, vtVar_t _v2);

void *VtVarGet(vtVar_t _v);
void VtVarSet(vtVar_t _v, void *_d);

#ifdef __cplusplus
};
#endif
#endif