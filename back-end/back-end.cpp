#include "back-end.h"

#define $printf(...)
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

    fprintf (ctx->fp, "\nglobal Func");
    fprintf (ctx->fp, "\nextern MySqrt");
    fprintf (ctx->fp, "\n\nsection .data");
    fprintf (ctx->fp, "\n\nsection .text");

    /**
     *  Compile main
     */
    fprintf (ctx->fp, "\n; начало main");
    fprintf (ctx->fp, "\n\nFunc:");
    fprintf (ctx->fp, "\n\n\t\tmov     rbp, rsp");

    ctx->node = oldNode->left;
    CompileMultipleOperations (ctx);
        
    fprintf (ctx->fp, "\n\n\t\tret");
    fprintf (ctx->fp, "\n\n; конец main");

    /**
     *  Compile functions
     */
    oldNode = oldNode->right;
    while (oldNode != nullptr)
    {
        ctx->nVarInFunc = 0;

        $printf ("\n\n\n\nFUNCTION N = %d\n\n\n\n", oldNode->data->function);

        fprintf (ctx->fp, "\n\n\n\n\n; начало функции");
        fprintf (ctx->fp, "\n\nFunc_%d_:", oldNode->data->function);
        fprintf (ctx->fp, "\n\n\t\tmov     rbp, rsp\n\n");

    /**
     *  Compile arguments
     */

        fprintf (ctx->fp, "\n\t\tpop     rcx");
        fprintf (ctx->fp, "\n\t\tpop     rdx");

        fprintf (ctx->fp, "\n\n\n;-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\n");
  
        ctx->node = oldNode->left->right;
        CompileFormalArguments (ctx);

        fprintf (ctx->fp, "\n\n;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-\n");

        fprintf (ctx->fp, "\n\t\tpush    rdx");
        fprintf (ctx->fp, "\n\t\tpush    rcx");

        fprintf (ctx->fp, "\n\n");

    /**
     *  Compile function
     */
        ctx->node = oldNode->left->left;
        CompileMultipleOperations (ctx);

        fprintf (ctx->fp, "\n\n\t\tret");
        fprintf (ctx->fp, "\n\n; конец функции");

        oldNode = oldNode->right;
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

        fprintf (ctx->fp, "\n");

        oldNode = oldNode->right;
    }

    return 0;
}

