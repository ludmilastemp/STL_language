front-end: 	front-end/main.cpp 								\
			front-end/RecRead.h  							\
			front-end/RecRead.cpp							\
			lib/STL_BinTree/STL_bintree_struct.h        	\
			lib/STL_BinTree/STL_bintree_struct.cpp      	\
			lib/STL_BinTree/STL_bintree_node_struct.h		\
			lib/STL_Graphviz/STL_graphviz.h					\
			lib/STL_Graphviz/STL_graphviz.cpp				\
			lib/STL_Onegin/STL_file_open.h					\
			lib/STL_Onegin/STL_file_open.cpp 				\
			lib/STL_Onegin/STL_string.h                 	\
			temp/stack_node_func.o                      	\
			temp/stack_node_error.o                     	\
			temp/stack_var_func.o                      		\
			temp/stack_var_error.o                     		\

	g++ front-end/main.cpp									\
		front-end/RecRead.cpp								\
		lib/STL_BinTree/STL_bintree_struct.cpp        		\
		lib/STL_Graphviz/STL_graphviz.cpp					\
		lib/STL_Onegin/STL_file_open.cpp        			\
		temp/stack_node_func.o                      		\
		temp/stack_node_error.o                     		\
		temp/stack_var_func.o                      			\
		temp/stack_var_error.o                     			\
		-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body\
		-Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE\
		-Wformat-security -Wformat=1


back-end: 	back-end/main.cpp 								\
			back-end/back-end.h  							\
			back-end/back-end.cpp							\
			lib/STL_BinTree/STL_bintree_struct.h        	\
			lib/STL_BinTree/STL_bintree_struct.cpp      	\
			lib/STL_BinTree/STL_bintree_node_struct.h		\
			lib/STL_Graphviz/STL_graphviz.h					\
			lib/STL_Graphviz/STL_graphviz.cpp				\
			lib/STL_Onegin/STL_file_open.h					\
			lib/STL_Onegin/STL_file_open.cpp 				\
			lib/STL_Onegin/STL_string.h                 	\

	g++ back-end/main.cpp									\
		back-end/back-end.cpp								\
		lib/STL_BinTree/STL_bintree_struct.cpp        		\
		lib/STL_Graphviz/STL_graphviz.cpp					\
		lib/STL_Onegin/STL_file_open.cpp        			\
		-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body\
		-Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE\
		-Wformat-security -Wformat=1

compile: 	lib/STL_Asm/main.cpp 							\
			lib/STL_Asm/STL_compile.h 						\
			lib/STL_Asm/STL_compile.cpp 					\
			lib/STL_Asm/STL_label.h        					\
			lib/STL_Asm/STL_label.cpp   					\
			lib/STL_Asm/STL_label_struct.h 					\
			lib/STL_Asm/stack_label_func.o 					\
			lib/STL_Asm/stack_label_error.o 				\
			lib/STL_Asm/stack_fixup_func.o 					\
			lib/STL_Asm/stack_fixup_error.o             	\
			lib/STL_Spu/include/STL_spu_struct_const.h  	\
			lib/STL_Spu/include/STL_spu_struct_const.cpp	\
			lib/STL_Onegin/STL_string.h                 	\
			lib/STL_Onegin/STL_header.h						\
			lib/STL_Onegin/STL_header.cpp					\
			lib/STL_Onegin/STL_file_open.h 					\
			lib/STL_Onegin/STL_file_open.cpp				\

	g++ lib/STL_Asm/main.cpp 								\
		lib/STL_Asm/STL_compile.cpp 						\
		lib/STL_Asm/STL_label.cpp 							\
		lib/STL_Asm/stack_label_func.o 						\
		lib/STL_Asm/stack_label_error.o 					\
		lib/STL_Asm/stack_fixup_func.o 						\
		lib/STL_Asm/stack_fixup_error.o 					\
		lib/STL_Spu/include/STL_spu_struct_const.cpp    	\
		lib/STL_Onegin/STL_header.cpp						\
		lib/STL_Onegin/STL_file_open.cpp					\
		-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

spu:    	lib/STL_Spu/main.cpp 							\
			lib/STL_Spu/STL_spu.h   						\
			lib/STL_Spu/STL_spu.cpp 						\
			lib/STL_Spu/STL_spu_struct.h 					\
			lib/STL_Spu/STL_spu_struct.cpp 					\
			lib/STL_Spu/stack_int_func.o 					\
			lib/STL_Spu/stack_int_error.o                   \
			lib/STL_Spu/include/STL_spu_struct_const.h      \
			lib/STL_Spu/include/STL_spu_struct_const.cpp    \
			lib/STL_Onegin/STL_string.h                     \
			lib/STL_Onegin/STL_file_open.h 					\
			lib/STL_Onegin/STL_file_open.cpp				\
			lib/STL_Onegin/STL_header.h						\
			lib/STL_Onegin/STL_header.cpp					\

	g++ lib/STL_Spu/main.cpp 								\
		lib/STL_Spu/STL_spu.cpp 							\
		lib/STL_Spu/STL_spu_struct.cpp 						\
		lib/STL_Spu/stack_int_func.o 						\
		lib/STL_Spu/stack_int_error.o 						\
		lib/STL_Spu/include/STL_spu_struct_const.cpp    	\
		lib/STL_Onegin/STL_file_open.cpp					\
		lib/STL_Onegin/STL_header.cpp						\
		-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

.PHONY: front-end back-end compile disasm spu clean

# Make variables

# .o для стека

#g++ -c lib/STL_Stack/STL_stack_functions.cpp   -DSTACK_FILE_TYPE='\"../../front-end/STACK_FILE_TYPE_variable.h\"' -o temp/stack_var_func.o

#g++ -c lib/STL_Stack/STL_stack_print_error.cpp -DSTACK_FILE_TYPE='\"../../front-end/STACK_FILE_TYPE_variable.h\"' -o temp/stack_var_error.o

#g++ -c lib/STL_Stack/STL_stack_functions.cpp   -DSTACK_FILE_TYPE='\"../STL_BinTree/STACK_FILE_TYPE_NODE.h\"' -o temp/stack_node_func.o
#g++ -c lib/STL_Stack/STL_stack_print_error.cpp -DSTACK_FILE_TYPE='\"../STL_BinTree/STACK_FILE_TYPE_NODE.h\"' -o temp/stack_node_error.o
