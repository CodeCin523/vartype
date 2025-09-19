#ifndef __VRT_RESULT_H__
#define __VRT_RESULT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint32_t VRTresult;
#define VRT_RESULT_SUCCESS      0
#define VRT_RESULT_FAILED       1
#define VRT_RESULT_CALL_NO_PTR  2
#define VRT_RESULT_CALL_NO_NAME 3

#ifndef VRT_NOFUNC
char *const VRT_StrResult(const VRTresult _r);
#else
#undef VRT_NOFUNC
#endif

#ifdef __cplusplus
};
#endif
#endif