#include "back-end.h"

#define $printf(...)
#define $fprintf(...)

/// enum
const char PREFIX_REX = 0x48;
const char OPCODE_MOV_RM_TO_R = 0x89;
const char OPCODE_MOV_R_TO_RM = 0x8b;
// const char OPCODE_PUSH = 0x50;
// const char OPCODE_POP  = 0x50;

// const char REG_RAX = 0x00;
// const char REG_RBX = 0x00;
// const char REG_RCX = 0x00;
// const char REG_RDX = 0x00;

// #define PRINT_FUNCTION() do { printf("I'm in %s\n", __PRETTY_FUNCTION__); } while (0)

static int CompileMultipleOperations (BackEndCtx* ctx);
static int CompileOperation          (BackEndCtx* ctx);
static int CompileActualArguments    (BackEndCtx* ctx);
static int CompileFormalArguments    (BackEndCtx* ctx);
static int CompileFunction           (BackEndCtx* ctx);
static int CompileFuncReturn         (BackEndCtx* ctx);
// static int CompilePrintf             (BackEndCtx* ctx);
static int CompileAssign             (BackEndCtx* ctx);
static int CompileConditionOp        (BackEndCtx* ctx);
static int CompileCreate             (BackEndCtx* ctx);
static int CompileExpression         (BackEndCtx* ctx);

int CompileProgram (BackEndCtx* ctx)
{
    assert (ctx);

    ctx->tempVar    = 1;
    ctx->nVarInFunc = 0;
    NodeBinTree* oldNode = ctx->node;

    $fprintf (ctx->fp, "\nglobal Func");
    $fprintf (ctx->fp, "\nextern MySqrt");
    $fprintf (ctx->fp, "\n\nsection .data");
    $fprintf (ctx->fp, "\n\nsection .text");

    /**
     *  Compile main
     */
    $fprintf (ctx->fp, "\n; начало main");
    $fprintf (ctx->fp, "\n\nFunc:");

    /// push rbp
    *(ctx->buffer) = 0x55; ctx->buffer++; /// orcode + reg
    
    $fprintf (ctx->fp, "\n\n\t\tmov     rbp, rsp");
    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = OPCODE_MOV_RM_TO_R; ctx->buffer++; /// opcode
    *(ctx->buffer) = 0xe5; ctx->buffer++; /// modrm

    ctx->node = oldNode->left;
    CompileMultipleOperations (ctx);
        
    /// pop rbp
    *(ctx->buffer) = 0x5d; ctx->buffer++; /// orcode + reg

    $fprintf (ctx->fp, "\n\n\t\tret");
    *(ctx->buffer) = 0xc3; ctx->buffer++; /// opcode
    $fprintf (ctx->fp, "\n\n; конец main");

    /**
     *  Compile functions
     */
    oldNode = oldNode->right;
    while (oldNode != nullptr)
    {
        ctx->nVarInFunc = 0;

        $printf ("\n\n\n\nFUNCTION N = %d\n\n\n\n", oldNode->data->function);

        ctx->funcIp[oldNode->data->function] = ctx->buffer;

        $fprintf (ctx->fp, "\n\n\n\n\n; начало функции");
        $fprintf (ctx->fp, "\n\nFunc_%d_:", oldNode->data->function);

        $fprintf (ctx->fp, "\n\n\t\tmov     rbp, rsp\n\n");
        *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
        *(ctx->buffer) = OPCODE_MOV_RM_TO_R; ctx->buffer++; /// opcode
        *(ctx->buffer) = 0xe5; ctx->buffer++; /// modrm

    /**
     *  Compile arguments
     */

        $fprintf (ctx->fp, "\n\t\tpop     rcx");
        $fprintf (ctx->fp, "\n\t\tpop     rdx");
        *(ctx->buffer) = 0x59; ctx->buffer++; /// orcode + reg
        *(ctx->buffer) = 0x5A; ctx->buffer++; /// orcode + reg

        $fprintf (ctx->fp, "\n\n\n;-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\n");
  
        ctx->node = oldNode->left->right;
        CompileFormalArguments (ctx);

        $fprintf (ctx->fp, "\n\n;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-\n");

        $fprintf (ctx->fp, "\n\t\tpush    rdx");
        $fprintf (ctx->fp, "\n\t\tpush    rcx");
        *(ctx->buffer) = 0x52; ctx->buffer++; /// orcode + reg
        *(ctx->buffer) = 0x51; ctx->buffer++; /// orcode + reg

        $fprintf (ctx->fp, "\n\n");

    /**
     *  Compile function
     */
        ctx->node = oldNode->left->left;
        CompileMultipleOperations (ctx);

        $fprintf (ctx->fp, "\n\n\t\tret");
        *(ctx->buffer) = 0xc3; ctx->buffer++; /// opcode
        $fprintf (ctx->fp, "\n\n; конец функции");

        oldNode = oldNode->right;
    }

    for (int i = 0; i < ctx->fixupSize; i++)
    {
        int value = ctx->funcIp[ctx->fixupNFunc[i]] - ctx->fixup[i] - 4;
        char* fix = ctx->fixup[i];
        *(fix) = ((char*)&value)[0]; fix++;
        *(fix) = ((char*)&value)[1]; fix++;
        *(fix) = ((char*)&value)[2]; fix++;
        *(fix) = ((char*)&value)[3]; fix++;
    }

    return 0;
}

