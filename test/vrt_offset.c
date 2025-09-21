#include <vartype/vrt_size.h>
#include <vartype/vrt_offset.h>

#include <stdio.h>
#include <stdlib.h>

void main() {
    VRToffset offset = {0};

    VRT_ByteToSize(32, &offset.size);
    offset.pool = malloc(32);

    offset.pool[0] = 4;
    offset.count = 1;

    for(int i = 0; i < offset.count; ++i) {
        printf("%d ", offset.pool[i]);
    }
    printf("\n");

    // Allocate 1 BYTE
    printf("Starting memory allocation for 1 byte...\n");
    VRToffsetAddr p1;
    VRToffset_Alloc(&offset, 0 /*Value 1 in VRTsize*/, &p1);  // Reserve 1 byte.
    if(p1 != 0) {
        printf("Allocation failed for 1 byte at expected offset. Actual offset: %d\n", p1);
        return 1;
    }
    printf("Successfully allocated 1 byte at offset: %d.\n", p1);

    // Print the current offset values after allocation
    for(int i = 0; i < offset.count; ++i) {
        printf("%d ", offset.pool[i]);
    }
    printf("\n");

    // Allocate 2 BYTES
    printf("Starting memory allocation for 2 bytes...\n");
    VRToffsetAddr p2;
    VRToffset_Alloc(&offset, 1 /*Value 2 in VRTsize*/, &p2);  // Reserve 1 byte.
    if(p2 != 2) {
        printf("Allocation failed for 2 bytes at expected offset. Actual offset: %d\n", p2);
        return 1;
    }
    printf("Successfully allocated 2 bytes at offset: %d.\n", p2);

    // Print the current offset values after allocation
    for(int i = 0; i < offset.count; ++i) {
        printf("%d ", offset.pool[i]);
    }
    printf("\n");

    // FREE 1 BYTE
    printf("Starting memory deallocation for 1 byte...\n");
    if(VRToffset_Free(&offset, p1) != VRT_RESULT_SUCCESS) {
        printf("Deallocation failed for 1 byte.\n");
        return 1;
    }
    printf("Successfully deallocated 1 byte.\n");

    // Print the current offset values after allocation
    for(int i = 0; i < offset.count; ++i) {
        printf("%d ", offset.pool[i]);
    }
    printf("\n");

    // FREE 2 BYTES
    printf("Starting memory deallocation for 2 bytes...\n");
    if(VRToffset_Free(&offset, p2) != VRT_RESULT_SUCCESS) {
        printf("Deallocation failed for 2 bytes.\n");
        return 1;
    }
    printf("Successfully deallocated 2 bytes.\n");

    // Print the current offset values after allocation
    for(int i = 0; i < offset.count; ++i) {
        printf("%d ", offset.pool[i]);
    }
    printf("\n");

    free(offset.pool);
    return 0;
}