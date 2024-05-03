#include "back-end.h"

static int CompileMultipleOperations (BackEndCtx* ctx);
static int CompileOperation          (BackEndCtx* ctx);
static int CompileActualArguments    (BackEndCtx* ctx);
static int CompileFormalArguments    (BackEndCtx* ctx);
static int CompileVariableBeforeCall (BackEndCtx* ctx);
static int CompileVariableAfterCall  (BackEndCtx* ctx);
static int CompileFunction           (BackEndCtx* ctx);
static int CompileFuncReturn         (BackEndCtx* ctx);
static int CompilePrintf             (BackEndCtx* ctx);
static int CompileAssign             (BackEndCtx* ctx);
static int CompileConditionOp        (BackEndCtx* ctx);
static int CompileCreate             (BackEndCtx* ctx);
static int CompileExpression         (BackEndCtx* ctx);
                           // сюда тоже IsOpCode и тп
static void PrintfVar (BackEndCtx* ctx);

const int nSpace = 20;

int CompileProgram (BackEndCtx* ctx)
{
    assert (ctx);

    NodeBinTree* oldNode = ctx->node;

    /**
     *  Compile main
     */
    fprintf (ctx->fp, ":main");
    fprintf (ctx->fp, "\n$ начало main");

    fprintf (ctx->fp, "\n\t\tpush 0\n\t\tpop rbp");

    ctx->node = oldNode->left;
    CompileMultipleOperations (ctx);

    oldNode = oldNode->right;

    fprintf (ctx->fp, "\n\n\t\thlt");
    fprintf (ctx->fp, "\n$ конец main");

    /**
     *  Compile functions
     */
    while (oldNode != nullptr)
    {
        printf ("\n\n\n\nFUNCTION N = %d\n\n\n\n", oldNode->data->function);

        ctx->nVarBefore = ctx->nVarInFunc + 1;

        fprintf (ctx->fp, "\n$ начало функции");
        fprintf (ctx->fp, "\n\n:func_%d_", oldNode->data->function);

    /**
     *  Compile arguments
     */
        ctx->node = oldNode->left->right;
        CompileFormalArguments (ctx);

        fprintf (ctx->fp, "\n\n");

    /**
     *  Compile function
     */
        ctx->node = oldNode->left->left;
        CompileMultipleOperations (ctx);

        fprintf (ctx->fp, "\n\n\t\tret");
        fprintf (ctx->fp, "\n$ конец функции");

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
            printf ("ERROR in CompileMultipleOperations!!!\n\n");
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
    printf ("\n\nI in CompileOperation\n");

    if (CompileFunction    (ctx) &&
        CompileFuncReturn  (ctx) &&
        CompilePrintf      (ctx) &&
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

//    printf ("I end CompileOperation\n\n\n");

    return 0;
}

static int CompileFormalArguments (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileFormalArguments\n");

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

    int n = fprintf (ctx->fp, "\n\t\tpop [%d rbp]", ctx->node->data->variable);

    for (int j = 0; j < nSpace - n; j++)
        fprintf (ctx->fp, " ");
    PrintfVar (ctx);

    return 0;
}

static int CompileActualArguments (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileActualArguments\n");

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

        ctx->node = oldNode;

        fprintf (ctx->fp, "\n");
    }

    return 0;
}

static int CompileVariableBeforeCall (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileVariableBeforeCall\n");

    for (int i = ctx->nVarBefore; i <= ctx->nVarInFunc; i++)
    {
        int n = fprintf (ctx->fp, "\n\t\tpush [%d rbp]", i);

        for (int j = 0; j < nSpace - n; j++)
            fprintf (ctx->fp, " ");

        fprintf (ctx->fp, "$ ");
        for (size_t j = 0; j < ctx->var->data[i].len; j++)
            fprintf (ctx->fp, "%c", ctx->var->data[i].name[j]);

    }

    fprintf (ctx->fp, "\n");

    return 0;
}

static int CompileVariableAfterCall (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileVariableAfterCall\n");

    for (int i = ctx->nVarInFunc; i >= ctx->nVarBefore; i--)
    {
        int n = fprintf (ctx->fp, "\n\t\tpop [%d rbp]", i);

        for (int j = 0; j < nSpace - n; j++)
            fprintf (ctx->fp, " ");

        fprintf (ctx->fp, "$ ");
        for (size_t j = 0; j < ctx->var->data[i].len; j++)
            fprintf (ctx->fp, "%c", ctx->var->data[i].name[j]);

    }

    fprintf (ctx->fp, "\n");

    return 0;
}

static int CompileFunction (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileFunction\n");

    if (ctx->node->data->type != NodeBinTreeData::T_FUNCTION)
        return ERROR_IN_CompileFunction;

    int nFunc = ctx->node->data->function;

    fprintf (ctx->fp, "\n$ сохраняем значения переменных, до вызова функции");
    CompileVariableBeforeCall (ctx);

    fprintf (ctx->fp, "\n$ передаем аргументы");
    CompileActualArguments (ctx);

    fprintf (ctx->fp, "\n$ вызов функции");
    fprintf (ctx->fp, "\n\t\tcall :func_%d_\n", nFunc);

    fprintf (ctx->fp, "\n$ возвращаем значения переменных, которые были до вызова функции");
    CompileVariableAfterCall (ctx);

    return 0;
}

static int CompileFuncReturn (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileFuncReturn\n");

    if (ctx->node->data->opCode != FUNC_RETURN)
        return ERROR_IN_CompileFuncReturn;

    if (ctx->node->right != nullptr)
    {
        ctx->node = ctx->node->right;
        CompileExpression (ctx);

        fprintf (ctx->fp, "\n$ сохраняем значения return");
        fprintf (ctx->fp, "\n\t\tpop rax");
    }

    fprintf (ctx->fp, "\n\n\t\tret");

    return 0;
}

static int CompilePrintf (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompilePrintf\n");

    if (ctx->node->data->opCode != PRINTF)
        return ERROR_IN_CompilePrintf;

    ctx->node = ctx->node->right;

    if (CompileExpression (ctx)) return ERROR_IN_CompilePrintf;

    fprintf (ctx->fp, "\n\t\tout");

    return 0;
}

static int CompileConditionOp (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileConditionOp\n");

    if (     ctx->node->data->opCode == IF)
        fprintf (ctx->fp, "\n\n:if_%d_condition", ctx->nIf);

    else if (ctx->node->data->opCode == WHILE)
        fprintf (ctx->fp, "\n\n:while_%d_condition", ctx->nWhile);

    else return ERROR_IN_CompileIf;

    NodeBinTree* oldNode = ctx->node;

    ctx->node = oldNode->left->left;
    CompileExpression (ctx);

    ctx->node = oldNode->left->right;
    CompileExpression (ctx);

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
        fprintf (ctx->fp, " :if_%d_body", ctx->nIf);
        fprintf (ctx->fp, "\n\t\tjmp :else_%d_body", ctx->nIf);

        fprintf (ctx->fp, "\n\n:if_%d_body", ctx->nIf);
    }

    else
    {
        fprintf (ctx->fp, " :while_%d_body"
                     "\n\t\tjmp :while_%d_end", ctx->nWhile, ctx->nWhile);

        fprintf (ctx->fp, "\n\n:while_%d_body", ctx->nWhile);
    }

    ctx->node = oldNode->right->left;
    CompileMultipleOperations (ctx);

    if (oldNode->data->opCode == IF)
    {
        fprintf (ctx->fp, "\n\t\tjmp :if_%d_end", ctx->nIf);

        fprintf (ctx->fp, "\n\n:else_%d_body", ctx->nIf);
        if (oldNode->right->right != nullptr)
        {
            ctx->node = oldNode->right->right;
            CompileMultipleOperations (ctx);
        }
        fprintf (ctx->fp, "\n\t\tjmp :if_%d_end", ctx->nIf);

        fprintf (ctx->fp, "\n\n:if_%d_end\n\n", ctx->nIf);
        ctx->nIf++;
    }

    else
    {
        fprintf (ctx->fp, "\n\t\tjmp :while_%d_condition", ctx->nWhile);

        fprintf (ctx->fp, "\n\n:while_%d_end\n\n", ctx->nWhile);
        ctx->nWhile++;
    }

    ctx->node = oldNode;

    return 0;
}

