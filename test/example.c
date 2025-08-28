#include <stdio.h>
#include <vartype/vartype.h>

int main() {
    // Initialize the vartype library
    VRTsetMemory memSetting = {
        .mmapSize = VRT_MB(1),
        .pageSize = VRT_KB(1),
        .pageCount = 16
    };
    VRTresult result = VRT_Init(memSetting);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to initialize vartype library: %u\n", result);
        return 1;
    }

    // Register a new page using the default page 0
    VRTpage page;
    result = VRT_RegisterPage(0, "MyPage", &page);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to register page: %u\n", result);
        return 1;
    }
    printf("Registered page: %u\n", page);

    // Register a variable in the registered page
    VRTvar myVar;
    int myData = 42; // Example data to store
    result = VRT_RegisterVar(page, "myVariable", sizeof(myData), &myVar);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to register variable: %u\n", result);
        return 1;
    }
    printf("Registered variable: %s\n", "myVariable");

    // Find the registered variable
    VRTvar foundVar;
    result = VRT_FindVar(page, "myVariable", &foundVar);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to find variable: %u\n", result);
        return 1;
    }
    printf("Found variable: %s\n", "myVariable");

    // Access the data of the registered variable
    void *data;
    result = VRT_GetData(foundVar, &data);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to get data for variable: %u\n", result);
        return 1;
    }

    // Store the data in the variable
    *((int *)data) = myData;

    // Print the data
    printf("Data in variable: %d\n", *((int *)data));

    return 0;
}