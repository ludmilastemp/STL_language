#include "STL_compile.h"

int main (const int /* argc */, const char** /* argv */)
{
    if (Compile ("examples/1asm.cpp", "examples/1bin.bin")) printf ("ERROE! ERROR! ERROR!");

    printf ("\n\nОК!!!\n\n");

    return 0;
}