static int CompileAssign (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileAssign\n");

    if (ctx->node      ->data->opCode   != ASSING ||
        ctx->node->left->data->variable == NodeBinTreeData::VARIABLE_POISON)
        return ERROR_IN_CompileAssign;

    NodeBinTree* oldNode = ctx->node;

    printf ("I in CompileAssign before right\n");

    ctx->node = oldNode->right;
    CompileExpression (ctx);

    printf ("I in CompileAssign after right\n");

    int n = fprintf (ctx->fp, "\n\t\tpop [%d rbp]", oldNode->left->data->variable);

    for (int j = 0; j < nSpace - n; j++)
        fprintf (ctx->fp, " ");

    fprintf (ctx->fp, "$ ");
    for (size_t j = 0; j < ctx->var->data[oldNode->left->data->variable].len; j++)
        fprintf (ctx->fp, "%c", ctx->var->data[oldNode->left->data->variable].name[j]);


    if (oldNode->left->data->variable > ctx->nVarInFunc)
        ctx->nVarInFunc = oldNode->left->data->variable;

    ctx->node = oldNode;

    printf ("I end CompileAssign\n");
    return 0;
}

static int CompileCreate (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileCreate\n");

    if (ctx->node->data->opCode != T_INT)
        return ERROR_IN_CompileCreate;

    return 0;
}

