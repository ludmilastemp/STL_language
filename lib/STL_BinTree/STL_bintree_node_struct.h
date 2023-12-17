#ifndef STL_bintree_node_struct_
#define STL_bintree_node_struct_

struct NodeBinTreeData
{
    int type;      /// тип
    int value;     /// значение если число
    int opCode;    /// код      если ключевое слово
    int variable;  /// номер    переменной
    int function;  /// номер    функции

    static const int T_VALUE    = 1;
    static const int T_OPCODE   = 2;
    static const int T_VARIABLE = 3;
    static const int T_FUNCTION = 4;

    static const int TYPE_POISON     = 0;
    static const int VALUE_POISON    = 0;
    static const int OPCODE_POISON   = -1;
    static const int VARIABLE_POISON = -1;
    static const int FUNCTION_POISON = -1;
};

#endif /* STL_bintree_node_struct_ */