static int CompileMultipleOperations (BackEndCtx* ctx)
{
    assert (ctx);

    NodeBinTree* oldNode = ctx->node;

    while (oldNode != nullptr)
    {
        if (oldNode->data->opCode != END_OPERATION)
        {
            $printf ("ERROR in CompileMultipleOperations!!!\n\n");
            return ERROR_IN_CompileMultipleOperations;
        }

        ctx->node = oldNode->left;
        CompileOperation (ctx);

        $fprintf (ctx->fp, "\n");

        oldNode = oldNode->right;
    }

    return 0;
}

static int CompileOperation (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("\n\nI in CompileOperation\n");

    for (int i = 0; i < 16; i++)
    {
        // *(ctx->buffer) = 0xAA; ctx->buffer++; /// 
    }

    $fprintf (ctx->fp, "\n\n\n;-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\n");
    // fprintf (ctx->fp, "\n;nVar = %d\n\n", ctx->nVarInFunc);
    if (
        CompileFunction    (ctx) &&
        CompileFuncReturn  (ctx) &&
        // CompilePrintf      (ctx) &&
        CompileConditionOp (ctx) &&
        CompileAssign      (ctx) &&
        CompileCreate      (ctx) && 
        1)

    {
        printf ("\nERROR in CompileOperation!!!\n\n");
        fprintf (ctx->fp, "\n\nERROR in CompileOperation!!!\n\n");

        printf ("\n\ttype     = %d\
                 \n\tvalue    = %d\
                 \n\topCode   = %d\
                 \n\tvariable = %d\
                 \n\tfunction = %d",
                 ctx->node->data->type,
                 ctx->node->data->value,
                 ctx->node->data->opCode,
                 ctx->node->data->variable,
                 ctx->node->data->function);

        return ERROR_IN_CompileOperation;
    }
    
    for (int i = 0; i < 16; i++)
    {
        // *(ctx->buffer) = 0xBB; ctx->buffer++; /// 
    }

    $fprintf (ctx->fp, "\n\n\n;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-\n");
    
    $printf ("I end CompileOperation\n\n\n");

    return 0;
}

