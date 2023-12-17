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

const int         nOperation      = 25;
const char* const operation []    = {"\\Z",         // 0
                                     "\\ni",        // 1
                                     "\\gravo",     // 2
                                     "::=",         // 3
                                     ":)",          // 4
                                     "+",           // 5
                                     "-",           // 6
                                     "*",           // 7
                                     "/",           // 8
                                     "^",           // 9
                                     "(",           // 10
                                     ")",           // 11
                                     "sin",         // 12
                                     "cos",         // 13
                                     "ln",          // 14
                                     "sqrt",        // 15
                                     "<=>",         // 16
                                     "\\katarce",   // 17
                                     "\\katapanma", // 18
                                     ">=",          // 19  // не выводится в графизе
                                     "<=",          // 20
                                     ">",           // 21
                                     "<",           // 22
                                     "==",          // 23
                                     "!=",          // 24
                                     "",            //
                                     "",            //
                                     };

const int         lenOperation [] = {2, 3, 6, 3, 2,
                                     1, 1, 1, 1, 1,
                                     1, 1, 3, 3, 2,
                                     4, 3, 8, 10, 2,
                                     2, 1, 1, 2, 2,
                                    };

enum OPCODE
{
    T_INT               = 0,
    SYMBOL_NI           = 1,
    PRINTF              = 2,
    ASSING              = 3,
    END_STR             = 4,
    ADD                 = 5,
    SUB                 = 6,
    MUL                 = 7,
    DIV                 = 8,
    POW                 = 9,
    OPEN_PARENTHESIS    = 10,
    CLOSE_PARENTHESIS   = 11,
    SIN                 = 12,
    COS                 = 13,
    LN                  = 14,
    SQRT                = 15,
    IF                  = 16,
    BEGIN_INSIDE        = 17,
    END_INSIDE          = 18,
    ABOVE_EQUAL         = 19,
    BELOW_EQUAL         = 20,
    ABOVE               = 21,
    BELOW               = 22,
    EQUAL               = 23,
    NO_EQUAL            = 24,
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
