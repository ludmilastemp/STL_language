
		push 3
		pop [0]

		push [0]
		sin
		pop [0]


:while_1_condition
		push [0]
		push 7
		jbe :while_1_body
		jmp :while_1_end

:while_1_body
		push [0]
		push 5
		add
		pop [0]

		jmp :while_1_condition

:while_1_end


		push [0]
		OUT