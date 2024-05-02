#include "RecRead.h"

/// TO DO: встретилось это, а ожидалось это
/// начать чекать тип

/**
 * Function     := '\oed' MultipleOperations '\lemma' ~nameFunc~
 * MultipleOperations := {Op}+
 * Op           := {CallFunc | FuncReturn | Create | Print | If | Assign } ':)'
 * CallFunc     := ':(' ~func~ [ '>|<' Expression ]* '):'
 * FuncReturn   := '(:' [ Expression ]
 * Create       := '\Z' '\in' ~var~
 * Print        := '\gravo' Expression
 * Assign       := ~var~ '::=' Eval
 * ConditionOp  := Cond ['<=>' '<?=?>'] Body
 * Cond         := '\katarce' Eval                           \
 *                  {['>=' '<=' '>' '<' '==' '!=']}  Eval '\katapanma'
 * Body         := '\katarce' MultipleOperations  '\katapanma'   // no Cicle
 * Expression   := Term  {['+' '-'] Term}*
 * Term         := Unary {['*' '/'] Unary}*
 * Unary        := ['sin' 'cos' 'ln' 'sqrt' ''] Primary
 * Primary      := '('Expression ')' | Number
 * Number       := ['0' - '9']+ | ~var~ | CallFunc
 */        /// no Number

static NodeBinTree* GetFunction             (RecursiveDescentCtx* ctx);
static NodeBinTree* GetMultipleOperations   (RecursiveDescentCtx* ctx);
static NodeBinTree* GetOperation            (RecursiveDescentCtx* ctx);
static NodeBinTree* GetCreate               (RecursiveDescentCtx* ctx);
static NodeBinTree* GetPrintf               (RecursiveDescentCtx* ctx);
static NodeBinTree* GetAssign               (RecursiveDescentCtx* ctx);
static NodeBinTree* GetCallFunc             (RecursiveDescentCtx* ctx);
static NodeBinTree* GetFuncReturn           (RecursiveDescentCtx* ctx);
static NodeBinTree* GetConditionOp          (RecursiveDescentCtx* ctx);
static NodeBinTree* GetCondition            (RecursiveDescentCtx* ctx);
static NodeBinTree* GetBodyCicle            (RecursiveDescentCtx* ctx);
static NodeBinTree* GetExpression           (RecursiveDescentCtx* ctx);
static NodeBinTree* GetTerm                 (RecursiveDescentCtx* ctx);
static NodeBinTree* GetUnary                (RecursiveDescentCtx* ctx);
static NodeBinTree* GetPrimary              (RecursiveDescentCtx* ctx);
static NodeBinTree* GetNumber               (RecursiveDescentCtx* ctx);

static void LexicalAnalysisPass  (RecursiveDescentCtx* ctx);
static void SkipSpaces           (RecursiveDescentCtx* ctx);
static size_t FindFunction       (RecursiveDescentCtx* ctx);
static size_t FindVariable       (RecursiveDescentCtx* ctx);
static size_t FindName           (RecursiveDescentCtx* ctx, Stack_Variable* stk);
static size_t StrlenName         (const char* const buf);

static int  NodeDataType         (RecursiveDescentCtx* ctx);
static int  NodeDataOpCode       (RecursiveDescentCtx* ctx);
static int  NodeDataFunction     (RecursiveDescentCtx* ctx);
static int  NodeDataVariable     (RecursiveDescentCtx* ctx);
static NodeBinTreeData* NodeData (RecursiveDescentCtx* ctx);

static NodeBinTree* NotThisFunc  (RecursiveDescentCtx* ctx,
                                  const size_t pos);
static NodeBinTree* NoMemoryError ();

#define NODE_CTOR(data,x,y)  NodeBinTreeCtor (data, x, y)

