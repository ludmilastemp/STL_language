#include "RecRead.h"

/// TO DO: встретилось это, а ожидалось это

static NodeBinTree* GetOperation  (RecursiveDescentCtx* ctx);
static NodeBinTree* GetCreate     (RecursiveDescentCtx* ctx);
static NodeBinTree* GetPrintf     (RecursiveDescentCtx* ctx);
static NodeBinTree* GetAssign     (RecursiveDescentCtx* ctx);
static NodeBinTree* GetIf         (RecursiveDescentCtx* ctx);
static NodeBinTree* GetCondition  (RecursiveDescentCtx* ctx);
static NodeBinTree* GetBodyCicle  (RecursiveDescentCtx* ctx);
static NodeBinTree* GetExspretion (RecursiveDescentCtx* ctx);
static NodeBinTree* GetTerm       (RecursiveDescentCtx* ctx);
static NodeBinTree* GetUnary      (RecursiveDescentCtx* ctx);
static NodeBinTree* GetPrimary    (RecursiveDescentCtx* ctx);
static NodeBinTree* GetNumber     (RecursiveDescentCtx* ctx);

static void LexicalAnalysisPass   (RecursiveDescentCtx* ctx);
static int  FindVariable          (RecursiveDescentCtx* ctx);
static void SkipSpaces            (RecursiveDescentCtx* ctx);
static int  StrlenVarName         (const char* const buf);

//static int NodeDataType          (RecursiveDescentCtx* ctx);
static int NodeDataOpCode         (RecursiveDescentCtx* ctx);
static int NodeDataVariable       (RecursiveDescentCtx* ctx);
static NodeBinTreeData* NodeData  (RecursiveDescentCtx* ctx);

static NodeBinTree* NotThisFunc  (RecursiveDescentCtx* ctx,
                                  const int pos);
static NodeBinTree* NoMemoryError ();

NodeBinTree* GetMultipleOperations (RecursiveDescentCtx* ctx)
{
    if (ctx == nullptr) return nullptr;

    ctx->pos = 0;

    NodeBinTree* node  = GetOperation (ctx);
    NodeBinTree* node2 = nullptr;

    while (ctx->pos < ctx->token->size)
    {
        node2 = GetOperation (ctx);
        if (node2 == nullptr) break;

        node2->right = node;
        node = node2;
    }

    return node;
}

#define NODE_CTOR(data,x,y)  NodeBinTreeCtor (data, x, y)

static NodeBinTree* GetOperation (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    NodeBinTree* node = nullptr;

    node = GetCreate (ctx);
    if (node == nullptr) node = GetPrintf (ctx);
    if (node == nullptr) node = GetIf     (ctx);
    if (node == nullptr) node = GetAssign (ctx);

    if (node == nullptr)
    {
        printf ("\nERROR1 in GetOperation!!!\n\n");
        printf ("\t\tpos    = %d\n\n", ctx->pos);
        printf ("\t\topCode = %d\n\n", NodeDataOpCode (ctx));

                /// Dtor Node - TO DO

        return nullptr;
    }

    if (NodeDataOpCode (ctx) != END_STR)
    {
        printf ("ERROR in GetOperation!!!\n\n");
        printf ("\t\tpos = %d\n\n", ctx->pos);
        return nullptr;
    }

    node = NODE_CTOR (&ctx->token->data[ctx->pos],
                      node, nullptr);
    ctx->pos++;

    return node;
}

static NodeBinTree* GetCreate (RecursiveDescentCtx* ctx)
    /// \Z \ni variable
{
        /// очень неаккуратно выглядит
    assert (ctx);

    int oldPos = ctx->pos;

    if (NodeDataOpCode (ctx) != T_INT) return NotThisFunc (ctx, oldPos);

    NodeBinTree* node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
    if (!node) return NoMemoryError();
    ctx->pos++;

    if (NodeDataOpCode (ctx) != SYMBOL_NI) return NotThisFunc (ctx, oldPos);
    ctx->pos++;

    if (NodeDataVariable (ctx) == NodeBinTreeData::VARIABLE_POISON) return NotThisFunc (ctx, oldPos);

    node->right = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
    if (!node) return NoMemoryError();
    ctx->pos++;

    return node;
}

