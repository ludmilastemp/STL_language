/* DEF_CMD (name, opCode, nArgs, action) */

/// номер команды не должен превышать 0x1A == 64

DEF_CMD (PUSH, 0x01, 1,
    {
    DO_PUSH (arg);
    })

DEF_CMD (POP, 0x02, 1,
    {
    DO_POP (argPtr);
    })

DEF_CMD (ADD, 0x03, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    DO_PUSH (var1 + var2);
    })

DEF_CMD (SUB, 0x04, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    DO_PUSH (var1 - var2);
    })

DEF_CMD (MUL, 0x05, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    DO_PUSH (var1 * var2 / floatPrecision);
    })

DEF_CMD (DIV, 0x06, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    if (var2 == 0) return ERROR_INCORRECT_VALUE;
    DO_PUSH (var1 * floatPrecision / var2);
    })

DEF_CMD (SQRT, 0x07, 0,
    {
    DO_POP (&var1);
    DO_PUSH ((int)(sqrt(var1 / floatPrecision) * floatPrecision));
    })

DEF_CMD (SIN, 0x08, 0,
    {
    DO_POP (&var1);
    DO_PUSH ((int)(sin(var1 / floatPrecision) * floatPrecision));       //
    })

DEF_CMD (COS, 0x09, 0, {
    DO_POP (&var1);
    DO_PUSH ((int)(cos(var1 / floatPrecision) * floatPrecision));
    })

DEF_CMD (IN, 0x0A, 0,
    {
    float var = 0;
    printf ("\nPlease, enter var: ");
    scanf ("%f", &var);
    DO_PUSH ((int)(var * floatPrecision));
    })

DEF_CMD (OUT, 0x0B, 0,
    {
    DO_POP (&var1);
    printf ("OUT = %g\n", var1 * 1.0 / floatPrecision);
    })

DEF_CMD (JMP, 0x0C, 1,
    {
    *ip = arg;
    })

DEF_CMD (CALL, 0x0D, 1,
    {
    ram[index_ret] = *ip;
    index_ret += 4;
    *ip = arg;
    })

DEF_CMD (RET, 0x0E, 0,
    {
    index_ret -= 4;
    *ip = ram[index_ret];
    ram[index_ret] = 0;
    })

DEF_CMD (HLT, 0, 0,
    {
    return -1;
    })

DEF_CMD (PRINT_MEMORY, 0x0F, 0,
    {
    DO_POP (&var2);
    DO_POP (&var1);
    VideoMemory (var1 / floatPrecision, var2 / floatPrecision);
    })
