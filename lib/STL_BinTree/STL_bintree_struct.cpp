#include "STL_bintree_struct.h"

static int
SubtreePrintPostorderWithoutNil (NodeBinTree* node,
                                 FILE* fp);

static NodeBinTree*
NodeBinTreeCtorPostorderWithoutNil  (char* const buf,
                                     int* len);

static NodeBinTreeData*
ReadDataNodeBinTree (char* const buf,
                     int* len);

static NodeBinTree*
ReadBranchNodeBinTreeWithoutNil (char* const buf,
                                 int* len);

//static int
//FindVariable (char* const buf, int* len, Stack_Variable* stk);

//static int
//StrlenVarName (char* buf);

NodeBinTree*
NodeBinTreeCtor (NodeBinTreeData* data,
                 NodeBinTree* left,
                 NodeBinTree* right,
                 NodeBinTree* parent,
                 BinTree*     binTree)
{
    NodeBinTree* node = (NodeBinTree*) calloc (1, sizeof (NodeBinTree));
    if (node == nullptr)
    {
        printf ("ERROR_NO_MEMORY\n");
        return nullptr;
    }

    node->data   = data;
    node->left   = left;
    node->right  = right;
    node->parent = parent;

    if (binTree != nullptr) binTree->size++;

    return node;
}

NodeBinTree*
NodeBinTreeDtor (NodeBinTree* node)
{
    if (node == nullptr) return nullptr;

    NodeBinTreeDtor (node->left);
    NodeBinTreeDtor (node->right);

    node->data->type     = NodeBinTreeData::TYPE_POISON;
    node->data->value    = NodeBinTreeData::VALUE_POISON;
    node->data->opCode   = NodeBinTreeData::OPCODE_POISON;
    node->data->variable = NodeBinTreeData::VARIABLE_POISON;
    node->data   = nullptr;            // free data?
    node->left   = nullptr;
    node->right  = nullptr;
    node->parent = nullptr;

    free (node);

    return nullptr;
}

BinTree*
BinTreeCtor (NodeBinTree* root, int size, char* buf)
{
    BinTree* binTree = (BinTree*) calloc (1, sizeof (BinTree));
    if (binTree == nullptr)
    {
        printf ("ERROR_NOT_MEMORY\n");
        return nullptr;
    }

//    binTree->variable = (Stack_Variable*) calloc (1, sizeof (Stack_Variable));
//    if (binTree->variable == nullptr)
//    {
//        printf ("ERROR_NOT_MEMORY\n");
//        return nullptr;
//    }
//    StackCtor (binTree->variable);

    binTree->root = root;
    binTree->size = size;
    binTree->buf  = buf;

    return binTree;
}

BinTree*
BinTreeDtor (BinTree* binTree)
{
    if (binTree == nullptr) return nullptr;

    NodeBinTreeDtor (binTree->root);
    if (binTree->buf      != nullptr) free (binTree->buf);
//    if (binTree->variable != nullptr) StackDtor (binTree->variable);

    binTree->root = nullptr;
    binTree->buf  = nullptr;
    binTree->size = 0;

    return nullptr;
}

NodeBinTreeData*
NodeBinTreeDataCtor (int type, int value, int opCode, int variable)
{
    NodeBinTreeData* data = (NodeBinTreeData*) calloc (1, sizeof (NodeBinTreeData));
    if (data == nullptr)
    {
        printf ("ERROR_NOT_MEMORY\n");
        return nullptr;
    }

    data->type     = type;
    data->value    = value;
    data->opCode   = opCode;
    data->variable = variable;

    return data;
}

//NodeBinTree*
//BinTreeCopy (NodeBinTree* node)
//{
//    if (node == nullptr) return nullptr;
//
//    return
//    NodeBinTreeCtor (NodeBinTreeDataCtor (node->data->value,
//                                          node->data->opCode,
//                                          node->data->variable),
//                     BinTreeCopy (node->left),
//                     BinTreeCopy (node->right),
//                     nullptr);
//}

//int
//SubtreeDump (NodeBinTree* node)
//{
//    if (node == nullptr)
//    {
//        printf ("%s ", NULL_POINTER);
//        return 0;
//    }
//
//    printf ("(");
//
//    SubtreeDump (node->left);
//    printf (BIN_TREE_DATA_PRINT_SPECIFIER " ", node->data);
//    SubtreeDump (node->right);
//
//    printf (") ");
//
//    return 0;
//}

