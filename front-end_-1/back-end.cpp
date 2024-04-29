#include "back-end.h"

static int CompileMultipleOperations  (BackEndCtx* ctx);
static int CompileOperation           (BackEndCtx* ctx);
static int CompileActualArguments     (BackEndCtx* ctx);
static int CompileFunctionDeclaration (BackEndCtx* ctx);
static int CompileFunction            (BackEndCtx* ctx);
static int CompileFuncReturn          (BackEndCtx* ctx);
static int CompilePrintf              (BackEndCtx* ctx);
static int CompileAssign              (BackEndCtx* ctx);
static int CompileConditionOp         (BackEndCtx* ctx);
static int CompileCreate              (BackEndCtx* ctx);
static int CompileExpression          (BackEndCtx* ctx);

int CompileProgram (BackEndCtx* ctx)
{
    assert (ctx);

    NodeBinTree* oldNode = ctx->node;

    /**
     *  Compile main
     */
    fprintf (ctx->fp, "\\VCEGO_XOROSHEGO");

    ctx->node = oldNode->left;

    fprintf (ctx->fp, "  \n");

    CompileMultipleOperations (ctx);

    fprintf (ctx->fp, "\n\\teornma");

    printf ("\n\n\n\nMAIN\n\n\n");

    oldNode = oldNode->right;

    /**
     *  Compile functions
     */
    while (oldNode != nullptr)
    {
        fprintf (ctx->fp, "\n\n%s\n", keyWords [END_FUNCTION]);

        /**
         *  Compile function
         */
        ctx->node = oldNode->left->left;
        CompileMultipleOperations (ctx);

        /**
         *  Compile declaration
         */
        ctx->node = oldNode;
        CompileFunctionDeclaration (ctx);

        oldNode = oldNode->right;
    }

    return 0;
}

static int CompileMultipleOperations (BackEndCtx* ctx)
{
    assert (ctx);

    NodeBinTree* oldNode = ctx->node;

    if (oldNode->data->opCode != END_OPERATION)
    {
        printf ("ERROR in CompileMultipleOperations!!!\n\n");
        return ERROR_IN_CompileMultipleOperations;
    }

    if (ctx->node->right != nullptr)
    {
        ctx->node = ctx->node->right;

        CompileMultipleOperations (ctx);
    }

    fprintf (ctx->fp, "\n\t");

    ctx->node = oldNode->left;

    CompileOperation (ctx);

    fprintf (ctx->fp, " :)\n");

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

        printf ("\n\ttype     = %d"
                "\n\tvalue    = %d"
                "\n\topCode   = %d"
                "\n\tvariable = %d"
                "\n\tfunction = %d",
                 ctx->node->data->type,
                 ctx->node->data->value,
                 ctx->node->data->opCode,
                 ctx->node->data->variable,
                 ctx->node->data->function);
        return ERROR_IN_CompileOperation;
    }

    printf ("I end CompileOperation\n\n\n");

    return 0;
}

static int CompileFunctionDeclaration (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileFormalArguments\n");

    if (ctx->node->data->type != NodeBinTreeData::T_FUNCTION)
        return ERROR_IN_CompileArguments;

    fprintf (ctx->fp, "\n%s", keyWords[BEGIN_FUNCTION]);
    fprintf (ctx->fp, " %s",  keyWords[BEGIN_PARAM_FUNC]);

    fprintf (ctx->fp, " Func_%d ", ctx->node->data->function);

    ctx->node = ctx->node->left->right;

    CompileActualArguments (ctx);

    fprintf (ctx->fp, " %s", keyWords[END_PARAM_FUNC]);

    return 0;
}

static int CompileActualArguments (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileActualArguments\n");

    NodeBinTree* oldNode = nullptr;

    while (ctx->node != nullptr)
    {
        if (ctx->node->data->opCode != END_OPERATION)
            return ERROR_IN_CompileFunction;

        fprintf (ctx->fp, " %s ", keyWords[BORDER_PARAM_FUNC]);

        oldNode   = ctx->node;
        ctx->node = ctx->node->left;

        if (CompileExpression (ctx))
            return ERROR_IN_CompileFunction;

        ctx->node = oldNode->right;
    }

    return 0;
}

//static int CompileVariableBeforeCall (BackEndCtx* ctx)
//{
//    assert (ctx);
//    printf ("I in CompileVariableBeforeCall\n");
//
//    for (int i = ctx->nVarBefore; i <= ctx->nVarInFunc; i++)
//    {
//        fprintf (ctx->fp, "\n\tpush [%d]", i);
//    }
//
//    fprintf (ctx->fp, "\n");
//
//    return 0;
//}
//
//static int CompileVariableAfterCall (BackEndCtx* ctx)
//{
//    assert (ctx);
//    printf ("I in CompileVariableAfterCall\n");
//
//    for (int i = ctx->nVarInFunc; i >= ctx->nVarBefore; i--)
//    {
//        fprintf (ctx->fp, "\n\tpop [%d]", i);
//    }
//
//    fprintf (ctx->fp, "\n");
//
//    return 0;
//}

