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

const int         nOperation      = 28;
const char* const operation []    = {":)",          // 0
                                     "\\katarce",   // 1
                                     "\\katapanma", // 2
                                     "\\gravo",     // 3
                                     "<=>",         // 4
                                     "<?=?>",       // 5
                                     "\\Z",         // 6
                                     "\\ni",        // 7
                                     "::=",         // 8
                                     "+",           // 9
                                     "-",           // 10
                                     "*",           // 11
                                     "/",           // 12
                                     "^",           // 13
                                     "(",           // 14
                                     ")",           // 15
                                     "sin",         // 16
                                     "cos",         // 17
                                     "ln",          // 18
                                     "sqrt",        // 19
                                     ">=",          // 20  // не выводится в графизе
                                     "<=",          // 21
                                     ">",           // 22
                                     "<",           // 23
                                     "==",          // 24
                                     "!=",          // 25
                                     "=^.",         // 26
                                     ".^=",         // 27
                                     "lemma",       //
                                     "",            //
                                     };

const int         lenOperation [] = {2, 8, 10, 6, 3, 5,
                                     2, 3, 3, 1, 1,
                                     1, 1, 1, 1, 1,
                                     3, 3, 2, 4, 2,
                                     2, 1, 1, 2, 2,
                                     3, 3, 5
                                    };


enum OPCODE
{
    END_STR             = 0,
    BEGIN_INSIDE        = 1,
    END_INSIDE          = 2,
    PRINTF              = 3,
    IF                  = 4,
    WHILE               = 5,
    T_INT               = 6,
    SYMBOL_NI           = 7,
    ASSING              = 8,
    ADD                 = 9,
    SUB                 = 10,
    MUL                 = 11,
    DIV                 = 12,
    POW                 = 13,
    OPEN_PARENTHESIS    = 14,
    CLOSE_PARENTHESIS   = 15,
    SIN                 = 16,
    COS                 = 17,
    LN                  = 18,
    SQRT                = 19,
    ABOVE_EQUAL         = 20,
    BELOW_EQUAL         = 21,
    ABOVE               = 22,
    BELOW               = 23,
    EQUAL               = 24,
    NO_EQUAL            = 25,
    BEGIN_COMMENT       = 26,
    END_COMMENT         = 27,
    BEGIN_FUNCTION      = 28,

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
