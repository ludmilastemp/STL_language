/* DEF_CMD (name, opCode, nArgs, action) */

/// ����� ������� �� ������ ��������� 0x1A == 64

DEF_CMD (push, 0x01, 1,
    {
    DO_PUSH (arg);
    })

DEF_CMD (pop, 0x02, 1,
    {
    DO_POP (argPtr);
    })

DEF_CMD (add, 0x03, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    DO_PUSH (var1 + var2);
    })

DEF_CMD (sub, 0x04, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    DO_PUSH (var1 - var2);
    })

DEF_CMD (mul, 0x05, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    DO_PUSH (var1 * var2 / floatPrecision);
    })

DEF_CMD (div, 0x06, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    if (var2 == 0) return ERROR_INCORRECT_VALUE;
    DO_PUSH (var1 * floatPrecision / var2);
    })

DEF_CMD (pow, 0x11, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    DO_PUSH ((int) (pow (var1 / floatPrecision, var2 / floatPrecision) * floatPrecision));
    })

DEF_CMD (sqrt, 0x07, 0,
    {
    DO_POP (&var1);
    DO_PUSH ((int)(sqrt(var1 / floatPrecision) * floatPrecision));
    })

DEF_CMD (sin, 0x08, 0,
    {
    DO_POP (&var1);
    DO_PUSH ((int)(sin(var1 / floatPrecision) * floatPrecision));       //
    })

DEF_CMD (cos, 0x09, 0, {
    DO_POP (&var1);
    DO_PUSH ((int)(cos(var1 / floatPrecision) * floatPrecision));
    })

DEF_CMD (ln, 0x10, 0, {
    DO_POP (&var1);
    DO_PUSH ((int)(log(var1 / floatPrecision) * floatPrecision));
    })

DEF_CMD (in, 0x0A, 0,
    {
    float var = 0;
    printf ("\nPlease, enter var: ");
    scanf ("%f", &var);
    DO_PUSH ((int)(var * floatPrecision));
    })

DEF_CMD (out, 0x0B, 0,
    {
    DO_POP (&var1);
    printf ("OUT = %g\n", var1 * 1.0 / floatPrecision);
    })

DEF_CMD (jmp, 0x0C, 1,
    {
    *ip = (size_t)arg;
    })

DEF_CMD (call, 0x0D, 1,
    {
    ram[index_ret] = (int)*ip;
    index_ret += 4;
    *ip = (size_t)arg;
    })

DEF_CMD (ret, 0x0E, 0,
    {
    index_ret -= 4;
    *ip = (size_t)ram[index_ret];
    ram[index_ret] = 0;
    })

DEF_CMD (hlt, 0, 0,
    {
    return -1;
    })

DEF_CMD (print_memory, 0x0F, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    VideoMemory (var1 / floatPrecision, var2 / floatPrecision);
    })
