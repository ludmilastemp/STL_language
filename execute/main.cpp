#include <stdio.h>

extern "C" int Func ();

int main ()
{
    printf ("%d\n", Func ());
}