NodeBinTree* ParseProgram (RecursiveDescentCtx* ctx)
{
    if (ctx == nullptr) return nullptr;

    ctx->pos = 0;

    NodeBinTree* node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
    if (!node) return NoMemoryError();

    node->data->type     = NodeBinTreeData::T_FUNCTION;
    node->data->function = NodeBinTreeData::FUNCTION_POISON;

    ctx->pos++;

    node->left = GetMultipleOperations (ctx);

    if (NodeDataOpCode (ctx) != MAIN)
    {
        /// free!!!!!!!!
        return nullptr;     
    }
    ctx->pos++;

//    for (int i = 0; i < ctx->func->size; i++)
//    {
//        printf ("size = %d\n", i);
//        for (int j = 0; j < ctx->func->data[i].len; j++)
//            printf ("name = %c\n", ctx->func->data[i].name[j]);
//    }

    NodeBinTree* node1 = node;

    while (ctx->pos < ctx->token->size)
    {
        node1->right = GetFunction (ctx);
        node1 = node1->right;

        if (node1 == nullptr) break;
    }

    for (size_t i = 0; i < ctx->func->size; i++)
    {
        printf ("\n\nsize = %lu\n name = ", i);
        for (size_t j = 0; j < ctx->func->data[i].len; j++)
            printf ("%c", ctx->func->data[i].name[j]);
    }

    return node;
}