int
BinTreePrintPostorderWithoutNil (BinTree* tree, const char* const fileName)
{
    if (fileName == nullptr) return ERROR_NOT_FILE;
    if (tree     == nullptr) return ERROR_NOT_TREE_POINTER;

    FILE* fp = fopen (fileName, "w");
    assert (fp);

    SubtreePrintPostorderWithoutNil (tree->root, fp);

    fclose (fp);

    return 0;
}

static int
SubtreePrintPostorderWithoutNil (NodeBinTree* node, FILE* fp)
{
    if (fp == nullptr)   return ERROR_NOT_FILE_POINTER;
    if (node == nullptr) return 0;

    fprintf (fp, "(");

    SubtreePrintPostorderWithoutNil (node->left,  fp);
    SubtreePrintPostorderWithoutNil (node->right, fp);

    fprintf (fp, "%d %d %d %d",
                 node->data->type,
                 node->data->value,
                 node->data->opCode,
                 node->data->variable);

    fprintf (fp, ")");

    return 0;
}

BinTree*
BinTreeReadPostorderWithoutNil (struct File* file)
{
    if (file == nullptr)
    {
        printf ("ERROR_NOT_FILE\n");
        return nullptr;
    }

    STL_Fread (file);

    BinTree* tree  = BinTreeCtor ();
    tree->buf = file->text;

    int fileIndex = 0;
    tree->root = NodeBinTreeCtorPostorderWithoutNil (tree->buf, &fileIndex);

    return tree;
}

static NodeBinTree*
NodeBinTreeCtorPostorderWithoutNil (char* const buf, int* len)
{
    if (buf == nullptr || len == nullptr)
    {
        printf ("ERROR_NOT_FILE_POINTER\n");
        return nullptr;
    }

    if (buf[(*len)++] != '(')
    {
        return nullptr;
    }

    NodeBinTree* node = NodeBinTreeCtor ();

    node->left   = ReadBranchNodeBinTreeWithoutNil (buf, len);
    node->right  = ReadBranchNodeBinTreeWithoutNil (buf, len);
    node->data   = ReadDataNodeBinTree (buf, len);

    if (buf[*len] == ')')
    {
        (*len)++;
        return node;
    }

    return nullptr;
}

static NodeBinTreeData*
ReadDataNodeBinTree (char* const buf, int* len)
{
    if (buf == nullptr || len == nullptr)
    {
        printf ("ERROR_NOT_FILE_POINTER\n");
        return nullptr;
    }

    NodeBinTreeData* data = NodeBinTreeDataCtor ();

    int nLen = 0;
    sscanf (buf + *len, "%d%n", &data->type, &nLen);
    *len += nLen;

    sscanf (buf + *len, "%d%n", &data->value, &nLen);
    *len += nLen;

    sscanf (buf + *len, "%d%n", &data->opCode, &nLen);
    *len += nLen;

    sscanf (buf + *len, "%d%n", &data->variable, &nLen);
    *len += nLen;

    return data;
}

static NodeBinTree*
ReadBranchNodeBinTreeWithoutNil (char* const buf, int* len)
{
    if (buf == nullptr || len == nullptr)
    {
        printf ("ERROR_NOT_FILE_POINTER\n");
        return nullptr;
    }

    if (buf[(*len)] == '(')
    {
        return NodeBinTreeCtorPostorderWithoutNil (buf, len);
    }

    return nullptr;
}

//static int
//FindVariable (char* const buf, int* len, Stack_Variable* stk)
//{
//    if (buf == nullptr) return 0;
//    if (len == nullptr) return 0;
//    if (stk == nullptr) return 0;
//
//    int nameLen = StrlenVarName (buf + *len);
//
//    int nVar = 0;
//    for (; nVar < stk->size; nVar++)
//    {
//        if (strncmp (stk->data[nVar].name, buf + *len, nameLen) == 0)
//        {
//            *len += nameLen;
//            return nVar;
//        }
//    }
//
//    Variable var = { .name = buf + *len, .value = 0 };
//
//    StackPush (stk, var);
//
//    *len += nameLen;
//
//    return nVar;
//}

/// так как после переменной нет пробела,
/// нельзя испольновать sscanf
//static int
//StrlenVarName (char* buf)
//{
//    int i = 0;
//    while (true)
//    {
//        switch (*(buf + i))
//        {
//            case ')':    // возможно, тут нужна только скобка
//            case '(':
//            case '+':
//            case '-':
//            case '*':
//            case '/':
//            case '^':
//            case SIN:
//            case COS:
//            case LN:
//            {
//                return i;
//            }
//        }
//        i++;
//    }
//}
