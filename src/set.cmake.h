#ifndef __SET_H__
#define __SET_H__


#define VRT_ON  1
#define VRT_OFF 0

// compare algorithm to use
#define SET_STRCMP_CHAR     ${VRT_SET_STRCMP_CHAR}
#define SET_STRCMP_ADDR     ${VRT_SET_STRCMP_ADDR}

// lookup/storage algorithm to use
#define SET_LOOKUP_BINTREE  ${VRT_SET_LOOKUP_BINTREE}
#define SET_LOOKUP_LINEAR   ${VRT_SET_LOOKUP_LINEAR}


#endif
