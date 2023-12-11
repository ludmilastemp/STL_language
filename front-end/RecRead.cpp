#include "RecRead.h"

/// TO DO: встретилось это, а ожидалось это

typedef NodeBinTree* DATA_T;

static DATA_T GetOperation  (RecursDescent* recDescent);
static DATA_T GetCreate     (RecursDescent* recDescent);
static DATA_T GetPrintf     (RecursDescent* recDescent);
static DATA_T GetAssign     (RecursDescent* recDescent);

static void PassLexicalAnalysis (RecursDescent* recDescent);
//static int  FindVariable        (RecursDescent* recDescent);
static void SkipSpace           (RecursDescent* recDescent);
//static int  StrlenVarName       (const char* const buf);

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

DATA_T GetCreate (RecursDescent* recDescent)
{          /// очень неаккуратно выглядит
    if (recDescent == nullptr) return nullptr;

    int oldPos = recDescent->pos;

    if (dataOpCode != T_INT)
    {
        recDescent->pos = oldPos;
        return nullptr;
    }
    DATA_T node = NODE_CTOR (currentData, nullptr, nullptr);
    if (node == nullptr)
    {
        printf ("ERROR in GetCreate!!!\n\n");
        return nullptr;
    }
    recDescent->pos++;
              // symbolNI
    if (dataOpCode != IN)
    {
        recDescent->pos = oldPos;
        return nullptr;
    }
    recDescent->pos++;

    if (dataVariable == NodeBinTreeData::VARIABLE_POISON)
    {
        recDescent->pos = oldPos;
        return nullptr;
    }
    node->right = NODE_CTOR (currentData, nullptr, nullptr);

    recDescent->pos++;

    return node;
}

DATA_T GetPrintf (RecursDescent* recDescent)
{
    if (recDescent == nullptr) return nullptr;

    if (dataOpCode != PRINTF) return nullptr;

    DATA_T node = NODE_CTOR (currentData, nullptr, nullptr);
    recDescent->pos++;             // no memory

    if (dataVariable != 0) return nullptr;

    node->right = NODE_CTOR (currentData, nullptr, nullptr);
    recDescent->pos++;

    return node;
}                                  // all error

DATA_T GetAssign (RecursDescent* recDescent)
{
    if (recDescent == nullptr) return nullptr;

    if (dataVariable != 0) return nullptr;

    DATA_T node = NODE_CTOR (currentData, nullptr, nullptr);
    recDescent->pos++;

    if (dataOpCode != ASSING) return nullptr;

    node = NODE_CTOR (currentData, node, nullptr);
    recDescent->pos++;

    if (dataType != NodeBinTreeData::T_VALUE) return nullptr;

    node->right = NODE_CTOR (currentData, nullptr, nullptr);
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

        SkipSpace (recDescent);
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

/// кодовое слово
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

    static int lastMAGIC = 0;

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
