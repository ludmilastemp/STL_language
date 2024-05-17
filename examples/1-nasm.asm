
global Func
extern MySqrt

section .data

section .text
; начало main

Func:

		mov     rbp, rsp


;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-



;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign n

		        ; const 8 
		mov     qword -16[rbp], 8

		mov     rax, -16[rbp]
		mov     -8[rbp], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign n

		; сохраняем локальные перменные
		sub     rsp, 16


		; передаем аргументы

		        ; variable n
		mov     rax, -8[rbp]
		mov     -16[rbp], rax

		xor     rax, rax
		mov     rax, -16[rbp]
		push    rax

		; Вызов функции
		push    rbp

		call Func_0_

		pop     rbp



		; возвращаем rsp
		add     rsp, 16


		; Значение return
		mov     -16[rbp], rax

		mov     rax, -16[rbp]
		mov     -8[rbp], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign n

		        ; variable n
		mov     rax, -8[rbp]
		mov     -16[rbp], rax

		mov     rax, -16[rbp]
		mov     -8[rbp], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-



		ret

; конец main




; начало функции

Func_0_:

		mov     rbp, rsp


		pop     rcx
		pop     rdx


;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

		pop     rax
		mov     -16[rbp], rax

;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-

		push    rdx
		push    rcx




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


.if_1_condition:

		        ; variable k
		mov     rax, -16[rbp]
		mov     -24[rbp], rax


		        ; const 2 
		mov     qword -32[rbp], 2

		mov     rax, -24[rbp]
		mov     rbx, -32[rbp]
		cmp    rax, rbx

		jbe .if_1_body
		jmp .else_1_body

.if_1_body:


;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		        ; const 1 
		mov     qword -24[rbp], 1

		; значениe return
		xor     rax, rax
		mov     rax, -24[rbp]

		ret


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-


		jmp .if_1_end

.else_1_body:
		jmp .if_1_end

.if_1_end:




;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign tmp

		        ; const 0 
		mov     qword -24[rbp], 0

		mov     rax, -24[rbp]
		mov     -24[rbp], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign tmp

		        ; variable tmp
		mov     rax, -24[rbp]
		mov     -32[rbp], rax


		; сохраняем локальные перменные
		sub     rsp, 40


		; передаем аргументы

		        ; variable k
		mov     rax, -16[rbp]
		mov     -40[rbp], rax


		        ; const 1 
		mov     qword -48[rbp], 1

		mov     rax, -40[rbp]
		mov     rbx, -48[rbp]
		sub     rax,  rbx

		mov     -40[rbp], rax

		xor     rax, rax
		mov     rax, -40[rbp]
		push    rax

		; Вызов функции
		push    rbp

		call Func_0_

		pop     rbp



		; возвращаем rsp
		add     rsp, 40


		; Значение return
		mov     -40[rbp], rax

		mov     rax, -32[rbp]
		mov     rbx, -40[rbp]
		add     rax, rbx

		mov     -32[rbp], rax

		mov     rax, -32[rbp]
		mov     -24[rbp], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign tmp

		        ; variable tmp
		mov     rax, -24[rbp]
		mov     -32[rbp], rax


		; сохраняем локальные перменные
		sub     rsp, 40


		; передаем аргументы

		        ; variable k
		mov     rax, -16[rbp]
		mov     -40[rbp], rax


		        ; const 2 
		mov     qword -48[rbp], 2

		mov     rax, -40[rbp]
		mov     rbx, -48[rbp]
		sub     rax,  rbx

		mov     -40[rbp], rax

		xor     rax, rax
		mov     rax, -40[rbp]
		push    rax

		; Вызов функции
		push    rbp

		call Func_0_

		pop     rbp



		; возвращаем rsp
		add     rsp, 40


		; Значение return
		mov     -40[rbp], rax

		mov     rax, -32[rbp]
		mov     rbx, -40[rbp]
		add     rax, rbx

		mov     -32[rbp], rax

		mov     rax, -32[rbp]
		mov     -24[rbp], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		        ; variable tmp
		mov     rax, -24[rbp]
		mov     -32[rbp], rax

		; значениe return
		xor     rax, rax
		mov     rax, -32[rbp]

		ret


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-



		ret

; конец функции