#ifndef STL_header_
#define STL_header_

#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "../include/STL_spu_struct_const.h"

struct STL_Header
{
   char magic[5] = {'S', 'T', 'L', ' ', 'v'};
   char version = 0;
   char xax = '\0';
};

int WriteSignature (const int version, char* str, int* ip);

int CheckFileSignature (const char* const str, int* ip, const int version);

#endif /* STL_header_ */
