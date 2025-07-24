#ifndef __SET_H__
#define __SET_H__


#define VRT_ON  1
#define VRT_OFF 0

// compare algorithm to use
#define SET_STRCMP_CHAR     ${VRT_SET_STRCMP_CHAR}
#define SET_CPYNAME         ${VRT_SET_CPYNAME}
#define SET_STRCMP_ADDR     ${VRT_SET_STRCMP_ADDR}

// lookup/storage algorithm to use
#define SET_LOOKUP_BINTREE  ${VRT_SET_LOOKUP_BINTREE}
#define SET_LOOKUP_LINEAR   ${VRT_SET_LOOKUP_LINEAR}

// if thread-safe
#define SET_THREAD_SAFETY   ${VRT_SET_THREAD_SAFETY}
// authorization to use calloc(), malloc(), and free() for a specific data type
#define SET_CALLOC_USAGE    ${VRT_SET_CALLOC_USAGE}

#endif
