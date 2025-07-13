#include <vartype/var.h>

int main() {
    // Initialize the library
    VtInit(1024, 512);

    vtVar_t myVar;
    vtPage_t myPage;

    // Register a memory page
    VtRegisterPage("MyPage", 0, &myPage);

    // Register a variable
    VtRegisterVar("MyVariable", myPage, &myVar);

    // Set a value to the variable
    int value = 42;
    VtVarSet(myVar, &value);

    // Get the value from the variable
    int *retrievedValue = (int *)VtVarGet(myVar);
    printf("Value: %d\n", *retrievedValue);

    return 0;
}