static int CompileFunction (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("I in CompileFunction\n");

    if (ctx->node->data->type != NodeBinTreeData::T_FUNCTION)
        return ERROR_IN_CompileFunction;

    int nFunc = ctx->node->data->function;

    $fprintf (ctx->fp, "\n\n\t\t; сохраняем локальные перменные");
    $fprintf (ctx->fp, "\n\t\tsub     rsp, %d\n\n", (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize);  

    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = 0x83; ctx->buffer++; /// opcode // imm8
    // *(ctx->buffer) = 0x81; ctx->buffer++; /// opcode // imm32

    char modrm = 0;
    modrm |= 0xC0; // 11 *** *** mod 
    modrm |= 0x28; // ** 101 *** reg  (5 - const param)
    modrm |= 0x04; // ** *** 100 rm 
    *(ctx->buffer) = modrm; ctx->buffer++; /// modrm
    
    int value = (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize;
    *(ctx->buffer) = ((char*)&value)[0]; ctx->buffer++; /// value
    // *(ctx->buffer) = ((char*)&value)[1]; ctx->buffer++; /// value
    // *(ctx->buffer) = ((char*)&value)[2]; ctx->buffer++; /// value
    // *(ctx->buffer) = ((char*)&value)[3]; ctx->buffer++; /// value

    $fprintf (ctx->fp, "\n\t\t; передаем аргументы");
    CompileActualArguments (ctx);

    $fprintf (ctx->fp, "\n\t\t; Вызов функции");
    $fprintf (ctx->fp, "\n\t\tpush    rbp");
    *(ctx->buffer) = 0x55; ctx->buffer++; /// orcode + reg

    $fprintf (ctx->fp, "\n\n\t\tcall Func_%d_\n", nFunc); 
    *(ctx->buffer) = 0xe8; ctx->buffer++; /// orcode 

    ctx->fixup[ctx->fixupSize]      = ctx->buffer;
    ctx->fixupNFunc[ctx->fixupSize] = nFunc;
    ctx->fixupSize++;

    ctx->buffer += 4;

    $fprintf (ctx->fp, "\n\t\tpop     rbp\n\n");
    *(ctx->buffer) = 0x5d; ctx->buffer++; /// orcode + reg

    $fprintf (ctx->fp, "\n\n\t\t; возвращаем rsp");

    $fprintf (ctx->fp, "\n\t\tadd     rsp, %d\n\n", (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize);
    
    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = 0x83; ctx->buffer++; /// opcode // imm8

    modrm = 0;
    modrm |= 0xC0; // 11 *** *** mod 
    modrm |= 0x00; // ** 000 *** reg (0 - const param)
    modrm |= 0x04; // ** *** 100 rm 
    *(ctx->buffer) = modrm; ctx->buffer++; /// modrm
    
    value = (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize;
    *(ctx->buffer) = ((char*)&value)[0]; ctx->buffer++; /// value
    // *(ctx->buffer) = ((char*)&value)[1]; ctx->buffer++; /// value
    // *(ctx->buffer) = ((char*)&value)[2]; ctx->buffer++; /// value
    // *(ctx->buffer) = ((char*)&value)[3]; ctx->buffer++; /// value

    return 0;
}

static int CompileFuncReturn (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("I in CompileFuncReturn\n");

    if (ctx->node->data->opCode != FUNC_RETURN)
        return ERROR_IN_CompileFuncReturn;

    if (ctx->node->right != nullptr)
    {
        ctx->node = ctx->node->right;
        CompileExpression (ctx);

        $fprintf (ctx->fp, "\n\t\t; значениe return");
        $fprintf (ctx->fp, "\n\t\txor     rax, rax");
        *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
        *(ctx->buffer) = 0x31; ctx->buffer++; /// opcode
        *(ctx->buffer) = 0xc0; ctx->buffer++; /// modrm

        $fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                 (ctx->tempVar + ctx->nVarInFunc) * variableSize);
        *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
        *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
        *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
        int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
        *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

        ctx->tempVar--;
    }

    $fprintf (ctx->fp, "\n\n\t\tret");
    *(ctx->buffer) = 0xc3; ctx->buffer++; /// opcode

    return 0;
}

static int CompileFormalArguments (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("I in CompileFormalArguments\n");

    if (ctx->node               == nullptr ||
        ctx->node->data->opCode != END_OPERATION)
        return ERROR_IN_CompileArguments;

    NodeBinTree* oldNode = ctx->node;

    if (ctx->node->right != nullptr)
    {
        ctx->node = ctx->node->right;

        CompileFormalArguments (ctx);
    }

    ctx->node = oldNode->left;

    if (ctx->node == nullptr ||
        ctx->node->data->type != NodeBinTreeData::T_VARIABLE)
        return ERROR_IN_CompileArguments;

    $fprintf (ctx->fp, "\n\t\tpop     rax");
    *(ctx->buffer) = 0x58; ctx->buffer++; /// opcode

    $fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax", 
        (ctx->node->data->variable + 1) * variableSize);
    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = OPCODE_MOV_RM_TO_R; ctx->buffer++; /// opcode
    *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
    int disp = -1 * (ctx->node->data->variable + 1) * variableSize;
    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

    if (ctx->node->data->variable >= ctx->nVarInFunc)
        ctx->nVarInFunc = ctx->node->data->variable;

    return 0;
}

static int CompileActualArguments (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("I in CompileActualArguments\n"); 

    NodeBinTree* oldNode = nullptr;

    while (ctx->node->right != nullptr)
    {
        ctx->node = ctx->node->right;

        if (ctx->node->data->opCode != END_OPERATION)
            return ERROR_IN_CompileFunction;

        oldNode   = ctx->node;
        ctx->node = ctx->node->left;

        if (CompileExpression (ctx))
            return ERROR_IN_CompileFunction;

        $fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                    (ctx->tempVar + ctx->nVarInFunc) * variableSize);
        *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
        *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
        *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
        int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
        *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

        $fprintf (ctx->fp, "\n\t\tpush    rax");
        *(ctx->buffer) = 0x50; ctx->buffer++; /// opcode
        ctx->tempVar--;

        ctx->node = oldNode;

        $fprintf (ctx->fp, "\n");
    }

    return 0;
}

static int CompileAssign (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("I in CompileAssign\n");

    if (ctx->node      ->data->opCode   != ASSING ||
        ctx->node->left->data->variable == NodeBinTreeData::VARIABLE_POISON)
        return ERROR_IN_CompileAssign;

    NodeBinTree* oldNode = ctx->node;

    $fprintf (ctx->fp, "\n\n\t\t; Assign ");
    // for (size_t i = 0; i < ctx->var->data[oldNode->left->data->variable].len; i++)
    //     fprintf (ctx->fp, "%c", ctx->var->data[oldNode->left->data->variable].name[i]);

    $printf ("I in CompileAssign before right\n");

    ctx->node = oldNode->right;
    CompileExpression (ctx);

    $printf ("I in CompileAssign after right\n");

    $fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                (ctx->tempVar + ctx->nVarInFunc) * variableSize);
    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
    *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
    int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

    ctx->tempVar--;

    $fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax", 
             (oldNode->left->data->variable + 1) * variableSize);
    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = OPCODE_MOV_RM_TO_R; ctx->buffer++; /// opcode
    *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
    disp = -1 * (oldNode->left->data->variable + 1) * variableSize;
    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

    if (oldNode->left->data->variable >= ctx->nVarInFunc)
        ctx->nVarInFunc = oldNode->left->data->variable;

    ctx->node = oldNode;

    $printf ("I end CompileAssign\n");
    return 0;
}

