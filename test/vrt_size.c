#include "vartype/vrt_result.h"
#include <vartype/vrt_size.h>

#include <stdio.h>

void main() {
    uint64_t byte = 0;
    VRTsize size = 0;

    // CHECK FOR ERRORS
    printf("VRT_ByteToSize Check for the pointer? %d\n", VRT_ByteToSize(1, NULL) != VRT_RESULT_SUCCESS);
    printf("VRT_SizeToByte Check for the pointer? %d\n", VRT_SizeToByte(1, NULL) != VRT_RESULT_SUCCESS);

    printf("VRT_ByteToSize Check for 0 bytes? %d\n", VRT_ByteToSize(0, &size) != VRT_RESULT_SUCCESS);
    printf("VRT_SizeToByte Check for lock? %d\n", VRT_SizeToByte(0b10000000, &byte) != VRT_RESULT_SUCCESS);

    // CHECK FOR CORRECT
    // start at one, since a size of 0 byte is invalid
    for(byte = 1; byte <= 16; ++byte) {
        size = 0;
        if(VRT_ByteToSize(byte, &size) != VRT_RESULT_SUCCESS)
            printf("Unexpected error !!\n");
        else
            printf("The VRTsize of %d is %d.\n", byte, size);
    }

    for(size = 0; size <= 16; ++size) {
        byte = 0;
        if(VRT_SizeToByte(size, &byte) != VRT_RESULT_SUCCESS)
            printf("Unexpected error !!\n");

        printf("The byte of %d is %d.\n", size, byte);
    }
}