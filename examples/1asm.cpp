
:main
$ начало main

		push 5
		pop [0]          $ no

$ сохраняем значения переменных, до вызова функции
		push [0]         $ no

$ передаем аргументы
		push [0]         $ no

$ вызов функции
		call :func_0_

$ возвращаем значения переменных, которые были до вызова функции
		pop [0]          $ no

$ push значение return
		push rax
		pop [0]          $ no

		push [0]         $ no
		OUT


		hlt
$ конец main
$ начало функции

:func_0_
		pop [2]          $ k


		push [2]         $ k
		push 1
		sub
		pop [1]          $ p


:if_1_condition
		push [2]         $ k
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
		push [1]         $ p
		push [2]         $ k

$ передаем аргументы
		push [1]         $ p

$ вызов функции
		call :func_0_

$ возвращаем значения переменных, которые были до вызова функции
		pop [2]          $ k
		pop [1]          $ p

$ push значение return
		push rax
		push [2]         $ k
		mul
$ сохраняем значения return
		pop rax

		ret


		ret
$ конец функции