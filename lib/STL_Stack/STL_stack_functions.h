#ifndef STL_stack_function_

#ifdef STACK_FILE_TYPE
    #include STACK_FILE_TYPE
    #define STL_stack_function_

    #include "STL_stack_initial_parameters.h"
    #include "STL_stack_struct.h"
    #include "STL_stack_print_error.h"
    #include "STL_stack_list_of_errors.h"
#endif

#ifndef DBG_E

#include <stdarg.h>

#endif /* DBG_E */

#ifdef STACK_DEBUG
    /// аргументы, при вызове функций из main
    #define STL_MSENT_ARGS __FILE__,                \
                           __LINE__,                \
                           __func__

    /// аргументы, при вызове функций из функции библиотеки
    #define STL_FSENT_ARGS CALL_FILE,               \
                           CALL_LINE,               \
                           CALL_FUNC

    /// аргументы, принимаемые функциями
    #define STL_FREC_ARGS  const char*  CALL_FILE,  \
                           const size_t CALL_LINE,  \
                           const char*  CALL_FUNC

    /// аргументы, при работе с DUMP
    #define STL_DUMP_ARGS stk->CREATE_NAME,         \
                          stk->CREATE_FILE,         \
                          stk->CREATE_LINE,         \
                          stk->CREATE_FUNC
#endif /* STACK_DEBUG */

/**
    \brief создать стек
    \param stk указатель на стек
    \return код ошибки
*/
#ifdef STACK_DEBUG
    #define StackCtor(stk)                                          \
            STL_StackCtor ((stk), #stk, STL_MSENT_ARGS)
#else
    #define StackCtor(stk)                                          \
            STL_StackCtor ((stk))
#endif

/**
    \brief создать стек заданного размера
    \param stk указатель на стек
           capacity изначальный размер стека
    \return код ошибки
*/
#ifdef STACK_DEBUG
    #define StackCtorCap(stk, capacity)                             \
            STL_StackCtor ((stk), #stk, STL_MSENT_ARGS, capacity)
#else
    #define StackCtorCap(stk, capacity)                             \
            STL_StackCtor ((stk), capacity)
#endif

/**
    \brief уничтожить стек
    \param stk указатель на стек
    \return 0
*/
#ifdef STACK_DEBUG
    #define StackDtor(stk)                                          \
            STL_StackDtor ((stk), STL_MSENT_ARGS)
#else
    #define StackDtor(stk)                                          \
            STL_StackDtor ((stk))
#endif

/**
    \brief добавить элемент в конец стека
    \param stk указатель на стек
           value значение элемента
    \return код ошибки
*/
#ifdef STACK_DEBUG
    #define StackPush(stk, value)                                   \
            do {                                                    \
                if (STL_StackPush ((stk), value, STL_MSENT_ARGS))   \
                {                                                   \
                    StackPrintErr (stk, STL_MSENT_ARGS);            \
                    StackDump (stk);                                \
                }                                                   \
            } while (false)
#else
    #define StackPush(stk, value)                                   \
            do {                                                    \
                if (STL_StackPush ((stk), value))                   \
                {                                                   \
                    StackPrintErr (stk);                            \
                    StackDump (stk);                                \
                }                                                   \
            } while (false)
#endif

/**
    \brief взять последний элемент из стека
    \param stk указатель на стек
           value указатель на элемент
    \return код ошибки
*/
#ifdef STACK_DEBUG
    #define StackPop(stk, value)                                    \
            do {                                                    \
                if (STL_StackPop ((stk), value, STL_MSENT_ARGS))    \
                {                                                   \
                    StackPrintErr (stk, STL_MSENT_ARGS);            \
                    StackDump (stk);                                \
                }                                                   \
            } while (false)
#else
    #define StackPop(stk, value)                                    \
            do {                                                    \
                if (STL_StackPop ((stk), value))                    \
                {                                                   \
                    StackPrintErr (stk);                            \
                    StackDump (stk);                                \
                }                                                   \
            } while (false)
#endif

/**
    \brief проверить стек на корректность и вывести стек в случае ошибки
    \param stk указатель на стек
    \return код ошибки
*/
#ifdef STACK_DEBUG
    #define StackVerificator(stk)                                   \
    do {                                                            \
                                                                    \
        if (STL_StackVerificator ((stk)))                           \
        {                                                           \
            StackPrintErr (stk, STL_MSENT_ARGS);                    \
            StackDump (stk);                                        \
        }                                                           \
    } while (false)
#else
    #define StackVerificator(stk)                                   \
    do {                                                            \
                                                                    \
        if (STL_StackVerificator ((stk)))                           \
        {                                                           \
            StackPrintErr (stk);                                    \
            StackDump (stk);                                        \
        }                                                           \
    } while (false)
#endif

/**
    \brief вывести стек
    \param stk указатель на стек
    \return void
*/
#ifdef STACK_DEBUG
    #define StackDump(stk)                                          \
        STL_StackDump ((stk), STL_MSENT_ARGS);
#else
    #define StackDump(stk)                                          \
        STL_StackDump ((stk));
#endif

StackErrorType
STL_StackCtor (STACK* stk

#ifdef STACK_DEBUG
               , const char*  CREATE_NAME
               , STL_FREC_ARGS
#endif
               );

StackErrorType
STL_StackDtor (STACK* stk

#ifdef STACK_DEBUG
               , STL_FREC_ARGS
#endif
               );

StackErrorType
STL_StackPush (STACK* stk, STACK_T value

#ifdef STACK_DEBUG
               , STL_FREC_ARGS
#endif
               );
StackErrorType
STL_StackPop  (STACK* stk, STACK_T* value

#ifdef STACK_DEBUG
               , STL_FREC_ARGS
#endif
               );

/**
    \brief проверка стека на корректность
    \param stk указатель на стек
    \return код ошибки
*/
StackErrorType
STL_StackVerificator (STACK* stk);

void
STL_StackDump (const STACK* stk

#ifdef STACK_DEBUG
               , STL_FREC_ARGS
#endif
               );

#endif /* STL_stack_function_ */
