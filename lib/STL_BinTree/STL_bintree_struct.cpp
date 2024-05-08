#include "STL_bintree_struct.h"

static int
SubtreePrintPostorderWithoutNil (NodeBinTree* node,
                                 Stack_Variable* var,
                                 FILE* fp);

static int
SubtreePrintAccordingToStandard (NodeBinTree* node,
                                 FILE* fp);

static NodeBinTree*
NodeBinTreeCtorPostorderWithoutNil (char* const buf,
                                    size_t* len, Stack_Variable* var);

static NodeBinTreeData*
ReadDataNodeBinTree (char* const buf,
                     size_t* len, Stack_Variable* var);

static size_t
FindName (Variable var1, Stack_Variable* var);

static NodeBinTree*
ReadBranchNodeBinTreeWithoutNil (char* const buf,
                                 size_t* len, Stack_Variable* var);

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

    // if (node->data != nullptr)
    // {
    //     node->data->type     = NodeBinTreeData::TYPE_POISON;
    //     node->data->value    = NodeBinTreeData::VALUE_POISON;
    //     node->data->opCode   = NodeBinTreeData::OPCODE_POISON;
    //     node->data->variable = NodeBinTreeData::VARIABLE_POISON;
    //     node->data->function = NodeBinTreeData::FUNCTION_POISON;
    // }
    if (node->data->freeData) free (node->data);
    node->data   = nullptr;           
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

    binTree->root = root;
    binTree->size = size;
    binTree->buf  = buf;
    binTree->var  = nullptr;

    return binTree;
}

BinTree*
BinTreeDtor (BinTree* binTree)
{
    if (binTree == nullptr) return nullptr;

    NodeBinTreeDtor (binTree->root);

    binTree->root = nullptr;
    binTree->buf  = nullptr;
    binTree->size = 0;
    binTree->var  = nullptr;

    free (binTree);

    return nullptr;
}

NodeBinTreeData*
NodeBinTreeDataCtor (int type, int value, int opCode, int variable, int function)
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
    data->function = function;
    data->freeData = true;

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
    if (fileName == nullptr) return ERROR_NO_FILE;
    if (tree     == nullptr) return ERROR_NO_TREE_POINTER;

    FILE* fp = fopen (fileName, "w");
    assert (fp);

    SubtreePrintPostorderWithoutNil (tree->root, tree->var, fp);

    fclose (fp);

    return 0;
}

static int
SubtreePrintPostorderWithoutNil (NodeBinTree* node, Stack_Variable* var, FILE* fp)
{
    if (fp == nullptr)   return ERROR_NO_FILE_POINTER;
    if (node == nullptr) return fprintf (fp, "_");;

    fprintf (fp, "(");

    SubtreePrintPostorderWithoutNil (node->left,  var, fp);
    SubtreePrintPostorderWithoutNil (node->right, var, fp);

    fprintf (fp, "%d %d %d %d %d",
                 node->data->type,
                 node->data->value,
                 node->data->opCode,
                 node->data->function,
                 node->data->variable);

    if (node->data->variable != NodeBinTreeData::VARIABLE_POISON)
    {
        fprintf (fp, " %lu ", var->data[node->data->variable].len);
        for (size_t i = 0; i < var->data[node->data->variable].len; i++)
            fprintf (fp, "%c", var->data[node->data->variable].name[i]);

        fprintf (fp, " ");
    }

    fprintf (fp, ")");

    return 0;
}

int
BinTreePrintAccordingToStandard (BinTree* tree, const char* const fileName)
{
    if (fileName == nullptr) return ERROR_NO_FILE;
    if (tree     == nullptr) return ERROR_NO_TREE_POINTER;

    FILE* fp = fopen (fileName, "w");
    assert (fp);

    SubtreePrintAccordingToStandard (tree->root, fp);

    fclose (fp);

    return 0;
}

enum TypeAccordingToStandard
{
    PUNCT    = 0,
    BIN_OP   = 1,
    UN_OP    = 2,
    KEY_OP   = 3,
    VALUE    = 4,
    VARIABLE = 5,
    FUNCTION = 6,
    STRING   = 7,
};

static int
SubtreePrintAccordingToStandard (NodeBinTree* node, FILE* fp)
{
    if (fp == nullptr)   return ERROR_NO_FILE_POINTER;
    if (node == nullptr) return fprintf (fp, "_");;

    fprintf (fp, "(");

    switch (node->data->type)
    {
        case NodeBinTreeData::T_VALUE:

            fprintf (fp, "%d ", VALUE);
            fprintf (fp, "%d ", node->data->value);

            break;

        case NodeBinTreeData::T_VARIABLE:

            fprintf (fp, "%d", VARIABLE);
            fprintf (fp, "%d ", node->data->variable);

            break;

        case NodeBinTreeData::T_FUNCTION:

            fprintf (fp, "%d", FUNCTION);
            fprintf (fp, "%d ", node->data->function);

            break;

        case NodeBinTreeData::T_OPCODE:

            break;

        default:

            printf ("ERROR IN SubtreePrintAccordingToStandard");
            return 0;
    }

    switch (node->data->opCode)
    {
        case SIN:
        case COS:
        case SQRT:
        case LN:
//        case !:
        case PRINTF:
//        case OUT_S:
//        case IN:
        case FUNC_RETURN:

            fprintf (fp, "%d ", UN_OP);

            break;

//        case CALL:   // убрали
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case POW:
        case ABOVE_EQUAL:
        case BELOW_EQUAL:
        case ABOVE:
        case BELOW:
        case EQUAL:
        case NO_EQUAL:
        case ASSING:

            fprintf (fp, "%d ", BIN_OP);

            break;

        case IF:
        case WHILE:

            fprintf (fp, "%d ", KEY_OP);

            break;

        case END_OPERATION:

            fprintf (fp, "%d ", PUNCT);

            break;

//        case WHILE:
//
//            fprintf (fp, "%d ", STRING);
//
//            break;

        default:

            printf ("ERROR IN SubtreePrintAccordingToStandard");
            return 0;
    }
      /// недописано

//                 node->data->opCode,

    SubtreePrintAccordingToStandard (node->left,  fp);
    SubtreePrintAccordingToStandard (node->right, fp);

    fprintf (fp, ")");

    return 0;
}

