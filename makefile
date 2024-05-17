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
			temp/stack_func_func.o                     		\
			temp/stack_func_error.o                    		\

	g++ front-end/main.cpp									\
		front-end/RecRead.cpp								\
		lib/STL_BinTree/STL_bintree_struct.cpp        		\
		lib/STL_Graphviz/STL_graphviz.cpp					\
		lib/STL_Onegin/STL_file_open.cpp        			\
		temp/stack_node_func.o                      		\
		temp/stack_node_error.o                     		\
		temp/stack_var_func.o                      			\
		temp/stack_var_error.o                     			\
		temp/stack_func_func.o                     			\
		temp/stack_func_error.o                    			\
		$(FLAGS)											\
		-o front-end.exe

back-end-NASM: 	back-end-NASM/main.cpp 						\
			back-end-NASM/back-end.h  						\
			back-end-NASM/back-end.cpp						\
			lib/STL_BinTree/STL_bintree_struct.h        	\
			lib/STL_BinTree/STL_bintree_struct.cpp      	\
			lib/STL_BinTree/STL_bintree_node_struct.h		\
			lib/STL_Graphviz/STL_graphviz.h					\
			lib/STL_Graphviz/STL_graphviz.cpp				\
			lib/STL_Onegin/STL_file_open.h					\
			lib/STL_Onegin/STL_file_open.cpp 				\
			lib/STL_Onegin/STL_string.h                 	\
			temp/stack_var_func.o                      		\
			temp/stack_var_error.o                     		\

	g++ back-end-NASM/main.cpp								\
		back-end-NASM/back-end.cpp							\
		lib/STL_BinTree/STL_bintree_struct.cpp        		\
		lib/STL_Graphviz/STL_graphviz.cpp					\
		lib/STL_Onegin/STL_file_open.cpp        			\
		temp/stack_var_func.o                      			\
		temp/stack_var_error.o                     			\
		$(FLAGS)											\
		-o back-end-NASM.exe

execute-NASM: 	execute-NASM/main.cpp							\
			execute-NASM/nasm.o
	g++ execute-NASM/main.cpp 								\
		execute-NASM/nasm.o 								\
		$(FLAGS) 											\
		-o execute-NASM.exe

execute-NASM/nasm.o: examples/1-nasm.asm
	nasm -f elf64 examples/1-nasm.asm -o execute-NASM/nasm.o

.PHONY: front-end back-end-NASM execute-NASM clean

FLAGS = -O3 										\
		-D											\
		_DEBUG										\
		-ggdb3										\
		-pie										\
		-std=c++17									\
		-Waggressive-loop-optimizations 			\
		-Wall										\
		-Wc++14-compat								\
		-Wcast-align								\
		-Wcast-qual									\
		-Wchar-subscripts							\
		-Wconditionally-supported					\
		-Wconversion								\
		-Wctor-dtor-privacy							\
		-Weffc++									\
		-Wempty-body								\
		-Werror=vla									\
		-Wextra										\
		-Wfloat-equal								\
		-Wformat-nonliteral							\
		-Wformat-security							\
		-Wformat-signedness							\
		-Wformat=2									\
		-Winit-self									\
		-Winline									\
		-Wlarger-than=8192							\
		-Wlogical-op								\
		-Wmissing-declarations						\
		-Wno-literal-suffix							\
		-Wno-missing-field-initializers				\
		-Wno-narrowing								\
		-Wno-old-style-cast							\
		-Wno-varargs								\
		-Wnon-virtual-dtor							\
		-Wopenmp-simd								\
		-Woverloaded-virtual						\
		-Wpacked									\
		-Wpointer-arith								\
		-Wredundant-decls							\
		-Wshadow									\
		-Wsign-conversion							\
		-Wsign-promo								\
		-Wstack-protector							\
		-Wstack-usage=8192							\
		-Wstrict-null-sentinel						\
		-Wstrict-overflow=2							\
		-Wsuggest-attribute=noreturn				\
		-Wsuggest-final-methods						\
		-Wsuggest-final-types						\
		-Wsuggest-override							\
		-Wswitch-default							\
		-Wswitch-enum								\
		-Wsync-nand									\
		-Wundef										\
		-Wunreachable-code							\
		-Wunused									\
		-Wuseless-cast								\
		-Wvariadic-macros							\
		-fcheck-new									\
		-flto-odr-type-merging						\
		-fno-omit-frame-pointer						\
		-fPIE										\
		-fsized-deallocation						\
		-fstack-protector							\
		-fstrict-overflow							\
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
		