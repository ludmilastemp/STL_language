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
const int         nOperation      = 36;
const char* const operation []    = {":)",          // 0
                                     "=^.",         // 1
                                     ".^=",         // 2
                                     "\\teornma",   // 3
                                     "\\lemma",     // 4
                                     "\\oed",       // 5
                                     ":(",          // 6
                                     "):",          // 7
                                     ">|<",         // 8
                                     "(:",          // 9
                                     "\\katarce",   // 10
                                     "\\katapanma", // 11
                                     "\\gravo",     // 12
                                     "<=>",         // 13
                                     "<?=?>",       // 14
                                     "\\Z",         // 15
                                     "\\ni",        // 16
                                     "::=",         // 17
                                     "+",           // 18
                                     "-",           // 19
                                     "*",           // 20
                                     "/",           // 21
                                     "^",           // 22
                                     "(",           // 23
                                     ")",           // 24
                                     "sin",         // 25
                                     "cos",         // 26
                                     "ln",          // 27
                                     "sqrt",        // 28
                                     ">=",          // 29  // не выводится в графизе
                                     "<=",          // 30
                                     ">",           // 31
                                     "<",           // 32
                                     "==",          // 33
                                     "!=",          // 34
                                     "\\VCEGO_XOROSHEGO", // 35
                                     };

const int         lenOperation [] = {2,  3, 3, 8, 6,
                                     4,  2, 2, 3, 2,
                                     8, 10, 6, 3, 5,
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
    BORDER_PARAM_FUNC   = 8,
    FUNC_RETURN         = 9,
    BEGIN_INSIDE        = 10,
    END_INSIDE          = 11,
    PRINTF              = 12,
    IF                  = 13,
    WHILE               = 14,
    T_INT               = 15,
    SYMBOL_NI           = 16,
    ASSING              = 17,
    ADD                 = 18,
    SUB                 = 19,
    MUL                 = 20,
    DIV                 = 21,
    POW                 = 22,
    OPEN_PARENTHESIS    = 23,
    CLOSE_PARENTHESIS   = 24,
    SIN                 = 25,
    COS                 = 26,
    LN                  = 27,
    SQRT                = 28,
    ABOVE_EQUAL         = 29,
    BELOW_EQUAL         = 30,
    ABOVE               = 31,
    BELOW               = 32,
    EQUAL               = 33,
    NO_EQUAL            = 34,
    VCEGO_XOROSHEGO     = 35,

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
