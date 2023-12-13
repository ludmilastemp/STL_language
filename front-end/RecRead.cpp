#include "RecRead.h"

/// TO DO: встретилось это, а ожидалось это

typedef NodeBinTree* DATA_T; // Node

// Context             ctx
//static DATA_T GetOperation  (RecursiveDescentCtx* ctx);
static DATA_T GetOperation  (RecursDescent* ctx);
static DATA_T GetCreate     (RecursDescent* recDescent);
static DATA_T GetPrintf     (RecursDescent* recDescent);
static DATA_T GetAssign     (RecursDescent* recDescent);

static void PassLexicalAnalysis (RecursDescent* recDescent); //pass in end
//static int  FindVariable        (RecursDescent* recDescent);
//static int  StrlenVarName       (const char* const buf);
static void SkipSpace           (RecursDescent* recDescent);

DATA_T GetMultipleOperations (RecursDescent* recDescent)
{
    if (recDescent == nullptr) return nullptr;

    DATA_T node  = GetOperation (recDescent);
    DATA_T node2 = nullptr;

    while (recDescent->pos < recDescent->token->size)
    {
        node2 = GetOperation (recDescent);
        if (node2 == nullptr) break;

        node2->right = node;
        node = node2;
    }

    return node;
}

// small hren
#define NODE_CTOR(data,x,y)  NodeBinTreeCtor (data, x, y)
#define dataType             recDescent->token->data[recDescent->pos].type
#define dataOpCode           recDescent->token->data[recDescent->pos].opCode
#define dataVariable         recDescent->token->data[recDescent->pos].variable
#define currentData          &recDescent->token->data[recDescent->pos]

DATA_T GetOperation (RecursDescent* recDescent)
{
    if (recDescent == nullptr) return nullptr;

    DATA_T node = nullptr;

    node = GetCreate (recDescent);
    if (node == nullptr) node = GetPrintf (recDescent);
    if (node == nullptr) node = GetAssign (recDescent);

    if (node == nullptr)
    {
        printf ("\nERROR1 in GetOperation!!!\n\n");
        printf ("\t\tpos = %d\n\n", recDescent->pos);

        return nullptr;
    }

    if (dataOpCode != END_STR)
    {
        printf ("ERROR2 in GetOperation!!!\n\n");
        return nullptr;
    }

    node = NODE_CTOR (&recDescent->token->data[recDescent->pos],
                      node, nullptr);
    recDescent->pos++;

    return node;
}

//hren
#define assertMemory(x)  if (x == nullptr)                         \
                         {                                         \
                             printf ("ERROR NO MEMORY!!!\n\n");    \
                             return nullptr;                       \
                         }

// hren
#define NotThisFunc      {                                         \
                         recDescent->pos = oldPos;                 \
                         return nullptr;                           \
                         }

DATA_T GetCreate (RecursDescent* recDescent)
/// \Z \ni variable
{
        /// очень неаккуратно выглядит
    if (recDescent == nullptr) return nullptr;

    int oldPos = recDescent->pos;

    if (dataOpCode != T_INT) NotThisFunc;

    DATA_T node = NODE_CTOR (currentData, nullptr, nullptr);
    assertMemory (node);/////!!!! RETURN_ON_NULL
//    if ( !node ) return sytax_error(here);   <- function

    recDescent->pos++;

    if (dataOpCode != SYMBOL_NI) NotThisFunc;
    recDescent->pos++;

    if (dataVariable == NodeBinTreeData::VARIABLE_POISON) NotThisFunc;

    node->right = NODE_CTOR (currentData, nullptr, nullptr);
    assertMemory (node);

    recDescent->pos++;

    return node;
}

DATA_T GetPrintf (RecursDescent* recDescent)
/// \gravo variable
{
    if (recDescent == nullptr) return nullptr;

    int oldPos = recDescent->pos;

    if (dataOpCode != PRINTF) NotThisFunc;

    DATA_T node = NODE_CTOR (currentData, nullptr, nullptr);
    assertMemory (node);
    recDescent->pos++;

    if (dataVariable != 0) NotThisFunc;

    node->right = NODE_CTOR (currentData, nullptr, nullptr);
    assertMemory (node);
    recDescent->pos++;

    return node;
}

