
		push 3
		pop [0]

		push [0]
		sin
		pop [0]

		push [0]
		push 2
		je :if_1
		jmp :else_1

:if_1
		push [0]
		push 5
		add
		pop [0]

		jmp :end_if_1

:else_1

:end_if_1
		push [0]
		OUT