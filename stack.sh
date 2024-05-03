
g++ -c lib/STL_Stack/STL_stack_functions.cpp   -DSTACK_FILE_TYPE=\"../../front-end/STACK_FILE_TYPE_variable.h\" -o temp/stack_var_func.o
g++ -c lib/STL_Stack/STL_stack_print_error.cpp -DSTACK_FILE_TYPE=\"../../front-end/STACK_FILE_TYPE_variable.h\" -o temp/stack_var_error.o
g++ -c lib/STL_Stack/STL_stack_functions.cpp   -DSTACK_FILE_TYPE=\"../../front-end/STACK_FILE_TYPE_function.h\" -o temp/stack_func_func.o
g++ -c lib/STL_Stack/STL_stack_print_error.cpp -DSTACK_FILE_TYPE=\"../../front-end/STACK_FILE_TYPE_function.h\" -o temp/stack_func_error.o
g++ -c lib/STL_Stack/STL_stack_functions.cpp   -DSTACK_FILE_TYPE=\"../STL_BinTree/STACK_FILE_TYPE_NODE.h\" -o temp/stack_node_func.o
g++ -c lib/STL_Stack/STL_stack_print_error.cpp -DSTACK_FILE_TYPE=\"../STL_BinTree/STACK_FILE_TYPE_NODE.h\" -o temp/stack_node_error.o

g++ -c lib/STL_Stack/STL_stack_functions.cpp   -DSTACK_FILE_TYPE=\"../STL_Asm/STACK_FILE_TYPE_label.h\" -o temp/stack_label_func.o
g++ -c lib/STL_Stack/STL_stack_print_error.cpp -DSTACK_FILE_TYPE=\"../STL_Asm/STACK_FILE_TYPE_label.h\" -o temp/stack_label_error.o
g++ -c lib/STL_Stack/STL_stack_functions.cpp   -DSTACK_FILE_TYPE=\"../STL_Asm/STACK_FILE_TYPE_fixup.h\" -o temp/stack_fixup_func.o
g++ -c lib/STL_Stack/STL_stack_print_error.cpp -DSTACK_FILE_TYPE=\"../STL_Asm/STACK_FILE_TYPE_fixup.h\" -o temp/stack_fixup_error.o

g++ -c lib/STL_Stack/STL_stack_functions.cpp   -DSTACK_FILE_TYPE=\"../STL_Spu/STACK_FILE_TYPE_int.h\" -o temp/stack_int_func.o
g++ -c lib/STL_Stack/STL_stack_print_error.cpp -DSTACK_FILE_TYPE=\"../STL_Spu/STACK_FILE_TYPE_int.h\" -o temp/stack_int_error.o