BinTree*
BinTreeReadPostorderWithoutNil (struct File* file, Stack_Variable* var)
{
    if (file == nullptr)
    {
        printf ("ERROR_NO_FILE\n");
        return nullptr;
    }

    STL_Fread (file);

    BinTree* tree  = BinTreeCtor ();
    tree->buf = file->text;

    size_t fileIndex = 0;
    tree->root = NodeBinTreeCtorPostorderWithoutNil (tree->buf, &fileIndex, var);

    return tree;
}

static NodeBinTree*
NodeBinTreeCtorPostorderWithoutNil (char* const buf, size_t* len, Stack_Variable* var)
{
    if (buf == nullptr || len == nullptr)
    {
        printf ("ERROR_NO_FILE_POINTER\n");
        return nullptr;
    }

    if (buf[(*len)++] != '(')
    {
        return nullptr;
    }

    NodeBinTree* node = NodeBinTreeCtor ();

    node->left   = ReadBranchNodeBinTreeWithoutNil (buf, len, var);
    node->right  = ReadBranchNodeBinTreeWithoutNil (buf, len, var);
    node->data   = ReadDataNodeBinTree (buf, len, var);

    if (buf[*len] == ')')
    {
        (*len)++;
        return node;
    }

    return nullptr;
}

static NodeBinTreeData*
ReadDataNodeBinTree (char* const buf, size_t* len, Stack_Variable* var)
{
    if (buf == nullptr || len == nullptr)
    {
        printf ("ERROR_NO_FILE_POINTER\n");
        return nullptr;
    }

    NodeBinTreeData* data = NodeBinTreeDataCtor ();

    int nLen = 0;
    sscanf (buf + *len, "%d%n", &data->type, &nLen);
    *len += (size_t)nLen;
    // printf ("type = %d\n", data->type);

    sscanf (buf + *len, "%d%n", &data->value, &nLen);
    *len += (size_t)nLen;
    // printf ("value = %d\n", data->value);

    sscanf (buf + *len, "%d%n", &data->opCode, &nLen);
    *len += (size_t)nLen;
    // printf ("opCode = %d\n", data->opCode);

    sscanf (buf + *len, "%d%n", &data->function, &nLen);
    *len += (size_t)nLen;
    // printf ("function = %d\n", data->function);

    sscanf (buf + *len, "%d%n", &data->variable, &nLen);
    *len += (size_t)nLen;
    // printf ("variable = %d\n", data->variable);

    if (data->variable != NodeBinTreeData::VARIABLE_POISON)
    {
        // printf ("TES\n");

        Variable var1 = { .name = 0, .len = 0 };

        size_t a = 0;

        sscanf (buf + *len, "%lu%n", &a, &nLen);
        *len += (size_t)nLen + 1;

        var1.name = buf + *len;
        var1.len  = a;

        // for (size_t i = 0; i < var1.len; i++)
        //     printf ("~%c~", var1.name[i]);

        data->variable = (int)FindName (var1, var);

        *len += var1.len + 1;
    }

    return data;
}

static NodeBinTree*
ReadBranchNodeBinTreeWithoutNil (char* const buf, size_t* len, Stack_Variable* var)
{
    if (buf == nullptr || len == nullptr)
    {
        printf ("ERROR_NO_FILE_POINTER\n");
        return nullptr;
    }

    if (buf[(*len)] == '(')
    {
        return NodeBinTreeCtorPostorderWithoutNil (buf, len, var);
    }

    if (buf[(*len)] == '_')
    {
        (*len)++;
        return nullptr;
    }

    printf ("ERROR IN ReadBranchNodeBinTreeWithoutNil\n\n");
    printf ("len = %lu\nsymbol = %d\nstring = %s\n", *len, buf[(*len)], buf + *len );

    return nullptr;
}

static size_t
FindName (Variable var1, Stack_Variable* var)
{
    assert (var);

    size_t elem = 0;
    for (; elem < var->size; elem++)
    {
        if (strncmp (var->data[elem].name, var1.name, var1.len) == 0)
        {
            // printf ("%lu\n", var1.len);
            return elem;
        }
    }

    StackPush (var, var1);

    return elem;
}