static int CompileCreate (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("I in CompileCreate\n");

    if (ctx->node->data->opCode != T_INT)
        return ERROR_IN_CompileCreate;

    return 0;
}

// static int CompilePrintf (BackEndCtx* ctx)
// {
//     assert (ctx);
//     $printf ("I in CompilePrintf\n");

//     if (ctx->node->data->opCode != PRINTF)
//         return ERROR_IN_CompilePrintf;

//     ctx->node = ctx->node->right;

//     if (CompileExpression (ctx)) return ERROR_IN_CompilePrintf;

//     fprintf (ctx->fp, "\n\t\tout");

//     return 0;
// }

static int CompileConditionOp (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("I in CompileConditionOp\n");

    char* condition = ctx->buffer;
    char* fixElse = nullptr;
    char* fixEnd  = nullptr;
    if (ctx->node->data->opCode == IF)
    {
        $fprintf (ctx->fp, "\n\n.if_%d_condition:", ctx->nIf);
    }
    else if (ctx->node->data->opCode == WHILE)
    {
        $fprintf (ctx->fp, "\n\n.while_%d_condition:", ctx->nWhile);
    }
    else return ERROR_IN_CompileIf;

    NodeBinTree* oldNode = ctx->node;

    ctx->node = oldNode->left->left;
    CompileExpression (ctx);

    ctx->node = oldNode->left->right;
    CompileExpression (ctx);

    $fprintf (ctx->fp, "\n\t\tmov      rax, -%d[rbp]", 
             ((ctx->tempVar + ctx->nVarInFunc) - 1)  * variableSize);
    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
    *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
    int disp = -1 * ((ctx->tempVar + ctx->nVarInFunc) - 1)  * variableSize;
    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

    $fprintf (ctx->fp, "\n\t\tmov     rbx, -%d[rbp]", 
             (ctx->tempVar + ctx->nVarInFunc) * variableSize);
    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
    *(ctx->buffer) = 0x5d; ctx->buffer++; /// modrm
    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

    $fprintf (ctx->fp, "\n\t\tcmp    rax, rbx\n");
    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
    *(ctx->buffer) = 0x39; ctx->buffer++; /// opcode
    char modrm = 0;
    modrm |= 0xC0; // 11 *** *** mod 
    modrm |= 0x18; // ** 011 *** reg
    modrm |= 0x00; // ** *** 000 rm 
    *(ctx->buffer) = modrm; ctx->buffer++; /// modrm

    ctx->tempVar--;
    ctx->tempVar--;

    switch (oldNode->left->data->opCode)
    {
        case ABOVE:

            $fprintf (ctx->fp, "\n\t\tja");
            *(ctx->buffer) = 0x77; ctx->buffer++; /// opcode

            break;

        case ABOVE_EQUAL:

            $fprintf (ctx->fp, "\n\t\tjae");
            *(ctx->buffer) = 0x73; ctx->buffer++; /// opcode

            break;

        case BELOW:

            $fprintf (ctx->fp, "\n\t\tjb");
            *(ctx->buffer) = 0x72; ctx->buffer++; /// opcode

            break;

        case BELOW_EQUAL:

            $fprintf (ctx->fp, "\n\t\tjbe");
            *(ctx->buffer) = 0x76; ctx->buffer++; /// opcode

            break;

        case EQUAL:

            $fprintf (ctx->fp, "\n\t\tje");
            *(ctx->buffer) = 0x74; ctx->buffer++; /// opcode

            break;

        case NO_EQUAL:

            $fprintf (ctx->fp, "\n\t\tjne");
            *(ctx->buffer) = 0x75; ctx->buffer++; /// opcode

            break;

        default:

            return ERROR_IN_CompileIf;
    }


    if (oldNode->data->opCode == IF)
    {
        $fprintf (ctx->fp, " .if_%d_body", ctx->nIf);
        *(ctx->buffer) = 0x05; ctx->buffer++; /// opcode // skip next jmp

        $fprintf (ctx->fp, "\n\t\tjmp .else_%d_body", ctx->nIf);
        *(ctx->buffer) = 0xe9; ctx->buffer++; /// opcode
        fixElse = ctx->buffer;
        ctx->buffer += 4;

        $fprintf (ctx->fp, "\n\n.if_%d_body:", ctx->nIf);
    }

    else
    {
        $fprintf (ctx->fp, " .while_%d_body", ctx->nWhile);
        *(ctx->buffer) = 0x05; ctx->buffer++; /// opcode // skip next jmp

        $fprintf (ctx->fp, "\n\t\tjmp .while_%d_end", ctx->nWhile);
        *(ctx->buffer) = 0xe9; ctx->buffer++; /// opcode
        fixEnd = ctx->buffer;
        ctx->buffer += 4;

        $fprintf (ctx->fp, "\n\n.while_%d_body:", ctx->nWhile);
    }

    ctx->node = oldNode->right->left;
    CompileMultipleOperations (ctx);

    if (oldNode->data->opCode == IF)
    {
        $fprintf (ctx->fp, "\n\t\tjmp .if_%d_end", ctx->nIf);
        *(ctx->buffer) = 0xe9; ctx->buffer++; /// opcode
        fixEnd = ctx->buffer;
        ctx->buffer += 4;

        $fprintf (ctx->fp, "\n\n.else_%d_body:", ctx->nIf);
        int value = ctx->buffer - (fixElse + 4);
        *(fixElse) = ((char*)&value)[0]; fixElse++; /// value
        *(fixElse) = ((char*)&value)[1]; fixElse++; /// value
        *(fixElse) = ((char*)&value)[2]; fixElse++; /// value
        *(fixElse) = ((char*)&value)[3]; fixElse++; /// value

        if (oldNode->right->right != nullptr)
        {
            ctx->node = oldNode->right->right;
            CompileMultipleOperations (ctx);
        }
        $fprintf (ctx->fp, "\n\t\tjmp .if_%d_end", ctx->nIf);

        $fprintf (ctx->fp, "\n\n.if_%d_end:\n\n", ctx->nIf);
        value = ctx->buffer - (fixEnd + 4);
        *(fixEnd) = ((char*)&value)[0]; fixEnd++; /// value
        *(fixEnd) = ((char*)&value)[1]; fixEnd++; /// value
        *(fixEnd) = ((char*)&value)[2]; fixEnd++; /// value
        *(fixEnd) = ((char*)&value)[3]; fixEnd++; /// value

        ctx->nIf++;
    }

    else
    {
        $fprintf (ctx->fp, "\n\t\tjmp .while_%d_condition", ctx->nWhile);

        *(ctx->buffer) = 0xe9; ctx->buffer++; /// opcode
        int value = (ctx->buffer + 4) - condition;
        *(ctx->buffer) = ((char*)&value)[0]; ctx->buffer++; /// value
        *(ctx->buffer) = ((char*)&value)[1]; ctx->buffer++; /// value
        *(ctx->buffer) = ((char*)&value)[2]; ctx->buffer++; /// value
        *(ctx->buffer) = ((char*)&value)[3]; ctx->buffer++; /// value

        $fprintf (ctx->fp, "\n\n.while_%d_end:\n\n", ctx->nWhile);
        value = ctx->buffer - (fixEnd + 4);
        *(fixEnd) = ((char*)&value)[0]; fixEnd++; /// value
        *(fixEnd) = ((char*)&value)[1]; fixEnd++; /// value
        *(fixEnd) = ((char*)&value)[2]; fixEnd++; /// value
        *(fixEnd) = ((char*)&value)[3]; fixEnd++; /// value
        ctx->nWhile++;
    }

    ctx->node = oldNode;

    return 0;
}

