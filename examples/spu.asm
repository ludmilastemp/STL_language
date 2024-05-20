:main
$ начало main
		push 0
		pop rbp

		push 42
		pop [0 rbp]      $ n

$ сохраняем значения переменных, до вызова функции
		push [0 rbp]     $ n

$ передаем аргументы
		push [0 rbp]     $ n

$ вызов функции
		call :func_0_

$ возвращаем значения переменных, которые были до вызова функции
		pop [0 rbp]      $ n

$ push значение return
		push rax
		pop [0 rbp]      $ n

		push [0 rbp]     $ n
		pop [0 rbp]      $ n


		hlt
$ конец main
$ начало функции

:func_0_
		pop [1 rbp]      $ k



:if_1_condition
		push [1 rbp]     $ k
		push 2
		jbe :if_1_body
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



		push 0
		pop [2 rbp]      $ tmp

		push [2 rbp]     $ tmp
$ сохраняем значения переменных, до вызова функции
		push [1 rbp]     $ k
		push [2 rbp]     $ tmp

$ передаем аргументы
		push [1 rbp]     $ k
		push 1
		sub

$ вызов функции
		call :func_0_

$ возвращаем значения переменных, которые были до вызова функции
		pop [2 rbp]      $ tmp
		pop [1 rbp]      $ k

$ push значение return
		push rax
		add
		pop [2 rbp]      $ tmp

		push [2 rbp]     $ tmp
$ сохраняем значения переменных, до вызова функции
		push [1 rbp]     $ k
		push [2 rbp]     $ tmp

$ передаем аргументы
		push [1 rbp]     $ k
		push 2
		sub

$ вызов функции
		call :func_0_

$ возвращаем значения переменных, которые были до вызова функции
		pop [2 rbp]      $ tmp
		pop [1 rbp]      $ k

$ push значение return
		push rax
		add
		pop [2 rbp]      $ tmp

		push [2 rbp]     $ tmp
$ сохраняем значения return
		pop rax

		ret


		ret
$ конец функции