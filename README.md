# vartype

`vartype` is a lightweight memory allocator library that allows you to access variables from anywhere in your application, as long as you have the correct name and page. This library is designed to simplify variable management in complex systems, enabling efficient memory usage and easy variable retrieval.

**Note:** The current implementation of `vartype` is more of a proof of concept than a fully optimized solution. It may not perform as well as other approaches and is intended primarily for educational purposes and experimentation.

## Features

- Register and manage variables across different pages.
- Access variables using their names and associated pages.
- Retrieve parent and child relationships between pages and variables.
- Flexible and easy-to-use API.

## Installation

To build and install the `vartype` library, follow these steps:

1. Clone the repository:

   ```bash
   git clone https://github.com/KurabeTV/vartype.git
   cd vartype
   ```

2. Run CMake to configure the project. If you want to generate the test executable, use the `-DVRT_TEST=ON` option:

   ```bash
   cmake -S . -B _build -DVRT_TEST=ON
   ```

3. Go in the newly created directory, build the library and the test executable if specified:

   ```bash
   cd _build
   make
   ```

   Alternatively, if you are using msbuild, you can compile the project with:

   ```bash
   cd _build
   msbuild vartype.sln
   ```

## Including in Your Project

If you want to include the `vartype` library in your own CMake project, you can do so by using `add_subdirectory` and linking against `${VRT_NAME}`. Here’s a simple example:

1. In your project's `CMakeLists.txt`, add the following lines:

   ```cmake
   add_subdirectory(path/to/vartype)  # Adjust the path to where the vartype library is located
   ```

2. Link against the `vartype` library in your target:

   ```cmake
   target_link_libraries(your_target PRIVATE ${VRT_NAME})
   ```

This will allow you to use the `vartype` library in your project without needing to manually manage its source files.

## API Reference

### Types

- `vtPage_t`: Represents a page identifier.
- `vtVar_t`: Structure representing a variable with its address, page, and count.

### Functions

- `vtResult VtInit(size_t size)`: Initializes the library with a specified size.
- `vtResult VtRegisterPage(const vtPage_t _page, const char *const _name, vtPage_t *p)`: Registers a new page.
- `vtResult VtRegisterVar(const vtPage_t _page, const char *const _name, const uint16_t uCount, vtVar_t *v)`: Registers a variable in a specified page.
- `vtResult VtFindPage(const vtPage_t _page, const char *const _name, vtPage_t *p)`: Finds a registered page by name.
- `vtResult VtFindVar(const vtPage_t _page, const char *const _name, vtVar_t *v)`: Finds a registered variable by name.
- `vtResult VtGetData(const vtVar_t _var, void **data)`: Retrieves the data associated with a variable.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE.txt) file for details.
