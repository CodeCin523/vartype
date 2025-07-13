# vartype

`vartype` is a C library designed for managing variable types and memory pages efficiently. It provides a simple API for registering and comparing variables, as well as getting and setting their values. This library is particularly useful in systems programming and embedded applications where memory management is critical.

## Features

- Define and manage variable types with different sizes.
- Register and compare memory pages.
- Get and set variable values easily.
- Lightweight and efficient design.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [API Reference](#api-reference)

## Installation

To build and install the `vartype` library, follow these steps:

1. Clone the repository:

   ```bash
   git clone https://github.com/KurabeTV/vartype.git
   cd vartype
   ```

2. Create a build directory and navigate into it:

   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:

   ```bash
   cmake ..
   ```

4. Build the library:

   ```bash
   make
   ```

5. Optionally, install the library:

   ```bash
   sudo make install
   ```

## Usage

To use the `vartype` library in your project, include the header file and link against the library. Below is a simple example demonstrating how to initialize the library, register a variable, and set its value.

### Example

```c
#include "vartype.h"

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
```

## API Reference

### Types

- `vtPage_t`: Represents a memory page.
- `vtType_t`: Represents the type of a variable (e.g., `VT_TYPE_b1`, `VT_TYPE_b32`, `VT_TYPE_b64`).
- `vtVar_t`: Structure representing a variable.

### Functions

- `vtResult VtInit(uint64_t defMem, uint64_t allocMem)`: Initializes the library with default and allocated memory sizes.
- `vtResult VtRegisterPage(const char *_n, vtPage_t _p, vtPage_t *_r)`: Registers a memory page.
- `vtResult VtRegisterVar(const char *_n, vtPage_t _p, vtVar_t *_r)`: Registers a variable.
- `vtResult VtComparePage(vtPage_t _p1, vtPage_t _p2)`: Compares two memory pages.
- `vtResult VtCompareVar(vtVar_t _v1, vtVar_t _v2)`: Compares two variables.
- `void *VtVarGet(vtVar_t _v)`: Gets the value of a variable.
- `void VtVarSet(vtVar_t _v, void *_d)`: Sets the value of a variable.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.