DATA_T GetAssign (RecursDescent* recDescent)
/// variable ::= value
{
    if (recDescent == nullptr) return nullptr;

    int oldPos = recDescent->pos;

    if (dataVariable != 0) NotThisFunc;

    DATA_T node = NODE_CTOR (currentData, nullptr, nullptr);
    assertMemory (node);
    recDescent->pos++;

    if (dataOpCode != ASSING) NotThisFunc;

    node = NODE_CTOR (currentData, node, nullptr);
    assertMemory (node);
    recDescent->pos++;

    if (dataType != NodeBinTreeData::T_VALUE) NotThisFunc;

    node->right = NODE_CTOR (currentData, nullptr, nullptr);
    assertMemory (node);
    recDescent->pos++;

    return node;
}

#undef NODE_CTOR

void
LexicalAnalysis (RecursDescent* recDescent)
{
    if (recDescent == nullptr) return;

    SkipSpace (recDescent);

    while (recDescent->str[recDescent->pos])
    {
        PassLexicalAnalysis (recDescent);

        SkipSpace/*s*/ (recDescent);
    }

    return;
}

static void
PassLexicalAnalysis (RecursDescent* recDescent)
{
    if (recDescent == nullptr) return;

    NodeBinTreeData data = { .type     = NodeBinTreeData::TYPE_POISON,
                             .value    = NodeBinTreeData::VALUE_POISON,
                             .opCode   = NodeBinTreeData::OPCODE_POISON,
                             .variable = NodeBinTreeData::VARIABLE_POISON };

    {
        /**
         *
         */
         //
        /// Get keyword/parse keyword
       /////
        for (int i = 0; i < nOperation; i++)
        {
            if (strncmp (recDescent->str + recDescent->pos, operation[i], lenOperation[i]) == 0)
            {
                data.type   = NodeBinTreeData::T_OPCODE;
                data.opCode = i;
                recDescent->pos += lenOperation[i];

                StackPush (recDescent->token, data);
                return;
            }
        }
    }

    /// константа
    int nLen = 0;
    sscanf (recDescent->str + recDescent->pos, BIN_TREE_DATA_VALUE_PRINT_SPECIFIER "%n", &data.value, &nLen);
    if (nLen)
    {
        data.type = NodeBinTreeData::T_VALUE;
        recDescent->pos += nLen;

        StackPush (recDescent->token, data);
        return;
    }

/// переменная
    if (recDescent->str [recDescent->pos] == 'x')
    {
        data.type     = NodeBinTreeData::T_VARIABLE;
        data.variable = 0;
        recDescent->pos++;

        StackPush (recDescent->token, data);
        return;
    }
//    data.variable = FindVariable (recDescent);

    printf ("ERROR in PassLexicalAnalysis\n\n");

    return;
}
                 /*
static int
FindVariable (RecursDescent* recDescent)
{
    if (recDescent == nullptr) return 0;

    int nameLen = StrlenVarName (recDescent->str + recDescent->pos);

    int variable = 0;
    for (; variable < recDescent->var->size; variable++)
    {
        if (strncmp (recDescent->var->data[variable].name, recDescent->str + recDescent->pos, nameLen) == 0)
        {
            recDescent->pos += nameLen;

            return variable;
        }
    }

    Variable var = { .name = recDescent->str + recDescent->pos, .value = 0 };

    StackPush (recDescent->var, var);

    recDescent->pos += nameLen;

    return variable;
}

/// так как после переменной может не быть пробела,
/// нельзя испольновать sscanf
static int
StrlenVarName (const char* const buf)
{
    int i = 0;     // for
    while (true)
    {
        switch (*(buf + i))
        {
            case ')':
            case '(':
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case ' ':
            case SIN:
            case COS:
            case LN:
            {
                return i;
            }
        }
        i++;
    }
}*/

static void
SkipSpace (RecursDescent* recDescent)
{
    if (recDescent == nullptr) return;

    while (isspace (recDescent->str[recDescent->pos])) recDescent->pos++;

    static int lastMAGIC = 0; // move it from this place enable/disable

    if (strncmp (recDescent->str + recDescent->pos, MAGIC, sizeof (MAGIC) - 1) == 0)
    {
        lastMAGIC = 0;
        recDescent->pos += sizeof (MAGIC) - 1;
    }
    else if (lastMAGIC++ == IHavePaws)
    {
        printf ("\n\n\nI have paws! I don't want to work!!!\n\n");
        return;
    }

    while (isspace (recDescent->str[recDescent->pos])) recDescent->pos++;
}
