#include "back-end.h"

#define GenPush(...)     GenPush     (&ctx->buffer, __VA_ARGS__)
#define GenPop(...)      GenPop      (&ctx->buffer, __VA_ARGS__)
#define GenMovRR(...)    GenMovRR    (&ctx->buffer, __VA_ARGS__)
#define GenMovRM(...)    GenMovRM    (&ctx->buffer, __VA_ARGS__)
#define GenMovMR(...)    GenMovMR    (&ctx->buffer, __VA_ARGS__)
#define GenMovRMIMM(...) GenMovRMIMM (&ctx->buffer, __VA_ARGS__)
#define GenMovRADD(...)  GenMovRADD  (&ctx->buffer, __VA_ARGS__)
#define GenMovRIMM(...)  GenMovRIMM  (&ctx->buffer, __VA_ARGS__)
#define GenCallAbs(...)  GenCallAbs  (&ctx->buffer, __VA_ARGS__)
#define GenAdd(...)      GenAdd      (&ctx->buffer, __VA_ARGS__)
#define GenAddRIMM(...)  GenAddRIMM  (&ctx->buffer, __VA_ARGS__)
#define GenSubRIMM(...)  GenSubRIMM  (&ctx->buffer, __VA_ARGS__)
#define GenSub(...)      GenSub      (&ctx->buffer, __VA_ARGS__)
#define GenMul(...)      GenMul      (&ctx->buffer, __VA_ARGS__)
#define GenXor(...)      GenXor      (&ctx->buffer, __VA_ARGS__)
#define GenCmp(...)      GenCmp      (&ctx->buffer, __VA_ARGS__)
#define GenRet()         GenRet      (&ctx->buffer)

static int CompileMultipleOperations (BackEndCtx* ctx);
static int CompileOperation          (BackEndCtx* ctx);
static int CompileActualArguments    (BackEndCtx* ctx);
static int CompileFormalArguments    (BackEndCtx* ctx);
static int CompileFunction           (BackEndCtx* ctx);
static int CompileFuncReturn         (BackEndCtx* ctx);
static int CompilePrintf             (BackEndCtx* ctx);
static int CompileAssign             (BackEndCtx* ctx);
static int CompileConditionOp        (BackEndCtx* ctx);
static int CompileCreate             (BackEndCtx* ctx);
static int CompileExpression         (BackEndCtx* ctx);

char* bufferBegin = nullptr;

int CompileProgram (BackEndCtx* ctx)
{
    assert (ctx);

    bufferBegin = ctx->buffer;

    ctx->tempVar    = 1;
    ctx->nVarInFunc = 0;
    NodeBinTree* oldNode = ctx->node;

    /**
     *  Compile main
     */

    GenPush (REG_RBP);
    GenMovRR (REG_RBP, REG_RSP);

    ctx->node = oldNode->left;
    CompileMultipleOperations (ctx);
        
    GenPop (REG_RBP);
    GenRet ();

    ctx->buffer += 20;

    /**
     *  Compile functions
     */
    oldNode = oldNode->right;
    while (oldNode != nullptr)
    {
        ctx->nVarInFunc = 0;
        ctx->funcIp[oldNode->data->function] = ctx->buffer;

        GenMovRR (REG_RBP, REG_RSP);

    /**
     *  Compile arguments
     */
        GenPop (REG_RCX);
        GenPop (REG_RDX);

        ctx->node = oldNode->left->right;
        CompileFormalArguments (ctx);

        GenPush (REG_RDX);
        GenPush (REG_RCX);

    /**
     *  Compile function
     */
        ctx->node = oldNode->left->left;
        CompileMultipleOperations (ctx);

        GenRet ();
        oldNode = oldNode->right;
    }

    for (int i = 0; i < ctx->fixupSize; i++)
    {
        int value = ctx->funcIp[ctx->fixupNFunc[i]] - ctx->fixup[i] - 4;
        char* fix = ctx->fixup[i];
        GenValue (&fix, value);
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
            printf ("ERROR in CompileMultipleOperations!!!\n\n");
            return ERROR_IN_CompileMultipleOperations;
        }

        ctx->node = oldNode->left;
        CompileOperation (ctx);

        oldNode = oldNode->right;
    }

    return 0;
}

