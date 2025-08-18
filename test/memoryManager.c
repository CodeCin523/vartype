#include "../src/var.c"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    vtResult res = 0;
    struct allocdt dt = {
        .dataCount = 4, // it needs the LEFT_MOST bit of the size, not the size. 16 bytes curently
        .dataPool = NULL,
        .offsetLCount = 1,
        .offsetPCount = 1,
        .offsetPool = NULL
    };
    dt.dataPool = malloc(1<<dt.dataCount);
    dt.offsetPool = malloc(dt.offsetPCount);
                    // dt.dataCount should already be the LEFT_MOST bit, so no need ot find its LEFT_MOST bit.
    dt.offsetPool[0] = dt.dataCount & 0b01111111;

    for(int i = 0; i < dt.offsetLCount; ++i) printf("%d ", dt.offsetPool[i]); printf("\n");

    // 1 BYTE
    printf("Start Allocating Memory\n");
    uint32_t p1 = allocdt_Alloc(&dt, 1);  // reserve 1 byte.
    if(p1!=0) {
        printf("FAILED ALLOC OF 1 BYTE AT EXPECTED OFFSET. %d\n", p1);
        return 1;
    }
    printf("Success for alloc of 1 byte at offset: %d.\n", p1);

    for(int i = 0; i < dt.offsetLCount; ++i) printf("%d ", dt.offsetPool[i]); printf("\n");

    // 2 BYTES
    printf("Start Size-Class Offset (SCO lol)\n");
    uint32_t p2 = allocdt_Alloc(&dt, 2);  // reserve 2 bytes.
    
    if(p2!=2) {
        printf("FAILED ALLOC OF 2 BYTES AT EXPECTED OFFSET. %d\n", p2);
        return 1;
    }
    printf("Success for alloc of 2 bytes at offset: %d.\n", p2);

    for(int i = 0; i < dt.offsetLCount; ++i) printf("%d ", dt.offsetPool[i]); printf("\n");

    // FREE 1 BYTE
    printf("Start Deallocating Memory\n");
    if(allocdt_Free(&dt, p1) != VT_RESULT_SUCCESS) {
        printf("FAILED DEALLOC OF 1 BYTE.\n");
        return 1;
    }
    printf("Success for dealloc of 1 byte.\n");

    for(int i = 0; i < dt.offsetLCount; ++i) printf("%d ", dt.offsetPool[i]); printf("\n");

    // FREE 2 BYTE
    printf("Start Deallocating SCO\n");
    if(allocdt_Free(&dt, p2) != VT_RESULT_SUCCESS) {
        printf("FAILED DEALLOC OF 2 BYTES.\n");
        return 1;
    }
    printf("Success for dealloc of 2 bytes.\n");
    
    for(int i = 0; i < dt.offsetLCount; ++i) printf("%d ", dt.offsetPool[i]); printf("\n");

}