static int CompileExpression (BackEndCtx* ctx)
{
    NodeBinTree* oldNode = ctx->node;

    if (ctx->node->data->type == NodeBinTreeData::T_FUNCTION)
    {
//        printf ("BEFORE CompileFunction in if\n\n");

        CompileFunction (ctx);

        fprintf (ctx->fp, "\n$ push значение return");
        fprintf (ctx->fp, "\n\t\tpush rax");

        return 0;
    }

    ctx->node = oldNode->left;
    if (ctx->node != nullptr) CompileExpression (ctx);

    ctx->node = oldNode->right;
    if (ctx->node != nullptr) CompileExpression (ctx);

    ctx->node = oldNode;

//        printf ("\n\ttype     = %d"
//                "\n\tvalue    = %d"
//                "\n\topCode   = %d"
//                "\n\tvariable = %d"
//                "\n\tfunction = %d\n",
//                 ctx->node->data->type,
//                 ctx->node->data->value,
//                 ctx->node->data->opCode,
//                 ctx->node->data->variable,
//                 ctx->node->data->function);

    switch (ctx->node->data->type)
    {
        case NodeBinTreeData::T_VALUE:

            fprintf (ctx->fp, "\n\t\tpush %d", ctx->node->data->value);

            break;

        case NodeBinTreeData::T_VARIABLE:

            {
            int n = fprintf (ctx->fp, "\n\t\tpush [%d rbp]", ctx->node->data->variable);

            for (int j = 0; j < nSpace - n; j++)
                fprintf (ctx->fp, " ");

            PrintfVar (ctx);

            if (ctx->node->data->variable > ctx->nVarInFunc)
                ctx->nVarInFunc = ctx->node->data->variable;
            }

            break;

        case NodeBinTreeData::T_FUNCTION:

            printf ("BEFORE CompileFunction\n\n");
            CompileFunction (ctx);
            fprintf (ctx->fp, "\n\t\tpush rax");

            break;

        case NodeBinTreeData::T_OPCODE:

            switch (ctx->node->data->opCode)
            {
                case ADD:

                    fprintf (ctx->fp, "\n\t\tadd");

                    break;

                case SUB:

                    fprintf (ctx->fp, "\n\t\tsub");

                    break;

                case MUL:

                    fprintf (ctx->fp, "\n\t\tmul");

                    break;

                case DIV:

                    fprintf (ctx->fp, "\n\t\tdiv");

                    break;

                case SQRT:

                    fprintf (ctx->fp, "\n\t\tsqrt");

                    break;

                case POW:

                    fprintf (ctx->fp, "\n\t\tpow");

                    break;

                case SIN:

                    fprintf (ctx->fp, "\n\t\tsin");

                    break;

                case COS:

                    fprintf (ctx->fp, "\n\t\tcos");

                    break;

                case LN:

                    fprintf (ctx->fp, "\n\t\tln");

                    break;

                case PAT_IN:

                    fprintf (ctx->fp, "\n\t\tin");

                    break;

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

static void PrintfVar (BackEndCtx* ctx)
{
    fprintf (ctx->fp, "$ ");
    for (size_t i = 0; i < ctx->var->data[ctx->node->data->variable].len; i++)
    fprintf (ctx->fp, "%c", ctx->var->data[ctx->node->data->variable].name[i]);
}
