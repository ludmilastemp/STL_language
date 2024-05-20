#include "STL_spu.h"

int main (const int argc, const char** argv)
{
    const char* argv1 = 0;

    if (argc == 1) argv1 = "examples/spu.bin";
    else argv1 = argv[1];

    SPU (argv1);

    printf ("\n\nEND!!!\n\n");

    return 0;
}

