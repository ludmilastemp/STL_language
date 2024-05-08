
global Func

section .data
variable: db 512 DUP (0)
endStack: db 1

section .text
; начало main

Func:

		mov     rdi, variable
		add     rdi, 64


;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-



;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign no

		        ; variableSize 7 
		mov     qword 8[rdi], 7

		mov     rax, 8[rdi]
		mov     -0[rdi], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign no
		; передаем аргументы

		        ; variable no
		mov     rax, -0[rdi]
		mov     8[rdi], rax

		xor     rax, rax
		mov     rax, 8[rdi]
		push    rax

		; Вызов функции
		push    rdi

		call Func_0_

		pop     rdi


		; Значение return
		mov     8[rdi], rax

		mov     rax, 8[rdi]
		mov     -0[rdi], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-



		xor     rax, rax
		mov     rax, [rdi]

		ret

; конец main




; начало функции

Func_0_:

		add     rdi, 64


		pop     rcx
		pop     rdx


;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

		pop     rax
		mov     -16[rdi], rax

;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-

		push    rdx
		push    rcx




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		; Assign p

		        ; variable k
		mov     rax, -16[rdi]
		mov     8[rdi], rax


		        ; variableSize 1 
		mov     qword 16[rdi], 1

		mov     rax, 8[rdi]
		mov     rbx, 16[rdi]
		sub    rax,  rbx

		mov     8[rdi], rax

		mov     rax, 8[rdi]
		mov     -8[rdi], rax


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


.if_1_condition:

		        ; variable k
		mov     rax, -16[rdi]
		mov     8[rdi], rax


		        ; variableSize 1 
		mov     qword 16[rdi], 1

		mov     rax, 8[rdi]
		mov     rbx, 16[rdi]
		cmp    rax, rbx

		je .if_1_body
		jmp .else_1_body

.if_1_body:


;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		        ; variableSize 1 
		mov     qword 8[rdi], 1

		; значениe return
		xor     rax, rax
		mov     rax, 8[rdi]

		ret


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-


		jmp .if_1_end

.else_1_body:
		jmp .if_1_end

.if_1_end:




;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-




;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

		; передаем аргументы

		        ; variable p
		mov     rax, -8[rdi]
		mov     8[rdi], rax

		xor     rax, rax
		mov     rax, 8[rdi]
		push    rax

		; Вызов функции
		push    rdi

		call Func_0_

		pop     rdi


		; Значение return
		mov     8[rdi], rax


		        ; variable k
		mov     rax, -16[rdi]
		mov     16[rdi], rax

		mov     rax, 8[rdi]
		mul     qword 16[rdi]
		mov     8[rdi], rax

		; значениe return
		xor     rax, rax
		mov     rax, 8[rdi]

		ret


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-



		ret

; конец функции