static int CompileExpression (BackEndCtx* ctx)
{
    assert (ctx);

    NodeBinTree* oldNode = ctx->node;

    if (ctx->node->data->type == NodeBinTreeData::T_FUNCTION)
    {
        $printf ("BEFORE CompileFunction in if\n\n");

        CompileFunction (ctx);

        $fprintf (ctx->fp, "\n\t\t; Значение return");
        ctx->tempVar++;
        $fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                (ctx->tempVar + ctx->nVarInFunc) * variableSize);
        *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
        *(ctx->buffer) = OPCODE_MOV_RM_TO_R; ctx->buffer++; /// opcode
        *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
        int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
        *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

        return 0;
    }

    ctx->node = oldNode->left;
    if (ctx->node != nullptr) CompileExpression (ctx);

    ctx->node = oldNode->right;
    if (ctx->node != nullptr) CompileExpression (ctx);

    ctx->node = oldNode;

    $printf ("\n\ttype     = %d"
            "\n\tvalue    = %d"
            "\n\topCode   = %d"
            "\n\tvariable = %d"
            "\n\tfunction = %d\n",
            ctx->node->data->type,
            ctx->node->data->value,
            ctx->node->data->opCode,
            ctx->node->data->variable,
            ctx->node->data->function);

    switch (ctx->node->data->type)
    {
        case NodeBinTreeData::T_VALUE:
        {
            $fprintf (ctx->fp, "\n\n\t\t        ; const %d ", ctx->node->data->value);

            ctx->tempVar++;
            $fprintf (ctx->fp, "\n\t\tmov     qword -%d[rbp], %d\n", 
                    (ctx->tempVar + ctx->nVarInFunc) * variableSize,
                    ctx->node->data->value);

            *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
            *(ctx->buffer) = 0xc7; ctx->buffer++; /// opcode

            char modrm = 0;
            modrm |= 0x40; // 01 *** *** mod (disp in 4 bit)
            modrm |= 0x00; // ** 000 *** reg (его нет)
            modrm |= 0x05; // ** *** 101 rm (rbp)
            *(ctx->buffer) = modrm; ctx->buffer++; /// modrm

            int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
            *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

            int value = ctx->node->data->value;
            // *(int*)(ctx->buffer) = value; ctx->buffer += sizeof (int); /// value
            *(ctx->buffer) = ((char*)&value)[0]; ctx->buffer++; /// value
            *(ctx->buffer) = ((char*)&value)[1]; ctx->buffer++; /// value
            *(ctx->buffer) = ((char*)&value)[2]; ctx->buffer++; /// value
            *(ctx->buffer) = ((char*)&value)[3]; ctx->buffer++; /// value

            break;
        }

        case NodeBinTreeData::T_VARIABLE:
        {
            $fprintf (ctx->fp, "\n\n\t\t        ; variable ");
            // for (size_t i = 0; i < ctx->var->data[ctx->node->data->variable].len; i++)
            //     fprintf (ctx->fp, "%c", ctx->var->data[ctx->node->data->variable].name[i]);

            ctx->tempVar++;
            $fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                    (ctx->node->data->variable + 1) * variableSize);
            *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
            *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
            *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
            int disp = -1 * (ctx->node->data->variable + 1) * variableSize;
            *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

            $fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                    (ctx->tempVar + ctx->nVarInFunc) * variableSize);
            *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
            *(ctx->buffer) = OPCODE_MOV_RM_TO_R; ctx->buffer++; /// opcode
            *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
            disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
            *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

            break;
        }

        case NodeBinTreeData::T_OPCODE:

            switch (ctx->node->data->opCode)
            {
                case ADD:
                {    
                    $fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                             ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize);
                    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
                    *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
                    *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
                    int disp = -1 * ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize;
                    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

                    $fprintf (ctx->fp, "\n\t\tmov     rbx, -%d[rbp]", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);
                    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
                    *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
                    *(ctx->buffer) = 0x5d; ctx->buffer++; /// modrm
                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp
                            
                    ctx->tempVar--;

                    $fprintf (ctx->fp, "\n\t\tadd     rax, rbx\n");
                    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
                    *(ctx->buffer) = 0x01; ctx->buffer++; /// opcode 

                    char modrm = 0;
                    modrm |= 0xC0; // 11 *** *** mod 
                    modrm |= 0x18; // ** 011 *** reg (rbx)
                    modrm |= 0x00; // ** *** 000 rm (rax)
                    *(ctx->buffer) = modrm; ctx->buffer++; /// modrm

                    $fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);
                    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
                    *(ctx->buffer) = OPCODE_MOV_RM_TO_R; ctx->buffer++; /// opcode
                    *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

                    break;
                }

                case SUB:
                {
                        
                    $fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                             ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize);
                    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
                    *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
                    *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
                    int disp = -1 * ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize;
                    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

                    $fprintf (ctx->fp, "\n\t\tmov     rbx, -%d[rbp]", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);
                    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
                    *(ctx->buffer) = OPCODE_MOV_R_TO_RM; ctx->buffer++; /// opcode
                    *(ctx->buffer) = 0x5d; ctx->buffer++; /// modrm
                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp
                            
                    ctx->tempVar--;

                    $fprintf (ctx->fp, "\n\t\tsub     rax,  rbx\n");
                    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
                    *(ctx->buffer) = 0x29; ctx->buffer++; /// opcode  ////

                    char modrm = 0;
                    modrm |= 0xC0; // 11 *** *** mod 
                    modrm |= 0x18; // ** 011 *** reg (rbx)
                    modrm |= 0x00; // ** *** 000 rm (rax)
                    *(ctx->buffer) = modrm; ctx->buffer++; /// modrm

                    $fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);
                    *(ctx->buffer) = PREFIX_REX; ctx->buffer++; /// prefix
                    *(ctx->buffer) = OPCODE_MOV_RM_TO_R; ctx->buffer++; /// opcode
                    *(ctx->buffer) = 0x45; ctx->buffer++; /// modrm
                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                    *(ctx->buffer) = char(disp); ctx->buffer += sizeof (char); /// disp

                    break;
                }

