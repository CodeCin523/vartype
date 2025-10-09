cmake_minimum_required(VERSION 3.10)


# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# STATIC VARIABLES
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# set(VRT_NAME VarType)
set(VRT_VERSION_MAJOR 2)
set(VRT_VERSION_MINOR 0)
set(VRT_VERSION_PATCH 0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# SETTINGS v2.0
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
option(VRT_SET_STRCMP "String Saving and Comparing Logic" VRT_SET_STRCMP_ADDR)
if(NOT(
    VRT_SET_STRCMP STREQUAL VRT_SET_STRCMP_ADDR OR
    VRT_SET_STRCMP STREQUAL VRT_SET_STRCMP_CHAR
))
    set(VRT_SET_STRCMP VRT_SET_STRCMP_ADDR)
endif()
option(VRT_SET_LOOKUP "Name Lookup Algorithm" VRT_SET_LOOKUP_LINEAR)
if(NOT(
    VRT_SET_LOOKUP STREQUAL VRT_SET_LOOKUP_LINEAR OR
    VRT_SET_LOOKUP STREQUAL VRT_SET_LOOKUP_BINTREE
))
    set(VRT_SET_LOOKUP VRT_SET_LOOKUP_LINEAR)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/set.cmake.h ${CMAKE_CURRENT_SOURCE_DIR}/include/vartype/set.h)


# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# BUILDING
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
project(
    VarType
    VERSION ${VRT_VERSION_MAJOR}.${VRT_VERSION_MINOR}.${VRT_VERSION_PATCH}
    LANGUAGES C
)

add_library(VarType)
add_library(VarType::VarType ALIAS VarType)
target_sources(
    VarType
    PRIVATE src/vrt.c
)

target_compile_features(
    VarType
    PUBLIC c_std_11
)

target_include_directories(
    VarType
    PRIVATE src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


# if using limited visibility, set CXX_VISIBILILTY_PRESET to "hidden"
include(GenerateExportHeader)
set_property(
    TARGET VarType
    PROPERTY CXX_VISIBILITY_PRESET "hidden"
)
# Hide inlined functions by default, reducing the size of the library
set_property(
    TARGET VarType
    PROPERTY VISIBILITY_INLINES_HIDDEN TRUE
)

# this command generates a header file in the CMAKE_CURRENT_BINARY_DIR which
# sets the visibility attributes according to the compiler settings
generate_export_header(
    VarType
    EXPORT_FILE_NAME
    export/vartype/export_vartype.h
)
# Add CMAKE_CURRENT_BINARY_DIR to the include path so the generated header can
# be found
target_include_directories(
    VarType
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


# include the GNUInstallDirs module to get the canonical install paths defined
include(GNUInstallDirs)

# Install the library and export the CMake targets
install(
    TARGETS VarType
    EXPORT VarTypeTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
# install the public headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install the generated export header
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/export/vartype/export_vartype.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vartype
)


# configure the CMake package file so the libray can be included with find_package() later
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "VarTypeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/VarTypeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VarTypeConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/vartype
)


# install the CMake targets
install(
    EXPORT VarTypeTargets
    FILE VarTypeTargets.cmake
    NAMESPACE VarType::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/vartype
)


# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# TEST
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
if(DEFINED VRT_TEST AND VRT_TEST STREQUAL "ON")
    add_subdirectory(test)
endif()