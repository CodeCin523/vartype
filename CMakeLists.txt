cmake_minimum_required(VERSION 3.10)


# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# STATIC VARIABLES
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
set(VRT_NAME vartype)
set(VRT_VERSION_MAJOR 1)
set(VRT_VERSION_MINOR 0)
set(VRT_VERSION_PATCH 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# SETTINGS v0.1
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
set(VRT_SET_STRCMP_LIST "addr" "char")
if(DEFINED VRT_SET_STRCMP)
else()
    set(VRT_SET_STRCMP 0)
endif()


set(VRT_SET_LOOKUP_LIST "linear" "bintree")
if(DEFINED VRT_SET_LOOKUP)
else()
    set(VRT_SET_LOOKUP 0)
endif()

# set(VRT_SETTING_SIMD OFF)
# set(VRT_SETTING_OPTIMIZER OFF)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/set.cmake.h ${CMAKE_CURRENT_SOURCE_DIR}/src/set.h)


# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# BUILDING
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
project(
    ${VRT_NAME}
    VERSION ${VRT_VERSION_MAJOR}.${VRT_VERSION_MINOR}.${VRT_VERSION_PATCH}
    LANGUAGES C
)

add_library(
    ${VRT_NAME} STATIC
    src/var.c
    src/mem.c
)
set_property(TARGET ${VRT_NAME} PROPERTY LINKER_LANGUAGE C)
target_include_directories(
    ${VRT_NAME}
    PRIVATE src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
# TEST
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
if(DEFINED VRT_TEST AND VRT_TEST STREQUAL "ON")
    add_subdirectory(test)
endif()