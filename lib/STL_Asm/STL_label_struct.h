#ifndef STL_label_struct_
#define STL_label_struct_

#include <stdio.h>

struct Label
{
    const char* name;
    size_t lenName;
    int address;
    static const int POISON = -1;
};

#endif /* STL_label_struct_ */
