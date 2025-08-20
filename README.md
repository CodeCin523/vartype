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
   git clone https://github.com/CodeCin523/vartype.git
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

 Name|Description|
|--|--|
|`VRTpage_t`|Represents a page identifier.|
|`VRTvar_t`|Structure representing a variable with its address, page, and size.|
|`VRTresult`|Represents either an error or a valid state.|

### Functions

|Name|Description|
|--|--|
|<div style="text-align: center;">**Initialization**</div>|
|`VRTresult VRT_Init()`| Initializes the library.|
|<div style="text-align: center;">**Register**</div>|
|`VRTresult VRT_RegisterPage(`<br>`const VRTpage_t _page, const char *const _name,`<br>`VRTpage_t *p)`|Registers a new page.|
|`VRTresult VRT_RegisterVar(`<br>`const VRTpage_t _page, const char *const _name, const uint16_t uCount,`<br>`VRTvar_t *v)` |Registers a variable in a specified page.|
|<div style="text-align: center;">**Find**</div>|
|`VRTresult VRT_FindPage(`<br>`const VRTpage_t _page, const char *const _name,`<br>`VRTpage_t *p)`|Finds a page by its name and retrieves its identifier.|
|`VRTresult VRT_FindVar(`<br>`const VRTpage_t _page, const char *const _name,`<br>`VRTvar_t *v)`|Finds a variable by its name and retrieves its details.|
|`VRTresult VRT_GetPParent(`<br>`const VRTpage_t _child,`<br>`VRTpage_t *page)`|Retrieves the parent page of a specified child page.|
|<div style="text-align: center;">**Hierarchy**</div>|
|`VRTresult VRT_GetPChild(`<br>`const VRTpage_t _parent, const uint64_t _offset,`<br>`VRTpage_t *page)`|Retrieves the child page at a specified offset from a parent page.|
|`VRTresult VRT_GetVParent(`<br>`const VRTvar_t _child`<br>`VRTpage_t *page)`|Retrieves the parent page of a specified variable.|
|`VRTresult VRT_GetVChild(`<br>`const VRTpage_t _parent, const uint64_t _offset,`<br>`VRTvar_t *var)`|Retrieves the var at a specified offset from a parent page.|
|<div style="text-align: center;">**Data**</div>|
|`VRTresult VRT_GetData(`<br>`const VRTvar_t _var,`<br>`void **data)`|Retrieves the data associated with a specified variable.|

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE.txt) file for details.
