# STL language

Копмилятор для языка программирования STL.

```
=^.                       .^=
=^. Посчитаем факториал 5 .^=
=^.                       .^=

\VCEGO_XOROSHEGO

    \gravo fact :)

    fact ::= :( Factorial >|< fact ): :)

    fact ::= 5 :)
    \Z \ni fact :)

\teornma

\oed
    (: n * :( Factorial >|< (n - 1) ): :)

    \katarce
        n == 1
    \katapanma
        <=>
    \katarce 
        (: 1 :)
    \katapanma  :)

\lemma :( Factorial >|< n ):
```
## Содержание

1. [Как установить](#Как-установить)
2. [Синтаксис](#Синтаксис)
3. [Front-end](#Front-end)
4. [Back-end SPU](#Back-end-SPU)
    - [Запуск](#запуск-SPU)
5. [Back-end NASM](#Back-end-NASM)
    - [Запуск](#запуск-NASM)
6. [Back-end JIT](#Back-end-JIT)
    - [Запуск](#запуск-JIT)
7. [Back-end ELF](#Back-end-ELF)
    - [Запуск](#запуск-ELF)
8. [Сравнение скоростей](#Сравнение-скоростей)
    - [Используемое оборудование](#Используемое-оборудование)
9. [Структура проекта](#Структура-проекта)

## Как установить
Сначала надо скачать папку с проектом или склонировать репозиторий:
```
git clone https://github.com/ludmilastemp/STL_language
```
После этого нужно собрать проект:
```
cd STL_language
./makeAll.sh
```
После этого в корне проекта должны появится
```
front-end.exe       - front-end (из языка STL в AST дерево) 
back-end-ELF.exe    - back-end  (из AST дерева в исполняемый файл ELF)
back-end-JIT.exe    - back-end  (JIT (Just-In-Time) AST дерева в бинарный файл x86)
back-end-NASM.exe   - back-end  (из AST дерева в файл на языке nasm)
back-end-SPU.exe    - back-end  (из AST дерева в файл на языке для STL-SPU)
compile-SPU.exe     - компиляция (из языка для STL-SPU в бинарный код STL-SPU)
spu.exe             - виртуальная машина (исполнение бинарного кода STL-SPU)(https://github.com/ludmilastemp/CPU)
```

<figure>
<img src="img/TreeProject.svg" width=800>
</figure>



## Синтаксис
```
=^..^=
=^. Вас приветстует древнегреческий latex                           .^=
=^. Пока греки изучали latex, они полностью запутались              .^=
=^. поэтому нам теперь нужно помнить несколько правил               .^=
=^..^=
=^. 0  и самое важное! Программа пишется снизу вверх                .^=
=^. 1. В конце каждой операции нужно улыбнуться :)                  .^=
=^. 2. Для присваивания значения переменной ::=                     .^=
=^. 3. Для создания переменной используйте \Z \ni                   .^=
=^. 4. Для вывода переменной используйте \gravo                     .^=
=^. 5. if    это    \katarce ~condition~  \katapanma
               <=>  \katarce ~body~       \katapanma  :)            .^=
=^. 6. while это    \katarce ~condition~  \katapanma
              <?=?> \katarce ~body~       \katapanma  :)            .^=
=^. 7. Начало функции - \lemma                                      .^=
=^. 8. Конец  функции - \oed (oper edei deixai)                     .^=
=^. 9. Возвращаемое значение (: :)                                  .^=
=^. 10. Для вызова функции используйте :( ):                        .^=
=^. 11. Для перечисления переменных используйте >|<                 .^=
=^..^=
=^. В конце нужно пожелать богам программы всего хорошего!          .^=
=^..^=
```
```
 * Function     	  := '\oed' MultipleOperations '\lemma' ~nameFunc~
 * MultipleOperations := {Op}+
 * Op         		  := {CallFunc | FuncReturn | Create | Print | If | Assign } ':)'
 * CallFunc   		  := ':(' ~func~ [ '>|<' Expression ]* '):'
 * FuncReturn  		  := '(:' [ Expression ]
 * Create     		  := '\Z' '\in' ~var~
 * Print      		  := '\gravo' Expression
 * Assign     		  := ~var~ '::=' Eval
 * ConditionOp 		  := Cond ['<=>' '<?=?>'] Body
 * Cond       		  := '\katarce' Eval  {['>=' '<=' '>' '<' '==' '!=']}  Eval '\katapanma'
 * Body       		  := '\katarce' MultipleOperations  '\katapanma'   
 * Expression	      := Term  {['+' '-'] Term}*
 * Term        		  := Unary {['*' '/'] Unary}*
 * Unary      		  := ['sin' 'cos' 'ln' 'sqrt' ''] Primary
 * Primary    		  := '('Expression ')' | Value
 * Value      		  := ['0' - '9']+ | ~var~ | CallFunc
```

## Инструкция по эксплуатации (нервов ментора)

Рассмотрим на примере программы подсчета факториала: 

https://github.com/ludmilastemp/STL_language/blob/main/examples/factorial.stl.

## Front-end
Компиляция из языка программирование в дерево общего стандарта (AST).
```
./front-end.exe examples/factorial.stl examples/tree.ast
```
В файле `examples/tree.ast` находится дерево в формате post-order: 
```
examples/tree.ast:
(((_(__3 0 -1 -1 0 4 fact )2 0 15 -1 -1)(((__3 0 -1 -1 0 4 fact )...
```

<figure>
<img src="img/AST.png" width=800>
</figure>


## Back-end SPU
Компилирует AST дерево в файл на языке псевдо ассемблера:
```
./back-end-SPU.exe examples/tree.ast examples/spu.asm
``` 

В файле `examples/spu.asm` находится код программы на языке псевдо ассемблера: 
```
examples/spu.asm:
:main
$ начало main
		push 0
		pop rbp

		push 5
		pop [0 rbp]      $ fact

$ сохраняем значения переменных, до вызова функции
		push [0 rbp]     $ fact

$ передаем аргументы
		push [0 rbp]     $ fact

$ вызов функции
		call :func_0_

$ возвращаем значения переменных, которые были до вызова функции
		pop [0 rbp]      $ fact

$ push значение return
		push rax
		pop [0 rbp]      $ fact


		hlt
$ конец main
...
```


### Запуск
Дальше необходимо запустить компиляцию созданного файла:
```
./compile-SPU.exe examples/spu.asm examples/spu.bin 
``` 

В файле `examples/spu.bin` находится бинарный код виртуальной машины STL-SPU: 
```
examples/spu.bin:
00000000: 5354 4c20 7607 8100 0000 0022 0681 f401  STL v......"....
00000010: 0000 e200 0000 0006 e100 0000 0006 e100  ................
00000020: 0000 0006 8d38 0000 00e2 0000 0000 0621  .....8.........!
00000030: 01e2 0000 0000 0600 e264 0000 0006 e164  .........d.....d
00000040: 0000 0006 8164 0000 009e 5300 0000 8c60  .....d....S....`
00000050: 0000 0081 6400 0000 2201 0e8c 6500 0000  ....d..."...e...
00000060: 8c65 0000 00e1 6400 0000 06e1 6400 0000  .e....d.....d...
00000070: 06e1 6400 0000 0681 6400 0000 048d 3800  ..d.....d.....8.
00000080: 0000 e264 0000 0006 2101 0522 010e 0e    ...d....!.."...
...
```

После нужно исполнить скомпилированный файл на виртуальной машине:
```
./spu.exe examples/spu.bin 
``` 

## Back-end NASM
Компилирует AST дерево в файл на языке ассемблера NASM:
```
./back-end-NASM.exe examples/tree.ast examples/nasm.asm
``` 

В файле `examples/nasm.asm` находится код программы на языке ассемблера NASM: 
```
examples/spu.asm:
global Func
extern MySqrt

section .text
; начало main

Func:

		mov     rbp, rsp

		; Assign fact
		        ; const 5 
		mov     qword -16[rbp], 5

		mov     rax, -16[rbp]
		mov     -8[rbp], rax

		; Assign fact

		; сохраняем локальные перменные
		sub     rsp, 16

		; передаем аргументы

		        ; variable fact
		mov     rax, -8[rbp]
		mov     -16[rbp], rax

		xor     rax, rax
		mov     rax, -16[rbp]
		push    rax

		; Вызов функции
		push    rbp

		call Func_0_

		pop     rbp
...
```

### Запуск
Дальше необходимо cкомпиляцию файл с ассемблером и слинковать его с `execute-NASM/main.cpp`:
```
nasm -f elf64 examples/nasm.asm -o execute-NASM/nasm.o
g++ execute-NASM/main.cpp execute-NASM/nasm.o -o execute-NASM.exe
``` 

После можно запустить программу:
```
./execute-NASM.exe
120
``` 

## Back-end JIT
Компилирует Just-In-Time AST дерево в бинарный код процессора x86 и исполняет его:
```
./back-end-JIT.exe examples/tree.ast examples/jit.bin
120
``` 

В файле `examples/jit.bin` находится бинарный код x86, который только что исполнился: 
```
examples/jit.bin:
00000000: 5548 89e5 48c7 45f0 0500 0000 488b 45f0  UH..H.E.....H.E.
00000010: 4889 45f8 4883 ec10 488b 45f8 4889 45f0  H.E.H...H.E.H.E.
00000020: 488b 45f0 5055 e813 0000 005d 4883 c410  H.E.PU.....]H...
00000030: 4889 45f0 488b 45f0 4889 45f8 5dc3 4889  H.E.H.E.H.E.].H.
00000040: e559 5a58 4889 45f0 5251 488b 45f0 4889  .YZXH.E.RQH.E.H.
00000050: 45e8 48c7 45e0 0100 0000 488b 45e8 488b  E.H.E.....H.E.H.
00000060: 5de0 4839 d874 05e9 1500 0000 48c7 45e8  ].H9.t......H.E.
00000070: 0100 0000 4831 c048 8b45 e8c3 e900 0000  ....H1.H.E......
00000080: 0048 8b45 f048 8945 e848 83ec 2048 8b45  .H.E.H.E.H.. H.E
00000090: f048 8945 e048 c745 d801 0000 0048 8b45  .H.E.H.E.....H.E
000000a0: e048 8b5d d848 29d8 4889 45e0 488b 45e0  .H.].H).H.E.H.E.
000000b0: 5055 e887 ffff ff5d 4883 c420 4889 45e0  PU.....]H.. H.E.
000000c0: 488b 45e8 48f7 65e0 4889 45e8 4831 c048  H.E.H.e.H.E.H1.H
000000d0: 8b45 e8c3 c300 0000 0000 0000 0000 0000  .E..............
```

### Запуск
Чтобы запустить заранее скомпилированный код, укажите опцию `--precompiled`:
```
./back-end-JIT.exe --precompiled examples/jit.bin
120
``` 

## Back-end ELF
Компилирует AST дерево в исполняемый ELF (Executable and Linkable Format) файл:
```
./back-end-ELF.exe examples/tree.ast examples/elf.exe
``` 

В файле `examples/elf.exe` находится ELF файл:
```
readelf -a examples/elf.exe

ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x401000
  Start of program headers:          64 (bytes into file)
  Start of section headers:          8224 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         2
  Size of section headers:           64 (bytes)
  Number of section headers:         3
  Section header string table index: 2

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000401000  00001000
       0000000000001000  0000000000000000  AX       0     0     64
  [ 2] .shstrtab         STRTAB           0000000000000000  00002000
       0000000000000020  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), l (large), p (processor specific)

There are no section groups in this file.

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x00000000000000b0 0x00000000000000b0  R      0x1000
  LOAD           0x0000000000001000 0x0000000000401000 0x0000000000401000
                 0x0000000000001000 0x0000000000001000  R E    0x1000

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .text 

There is no dynamic section in this file.

There are no relocations in this file.
No processor specific unwind information to decode

No version information found in this file.
```
```
examples/elf.exe:
00000000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00000010: 0200 3e00 0100 0000 0010 4000 0000 0000  ..>.......@.....
00000020: 4000 0000 0000 0000 2020 0000 0000 0000  @.......  ......
00000030: 0000 0000 4000 3800 0200 4000 0300 0200  ....@.8...@.....
00000040: 0100 0000 0400 0000 0000 0000 0000 0000  ................
00000050: 0000 4000 0000 0000 0000 4000 0000 0000  ..@.......@.....
00000060: b000 0000 0000 0000 b000 0000 0000 0000  ................
00000070: 0010 0000 0000 0000 0100 0000 0500 0000  ................
00000080: 0010 0000 0000 0000 0010 4000 0000 0000  ..........@.....
00000090: 0010 4000 0000 0000 0010 0000 0000 0000  ..@.............
000000a0: 0010 0000 0000 0000 0010 0000 0000 0000  ................
...
00001000: 5548 89e5 48c7 45f0 0900 0000 488b 45f0  UH..H.E.....H.E.
00001010: 4889 45f8 4883 ec10 488b 45f8 4889 45f0  H.E.H...H.E.H.E.
00001020: 488b 45f0 5055 e823 0000 005d 4883 c410  H.E.PU.#...]H...
00001030: 4889 45f0 488b 45f0 4889 45f8 488b 45f8  H.E.H.E.H.E.H.E.
00001040: 4889 45f0 488b 45f0 4889 45f8 5d48 c7c7  H.E.H.E.H.E.]H..
00001050: 3900 0000 48c7 c03c 0000 000f 05c3 4889  9...H..<......H.
00001060: e559 5a58 4889 45f0 5251 488b 45f0 4889  .YZXH.E.RQH.E.H.
00001070: 45e8 48c7 45e0 0200 0000 488b 45e8 488b  E.H.E.....H.E.H.
00001080: 5de0 4839 d876 05e9 1500 0000 48c7 45e8  ].H9.v......H.E.
00001090: 0100 0000 4831 c048 8b45 e8c3 e900 0000  ....H1.H.E......
000010a0: 0048 c745 e800 0000 0048 8b45 e848 8945  .H.E.....H.E.H.E
000010b0: e848 8b45 e848 8945 e048 83ec 2848 8b45  .H.E.H.E.H..(H.E
...
```

### Запуск
Чтобы запустить программу, необходимо настроить права доступа:
```
chmod +x examples/elf.exe
./examples/elf.exe
```
В ответе как и раньше получим:
```
echo $?
120
```

## Сравнение скоростей
С помощью утилиты perf stat измерим время подсчета 42 число Фибоначчи (267914296)
```
perf stat ./spu-fast.exe
perf stat ./execute-NASM.exe
perf stat ./back-end-JIT.exe --precompiled
perf stat ./examples/elf.exe 
```
Результаты:
```
 Performance counter stats for './spu-fast.exe':

         45 797,52 msec task-clock                       #    1,000 CPUs utilized             
               552      context-switches                 #   12,053 /sec                      
               135      cpu-migrations                   #    2,948 /sec                      
                72      page-faults                      #    1,572 /sec                      
   193 484 169 612      cpu_core/cycles/                 #    4,225 GHz                         (99,09%)
   127 136 491 822      cpu_atom/cycles/                 #    2,776 GHz                         (0,52%)
 1 154 261 317 656      cpu_core/instructions/           #    5,97  insn per cycle              (99,09%)
   511 681 285 668      cpu_atom/instructions/           #    2,64  insn per cycle              (0,60%)
   199 827 346 217      cpu_core/branches/               #    4,363 G/sec                       (99,09%)
    88 199 842 914      cpu_atom/branches/               #    1,926 G/sec                       (0,61%)
       306 033 532      cpu_core/branch-misses/          #    0,15% of all branches             (99,09%)
       238 214 095      cpu_atom/branch-misses/          #    0,12% of all branches             (0,60%)
             TopdownL1 (cpu_core)                 #      0,8 %  tma_backend_bound      
                                                  #      4,0 %  tma_bad_speculation    
                                                  #      7,0 %  tma_frontend_bound     
                                                  #     88,2 %  tma_retiring             (99,09%)
             TopdownL1 (cpu_atom)                 #      4,1 %  tma_bad_speculation      (0,59%)
                                                  #     12,7 %  tma_frontend_bound       (0,57%)
                                                  #      2,3 %  tma_backend_bound      
                                                  #      2,3 %  tma_backend_bound_aux    (0,57%)
                                                  #     79,6 %  tma_retiring             (0,55%)

      45,813827385 seconds time elapsed
      45,798106000 seconds user
       0,004000000 seconds sys

```
```
 Performance counter stats for './execute-NASM.exe':

          1 362,75 msec task-clock                       #    0,998 CPUs utilized             
               198      context-switches                 #  145,295 /sec                      
                 0      cpu-migrations                   #    0,000 /sec                      
             4 279      page-faults                      #    3,140 K/sec                     
     6 075 967 045      cpu_core/cycles/                 #    4,459 GHz                       
     <not counted>      cpu_atom/cycles/                                                        (0,00%)
    24 163 154 464      cpu_core/instructions/           #    3,98  insn per cycle            
     <not counted>      cpu_atom/instructions/                                                  (0,00%)
     2 155 011 376      cpu_core/branches/               #    1,581 G/sec                     
     <not counted>      cpu_atom/branches/                                                      (0,00%)
           172 214      cpu_core/branch-misses/          #    0,01% of all branches           
     <not counted>      cpu_atom/branch-misses/                                                 (0,00%)
             TopdownL1 (cpu_core)                 #     32,4 %  tma_backend_bound      
                                                  #      0,8 %  tma_bad_speculation    
                                                  #      0,8 %  tma_frontend_bound     
                                                  #     66,0 %  tma_retiring           

       1,365373974 seconds time elapsed
       1,360252000 seconds user
       0,002930000 seconds sys
```
```
 Performance counter stats for './back-end-JIT.exe --precompiled':

          1 363,69 msec task-clock                       #    0,997 CPUs utilized             
               225      context-switches                 #  164,994 /sec                      
                 0      cpu-migrations                   #    0,000 /sec                      
             4 348      page-faults                      #    3,188 K/sec                     
     6 059 117 640      cpu_core/cycles/                 #    4,443 GHz                       
     <not counted>      cpu_atom/cycles/                                                        (0,00%)
    23 362 696 688      cpu_core/instructions/           #    3,86  insn per cycle            
     <not counted>      cpu_atom/instructions/                                                  (0,00%)
     1 887 789 563      cpu_core/branches/               #    1,384 G/sec                     
     <not counted>      cpu_atom/branches/                                                      (0,00%)
           176 060      cpu_core/branch-misses/          #    0,01% of all branches           
     <not counted>      cpu_atom/branch-misses/                                                 (0,00%)
             TopdownL1 (cpu_core)                 #     34,0 %  tma_backend_bound      
                                                  #      1,2 %  tma_bad_speculation    
                                                  #      0,8 %  tma_frontend_bound     
                                                  #     64,0 %  tma_retiring           

       1,367353566 seconds time elapsed
       1,360543000 seconds user
       0,003981000 seconds sys
```
```
 Performance counter stats for './examples/elf.exe':

          1 364,15 msec task-clock                       #    0,999 CPUs utilized             
                97      context-switches                 #   71,106 /sec                      
                 3      cpu-migrations                   #    2,199 /sec                      
                 3      page-faults                      #    2,199 /sec                      
     6 021 369 962      cpu_core/cycles/                 #    4,414 GHz                       
     <not counted>      cpu_atom/cycles/                                                        (0,00%)
    23 311 536 015      cpu_core/instructions/           #    3,87  insn per cycle            
     <not counted>      cpu_atom/instructions/                                                  (0,00%)
     1 875 910 259      cpu_core/branches/               #    1,375 G/sec                     
     <not counted>      cpu_atom/branches/                                                      (0,00%)
           126 645      cpu_core/branch-misses/          #    0,01% of all branches           
     <not counted>      cpu_atom/branch-misses/                                                 (0,00%)
             TopdownL1 (cpu_core)                 #     34,5 %  tma_backend_bound      
                                                  #      0,8 %  tma_bad_speculation    
                                                  #      0,4 %  tma_frontend_bound     
                                                  #     64,3 %  tma_retiring           

       1,365750729 seconds time elapsed
       1,364664000 seconds user
       0,000000000 seconds sys
```

| Реализация | Время, с |
|------|------|
| SPU  | 45,813 |
| NASM | 1,365 |
| JIT  | 1,367 |
| ELF  | 1,365 |

Получили ускорение в 33,6 раз!

### Используемое оборудование
```
Architecture:            x86_64
  CPU op-mode(s):        32-bit, 64-bit
  Address sizes:         39 bits physical, 48 bits virtual
  Byte Order:            Little Endian
CPU(s):                  12
  On-line CPU(s) list:   0-11
Vendor ID:               GenuineIntel
  Model name:            12th Gen Intel(R) Core(TM) i7-1255U
    CPU family:          6
    Model:               154
    Thread(s) per core:  2
    Core(s) per socket:  10
    Socket(s):           1
    Stepping:            4
    CPU max MHz:         4700,0000
    CPU min MHz:         400,0000
    BogoMIPS:            5222.40
```

## Структура проекта 
```
.
├── back-end-ELF        - back-end (из AST дерева в исполняемый файл ELF)
├── back-end-JIT        - back-end (JIT (Just-In-Time) AST дерева в бинарный файл x86)
├── back-end-NASM       - back-end (из AST дерева в файл на языке nasm)
├── back-end-SPU        - back-end (из AST дерева в файл на языке для STL-SPU)
├── examples            - примеры и промежуточные файлы
├── execute-NASM        - запуск программы после обработки back-end-NASM
├── front-end           - front-end (из языка STL в AST дерево) 
├── img                 - картинки для README
├── lib                 - вспомогательные программы написанные ранее
│   ├── STL_Asm         - компиляция (из языка для STL-SPU в бинарный код STL-SPU)
│   ├── STL_BinTree     - работа с бинарным деревом
│   ├── STL_Graphviz    - построение графического AST
│   ├── STL_Onegin      - обработка и работа с файлом
│   ├── STL_Spu         - виртуальная машина 
│   │   └── include     - константы и структуры 
│   └── STL_Stack       - структура данных stack
└── temp                - папка с временными файлами
```

