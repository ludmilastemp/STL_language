
:main
		push 3
		pop [0]

		push [0]
		push 3
		mul
		OUT

		call :func_0_

		push rax
		pop [0]

		push [0]
		OUT

		hlt

:func_0_
		push 13
		pop [0]

		push 512
		OUT

		push [0]
		push 1
		add
		pop rax

		ret

		ret