static NodeBinTree* GetPrintf (RecursiveDescentCtx* ctx)
    /// \gravo variable
{
    assert (ctx);

    int oldPos = ctx->pos;

    if (NodeDataOpCode (ctx) != PRINTF) return NotThisFunc (ctx, oldPos);

    NodeBinTree* node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
    if (!node) return NoMemoryError();
    ctx->pos++;

    if (NodeDataVariable (ctx) == NodeBinTreeData::VARIABLE_POISON) return NotThisFunc (ctx, oldPos);

    node->right = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
    if (!node) return NoMemoryError();
    ctx->pos++;

    return node;
}
          // проверять по следующему токену - TO DO
static NodeBinTree* GetAssign (RecursiveDescentCtx* ctx)
    /// variable ::= eval
{
    assert (ctx);

    int oldPos = ctx->pos;

    if (NodeDataVariable (ctx) == NodeBinTreeData::VARIABLE_POISON) return NotThisFunc (ctx, oldPos);

    NodeBinTree* node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
    if (!node) return NoMemoryError();
    ctx->pos++;

    if (NodeDataOpCode (ctx) != ASSING) return NotThisFunc (ctx, oldPos);

    node = NODE_CTOR (NodeData (ctx), node, nullptr);
    if (!node) return NoMemoryError();
    ctx->pos++;

    node->right = GetExspretion (ctx);

    return node;
}

static NodeBinTree* GetIf (RecursiveDescentCtx* ctx)
    /// Condition <=> Body
{
    assert (ctx);
    printf ("I in GetIf\n"
            "\tpos = %d\n", ctx->pos);

    int oldPos = ctx->pos;

    NodeBinTree* node1 = GetCondition (ctx);

    printf ("\tpos = %d\n", ctx->pos);
    if (node1 == nullptr) return NotThisFunc (ctx, oldPos);

    printf ("\tpos = %d\n", ctx->pos);
    if (NodeDataOpCode (ctx) != IF) return NotThisFunc (ctx, oldPos);

    NodeBinTree* node = NODE_CTOR (NodeData (ctx), node1, nullptr);
    if (!node) return NoMemoryError();

    node->right = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
    if (!node->right) return NoMemoryError();
    ctx->pos++;

    node->right->left = GetBodyCicle (ctx);

    if (node->right->left == nullptr)
    {
        free (node->right);           /// так сделать везде
        free (node);
        return NotThisFunc (ctx, oldPos);
    }

    return node;
}

static NodeBinTree* GetCondition (RecursiveDescentCtx* ctx)
    /// \katarce усл \katapanma
{
    assert (ctx);
    printf ("I in GetCondition\n");

    int oldPos = ctx->pos;

    printf ("\tpos = %d\n", ctx->pos);
    if (NodeDataOpCode (ctx) != BEGIN_INSIDE) return NotThisFunc (ctx, oldPos);
    ctx->pos++;

    NodeBinTree* node1 = GetExspretion (ctx);
    if (node1 == nullptr) return NotThisFunc (ctx, oldPos);

    if (NodeDataOpCode (ctx) != ABOVE       &&
        NodeDataOpCode (ctx) != ABOVE_EQUAL &&
        NodeDataOpCode (ctx) != BELOW       &&
        NodeDataOpCode (ctx) != BELOW_EQUAL &&
        NodeDataOpCode (ctx) != EQUAL       &&
        NodeDataOpCode (ctx) != NO_EQUAL)
        return NotThisFunc (ctx, oldPos);

    NodeBinTree* node = NODE_CTOR (NodeData (ctx), node1, nullptr);
    if (!node) return NoMemoryError();
    ctx->pos++;

    node->right = GetExspretion (ctx);

    printf ("\tpos = %d\n", ctx->pos);
    if (node->right == nullptr)
    {
        free (node);
        return NotThisFunc (ctx, oldPos);
    }
    printf ("\tpos = %d\n", ctx->pos);
    printf ("\topC = %d\n", NodeDataOpCode (ctx));

    if (NodeDataOpCode (ctx) != END_INSIDE) return NotThisFunc (ctx, oldPos);
    ctx->pos++;
    printf ("\tpos = %d\n", ctx->pos);

    return node;
}

