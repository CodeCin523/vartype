#include "../src/vrt.c"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    VRTresult res = 0;
    struct allocdt dt = {
        .dataCount = 4, // Represents the LEFT_MOST bit of the size, currently set to 16 bytes.
        .dataPool = NULL,
        .offsetLCount = 1,
        .offsetPCount = 1,
        .offsetPool = NULL
    };

    // Allocate memory for the data pool based on dataCount
    dt.dataPool = malloc(1 << dt.dataCount);
    dt.offsetPool = malloc(dt.offsetPCount);
    
    // Set the first offset in the offset pool to the LEFT_MOST bit of dataCount
    dt.offsetPool[0] = dt.dataCount & 0b01111111;

    // Print the current offset values
    for(int i = 0; i < dt.offsetLCount; ++i) {
        printf("%d ", dt.offsetPool[i]);
    }
    printf("\n");

    // Allocate 1 BYTE
    printf("Starting memory allocation for 1 byte...\n");
    uint32_t p1 = allocdt_Alloc(&dt, 1);  // Reserve 1 byte.
    if(p1 != 0) {
        printf("Allocation failed for 1 byte at expected offset. Actual offset: %d\n", p1);
        return 1;
    }
    printf("Successfully allocated 1 byte at offset: %d.\n", p1);

    // Print the current offset values after allocation
    for(int i = 0; i < dt.offsetLCount; ++i) {
        printf("%d ", dt.offsetPool[i]);
    }
    printf("\n");

    // Allocate 2 BYTES
    printf("Starting memory allocation for 2 bytes...\n");
    uint32_t p2 = allocdt_Alloc(&dt, 2);  // Reserve 2 bytes.
    
    if(p2 != 2) {
        printf("Allocation failed for 2 bytes at expected offset. Actual offset: %d\n", p2);
        return 1;
    }
    printf("Successfully allocated 2 bytes at offset: %d.\n", p2);

    // Print the current offset values after allocation
    for(int i = 0; i < dt.offsetLCount; ++i) {
        printf("%d ", dt.offsetPool[i]);
    }
    printf("\n");

    // FREE 1 BYTE
    printf("Starting memory deallocation for 1 byte...\n");
    if(allocdt_Free(&dt, p1) != VRT_RESULT_SUCCESS) {
        printf("Deallocation failed for 1 byte.\n");
        return 1;
    }
    printf("Successfully deallocated 1 byte.\n");

    // Print the current offset values after deallocation
    for(int i = 0; i < dt.offsetLCount; ++i) {
        printf("%d ", dt.offsetPool[i]);
    }
    printf("\n");

    // FREE 2 BYTES
    printf("Starting memory deallocation for 2 bytes...\n");
    if(allocdt_Free(&dt, p2) != VRT_RESULT_SUCCESS) {
        printf("Deallocation failed for 2 bytes.\n");
        return 1;
    }
    printf("Successfully deallocated 2 bytes.\n");
    
    // Print the current offset values after deallocation
    for(int i = 0; i < dt.offsetLCount; ++i) {
        printf("%d ", dt.offsetPool[i]);
    }
    printf("\n");

    // Free allocated memory for dataPool and offsetPool
    free(dt.dataPool);
    free(dt.offsetPool);

    return 0;
}
