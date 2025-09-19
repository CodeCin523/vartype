#include <vartype/vrt_result.h>

char *const VRT_StrResult(const VRTresult _r) {
    static const char * const MSG[] = {
        "Success",
        "Failed",
        "No pointer passed when calling function"
        "No name passed when calling function"
    };

    if(_r > 4 || _r < 0) return NULL;
    return MSG[_r];
}