static int CompileOperation (BackEndCtx* ctx)
{
    assert (ctx);

    if (CompileFunction    (ctx) &&
        CompileFuncReturn  (ctx) &&
        // CompilePrintf      (ctx) &&
        CompileConditionOp (ctx) &&
        CompileAssign      (ctx) &&
        CompileCreate      (ctx))

    {
        printf ("\nERROR in CompileOperation!!!\n\n");

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

    return 0;
}

static int CompileFunction (BackEndCtx* ctx)
{
    assert (ctx);

    if (ctx->node->data->type != NodeBinTreeData::T_FUNCTION)
        return ERROR_IN_CompileFunction;

    int nFunc = ctx->node->data->function;
    int value = (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize;
    GenSubRIMM (REG_RSP, value);

    CompileActualArguments (ctx);

    GenPush (REG_RBP);

    *(ctx->buffer) = OPCODE_CALL_NEAR_REL; ctx->buffer++; 

    ctx->fixup[ctx->fixupSize]      = ctx->buffer;
    ctx->fixupNFunc[ctx->fixupSize] = nFunc;
    ctx->fixupSize++;
    ctx->buffer += 4;

    GenPop (REG_RBP);

    value = (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize;
    GenAddRIMM (REG_RSP, value);

    return 0;
}

static int CompileFuncReturn (BackEndCtx* ctx)
{
    assert (ctx);

    if (ctx->node->data->opCode != FUNC_RETURN)
        return ERROR_IN_CompileFuncReturn;

    if (ctx->node->right != nullptr)
    {
        ctx->node = ctx->node->right;
        CompileExpression (ctx);

        GenXor (REG_RAX, REG_RAX);

        int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
        GenMovRM (REG_RAX, REG_RBP, disp);

        ctx->tempVar--;
    }

    GenRet ();

    return 0;
}

static int CompileFormalArguments (BackEndCtx* ctx)
{
    assert (ctx);

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

    GenPop (REG_RAX);

    int disp = -1 * (ctx->node->data->variable + 1) * variableSize;
    GenMovMR (REG_RAX, REG_RBP, disp);

    if (ctx->node->data->variable >= ctx->nVarInFunc)
        ctx->nVarInFunc = ctx->node->data->variable;

    return 0;
}

static int CompileActualArguments (BackEndCtx* ctx)
{
    assert (ctx);

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

        int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
        GenMovRM (REG_RAX, REG_RBP, disp);

        GenPush (REG_RAX);
        ctx->tempVar--;

        ctx->node = oldNode;
    }

    return 0;
}

static int CompileAssign (BackEndCtx* ctx)
{
    assert (ctx);

    if (ctx->node      ->data->opCode   != ASSING ||
        ctx->node->left->data->variable == NodeBinTreeData::VARIABLE_POISON)
        return ERROR_IN_CompileAssign;

    NodeBinTree* oldNode = ctx->node;
    ctx->node = oldNode->right;
    CompileExpression (ctx);

    int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
    GenMovRM (REG_RAX, REG_RBP, disp);

    ctx->tempVar--;

    disp = -1 * (oldNode->left->data->variable + 1) * variableSize;
    GenMovMR (REG_RAX, REG_RBP, disp);

    if (oldNode->left->data->variable >= ctx->nVarInFunc)
        ctx->nVarInFunc = oldNode->left->data->variable;

    ctx->node = oldNode;

    return 0;
}

static int CompileCreate (BackEndCtx* ctx)
{
    assert (ctx);

    if (ctx->node->data->opCode != T_INT)
        return ERROR_IN_CompileCreate;

    return 0;
}

// static int CompilePrintf (BackEndCtx* ctx)
// {
//     assert (ctx);

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

    char* condition = ctx->buffer;
    char* fixElse = nullptr;
    char* fixEnd  = nullptr;

    if (ctx->node->data->opCode != IF && 
        ctx->node->data->opCode != WHILE)
        return ERROR_IN_CompileIf;

    NodeBinTree* oldNode = ctx->node;

    ctx->node = oldNode->left->left;
    CompileExpression (ctx);

    ctx->node = oldNode->left->right;
    CompileExpression (ctx);

    int disp = -1 * ((ctx->tempVar + ctx->nVarInFunc) - 1)  * variableSize;
    GenMovRM (REG_RAX, REG_RBP, disp);

    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize; 
    GenMovRM (REG_RBX, REG_RBP, disp);

    GenCmp (REG_RAX, REG_RBX);

    ctx->tempVar--;
    ctx->tempVar--;

    switch (oldNode->left->data->opCode)
    {
        case ABOVE:

            *(ctx->buffer) = OPCODE_JCC_JA; ctx->buffer++;

            break;

        case ABOVE_EQUAL:

            *(ctx->buffer) = OPCODE_JCC_JAE; ctx->buffer++;

            break;

        case BELOW:

            *(ctx->buffer) = OPCODE_JCC_JB; ctx->buffer++; 

            break;

        case BELOW_EQUAL:

            *(ctx->buffer) = OPCODE_JCC_JBE; ctx->buffer++; 

            break;

        case EQUAL:

            *(ctx->buffer) = OPCODE_JCC_JE; ctx->buffer++; 

            break;

        case NO_EQUAL:

            *(ctx->buffer) = OPCODE_JCC_JNE; ctx->buffer++; 

            break;

        default:

            return ERROR_IN_CompileIf;
    }


    if (oldNode->data->opCode == IF)
    {
        *(ctx->buffer) = 0x05; ctx->buffer++; // skip next jmp (const)

        /* jmp to else */
        *(ctx->buffer) = OPCODE_JMP_NEAR; ctx->buffer++; 
        fixElse = ctx->buffer;
        ctx->buffer += 4;
    }

    else
    {
        *(ctx->buffer) = 0x05; ctx->buffer++;  // skip next jmp (const)

        /* jmp to end */
        *(ctx->buffer) = OPCODE_JMP_NEAR; ctx->buffer++; 
        fixEnd = ctx->buffer;
        ctx->buffer += 4;
    }

    ctx->node = oldNode->right->left;
    CompileMultipleOperations (ctx);

    if (oldNode->data->opCode == IF)
    {
        /* jmp to end */
        *(ctx->buffer) = OPCODE_JMP_NEAR; ctx->buffer++; 
        fixEnd = ctx->buffer;
        ctx->buffer += 4;

        int value = ctx->buffer - (fixElse + 4);
        GenValue (&fixElse, value);

        if (oldNode->right->right != nullptr)
        {
            ctx->node = oldNode->right->right;
            CompileMultipleOperations (ctx);
        }

        value = ctx->buffer - (fixEnd + 4);
        GenValue (&fixEnd, value);

        ctx->nIf++;
    }

    else
    {
        /* jmp to condition */
        *(ctx->buffer) = OPCODE_JMP_NEAR; ctx->buffer++;
        int value = (ctx->buffer + 4) - condition;
        GenValue (&ctx->buffer, value);

        value = ctx->buffer - (fixEnd + 4);
        GenValue (&fixEnd, value);
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
        CompileFunction (ctx);
        ctx->tempVar++;

        int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
        GenMovMR (REG_RAX, REG_RBP, disp);

        return 0;
    }

    ctx->node = oldNode->left;
    if (ctx->node != nullptr) CompileExpression (ctx);

    ctx->node = oldNode->right;
    if (ctx->node != nullptr) CompileExpression (ctx);

    ctx->node = oldNode;

    switch (ctx->node->data->type)
    {
        case NodeBinTreeData::T_VALUE:
        {
            ctx->tempVar++;
            int disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
            int value = ctx->node->data->value;

            GenMovRMIMM (REG_RBP, disp, value);

            break;
        }

        case NodeBinTreeData::T_VARIABLE:
        {
            ctx->tempVar++;
            int disp = -1 * (ctx->node->data->variable + 1) * variableSize;
            GenMovRM (REG_RAX, REG_RBP, disp);

            disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
            GenMovMR (REG_RAX, REG_RBP, disp);

            break;
        }

        case NodeBinTreeData::T_OPCODE:

            switch (ctx->node->data->opCode)
            {
                case ADD:
                {    
                    int disp = -1 * ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize;   
                    GenMovRM (REG_RAX, REG_RBP, disp);

                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize; 
                    GenMovRM (REG_RBX, REG_RBP, disp);
                            
                    ctx->tempVar--;

                    GenAdd (REG_RAX, REG_RBX);

                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                    GenMovMR (REG_RAX, REG_RBP, disp);

                    break;
                }

                case SUB:
                {
                    int disp = -1 * ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize;
                    GenMovRM (REG_RAX, REG_RBP, disp);

                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize; 
                    GenMovRM (REG_RBX, REG_RBP, disp);
                            
                    ctx->tempVar--;

                    GenSub (REG_RAX, REG_RBX);

                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                    GenMovMR (REG_RAX, REG_RBP, disp);

                    break;
                }

                case MUL:
                {

                    int disp = -1 * ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize;
                    GenMovRM (REG_RAX, REG_RBP, disp);

                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                    GenMul (REG_RBP, disp);

                    ctx->tempVar--;

                    disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                    GenMovMR (REG_RAX, REG_RBP, disp);

                    break;
                }

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

                case SQRT:
                    {
                        int value = (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize;
                        GenSubRIMM (REG_RSP, value);

                        int disp = -1 * ((ctx->tempVar + ctx->nVarInFunc)) * variableSize;
                        GenMovRM (REG_RAX, REG_RBP, disp);

                        GenMovRR (REG_RDI, REG_RAX);
                        GenPush (REG_RBP);

                        GenMovRADD (REG_RAX, (char*)MySqrt);
                        GenCallAbs (REG_RAX);

                        GenPop (REG_RBP);

                        value = (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize;
                        GenAddRIMM (REG_RSP, value);

                        disp = -1 * (ctx->tempVar + ctx->nVarInFunc) * variableSize;
                        GenMovMR (REG_RAX, REG_RBP, disp);

                    break;

                    }

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

#undef GenPush     
#undef GenPop      
#undef GenMovRR    
#undef GenMovRM    
#undef GenMovMR    
#undef GenMovRMIMM 
#undef GenMovRADD  
#undef GenMovRIMM  
#undef GenCallAbs  
#undef GenAdd      
#undef GenAddRIMM  
#undef GenSubRIMM  
#undef GenSub      
#undef GenMul      
#undef GenXor      
#undef GenCmp      
#undef GenRet       
