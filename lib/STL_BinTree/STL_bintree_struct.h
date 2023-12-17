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
                /// no operation -> key words
const int         nOperation      = 35;
const char* const operation []    = {":)",          // 0
                                     "=^.",         // 1
                                     ".^=",         // 2
                                     "\\teornma",   // 3
                                     "\\lemma",     // 4
                                     "\\oed",       // 5
                                     ":(",          // 6
                                     "):",          // 7
                                     "(:",          // 8
                                     "\\katarce",   // 9
                                     "\\katapanma", // 10
                                     "\\gravo",     // 11
                                     "<=>",         // 12
                                     "<?=?>",       // 13
                                     "\\Z",         // 14
                                     "\\ni",        // 15
                                     "::=",         // 16
                                     "+",           // 17
                                     "-",           // 18
                                     "*",           // 19
                                     "/",           // 20
                                     "^",           // 21
                                     "(",           // 22
                                     ")",           // 23
                                     "sin",         // 24
                                     "cos",         // 25
                                     "ln",          // 26
                                     "sqrt",        // 27
                                     ">=",          // 28  // не выводится в графизе
                                     "<=",          // 29
                                     ">",           // 30
                                     "<",           // 31
                                     "==",          // 32
                                     "!=",          // 33
                                     "\\VCEGO_XOROSHEGO", // 34
                                     };

const int         lenOperation [] = {2,  3, 3, 8, 6,
                                     4,  2, 2, 2, 8,
                                     10, 6, 3, 5,
                                     2,  3, 3, 1, 1,
                                     1,  1, 1, 1, 1,
                                     3,  3, 2, 4, 2,
                                     2,  1, 1, 2, 2,
                                     16
                                    };


enum OPCODE
{
    END_STR             = 0,   // end operation
    BEGIN_COMMENT       = 1,
    END_COMMENT         = 2,
    MAIN                = 3,
    BEGIN_FUNCTION      = 4,
    END_FUNCTION        = 5,
    BEGIN_PARAM_FUNC    = 6,
    END_PARAM_FUNC      = 7,
    FUNC_RETURN         = 8,
    BEGIN_INSIDE        = 9,
    END_INSIDE          = 10,
    PRINTF              = 11,
    IF                  = 12,
    WHILE               = 13,
    T_INT               = 14,
    SYMBOL_NI           = 15,
    ASSING              = 16,
    ADD                 = 17,
    SUB                 = 18,
    MUL                 = 19,
    DIV                 = 20,
    POW                 = 21,
    OPEN_PARENTHESIS    = 22,
    CLOSE_PARENTHESIS   = 23,
    SIN                 = 24,
    COS                 = 25,
    LN                  = 26,
    SQRT                = 27,
    ABOVE_EQUAL         = 28,
    BELOW_EQUAL         = 29,
    ABOVE               = 30,
    BELOW               = 31,
    EQUAL               = 32,
    NO_EQUAL            = 33,
    VCEGO_XOROSHEGO     = 34,

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
                     int variable = NodeBinTreeData::VARIABLE_POISON,
                     int function = NodeBinTreeData::FUNCTION_POISON);

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
