a.exe:  main.cpp generic.h int_stack.o int_error.o
	g++ main.cpp int_stack.o int_error.o STL_stack_hash.o

int_stack.o: STL_stack_functions.cpp STL_stack_functions.h STL_stack_print_error.cpp STL_stack_print_error.h STL_stack_initial_parameters.h STL_stack_list_of_errors.h STL_stack_struct.h STACK_FILE_TYPE_int.h
	#int_stack.bat
	g++ -c STL_stack_functions.cpp   -DSTACK_FILE_TYPE='\"STACK_FILE_TYPE_int.h\"' -o stack_int_func.o

int_error.o: STL_stack_print_error.cpp STL_stack_print_error.h STL_stack_initial_parameters.h STL_stack_list_of_errors.h STL_stack_struct.h STACK_FILE_TYPE_int.h
	g++ -c STL_stack_print_error.cpp -DSTACK_FILE_TYPE='\"STACK_FILE_TYPE_int.h\"' -o stack_int_error.o

STL_stack_hash.o: STL_stack_hash.cpp STL_stack_hash.h
	g++ -c STL_stack_hash.cpp -o STL_stack_hash.o
