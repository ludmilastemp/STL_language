#include "back-end.h"

static int CompileMultipleOperations (BackEndCtx* ctx);
static int CompileOperation          (BackEndCtx* ctx);
static int CompileFunction           (BackEndCtx* ctx);
static int CompileFuncReturn         (BackEndCtx* ctx);
static int CompilePrintf             (BackEndCtx* ctx);
static int CompileAssign             (BackEndCtx* ctx);
static int CompileConditionOp        (BackEndCtx* ctx);
static int CompileCreate             (BackEndCtx* ctx);
static int CompileExpression         (BackEndCtx* ctx);
                           // сюда тоже IsOpCode и тп

int CompileProgram (BackEndCtx* ctx)
{
    assert (ctx);

    NodeBinTree* oldNode = ctx->node;

    /**
     *  Compile main
     */
    fprintf (ctx->fp, "\n:main");

    ctx->node = oldNode->left;
    CompileMultipleOperations (ctx);

    oldNode = oldNode->right;

    fprintf (ctx->fp, "\n\n\t\thlt");

    /**
     *  Compile functions
     */
    while (oldNode != nullptr)
    {
        fprintf (ctx->fp, "\n\n:func_%d_", oldNode->data->function);

        ctx->node = oldNode->left;
        CompileMultipleOperations (ctx);

        fprintf (ctx->fp, "\n\n\t\tret");

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
        if (oldNode->data->opCode != END_STR)
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
//    printf ("\n\nI in CompileOperation\n");

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

static int CompileFunction (BackEndCtx* ctx)
{
    assert (ctx);

    if (ctx->node->data->type != NodeBinTreeData::T_FUNCTION)
        return ERROR_IN_CompileFunction;

    fprintf (ctx->fp, "\n\n\t\tcall :func_%d_\n", ctx->node->data->function);

    return 0;
}

static int CompileFuncReturn (BackEndCtx* ctx)
{
    assert (ctx);
//    printf ("I in CompileFuncReturn\n");

    if (ctx->node->data->opCode != FUNC_RETURN)
        return ERROR_IN_CompileFuncReturn;

    if (ctx->node->right != nullptr)
    {
        fprintf (ctx->fp, "\n");

        ctx->node = ctx->node->right;
        CompileExpression (ctx);

        fprintf (ctx->fp, "\n\t\tpop rax");
    }

    fprintf (ctx->fp, "\n\n\t\tret");

    return 0;
}

static int CompilePrintf (BackEndCtx* ctx)
{
    assert (ctx);
//    printf ("I in CompilePrintf\n");

    if (ctx->node->data->opCode != PRINTF)
        return ERROR_IN_CompilePrintf;

    ctx->node = ctx->node->right;

    if (CompileExpression (ctx)) return ERROR_IN_CompilePrintf;

    fprintf (ctx->fp, "\n\t\tOUT");

    return 0;
}

static int CompileConditionOp (BackEndCtx* ctx)
{
    assert (ctx);
//    printf ("I in CompileConditionOp\n");

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
        fprintf (ctx->fp, " :if_%d_body"
                     "\n\t\tjmp :else_%d_body", ctx->nIf, ctx->nIf);

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
//    printf ("I in CompileAssign\n");

    if (ctx->node      ->data->opCode   != ASSING ||
        ctx->node->left->data->variable == NodeBinTreeData::VARIABLE_POISON)
        return ERROR_IN_CompileAssign;

    NodeBinTree* oldNode = ctx->node;

    ctx->node = oldNode->right;
    CompileExpression (ctx);

    fprintf (ctx->fp, "\n\t\tpop [%d]\n", oldNode->left->data->variable);

    ctx->node = oldNode;

    return 0;
}

static int CompileCreate (BackEndCtx* ctx)
{
    assert (ctx);
//    printf ("I in CompileCreate\n");

    if (ctx->node->data->opCode != T_INT)
        return ERROR_IN_CompileCreate;

    return 0;
}

static int CompileExpression (BackEndCtx* ctx)
{
    NodeBinTree* oldNode = ctx->node;

    ctx->node = oldNode->left;
    if (ctx->node != nullptr) CompileExpression (ctx);

    ctx->node = oldNode->right;
    if (ctx->node != nullptr) CompileExpression (ctx);

    ctx->node = oldNode;

    switch (ctx->node->data->type)
    {
        case NodeBinTreeData::T_VALUE:

            fprintf (ctx->fp, "\n\t\tpush %d", ctx->node->data->value);

            break;

        case NodeBinTreeData::T_VARIABLE:

            fprintf (ctx->fp, "\n\t\tpush [%d]", ctx->node->data->variable);

            break;

        case NodeBinTreeData::T_FUNCTION:

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
        //
        //        case ADD:
        //
        //            fprintf (ctx->fp, "\n\t\tadd");
        //
        //            break;
        //
        //        case ADD:
        //
        //            fprintf (ctx->fp, "\n\t\tadd");
        //
        //            break;
        //
        //        case ADD:
        //
        //            fprintf (ctx->fp, "\n\t\tadd");
        //
        //            break;
        //
        //        case ADD:
        //
        //            fprintf (ctx->fp, "\n\t\tadd");
        //
        //            break;
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
