#include <vartype/var.h>

#include <vartype/strerror.h>
#include <vartype/mem.h>

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// INTERFACE
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
vtResult VtInit(
    
) {

}

vtResult VtRegisterPage(
    const vtPage_t _page, const char *const _name,
    vtPage_t *p
) {
    
}
vtResult VtRegisterVar(
    const vtPage_t _page, const char *const _name, const uint16_t uCount,
    vtVar_t *v
) {
    
}

vtResult VtFindPage(
    const vtPage_t _page, const char *const _name,
    vtPage_t *p
) {
    
}
vtResult VtFindVar(
    const vtPage_t _page, const char *const _name,
    vtVar_t *v
) {
    
}

vtResult VtGetPParent(
    const vtPage_t _child,
    vtPage_t *page
) {
    
}
vtResult VtGetVParent(
    const vtVar_t _child,
    vtPage_t *page
) {
    
}

vtResult VtGetPChild(
    const vtPage_t _parent, const uint64_t _offset,
    vtPage_t *page
) {
    
}

vtResult VtGetData(
    const vtVar_t _var,
    void **data
) {
    
}

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// STRING ERROR
// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
static const char * const errname[] = {
    NULL,
    "This name is already registered in one of the parents of the requested page",
    "This name is already registered in the requested page",
    "This name is already registered in on of the childrens of the requested page",
};
const char *const VtStrError(vtResult _r) {
    if(_r > 2) return NULL;
    return errname[_r];
}