#include <stdio.h>
#include <math.h>

extern "C" int Func ();
extern "C" int MySqrt (int);

// extern "C" void MyPrint ();

int main ()
{
    printf ("%d\n", Func ());
}

int MySqrt (int x)
{
    return (int)(sqrt(double(x)));
}

// void MyPrint ()
// {
//     printf ("HI!\n");
// }