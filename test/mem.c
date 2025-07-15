#include <stdio.h>
#include <vartype/mem.h>

int main() {
    struct vtAlloc p_alloc; // Parent allocator
    struct vtAlloc c_alloc; // Child allocator

    // Test initialization of the parent allocator
    if (VtMemInit(NULL, 1024 * 1024, &p_alloc) != VT_RESULT_SUCCESS) {
        printf("VtMemInit parent result bad\n");
        return 1;
    } else {
        printf("VtMemInit parent successful\n");
    }

    // Test initialization of the child allocator
    if (VtMemInit(&p_alloc, 1024, &c_alloc) != VT_RESULT_SUCCESS) {
        printf("VtMemInit child result bad\n");
        return 1;
    } else {
        printf("VtMemInit child successful\n");
    }

    // Test memory allocation
    uint64_t alloc_size = 256; // Size for allocation
    void *ptr = VtMemAlloc(&c_alloc, alloc_size);
    if (ptr == NULL) {
        printf("VtMemAlloc failed\n");
        return 1;
    } else {
        printf("VtMemAlloc successful, allocated pointer: %p\n", ptr);
    }

    // Test freeing memory
    if (VtMemFree(&c_alloc, ptr) != VT_RESULT_SUCCESS) {
        printf("VtMemFree failed\n");
        return 1;
    } else {
        printf("VtMemFree successful\n");
    }

    // Clean up (if necessary)
    VtMemExit(&p_alloc, &c_alloc);
    VtMemExit(NULL, &p_alloc);
    printf("VtMemExit called for both allocators\n");

    return 0;
}