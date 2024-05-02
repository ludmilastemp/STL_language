#include "STL_header.h"

int WriteSignature (const int version, char* str, size_t* ip)
{
    assert (str);
    assert (ip);

    STL_Header header = { };
    header.version = (char)version;

    strcpy (str, header.magic);
    *ip += strlen (header.magic);

    return 0;
}

int CheckFileSignature (const char* const str, size_t* ip, const int version)
{
    assert (str);
    assert (ip);

    STL_Header header = { };
    header.version = (char)version;

    if (strncmp (str, header.magic, strlen (header.magic)) != 0)
    {
        return 1;
    }

    *ip += strlen (header.magic);

    return 0;
}