static NodeBinTree* GetBodyCicle (RecursiveDescentCtx* ctx)
    /// \katarce multiple operations \katapanma
{
    assert (ctx);

    int oldPos = ctx->pos;

    if (NodeDataOpCode (ctx) != BEGIN_INSIDE) return NotThisFunc (ctx, oldPos);
    ctx->pos++;

    NodeBinTree* node  = GetOperation (ctx);
    if (node == nullptr) return NotThisFunc (ctx, oldPos);

    NodeBinTree* node2 = nullptr;

    while (NodeDataOpCode (ctx) != END_INSIDE &&
           ctx->pos < ctx->token->size)
    {
        node2 = GetOperation (ctx);
        if (node2 == nullptr) break;

        node2->right = node;
        node = node2;
    }

    if (NodeDataOpCode (ctx) != END_INSIDE) return NotThisFunc (ctx, oldPos);
    ctx->pos++;

    return node;
}

static NodeBinTree* GetExspretion (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    NodeBinTree* node = GetTerm (ctx);

    while (NodeDataOpCode (ctx) == ADD ||
           NodeDataOpCode (ctx) == SUB)
    {
        node = NODE_CTOR (NodeData (ctx), node, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;

        node->right = GetTerm (ctx);
    }

    return node;
}

static NodeBinTree* GetTerm (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    NodeBinTree* node = GetUnary (ctx);

    while (NodeDataOpCode (ctx) == MUL ||
           NodeDataOpCode (ctx) == DIV ||
           NodeDataOpCode (ctx) == POW)
    {
        node = NODE_CTOR (NodeData (ctx), node, nullptr);

        ctx->pos++;

        node->right = GetUnary (ctx);
    }

    return node;
}

static NodeBinTree* GetUnary (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    bool i = 0;

    NodeBinTree* node = nullptr;

    if (NodeDataOpCode (ctx) == SIN ||
        NodeDataOpCode (ctx) == COS ||
        NodeDataOpCode (ctx) == LN  ||
        NodeDataOpCode (ctx) == SQRT)
    {
        node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
        ctx->pos++;
        i = true;
    }

    NodeBinTree* node2 = GetPrimary (ctx);

    if (i) node->right = node2;
    else   node = node2;

    return node;
}

static NodeBinTree* GetPrimary (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    if (NodeDataOpCode (ctx) == OPEN_PARENTHESIS)
    {
        ctx->pos++;

        NodeBinTree* node = GetExspretion (ctx);

        if (NodeDataOpCode (ctx) != CLOSE_PARENTHESIS)
        {
            printf ("NO CLOSE PARENTHESIS"
                    "\n\tpos = %d\n", ctx->pos);
            return nullptr;
        }
        ctx->pos++;

        return node;
    }

    return GetNumber (ctx);
}

static NodeBinTree* GetNumber (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    NodeBinTree* node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
    ctx->pos++;

    return node;
}

#undef NODE_CTOR

void
LexicalAnalysis (RecursiveDescentCtx* ctx)
{
    if (ctx == nullptr) return;

    SkipSpaces (ctx);

    while (ctx->str[ctx->pos])
    {
        LexicalAnalysisPass (ctx);

        SkipSpaces (ctx);
    }

    return;
}

static void
LexicalAnalysisPass (RecursiveDescentCtx* ctx)
{
    assert (ctx);
    printf ("\n\nbegin LexicalAnalysisPass"
            "\n\tsize = %d", ctx->token->size);
    printf ("\n\tpos  = %d", ctx->pos);
    printf ("\n\tstr  = %d", ctx->str[ctx->pos]);

    if (ctx->token->size >= 40) assert (false);

    NodeBinTreeData data = { .type     = NodeBinTreeData::TYPE_POISON,
                             .value    = NodeBinTreeData::VALUE_POISON,
                             .opCode   = NodeBinTreeData::OPCODE_POISON,
                             .variable = NodeBinTreeData::VARIABLE_POISON };

    {
        /**
         *  Parse keyword
         */
        for (int i = 0; i < nOperation; i++)
        {
            if (strncmp (ctx->str + ctx->pos, operation[i], lenOperation[i]) == 0)
            {
                printf ("\n\nFind KeyWord: opcode = %d", i);
                data.type   = NodeBinTreeData::T_OPCODE;
                data.opCode = i;
                ctx->pos += lenOperation[i];

                StackPush (ctx->token, data);
                return;
            }
        }
    }

    {
        /**
         *  Pass constant
         */
        int nLen = 0;
        sscanf (ctx->str + ctx->pos, BIN_TREE_DATA_VALUE_PRINT_SPECIFIER "%n", &data.value, &nLen);
        if (nLen)
        {
            data.type = NodeBinTreeData::T_VALUE;
            ctx->pos += nLen;

            StackPush (ctx->token, data);
            return;
        }
    }

    {
        /**
         *  Parse variable
         */
        data.type     = NodeBinTreeData::T_VARIABLE;
        data.variable = FindVariable (ctx);

        StackPush (ctx->token, data);
        return;
    }

    printf ("ERROR in LexicalAnalysisPass\n\n");

    return;
}

static int
FindVariable (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    int nameLen = StrlenVarName (ctx->str + ctx->pos);

    int variable = 0;
    for (; variable < ctx->var->size; variable++)
    {
        if (strncmp (ctx->var->data[variable].name, ctx->str + ctx->pos, nameLen) == 0)
        {
    printf ("\n\nНайдена переменная: \n"
            "\tlen = %d\n"
            "\tname = ", nameLen);
    for (int i = 0; i < nameLen; i++) printf ("%c", ctx->str[ctx->pos]);

            ctx->pos += nameLen;

            return variable;
        }
    }

    Variable var = { .name = ctx->str + ctx->pos, .len = nameLen };

    printf ("\n\nНайдена новая переменная: \n"
            "\tlen = %d\n"
            "\tname = ", nameLen);
    for (int i = 0; i < nameLen; i++) printf ("%c", ctx->str[ctx->pos]);

    StackPush (ctx->var, var);

    ctx->pos += nameLen;

    return variable;
}

/// так как после переменной может не быть пробела,
/// нельзя испольновать sscanf
static int
StrlenVarName (const char* const buf)
{
    int i = 0;     // for
                   // isalpha
    while (true)
    {
        switch (*(buf + i))
        {
            case ')':
            case '(':
            case '{':
            case '}':
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '<':
            case '>':
            case '=':
            case ' ':
            case '\t':
            case '\n':

                return i;

            default:

                break;
        }
        i++;
    }
}

static void
SkipSpaces (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    while (isspace (ctx->str[ctx->pos])) ctx->pos++;

    /// TO DO это все должно быть в отдельной функции
//    static int lastMAGIC = 0; // move it from this place enable/disable
//
//    if (strncmp (ctx->str + ctx->pos, MAGIC, sizeof (MAGIC) - 1) == 0)
//    {
//        lastMAGIC = 0;
//        ctx->pos += sizeof (MAGIC) - 1;
//    }
//    else if (lastMAGIC++ == IHavePawsPeriod)
//    {
//        printf ("\n\n\nI have paws! I don't want to work!!!\n\n");
//        return;
//    }

    while (isspace (ctx->str[ctx->pos])) ctx->pos++;
}
    // in BinTreeStruct.cpp - ?
//static int NodeDataType (RecursiveDescentCtx* ctx)
//{
//    assert (ctx);
//    return ctx->token->data[ctx->pos].type;
//}

static int NodeDataOpCode (RecursiveDescentCtx* ctx)
{
    assert (ctx);
    return ctx->token->data[ctx->pos].opCode;
}

static int NodeDataVariable (RecursiveDescentCtx* ctx)
{
    assert (ctx);
    return ctx->token->data[ctx->pos].variable;
}

static NodeBinTreeData* NodeData (RecursiveDescentCtx* ctx)
{
    assert (ctx);
    return &ctx->token->data[ctx->pos];
}

static NodeBinTree* NoMemoryError ()
{
    printf ("ERROR NO MEMORY!!!\n\n");
    return nullptr;
}

static NodeBinTree* NotThisFunc (RecursiveDescentCtx* ctx,
                                 const int pos)
{
    assert (ctx);
    ctx->pos = pos;
    return nullptr;
}
