
:main

		push 3
		pop [0]

		push [0]

		call :func_0_

		push rax
		pop [0]

		push [0]
		OUT


		hlt

:func_0_
		pop [1]


		push 7
		pop rax

		ret


		ret

:aaa