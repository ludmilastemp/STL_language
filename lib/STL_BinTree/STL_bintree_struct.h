#ifndef STL_bintree_struct_
#define STL_bintree_struct_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <assert.h>

#include "STL_bintree_node_struct.h"

#include "../STL_Onegin/STL_file_open.h"

#define BIN_TREE_DATA_VALUE_PRINT_SPECIFIER    "%d"
#define BIN_TREE_DATA_OPCODE_PRINT_SPECIFIER   "%d"
#define BIN_TREE_DATA_VARIABLE_PRINT_SPECIFIER "%s"

//const int MAX_LEN_VAR_NAME = 100;

const int         nOperation      = 5;
const char* const operation []    = {"\\Z",
                                     "\\ni",
                                     "\\gravo",
                                     "::=",
                                     ":)"};

const int         lenOperation [] = {2, 3, 6, 3, 2};

enum OPCODE
{
    T_INT       = 0,
    SYMBOL_NI   = 1,
    PRINTF      = 2,
    ASSING      = 3,
    END_STR     = 4,
};

struct NodeBinTree
{
    NodeBinTreeData* data;
    NodeBinTree* left;
    NodeBinTree* right;
    NodeBinTree* parent;
};

struct BinTree
{
    NodeBinTree* root;
    int size;
    char* buf;
//    Stack_Variable* variable;
};

enum BinTreeError
{                       // NO not not
    ERROR_NOT_BIN_TREE_POINTER  = 1 << 10,
    ERROR_NOT_TREE_POINTER      = 1 << 0,
    ERROR_NOT_NODE_POINTER      = 1 << 1,
    ERROR_NOT_FILE_POINTER      = 1 << 2,
    ERROR_NOT_FILE              = 1 << 3,
    ERROR_NOT_MEMORY            = 1 << 4,
};

NodeBinTree*
NodeBinTreeCtor (NodeBinTreeData* data = nullptr,
                 NodeBinTree* left     = nullptr,
                 NodeBinTree* right    = nullptr,
                 NodeBinTree* parent   = nullptr,
                 BinTree*     binTree  = nullptr);

NodeBinTree*
NodeBinTreeDtor (NodeBinTree* node);

BinTree*
BinTreeCtor (NodeBinTree* root = nullptr,
             int size          = 1,
             char* buf         = nullptr);

BinTree*
BinTreeDtor (BinTree* tree);

NodeBinTreeData*
NodeBinTreeDataCtor (int type     = NodeBinTreeData::TYPE_POISON,
                     int value    = NodeBinTreeData::VALUE_POISON,
                     int opCode   = NodeBinTreeData::OPCODE_POISON,
                     int variable = NodeBinTreeData::VARIABLE_POISON);

//NodeBinTree*
//BinTreeCopy (NodeBinTree* node);
//
//int
//SubtreeDump (NodeBinTree* node);

int
BinTreePrintPostorderWithoutNil (BinTree* tree, const char* const fileName);

BinTree*
BinTreeReadPostorderWithoutNil (struct File* file);

#endif /* STL_bintree_struct_ */
