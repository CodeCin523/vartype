#ifndef __VRT_RESULT_H__
#define __VRT_RESULT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


typedef uint32_t VRTresult;
#define VRT_RESULT_SUCCESS          0
#define VRT_RESULT_FAILED           1
#define VRT_RESULT_CALL_NO_PTR      2
#define VRT_RESULT_CALL_NO_NAME     3
// #define VRT_RESULT_MEM_NO_ADDR   5

#define VRT_RESULT_NO_SPACE_IARRAY  4
#define VRT_RESULT_NO_SPACE_IALLOC  5
#define VRT_RESULT_NO_SPACE_CALLOC  6
#define VRT_RESULT_NO_SPACE_VIRMEM  7

#define VRT_RESULT_INVALID_STATE    8


#ifdef VRT_RESULT_NOFUNC
#include <vartype/export_vartype.h>

const char * VARTYPE_EXPORT VRT_StrResult(const VRTresult _r);

#undef VRT_RESULT_NOFUNC
#endif


#ifdef __cplusplus
};
#endif
#endif