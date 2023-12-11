#ifndef STL_stack_print_error_

#ifdef STACK_FILE_TYPE
    #include STACK_FILE_TYPE
    #define STL_stack_print_error_

    #include "STL_stack_initial_parameters.h"
    #include "STL_stack_list_of_errors.h"
    #include "STL_stack_struct.h"
#endif

#ifndef DBG_E

#include <string.h>
#include <stdarg.h>

#endif /* DBG_E */

#ifdef STACK_HASH_PROTECTION
#include "STL_stack_hash.h"
#endif

#ifdef STACK_DEBUG
/// аргументы, принимаемые функциями
#define STL_FREC_ARGS  const char*  CALL_FILE, \
                       const size_t CALL_LINE, \
                       const char*  CALL_FUNC


/// аргументы, при вызове функций из функции библиотеки
#define STL_FSENT_ARGS CALL_FILE, \
                       CALL_LINE, \
                       CALL_FUNC
#endif /* STACK_DEBUG */

/**
    \brief расшифровка ошибок корректности стека
    \param stk указатель на стек
    \return указатель на строку
*/
char*
StackPrintErr (const STACK* stk

#ifdef STACK_DEBUG
               , STL_FREC_ARGS
#endif
               );

#endif /* STL_stack_print_error_ */
