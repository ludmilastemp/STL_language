#include "STL_compile.h"

int main (const int argc, const char** argv)
{
    const char* argv1 = 0;
    const char* argv2 = 0;

    if (argc <= 1) argv1 = "examples/1asm.cpp";
    else argv1 = argv[1];

    if (argc <= 2) argv2 = "examples/1bin.bin";
    else argv2 = argv[2];

    if (Compile (argv1, argv2)) printf ("ERROE! ERROR! ERROR!");

    printf ("\n\nОК!!!\n\n");

    return 0;
}

