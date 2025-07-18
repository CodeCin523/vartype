#ifndef __SET_H__
#define __SET_H__


#if ${VRT_SETTING_STR_COMPARE} == char
#define SET_STRCMP_CHAR     ON
#else // ${VRT_SETTING_STR_COMPARE} == addr
#define SET_STRCMP_ADDR     ON
#endif

#if ${VRT_SETTING_LOOKUP} == bintree
#define SET_LOOKUP_BINTREE  ON
#else // ${VRT_SETTING_LOOKUP} == linear
#define SET_LOOKUP_LINEAR   ON
#endif

#endif