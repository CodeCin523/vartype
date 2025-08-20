#include <stdio.h>
#include <vartype/vrt.h>

int main() {
    // Initialize the vartype library
    size_t size = 1024; // Example size for the memory allocator
    VRTresult result = VtInit(size);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to initialize vartype library: %u\n", result);
        return 1;
    }

    // Register a new page using the default page 0
    VRTpage_t page;
    result = VtRegisterPage(0, "MyPage", &page);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to register page: %u\n", result);
        return 1;
    }
    printf("Registered page: %u\n", page);

    // Register a variable in the registered page
    VRTvar_t myVar;
    int myData = 42; // Example data to store
    result = VtRegisterVar(page, "myVariable", sizeof(myData), &myVar);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to register variable: %u\n", result);
        return 1;
    }
    printf("Registered variable: %s\n", "myVariable");

    // Find the registered variable
    VRTvar_t foundVar;
    result = VtFindVar(page, "myVariable", &foundVar);
    if (result != VRT_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to find variable: %u\n", result);
        return 1;
    }
    printf("Found variable: %s\n", "myVariable");

    // Access the data of the registered variable
    void *data;
    result = VtGetData(foundVar, &data);
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