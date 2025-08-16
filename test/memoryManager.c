#include "../src/var.c"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    vtResult res = 0;
    uint32_t p = 0;
    struct allocdt dt = {
        .dataCount = 4, // 4 bytes * 8 = 64 bits INVALID, it needs the LEFT_MOST bit of the size, not the size
        .dataPool = NULL,
        .offsetLCount = 1,
        .offsetPCount = 2,
        .offsetPool = NULL
    };
    dt.dataPool = malloc(1<<dt.dataCount);
    dt.offsetPool = malloc(dt.offsetPCount);
                    // dt.dataCount should already be the LEFT_MOST bit, so no need ot find its LEFT_MOST bit.
    dt.offsetPool[0] = dt.dataCount & 0b01111111;

    printf("Start Allocating Memory\n");
    p = allocdt_Alloc(&dt, 1);  // reserve 1 byte.
    for (int i = 0; i < dt.offsetLCount; ++i) printf("[%d] %d\n", i, dt.offsetPool[i]);
    if(p!=0) {
        printf("FAILED ALLOC OF 1 BYTE AT EXPECTED OFFSET. %d\n", p);
        return 1;
    }
    printf("Success for alloc of 1 byte at offset: %d.\n", p);

    printf("Start Size-Class Offset (SCO lol)\n");
    p = allocdt_Alloc(&dt, 2);  // reserve 2 bytes.
    
    if(p!=2) {
        printf("FAILED ALLOC OF 2 BYTES AT EXPECTED OFFSET. %d\n", p);
        return 1;
    }
    printf("Success for alloc of 2 bytes at offset: %d.\n", p);
}