#include <vartype/var.h>
#include <vartype/strerror.h>

#include <stddef.h>


#if defined(unix) || defined(__unix) || defined(__unix__)

#include <sys/mman.h>

vtResult VtInit(uint64_t defMem, uint64_t allocMem) {
    return VT_RESULT_SUCCESS;
}

vtResult VtRegisterPage(
    const char *_n, vtPage_t _p,
    vtPage_t *_r) {
        return VT_RESULT_SUCCESS;
}
vtResult VtRegisterVar(
    const char *_n, vtPage_t _p,
    vtVar_t *_r) {
        return VT_RESULT_SUCCESS;
}

#elif defined(_WIN32)

#endif

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// COMPARE
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
static vtResult compb1(void *_a, void *_b) {
    vtResult _r = VT_RESULT_EQUAL;
    uint8_t a = *(uint8_t*)_a & 1, b = *(uint8_t*)_b & 1;

    if(a > b) _r = VT_RESULT_GREATER;
    else if(a < b) _r = VT_RESULT_SMALLER;
    return _r;
}
static vtResult compb32(void *_a, void *_b) {
    vtResult _r = VT_RESULT_EQUAL;
    uint32_t a = *(uint32_t*)_a, b = *(uint32_t*)_b;

    if(a > b) _r = VT_RESULT_GREATER;
    else if(a < b) _r = VT_RESULT_SMALLER;
    return _r;
}
static vtResult compb64(void *_a, void *_b) {
    vtResult _r = VT_RESULT_EQUAL;
    uint64_t a = *(uint64_t*)_a, b = *(uint64_t*)_b;

    if(a > b) _r = VT_RESULT_GREATER;
    else if(a < b) _r = VT_RESULT_SMALLER;
    return _r;
}

static vtResult(*comp[])(void *_a, void *_b) = {
    &compb1,
    &compb32,
    &compb64
};

vtResult VtComparePage(
    vtPage_t _p1, vtPage_t _p2) {
    return _p1 == _p2? VT_RESULT_EQUAL : VT_RESULT_NOT_EQUAL;
}
vtResult VtCompareVar(
    vtVar_t _v1, vtVar_t _v2) {
    if(_v1.type != _v2.type) return VT_RESULT_NOT_EQUAL;

    return comp[_v1.type](VtVarGet(_v1), VtVarGet(_v2));
}


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// STRING ERROR
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
static const char * const errname[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};
const char *VtStrError(vtResult _r) {
    if(_r > 1) return NULL;
    return errname[_r];
}