static NodeBinTree* GetFunction (RecursiveDescentCtx* ctx)
{
    /**
     * Function := '\oed' MultipleOperations '\lemma' ~nameFunc~
     */
    assert (ctx);

    size_t oldPos = ctx->pos;
    NodeBinTree* node  = nullptr;
    NodeBinTree* node1 = nullptr;

    {
        /**
         * '\oed'
         */
        if (NodeDataOpCode (ctx) != END_FUNCTION) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    {
        /**
         * MultipleOperations
         */
        node1 = GetMultipleOperations (ctx);
        if (node1 == nullptr) return NotThisFunc (ctx, oldPos);

        ctx->pos--;
        node1 = NODE_CTOR (NodeData (ctx), node1, nullptr);
        if (!node1) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         * '\lemma'
         */
        if (NodeDataOpCode (ctx) != BEGIN_FUNCTION) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    {
        /**
         * ~nameFunc~ [ '>|<' Expression ]*
         */
        node = GetCallFunc (ctx);
        if (node == nullptr) return NotThisFunc (ctx, oldPos);
    }

    {
        /**
         * for standart
         */
         node->left        = node1;
         node->left->right = node->right;
         node->right       = nullptr;
    }

    return node;
}

static NodeBinTree* GetMultipleOperations (RecursiveDescentCtx* ctx)
{
    /**
     * MultipleOperations := {Op}+
     */
    assert (ctx);

    NodeBinTree* node = GetOperation (ctx);
    if (node == nullptr) return nullptr;

    NodeBinTree* node2 = nullptr;

    while (NodeDataOpCode (ctx) != MAIN           &&
           NodeDataOpCode (ctx) != BEGIN_FUNCTION &&
           NodeDataOpCode (ctx) != END_FUNCTION   &&
           NodeDataOpCode (ctx) != END_INSIDE)
    {
        node2 = GetOperation (ctx);
        if (node2 == nullptr) break;

        node2->right = node;
        node = node2;
    }

    return node;
}

static NodeBinTree* GetOperation (RecursiveDescentCtx* ctx)
{
    /**
     * Op := { CallFunc | FuncReturn | Create | Print | If | Assign} ':)'
     */
    assert (ctx);

    NodeBinTree* node = nullptr;

    {
        /**
         * CallFunc | FuncReturn | Create | Print | If | Assign
         */
        node = GetCallFunc (ctx);
        if (node == nullptr) node = GetFuncReturn  (ctx);
        if (node == nullptr) node = GetCreate      (ctx);
        if (node == nullptr) node = GetPrintf      (ctx);
        if (node == nullptr) node = GetConditionOp (ctx);
        if (node == nullptr) node = GetAssign      (ctx);

        if (node == nullptr)
        {
            printf ("\nERROR1 in GetOperation!!!\n\n");
            printf ("\t\tpos    = %lu\n\n", ctx->pos);

            printf ("\n\ttype     = %d"
                    "\n\tvalue    = %d"
                    "\n\topCode   = %d"
                    "\n\tvariable = %d"
                    "\n\tfunction = %d\n",
                     ctx->token->data[ctx->pos].type,
                     ctx->token->data[ctx->pos].value,
                     ctx->token->data[ctx->pos].opCode,
                     ctx->token->data[ctx->pos].variable,
                     ctx->token->data[ctx->pos].function);

                    /// Dtor Node - TO DO

            return nullptr;
        }
    }

    {
        /**
         * ':)'
         */
        if (NodeDataOpCode (ctx) != END_OPERATION)
        {
            printf ("ERROR in GetOperation!!!\n\n");
            printf ("\t\tpos = %lu\n\n", ctx->pos);
            return nullptr;
        }

        node = NODE_CTOR (&ctx->token->data[ctx->pos],
                          node, nullptr);
        ctx->pos++;
    }

    return node;
}

static NodeBinTree* GetCallFunc (RecursiveDescentCtx* ctx)
{
    /**
     * CallFunc := ':(' ~func~ [ '>|<' Expression ]* '):'
     */
    assert (ctx);

    size_t oldPos = ctx->pos;
    NodeBinTree* node  = nullptr;
    NodeBinTree* node1 = nullptr;

    {
        /**
         * ':('
         */
        if (NodeDataOpCode (ctx) != BEGIN_PARAM_FUNC) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    {
        /**
         * ~func~
         */
        if (NodeDataFunction (ctx) == NodeBinTreeData::FUNCTION_POISON) return NotThisFunc (ctx, oldPos);

        node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         * [ '>|<' Expression ]*
         */
         node1 = node;

         while (NodeDataOpCode (ctx) != END_PARAM_FUNC)
         {
            /**
             * '>|<'
             */
            if (NodeDataOpCode (ctx) != BORDER_PARAM_FUNC) return NotThisFunc (ctx, oldPos);

            node1->right = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
            if (!node) return NoMemoryError();
            ctx->pos++;

            /**
             * '>|<'  ->  ':)'
             */
            node1->right->data->opCode = END_OPERATION;

            /**
             * Expression
             */
            node1       = node1->right;
            node1->left = GetExpression (ctx);
         }
    }

    {
        /**
         * '):'
         */
        if (NodeDataOpCode (ctx) != END_PARAM_FUNC) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    return node;
}

static NodeBinTree* GetFuncReturn (RecursiveDescentCtx* ctx)
{
    /**
     * FuncReturn := '(:' Expression
     */
    assert (ctx);

    size_t oldPos = ctx->pos;
    NodeBinTree* node  = nullptr;

    {
        /**
         * '(:'
         */
        if (NodeDataOpCode (ctx) != FUNC_RETURN) return NotThisFunc (ctx, oldPos);

        node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         * Expression
         */
        if (NodeDataType (ctx) != END_OPERATION)
        {
            node->right = GetExpression (ctx);
        }
    }

    return node;
}

static NodeBinTree* GetCreate (RecursiveDescentCtx* ctx)
{
    /**
     * Create := '\Z' '\in' ~var~
     */
    assert (ctx);

    size_t oldPos = ctx->pos;

    NodeBinTree* node  = nullptr;

    {
        /**
         * '\Z'
         */
        if (NodeDataOpCode (ctx) != T_INT) return NotThisFunc (ctx, oldPos);

        node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         * '\in'
         */
        if (NodeDataOpCode (ctx) != SYMBOL_NI) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    {
        /**
         * ~var~
         */
        if (NodeDataVariable (ctx) == NodeBinTreeData::VARIABLE_POISON) return NotThisFunc (ctx, oldPos);

        node->right = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;
    }

    return node;
}

static NodeBinTree* GetPrintf (RecursiveDescentCtx* ctx)
{
    /**
     * Print := '\gravo' Expression
     */
    assert (ctx);

    size_t oldPos = ctx->pos;
    NodeBinTree* node  = nullptr;

    {
        /**
         * '\gravo'
         */
        if (NodeDataOpCode (ctx) != PRINTF) return NotThisFunc (ctx, oldPos);

        node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         * Expression
         */
        node->right = GetExpression (ctx);
        if (node->right == nullptr) return NotThisFunc (ctx, oldPos);

        return node;
    }
}
          /// проверять по следующему токену - TO DO
static NodeBinTree* GetAssign (RecursiveDescentCtx* ctx)
{
    /**
     * Assign := ~var~ '::=' Eval
     */
    assert (ctx);

    size_t oldPos = ctx->pos;
    NodeBinTree* node  = nullptr;

    {
        /**
         * ~var~
         */
        if (NodeDataVariable (ctx) == NodeBinTreeData::VARIABLE_POISON) return NotThisFunc (ctx, oldPos);

        node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         * '::='
         */
        if (NodeDataOpCode (ctx) != ASSING) return NotThisFunc (ctx, oldPos);

        node = NODE_CTOR (NodeData (ctx), node, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         *  Eval
         */
        node->right = GetExpression (ctx);
    }

    return node;
}

static NodeBinTree* GetConditionOp (RecursiveDescentCtx* ctx)
{
    /**
     * ConditionOp  := Cond ['<=>' '<?=?>'] Body
     */
    assert (ctx);
//    printf ("I in GetConditionOp\n"
//            "\tpos = %d\n", ctx->pos);

    size_t oldPos = ctx->pos;
    NodeBinTree* node  = nullptr;
    NodeBinTree* node1 = nullptr;

    {
        /**
         * Cond
         */
        node1 = GetCondition (ctx);
        if (node1 == nullptr) return NotThisFunc (ctx, oldPos);
    }

    {
        /**
         * ['<=>' '<?=?>']
         */
        if (NodeDataOpCode (ctx) != IF &&
            NodeDataOpCode (ctx) != WHILE) return NotThisFunc (ctx, oldPos);

        node = NODE_CTOR (NodeData (ctx), node1, nullptr);
        if (!node) return NoMemoryError();

        node->right = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
        if (!node->right) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         * Body
         */
        node->right->left = GetBodyCicle (ctx);

        if (node->right->left == nullptr)
        {
            free (node->right);           /// так сделать везде
            free (node);
            return NotThisFunc (ctx, oldPos);
        }
    }

    return node;
}

static NodeBinTree* GetCondition (RecursiveDescentCtx* ctx)
{
    /**
     * Cond := '\katarce' Eval
     *       {['>=' '<=' '>' '<' '==' '!=']}  Eval '\katapanma'
     */
    assert (ctx);
//    printf ("I in GetCondition\n");

    size_t oldPos = ctx->pos;
    NodeBinTree* node  = nullptr;
    NodeBinTree* node1 = nullptr;

    {
        /**
         * '\katarce'
         */
        if (NodeDataOpCode (ctx) != BEGIN_INSIDE) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    {
        /**
         * Eval
         */
        node1 = GetExpression (ctx);
        if (node1 == nullptr) return NotThisFunc (ctx, oldPos);
    }

    {
        /**
         * {['>' '>=' '<' '<=' '==' '!=']}
         */
        if (NodeDataOpCode (ctx) != ABOVE       &&
            NodeDataOpCode (ctx) != ABOVE_EQUAL &&
            NodeDataOpCode (ctx) != BELOW       &&
            NodeDataOpCode (ctx) != BELOW_EQUAL &&
            NodeDataOpCode (ctx) != EQUAL       &&
            NodeDataOpCode (ctx) != NO_EQUAL)
            return NotThisFunc (ctx, oldPos);

        node = NODE_CTOR (NodeData (ctx), node1, nullptr);
        if (!node) return NoMemoryError();
        ctx->pos++;
    }

    {
        /**
         * Eval
         */
        node->right = GetExpression (ctx);

        if (node->right == nullptr)
        {
            free (node);
            return NotThisFunc (ctx, oldPos);
        }
    }

    {
        /**
         * '\katapanma'
         */
        if (NodeDataOpCode (ctx) != END_INSIDE) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    return node;
}

static NodeBinTree* GetBodyCicle (RecursiveDescentCtx* ctx)
{
    /**
     * Body := '\katarce' MultipleOperations  '\katapanma'
     */
    assert (ctx);

    size_t oldPos = ctx->pos;
    NodeBinTree* node  = nullptr;

    {
        /**
         * '\katarce'
         */
        if (NodeDataOpCode (ctx) != BEGIN_INSIDE) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    {
        /**
         * MultipleOperations
         */
        node = GetMultipleOperations (ctx);
        if (node == nullptr) return NotThisFunc (ctx, oldPos);
    }

    {
        /**
         * '\katapanma'
         */
        if (NodeDataOpCode (ctx) != END_INSIDE) return NotThisFunc (ctx, oldPos);
        ctx->pos++;
    }

    return node;
}

static NodeBinTree* GetExpression (RecursiveDescentCtx* ctx)
{
    /**
     * Expression := Term  {['+' '-'] Term}*
     */
    assert (ctx);

    NodeBinTree* node  = nullptr;

    {
        /**
         * Term
         */
        node = GetTerm (ctx);
    }

    {
        /**
         * {['+' '-'] Term}*
         */
        while (NodeDataOpCode (ctx) == ADD ||
               NodeDataOpCode (ctx) == SUB)
        {
            node = NODE_CTOR (NodeData (ctx), node, nullptr);
            if (!node) return NoMemoryError();
            ctx->pos++;

            node->right = GetTerm (ctx);
        }
    }

    return node;
}

static NodeBinTree* GetTerm (RecursiveDescentCtx* ctx)
{
    /**
     * Term := Unary {['*' '/'] Unary}*
     */
    assert (ctx);

    NodeBinTree* node = nullptr;

    {
        /**
         * Unary
         */
        node = GetUnary (ctx);
    }

    {
        /**
         * {['*' '/'] Unary}*
         */
        while (NodeDataOpCode (ctx) == MUL ||
               NodeDataOpCode (ctx) == DIV ||
               NodeDataOpCode (ctx) == POW)
        {
            node = NODE_CTOR (NodeData (ctx), node, nullptr);

            ctx->pos++;

            node->right = GetUnary (ctx);
        }
    }

    return node;
}

static NodeBinTree* GetUnary (RecursiveDescentCtx* ctx)
{
    /**
     * Unary := ['sin' 'cos' 'ln' 'sqrt' ''] Primary
     */
    assert (ctx);

    bool isUnaryOperation = 0;

    NodeBinTree* node = nullptr;
    NodeBinTree* node1 = nullptr;

    {
        /**
         * 'sin' 'cos' 'ln' 'sqrt' Primary
         */
        if (NodeDataOpCode (ctx) == SIN ||
            NodeDataOpCode (ctx) == COS ||
            NodeDataOpCode (ctx) == LN  ||
            NodeDataOpCode (ctx) == SQRT)
        {
            node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
            ctx->pos++;
            isUnaryOperation = true;
        }
    }

    {
        /**
         * Primary
         */
        node1 = GetPrimary (ctx);
    }

    if (isUnaryOperation) node->right = node1;
    else                  node = node1;

    return node;
}

static NodeBinTree* GetPrimary (RecursiveDescentCtx* ctx)
{
    /**
     * Primary := '('Expression ')' | Number
     */
    assert (ctx);

    {
        /**
         * '('
         */
        if (NodeDataOpCode (ctx) == OPEN_PARENTHESIS)
        {
            ctx->pos++;

            NodeBinTree* node = nullptr;

            {
                /**
                 * Expression
                 */
                node = GetExpression (ctx);
            }

            {
                /**
                 * ')'
                 */
                if (NodeDataOpCode (ctx) != CLOSE_PARENTHESIS)
                {
                    printf ("NO CLOSE PARENTHESIS"
                            "\n\tpos = %lu\n", ctx->pos);
                    return nullptr;
                }
                ctx->pos++;
            }

            return node;
        }
    }

    /**
     * Number
     */
    return GetNumber (ctx);
}

static NodeBinTree* GetNumber (RecursiveDescentCtx* ctx)
{
    /**
     * Number := ['0' - '9']+ | ~var~ | CallFunc
     */
    assert (ctx);

    {
        /**
         * ['0' - '9']+ | ~var~
         */
        if (NodeDataType (ctx) == NodeBinTreeData::T_VALUE ||
            NodeDataType (ctx) == NodeBinTreeData::T_VARIABLE)
        {
            NodeBinTree* node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
            ctx->pos++;

            return node;
        }
    }

    {
        /**
         *  '\pat'
         */
        if (NodeDataOpCode (ctx) == PAT_IN)
        {
            NodeBinTree* node = NODE_CTOR (NodeData (ctx), nullptr, nullptr);
            ctx->pos++;

            return node;
        }
    }

    {
        /**
         * CallFunc
         */
        return GetCallFunc (ctx);
    }
}

#undef NODE_CTOR

void
LexicalAnalysis (RecursiveDescentCtx* ctx)
{
    if (ctx == nullptr) return;

    SkipSpaces (ctx);

    size_t oldPos = 0;

    while (ctx->str[ctx->pos])
    {
        oldPos = ctx->pos;

        LexicalAnalysisPass (ctx);

        if (ctx->pos == oldPos)
        {
            printf ("Лексер закончил в if\n\n");
            return;
        }

        SkipSpaces (ctx);
    }

    return;
}

static void // в отдельный файл
LexicalAnalysisPass (RecursiveDescentCtx* ctx)
{
    assert (ctx);
//    printf ("\n\nbegin LexicalAnalysisPass"
//            "\n\tsize = %d", ctx->token->size);
//    printf ("\n\tpos  = %d", ctx->pos);
//    printf ("\n\tstr  = %d", ctx->str[ctx->pos]);
//
//    if (ctx->token->size >= 60) assert (false);

    NodeBinTreeData data = { .type     = NodeBinTreeData::TYPE_POISON,
                             .value    = NodeBinTreeData::VALUE_POISON,
                             .opCode   = NodeBinTreeData::OPCODE_POISON,
                             .variable = NodeBinTreeData::VARIABLE_POISON,
                             .function = NodeBinTreeData::FUNCTION_POISON,
                             .freeData = false}; // callocData
    {
        /**
         *  Parse keyword
         */
        for (int i = 0; i < nkeyWords; i++)
        {
            if (strncmp (ctx->str + ctx->pos, keyWords[i], lenKeyWords[i]) == 0)
            {
                data.type   = NodeBinTreeData::T_OPCODE;
                data.opCode = i;
                ctx->pos += lenKeyWords[i];

                /**
                 *  Parse comment
                 */
                if (i == BEGIN_COMMENT)
                {
                    while (strncmp (ctx->str + ctx->pos,
                                    keyWords[END_COMMENT],
                                    lenKeyWords[END_COMMENT]) != 0)
                        ctx->pos++;
                    ctx->pos += lenKeyWords[END_COMMENT];

                    return;
                }

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
            ctx->pos += (size_t)nLen;

            StackPush (ctx->token, data);
            return;
        }
    }

    {
        /**
         *  Parse function name
         */
        if (ctx->token->size >= 1 &&
            (ctx->token->data[ctx->token->size - 1].opCode == BEGIN_FUNCTION ||
             ctx->token->data[ctx->token->size - 1].opCode == BEGIN_PARAM_FUNC))
        {
            data.type     = NodeBinTreeData::T_FUNCTION;
            data.function = (int)FindFunction (ctx);

            StackPush (ctx->token, data);
            return;
        }
    }

    {
        /**
         *  Parse variable
         */
        data.type     = NodeBinTreeData::T_VARIABLE;
        data.variable = (int)FindVariable (ctx);

        StackPush (ctx->token, data);
        return;
    }


    printf ("ERROR in LexicalAnalysisPass\n\n");

    return;
}

static size_t
FindVariable (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    return FindName (ctx, ctx->var);
}

static size_t
FindFunction (RecursiveDescentCtx* ctx)
{
    assert (ctx);

    return FindName (ctx, ctx->func);
}

/// функция поиска переменной или функции по имени
static size_t
FindName (RecursiveDescentCtx* ctx, Stack_Variable* stk)
{
    assert (ctx);
                       /// no var
    size_t nameLen = StrlenName (ctx->str + ctx->pos);

    size_t elem = 0;
    for (; elem < stk->size; elem++)
    {
        if (strncmp (stk->data[elem].name, ctx->str + ctx->pos, nameLen) == 0)
        {
            ctx->pos += nameLen;

            return elem;
        }
    }

    Variable data = { .name = ctx->str + ctx->pos, .len = nameLen };

    StackPush (stk, data);

    ctx->pos += nameLen;

    return elem;
}

/// так как после переменной может не быть пробела,
/// нельзя испольновать sscanf
static size_t
StrlenName (const char* const buf)
{
    size_t i = 0;     // for
                   // isalpha
    while (true)
    {
        switch (*(buf + i))
        {
            case ')':
            case '(':
            case '{':
            case '}':
            case ':':
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '<':
            case '>':
            case '=':
            case ' ':
            case '\r':
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
static int NodeDataType (RecursiveDescentCtx* ctx)
{
    assert (ctx);
    return ctx->token->data[ctx->pos].type;
}

static int NodeDataOpCode (RecursiveDescentCtx* ctx)
{
    assert (ctx);
    return ctx->token->data[ctx->pos].opCode;
}

static int NodeDataFunction (RecursiveDescentCtx* ctx)
{
    assert (ctx);
    return ctx->token->data[ctx->pos].function;
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
                                 const size_t pos)
{
    assert (ctx);
    ctx->pos = pos;
    return nullptr;
}
