# vartype (or shitty database)

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

*work in progress.*

## API Reference v1

*work in progress.*

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE.txt) file for details.
