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
# SETTINGS v1.1
# ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
if(NOT DEFINED VRT_SETTING_STRCMP)
    message("VRT_SETTING_STRCMP not defined.")
    set(VRT_SET_STRCMP_CHAR         VRT_OFF)
    set(VRT_SET_STRCMP_ADDR         VRT_ON)
else()
    if(VRT_SETTING_STRCMP STREQUAL "char")
        set(VRT_SET_STRCMP_CHAR     VRT_ON)
        set(VRT_SET_STRCMP_ADDR     VRT_OFF)
    elseif(VRT_SETTING_STRCMP STREQUAL "addr")
        set(VRT_SET_STRCMP_CHAR     VRT_OFF)
        set(VRT_SET_STRCMP_ADDR     VRT_ON)
    else()
        message("VRT_SETTING_STRCMP invalid.")
        set(VRT_SET_STRCMP_CHAR     VRT_OFF)
        set(VRT_SET_STRCMP_ADDR     VRT_ON)
    endif()
endif()
if(NOT DEFINED VRT_SETTING_LOOKUP)
    message("VRT_SETTING_LOOKUP not defined.")
    set(VRT_SET_LOOKUP_BINTREE  VRT_OFF)
    set(VRT_SET_LOOKUP_LINEAR   VRT_ON)
else()
    if(VRT_SETTING_LOOKUP STREQUAL "bintree")
        set(VRT_SET_LOOKUP_BINTREE  VRT_ON)
        set(VRT_SET_LOOKUP_LINEAR   VRT_OFF)
    elseif(VRT_SETTING_LOOKUP STREQUAL "linear")
        set(VRT_SET_LOOKUP_BINTREE  VRT_OFF)
        set(VRT_SET_LOOKUP_LINEAR   VRT_ON)
    else()
        message("VRT_SETTING_LOOKUP invalid.")
        set(VRT_SET_LOOKUP_BINTREE  VRT_OFF)
        set(VRT_SET_LOOKUP_LINEAR   VRT_ON)
    endif()
endif()
if(NOT DEFINED VRT_SETTING_THREAD_SAFETY)
    message("VRT_SETTING_THREAD_SAFETY not defined.")
    set(VRT_SET_THREAD_SAFETY VRT_OFF)
endif()
if(NOT DEFINED VRT_SETTING_CALLOC_USAGE)
    message("VRT_SETTING_CALLOC_USAGE not defined.")
    set(VRT_SET_CALLOC_USAGE VRT_ON)
endif()

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