static int CompileFunction (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileFunction\n");

    if (ctx->node->data->type != NodeBinTreeData::T_FUNCTION)
        return ERROR_IN_CompileFunction;

    fprintf (ctx->fp, " %s",  keyWords[BEGIN_PARAM_FUNC]);

    fprintf (ctx->fp, " Func_%d ", ctx->node->data->function);

    ctx->node = ctx->node->right;
    CompileActualArguments (ctx);

    fprintf (ctx->fp, "%s ", keyWords[END_PARAM_FUNC]);

    return 0;
}

static int CompileFuncReturn (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileFuncReturn\n");

    if (ctx->node->data->opCode != FUNC_RETURN)
        return ERROR_IN_CompileFuncReturn;

    fprintf (ctx->fp, "%s ", keyWords[FUNC_RETURN]);

    ctx->node = ctx->node->right;

    if (ctx->node != nullptr)
        CompileExpression (ctx);

    return 0;
}

static int CompilePrintf (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompilePrintf\n");

    if (ctx->node->data->opCode != PRINTF)
        return ERROR_IN_CompilePrintf;

    fprintf (ctx->fp, "%s ", keyWords [PRINTF]);

    ctx->node = ctx->node->right;

    if (CompileExpression (ctx)) return ERROR_IN_CompilePrintf;

    return 0;
}

static int CompileConditionOp (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileConditionOp\n");

    if (ctx->node->data->opCode != IF &&
        ctx->node->data->opCode != WHILE)
        return ERROR_IN_CompileIf;

    NodeBinTree* oldNode = ctx->node;

    fprintf (ctx->fp, "%s\n\t\t", keyWords[BEGIN_INSIDE]);

    ctx->node = oldNode->left->left;
    CompileExpression (ctx);

    fprintf (ctx->fp, " %s ", keyWords[oldNode->left->data->opCode]);

    ctx->node = oldNode->left->right;
    CompileExpression (ctx);

    fprintf (ctx->fp, "\n\t%s\n\t\t", keyWords[END_INSIDE]);

    fprintf (ctx->fp, "%s", keyWords[oldNode->data->opCode]);

    fprintf (ctx->fp, "\n\t%s\t\t\t", keyWords[BEGIN_INSIDE]);

    ctx->node = oldNode->right->left;
    CompileMultipleOperations (ctx);

    fprintf (ctx->fp, "\t%s", keyWords[END_INSIDE]);

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

    fprintf (ctx->fp, "var%d ", oldNode->left->data->variable);
    fprintf (ctx->fp, "%s", keyWords[ASSING]);

    ctx->node = oldNode->right;
    CompileExpression (ctx);

    printf ("I end CompileAssign\n");
    return 0;
}

static int CompileCreate (BackEndCtx* ctx)
{
    assert (ctx);
    printf ("I in CompileCreate\n");

    if (ctx->node->data->opCode != T_INT)
        return ERROR_IN_CompileCreate;

    fprintf (ctx->fp, "%s ", keyWords[T_INT]);
    fprintf (ctx->fp, "%s ",     keyWords[SYMBOL_NI]);
    fprintf (ctx->fp, "var%d ", ctx->node->right->data->variable);

    return 0;
}

static int CompileExpression (BackEndCtx* ctx)
{
    NodeBinTree* oldNode = ctx->node;

//*
        printf ("\n\ttype     = %d"
                "\n\tvalue    = %d"
                "\n\topCode   = %d"
                "\n\tvariable = %d"
                "\n\tfunction = %d\n",
                 ctx->node->data->type,
                 ctx->node->data->value,
                 ctx->node->data->opCode,
                 ctx->node->data->variable,
                 ctx->node->data->function);
//*/

    switch (ctx->node->data->type)
    {
        case NodeBinTreeData::T_VALUE:

            fprintf (ctx->fp, " %d ", ctx->node->data->value);

            break;

        case NodeBinTreeData::T_VARIABLE:

            fprintf (ctx->fp, " var%d ", ctx->node->data->variable);

            break;

        case NodeBinTreeData::T_FUNCTION:

            CompileFunction (ctx);

            break;

        case NodeBinTreeData::T_OPCODE:

            switch (ctx->node->data->opCode)
            {
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case POW:

                    ctx->node = oldNode->left;
                    if (ctx->node != nullptr)
                        CompileExpression (ctx);

                    ctx->node = oldNode;

                    break;

                case SQRT:
                case SIN:
                case COS:
                case LN:

                    fprintf (ctx->fp, " ");

                    break;

                default:
                    printf ("ERROR in CompileExpression\n"
                            "opCode = %d\n", ctx->node->data->opCode);
            }

            switch (ctx->node->data->opCode)
            {
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case SQRT:
                case POW:
                case SIN:
                case COS:
                case LN:

                    fprintf (ctx->fp, "%s", keyWords [ctx->node->data->opCode]);

                    break;

//                case ADD:
//
//                    fprintf (ctx->fp, "\n\t\tadd");
//
//                    break;

                default:
                    printf ("ERROR in CompileExpression\n"
                            "opCode = %d\n", ctx->node->data->opCode);
            }

            ctx->node = oldNode->right;
            if (ctx->node != nullptr) CompileExpression (ctx);

            break;

        default:
            printf ("ERROR in CompileExpression\n"
                    "type = %d\n", ctx->node->data->type);
    }

    return 0;
}
