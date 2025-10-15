#include <vartype/vrt_mem.h>
#include <vartype/vrt_size.h>

#include <stdio.h>

int main() {
    VRTmem mem = {0};
    VRTmem_Init(&mem);

    VRTsize size = 0;
    VRT_ByteToSize(8, &size);

    VRTresult res = 0;

    void * addr1 = 0;
    res = VRTmem_Alloc(&mem, size, &addr1);
    printf("Given address for 8 is %p.\n", addr1);
    if(addr1 == NULL)
        return 1;

    void * addr2 = 0;
    res = VRTmem_Alloc(&mem, size, &addr2);
    printf("Given address for 8 is %p.\n", addr2);
    if(addr2 == NULL)
        return 1;
}