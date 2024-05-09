
global Func
extern MySqrt

section .data

section .text
; начало main

Func:

		mov     rbp, rsp


;-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-


		        ; const 16 
		mov     qword -16[rbp], 16


		        ; const 5 
		mov     qword -24[rbp], 5


		        ; const 1 
		mov     qword -32[rbp], 1

		mov     rax, -24[rbp]
		mov     rbx, -32[rbp]
		add     rax, rbx

		mov     -24[rbp], rax


		; сохраняем локальные перменные
		sub     rsp, 32

		; передаем аргументы
		mov     rax, -24[rbp]
		mov     rdi, rax

		; Вызов функции

		push    rbp

		call MySqrt

		pop     rbp

		; возвращаем rsp
		add     rsp, 32

		; значениe return
		mov     -24[rbp], rax

		mov     rax, -16[rbp]
		xor     rdx, rdx
		div     qword -24[rbp]
		mov     -16[rbp], rax

		; значениe return
		xor     rax, rax
		mov     rax, -16[rbp]

		ret


;-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-



		ret

; конец main