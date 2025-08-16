#ifndef __SET_H__
#define __SET_H__

// Enable/Disable features
#define VRT_ON  1
#define VRT_OFF 0

// Comparison Algorithm Settings
// CMake Variables: VRT_SET_STRCMP_CHAR, VRT_SET_CPYNAME, VRT_SET_STRCMP_ADDR
#define SET_STRCMP_CHAR     ${VRT_SET_STRCMP_CHAR}  // Use strcmp for character comparison
#define SET_CPYNAME         ${VRT_SET_CPYNAME}      // Use custom copy name function
#define SET_STRCMP_ADDR     ${VRT_SET_STRCMP_ADDR}  // Use address comparison

// Lookup/Storage Algorithm Settings
// CMake Variables: VRT_SET_LOOKUP_BINTREE, VRT_SET_LOOKUP_LINEAR
#define SET_LOOKUP_BINTREE  ${VRT_SET_LOOKUP_BINTREE}   // Use binary tree for lookup
#define SET_LOOKUP_LINEAR   ${VRT_SET_LOOKUP_LINEAR}    // Use linear search for lookup

// Thread Safety Settings
// CMake Variable: VRT_SET_THREAD_SAFETY
#define SET_THREAD_SAFETY   ${VRT_SET_THREAD_SAFETY}    // 1 for enabled, 0 for disabled

// Memory Allocation Settings
// CMake Variable: VRT_SET_CALLOC_USAGE
#define SET_CALLOC_USAGE    ${VRT_SET_CALLOC_USAGE}     // 1 for allowed, 0 for disallowed


#endif