static int CompileOperation (BackEndCtx* ctx)
{
    assert (ctx);
    $printf ("\n\nI in CompileOperation\n");

    fprintf (ctx->fp, "\n\n\n;-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\n");
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

    fprintf (ctx->fp, "\n\n\n;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-\n");
    
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

    fprintf (ctx->fp, "\n\n\t\t; сохраняем локальные перменные");
    fprintf (ctx->fp, "\n\t\tsub     rsp, %d\n\n", (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize); 

    fprintf (ctx->fp, "\n\t\t; передаем аргументы");
    CompileActualArguments (ctx);

    fprintf (ctx->fp, "\n\t\t; Вызов функции");
    fprintf (ctx->fp, "\n\t\tpush    rbp");
    fprintf (ctx->fp, "\n\n\t\tcall Func_%d_\n", nFunc);
    fprintf (ctx->fp, "\n\t\tpop     rbp\n\n");

    fprintf (ctx->fp, "\n\n\t\t; возвращаем rsp");
    fprintf (ctx->fp, "\n\t\tadd     rsp, %d\n\n", (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize);

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

        fprintf (ctx->fp, "\n\t\t; значениe return");
        fprintf (ctx->fp, "\n\t\txor     rax, rax");
        fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                 (ctx->tempVar + ctx->nVarInFunc) * variableSize);
        ctx->tempVar--;
    }

    fprintf (ctx->fp, "\n\n\t\tret");

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

    fprintf (ctx->fp, "\n\t\tpop     rax");
    fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax", (ctx->node->data->variable + 1) * variableSize);

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

        fprintf (ctx->fp, "\n\t\txor     rax, rax");
        fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                 (ctx->tempVar + ctx->nVarInFunc) * variableSize);
        fprintf (ctx->fp, "\n\t\tpush    rax");
        ctx->tempVar--;

        ctx->node = oldNode;

        fprintf (ctx->fp, "\n");
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

    fprintf (ctx->fp, "\n\n\t\t; Assign ");
    for (size_t i = 0; i < ctx->var->data[oldNode->left->data->variable].len; i++)
        fprintf (ctx->fp, "%c", ctx->var->data[oldNode->left->data->variable].name[i]);

    $printf ("I in CompileAssign before right\n");

    ctx->node = oldNode->right;
    CompileExpression (ctx);

    $printf ("I in CompileAssign after right\n");

    fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
             (ctx->tempVar + ctx->nVarInFunc) * variableSize);

    ctx->tempVar--;

    fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax", 
             (oldNode->left->data->variable + 1) * variableSize);

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

    if (     ctx->node->data->opCode == IF)
        fprintf (ctx->fp, "\n\n.if_%d_condition:", ctx->nIf);

    else if (ctx->node->data->opCode == WHILE)
        fprintf (ctx->fp, "\n\n.while_%d_condition:", ctx->nWhile);

    else return ERROR_IN_CompileIf;

    NodeBinTree* oldNode = ctx->node;

    ctx->node = oldNode->left->left;
    CompileExpression (ctx);

    ctx->node = oldNode->left->right;
    CompileExpression (ctx);

    fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
             ((ctx->tempVar + ctx->nVarInFunc) - 1)  * variableSize);
    fprintf (ctx->fp, "\n\t\tmov     rbx, -%d[rbp]", 
             (ctx->tempVar + ctx->nVarInFunc) * variableSize);
    fprintf (ctx->fp, "\n\t\tcmp    rax, rbx\n");

    ctx->tempVar--;
    ctx->tempVar--;

    switch (oldNode->left->data->opCode)
    {
        case ABOVE:

            fprintf (ctx->fp, "\n\t\tja");

            break;

        case ABOVE_EQUAL:

            fprintf (ctx->fp, "\n\t\tjae");

            break;

        case BELOW:

            fprintf (ctx->fp, "\n\t\tjb");

            break;

        case BELOW_EQUAL:

            fprintf (ctx->fp, "\n\t\tjbe");

            break;

        case EQUAL:

            fprintf (ctx->fp, "\n\t\tje");

            break;

        case NO_EQUAL:

            fprintf (ctx->fp, "\n\t\tjne");

            break;

        default:

            return ERROR_IN_CompileIf;
    }


    if (oldNode->data->opCode == IF)
    {
        fprintf (ctx->fp, " .if_%d_body", ctx->nIf);
        fprintf (ctx->fp, "\n\t\tjmp .else_%d_body", ctx->nIf);

        fprintf (ctx->fp, "\n\n.if_%d_body:", ctx->nIf);
    }

    else
    {
        fprintf (ctx->fp, " .while_%d_body"
                     "\n\t\tjmp .while_%d_end", ctx->nWhile, ctx->nWhile);

        fprintf (ctx->fp, "\n\n.while_%d_body:", ctx->nWhile);
    }

    ctx->node = oldNode->right->left;
    CompileMultipleOperations (ctx);

    if (oldNode->data->opCode == IF)
    {
        fprintf (ctx->fp, "\n\t\tjmp .if_%d_end", ctx->nIf);

        fprintf (ctx->fp, "\n\n.else_%d_body:", ctx->nIf);
        if (oldNode->right->right != nullptr)
        {
            ctx->node = oldNode->right->right;
            CompileMultipleOperations (ctx);
        }
        fprintf (ctx->fp, "\n\t\tjmp .if_%d_end", ctx->nIf);

        fprintf (ctx->fp, "\n\n.if_%d_end:\n\n", ctx->nIf);
        ctx->nIf++;
    }

    else
    {
        fprintf (ctx->fp, "\n\t\tjmp .while_%d_condition", ctx->nWhile);

        fprintf (ctx->fp, "\n\n.while_%d_end:\n\n", ctx->nWhile);
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

        fprintf (ctx->fp, "\n\t\t; Значение return");
        ctx->tempVar++;
        fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                (ctx->tempVar + ctx->nVarInFunc) * variableSize);

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

            fprintf (ctx->fp, "\n\n\t\t        ; const %d ", ctx->node->data->value);

            ctx->tempVar++;
            fprintf (ctx->fp, "\n\t\tmov     qword -%d[rbp], %d\n", 
                    (ctx->tempVar + ctx->nVarInFunc) * variableSize,
                    ctx->node->data->value);

            break;

        case NodeBinTreeData::T_VARIABLE:

            fprintf (ctx->fp, "\n\n\t\t        ; variable ");
            for (size_t i = 0; i < ctx->var->data[ctx->node->data->variable].len; i++)
                fprintf (ctx->fp, "%c", ctx->var->data[ctx->node->data->variable].name[i]);

            ctx->tempVar++;
            fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                    (ctx->node->data->variable + 1) * variableSize);
            fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                    (ctx->tempVar + ctx->nVarInFunc) * variableSize);

            break;

        case NodeBinTreeData::T_OPCODE:

            switch (ctx->node->data->opCode)
            {
                case ADD:
                        
                    fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]", 
                             ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize);
                    fprintf (ctx->fp, "\n\t\tmov     rbx, -%d[rbp]", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);

                    ctx->tempVar--;

                    fprintf (ctx->fp, "\n\t\tadd     rax, rbx\n");
                    fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);

                    break;

                case SUB:
                        
                    fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]",  
                             ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize);
                    fprintf (ctx->fp, "\n\t\tmov     rbx, -%d[rbp]", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);

                    ctx->tempVar--;

                    fprintf (ctx->fp, "\n\t\tsub     rax,  rbx\n");
                    fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);

                    break;

                case MUL:

                    fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]",  
                             ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize);
                    fprintf (ctx->fp, "\n\t\tmul     qword -%d[rbp]", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);

                    ctx->tempVar--;

                    fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);

                    break;

                case DIV:

                    fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]",  
                             ((ctx->tempVar + ctx->nVarInFunc) - 1) * variableSize);
                    fprintf (ctx->fp, "\n\t\txor     rdx, rdx");  
                    fprintf (ctx->fp, "\n\t\tdiv     qword -%d[rbp]", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);

                    ctx->tempVar--;

                    fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);


                    break;

                case SQRT:

                        fprintf (ctx->fp, "\n\n\t\t; сохраняем локальные перменные");
                        fprintf (ctx->fp, "\n\t\tsub     rsp, %d\n", (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize); 

                        fprintf (ctx->fp, "\n\t\t; передаем аргументы");
                    fprintf (ctx->fp, "\n\t\tmov     rax, -%d[rbp]",  
                                ((ctx->tempVar + ctx->nVarInFunc)) * variableSize);
                    fprintf (ctx->fp, "\n\t\tmov     rdi, rax");

                        fprintf (ctx->fp, "\n\n\t\t; Вызов функции");
                        fprintf (ctx->fp, "\n\n\t\tpush    rbp");
                    fprintf (ctx->fp, "\n\n\t\tcall MySqrt");
                        fprintf (ctx->fp, "\n\n\t\tpop     rbp");

                        fprintf (ctx->fp, "\n\n\t\t; возвращаем rsp");
                        fprintf (ctx->fp, "\n\t\tadd     rsp, %d", (ctx->tempVar + ctx->nVarInFunc + 1) * variableSize);

                    fprintf (ctx->fp, "\n\n\t\t; значениe return");
                    fprintf (ctx->fp, "\n\t\tmov     -%d[rbp], rax\n", 
                            (ctx->tempVar + ctx->nVarInFunc) * variableSize);


                    break;

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
