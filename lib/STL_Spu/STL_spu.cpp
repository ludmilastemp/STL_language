#include "STL_spu.h"

const int RAM_SIZE = 1000;

static SPU_DATA_TYPE ram[RAM_SIZE] = { 0 };
static int index_ret = 610;

static int Execute (const char* const str, size_t ip);

static int ExecuteCommand (const char* const str, size_t* ip, SPU_Struct* spu);

static int DecodeArg (const char* const str, size_t* ip, int command,
        SPU_Struct* spu, SPU_DATA_TYPE* arg, int** argPtr);

static int VideoMemory (const int length, const int width);

static int ReadFile (char** str, const char* const fileName, size_t* ip);

// #define verified  || AssertFailed ((__LINE__)
// #define verified  || ({ return 1; })

int SPU (const char* fileName)
{
    assert (fileName);

    char* str = 0;
    size_t ip = 0;

    if (ReadFile (&str, fileName, &ip))
    {
        printf ("ERROR FILE FORMAT");
        return ERROR_FILE_FORMAT;
    }

    Execute (str, ip);

    free (str);

    return 0;
}


static int Execute (const char* const str, size_t ip)
{
    assert (str);

    struct SPU_Struct spu = { };
    SpuStructCtor (&spu);

    while (true)
    {
        spu.err = ExecuteCommand (str, &ip, &spu);

//        StackDump (&spu.stk);

        if (spu.err == -1) break;

        if (spu.err)
        {
            STL_SpuErrPrint (spu.err);
            return spu.err;
        }
    }

    SpuStructDtor (&spu);

    return 0;
}

#define DO_PUSH(var) StackPush (&(spu->stk), (var));
#define DO_POP(var)  StackPop  (&(spu->stk), (var));

#define DEF_CMD(name, opCode, arg, action)                              \
    case opCode:                                                        \
        (action);                                                       \
        break;

#define MAKE_COND_JMP(name, opCode, sign)                               \
    case opCode:                                                        \
        DO_POP (&var2);                                                 \
        DO_POP (&var1);                                                 \
        if (var1 sign var2) *ip = (size_t)arg;                          \
        break;

static int ExecuteCommand (const char* const str, size_t* ip, SPU_Struct* spu)
{
    assert (str);
    assert (ip);
    assert (spu);

    int command = 0; // char
    SPU_DATA_TYPE var1 = 0, var2 = 0;

//    printf ("AAAAAAAAAAAA ip = %d\n", *ip);
    command = str[(*ip)++];

//    printf ("ip = %d\n", *ip - 1);
//    printf ("command = 0x%x\n\n", command);

//    printf ("rax = %d\n", spu->registers[1] / 100);
//    printf ("rbx = %d\n", spu->registers[2] / 100);
//    printf ("\n\n\n");

//    struct Arg_t
//    {
//        SPU_DATA_TYPE value;
//        SPU_DATA_TYPE* location;
//    };

    SPU_DATA_TYPE arg = 0;
    int* argPtr = 0;

    DecodeArg (str, ip, command, spu, &arg, &argPtr);

    // push 10
    // pop rax
    //
    /*
    struct Argument
    {
        ArgType_t type;
        union {
            int reg_id;
            float imm;
        };
    }; */

    //StackType_t GetValue( Argument* arg) { switch (arg->type) {regs[reg_id] imm}}

    // push rax + rbx
    // push rax + 10
    // push 10 + 30

    // + : GetValue(arg1) + GetValue(arg2)

//    printf ("BBBBBBBBBBBB ip = %d\n\n", *ip);

    switch (command & 0x1F) /// 00 01 11 11
    {
        #include "include/STL_commands.h"

        #include "include/STL_jmp.h"

        default:
            printf ("I'm default\n");
            printf ("command = 0x%x\n", (unsigned int)command);
            printf ("        = 0x%x\n", (unsigned int)command & 0x1F);
            printf ("ip = %lu\n", *ip);

            return ERROR_INCORRECT_FUNC;
    }

    return 0;
}
#undef DO_PUSH
#undef DO_POP

#undef DEF_CMD

#undef MAKE_COND_JMP

static int DecodeArg (const char* const str, size_t* ip, int command,
        SPU_Struct* spu, SPU_DATA_TYPE* arg, int** argPtr)
{
    assert (str);
    assert (ip);
    assert (spu);
    assert (arg);
    assert (argPtr);

    if (command & T_ARG_IMM)
    {
        // *arg = *(const SPU_DATA_TYPE*)(str + *ip);
        for (size_t i = 0; i < sizeof (SPU_DATA_TYPE); i++)
        {
            ((char*)arg)[i] = *(const char*)(str + *ip + i);
        }

        *ip += sizeof (SPU_DATA_TYPE);
    }

    if (command & T_ARG_REG)
    {
        int reg = *(str + *ip);

        if (0 <= reg && reg <= nRegisters) ;
        else return ERROR_INCORRECT_VALUE;

        *ip += sizeof (char);

        *arg += spu->registers[reg];
        *argPtr = &(spu->registers[reg]);
    }

    if (command & T_ARG_RAM)
    {
        *argPtr = &(ram[*arg / floatPrecision]);
        *arg = ram[*arg / floatPrecision];
    }

    return 0;
}

static int VideoMemory (const int length, const int width)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < length; ++j)
        {
            if (ram[i * length + j] == 0) printf (".");
            else printf ("*");
        }
        printf ("\n");
    }

    return 0;
}

static int ReadFile (char** str, const char* const fileName, size_t* ip)
{
    assert (str);
    assert (fileName);
    assert (ip);

    struct File file = { .name = fileName };
    STL_Fread (&file);

    *(file.text + file.size - 1) = '\0';

    *str = file.text;

    if (CheckFileSignature (*str, ip, 7)) return ERROR_FILE_FORMAT;

    return 0;
}
