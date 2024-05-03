:main
$ начало main //у main 1 локальная переменная

$ // rsp += 1

		push 0 
		pop rbp

		push 7
		pop [0 rbp]          $ no

$ сохраняем значения переменных, до вызова функции
		push [0 rbp]         $ no

$ передаем аргументы
		push [0 rbp]         $ no

		push rbp
		push rbp
		push 1
		add 
		pop rbp
$ вызов функции
		call :func_0_
		pop rbp

$ возвращаем значения переменных, которые были до вызова функции
		pop [0 rbp]          $ no

$ push значение return
		push rax
		pop [0 rbp]          $ no

		push [0 rbp]         $ no
		out

$ // rsp -= 1

		hlt
$ конец main




$ начало функции // тут 2 переменные 

:func_0_
		pop [2 rbp]          $ k


		push [2 rbp]         $ k
		push 1
		sub
		pop [1 rbp]          $ p


:if_1_condition
		push [2 rbp]         $ k
		push 1
		je :if_1_body
		jmp :else_1_body

:if_1_body
		push 1
$ сохраняем значения return
		pop rax

		ret

		jmp :if_1_end

:else_1_body
		jmp :if_1_end

:if_1_end



$ сохраняем значения переменных, до вызова функции
		push [1 rbp]         $ p
		push [2 rbp]         $ k

$ передаем аргументы
		push [1 rbp]         $ p

$ вызов функции
		call :func_0_

$ возвращаем значения переменных, которые были до вызова функции
		pop [2 rbp]          $ k
		pop [1 rbp]          $ p

$ push значение return
		push rax
		push [2 rbp]         $ k
		mul
$ сохраняем значения return
		pop rax

		ret


		ret
$ конец функции