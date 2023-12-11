#include "STL_stack_hash.h"

long long
CountHash (void* data, long long size)
{
    long long hash = 0;

    for (int i = 0; i < size; ++i)
    {
        hash += ((char*)data)[i];
    }

    return hash;
}
