#include <stdio.h>
#include <math.h>

extern "C" int Func ();
extern "C" int MySqrt (int);

extern "C" int MyPrint ();

int main ()
{
    printf ("%d\n", Func ());
}

int MySqrt (int x)
{
    return (int)(sqrt((double)x));
}

int MyPrint ()
{
    printf ("HI!\n");

    return 0;
}
