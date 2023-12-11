#ifndef STL_label_struct_
#define STL_label_struct_

struct Label
{
    const char* name;
    int lenName;
    int address;
    static const int POISON = -1;
};

#endif /* STL_label_struct_ */