//                 case MUL:

//                     fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]",  
//                              ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize);
//                     fprintf (ctx->fp, "\n\t\tmul     qword -%d[rbp]", 
//                             (ctx->tempVar + ctx->nVarInFunc) * variableSize);

//                     ctx->tempVar--;

//                     fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
//                             (ctx->tempVar + ctx->nVarInFunc) * variableSize);

//                     break;

//                 case DIV:

//                     fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]",  
//                              ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize);
//                     fprintf (ctx->fp, "\n\t\txor     rdx, rdx");  
//                     fprintf (ctx->fp, "\n\t\tdiv     qword -%d[rbp]", 
//                             (ctx->tempVar + ctx->nVarInFunc) * variableSize);

//                     ctx->tempVar--;

//                     fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
//                             (ctx->tempVar + ctx->nVarInFunc) * variableSize);


//                     break;

//                 case SQRT:

//                         fprintf (ctx->fp, "\n\n\t\t; сохраняем локальные перменные");
//                         fprintf (ctx->fp, "\n\t\tsub     rsp, %d\n", (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize); 

//                         fprintf (ctx->fp, "\n\t\t; передаем аргументы");
//                     fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]",  
//                                 ((ctx->tempVar + ctx->nVarInFunc)) * variableSize);
//                     fprintf (ctx->fp, "\n\t\tmov     rdi, rax");

//                         fprintf (ctx->fp, "\n\n\t\t; Вызов функции");
//                         fprintf (ctx->fp, "\n\n\t\tpush    rbp");
//                     fprintf (ctx->fp, "\n\n\t\tcall MySqrt");
//                         fprintf (ctx->fp, "\n\n\t\tpop     rbp");

//                         fprintf (ctx->fp, "\n\n\t\t; возвращаем rsp");
//                         fprintf (ctx->fp, "\n\t\tadd     rsp, %d", (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize);

//                     fprintf (ctx->fp, "\n\n\t\t; значениe return");
//                     fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
//                             (ctx->tempVar + ctx->nVarInFunc) * variableSize);


//                     break;

//                 case POW:

//                     fprintf (ctx->fp, "\n\t\tpow");

//                     break;

//                 case SIN:

//                     fprintf (ctx->fp, "\n\t\tsin");

//                     break;

//                 case COS:

//                     fprintf (ctx->fp, "\n\t\tcos");

//                     break;

//                 case LN:

//                     fprintf (ctx->fp, "\n\t\tln");

//                     break;

//                 case PAT_IN:

//                     fprintf (ctx->fp, "\n\t\tin");

//                     break;

                default:
                    printf ("ERROR in CompileExpression\n"
                            "opCode = %d\n", ctx->node->data->opCode);
            }

            break;

        default:
            printf ("ERROR in CompileExpression\n"
                    "type = %d\n", ctx->node->data->type);
    }

    return 0;
}

#undef $printf
