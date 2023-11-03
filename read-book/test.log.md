```c++
int main()
{
    int a = 1;
    return 0;
}
```
After `g++ -c test1.cpp` , we will get test1.o
```bash
[root@9d0875627904 read-book]# g++ -c test1.cpp 
[root@9d0875627904 read-book]# ls
test1.cpp  test1.o
[root@9d0875627904 read-book]# file test1.o
test1.o: ELF 64-bit LSB relocatable, ARM aarch64, version 1 (SYSV), not stripped
[root@9d0875627904 read-book]# g++ test1.cpp 
[root@9d0875627904 read-book]# file a.out 
a.out: ELF 64-bit LSB executable, ARM aarch64, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, for GNU/Linux 3.7.0, BuildID[sha1]=31a803a88930280fdb747312ba855daaeaa3715f, not stripped
```




```c++
int g1 = 2;
double g2;
int main()
{
    int a = 1;
    return 0;
}
```
`g++ -c test1.cpp`


```bash
[root@9d0875627904 read-book]# objdump -h test1.o

test1.o:     file format elf64-littleaarch64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000018  0000000000000000  0000000000000000  00000040  2**2     # code
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000004  0000000000000000  0000000000000000  00000058  2**2     # int g1 = 2;   // global initialized var
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000008  0000000000000000  0000000000000000  00000060  2**3     # int g2;     // global uninitialized var
                  ALLOC
  3 .comment      0000002d  0000000000000000  0000000000000000  00000060  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  0000008d  2**0
                  CONTENTS, READONLY
  5 .eh_frame     00000030  0000000000000000  0000000000000000  00000090  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
```


`test2.c`
```c
int g1 = 2;      // store in .data
double g2 = 0;  // will store in .bss, treat it as uninitialized var
int main()
{
    int a = 1;
    return 0;
}
```
g2 will store in .bss
```bash
[root@9d0875627904 read-book]# objdump -h test2.o

test2.o:     file format elf64-littleaarch64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000018  0000000000000000  0000000000000000  00000040  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000004  0000000000000000  0000000000000000  00000058  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000008  0000000000000000  0000000000000000  00000060  2**3
                  ALLOC
  3 .comment      0000002d  0000000000000000  0000000000000000  00000060  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  0000008d  2**0
                  CONTENTS, READONLY
  5 .eh_frame     00000030  0000000000000000  0000000000000000  00000090  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
```
if change `double g2 = 0;` to `double g2 = 1;`, then it will store in .data section
like below:
```bash
[root@9d0875627904 read-book]# objdump -h test2.o

test2.o:     file format elf64-littleaarch64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000018  0000000000000000  0000000000000000  00000040  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000010  0000000000000000  0000000000000000  00000058  2**3   #int g1 and double g2, totally occupy 16 bytes
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  00000068  2**0   # nothing
                  ALLOC
  3 .comment      0000002d  0000000000000000  0000000000000000  00000068  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  00000095  2**0
                  CONTENTS, READONLY
  5 .eh_frame     00000030  0000000000000000  0000000000000000  00000098  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA

```


`test3.c`
```c
__attribute((section("FOO"))) int g1 = 2;      // store in .data
double g2 = 0;  // will store in .bss, treat it as uninitialized var
int main()
{
    int a = 1;
    return 0;
}
```

```bash
[root@9d0875627904 read-book]# objdump -h test3.o

test3.o:     file format elf64-littleaarch64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000018  0000000000000000  0000000000000000  00000040  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000000  0000000000000000  0000000000000000  00000058  2**0  
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000008  0000000000000000  0000000000000000  00000058  2**3   # double g2 = 0
                  ALLOC
  3 FOO           00000004  0000000000000000  0000000000000000  00000058  2**2   # self define section: int g1 = 2
                  CONTENTS, ALLOC, LOAD, DATA
  4 .comment      0000002d  0000000000000000  0000000000000000  0000005c  2**0
                  CONTENTS, READONLY
  5 .note.GNU-stack 00000000  0000000000000000  0000000000000000  00000089  2**0
                  CONTENTS, READONLY
  6 .eh_frame     00000030  0000000000000000  0000000000000000  00000090  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
```



```bash
[root@9d0875627904 read-book]# readelf -h test2.o 
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          696 (bytes into file)    # e_shoff, 段表的起始位置
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         11
  Section header string table index: 10
  ```

```bash
[root@9d0875627904 read-book]# readelf -h test1.o
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          832 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         13
  Section header string table index: 12 # 指示了shstrtab, 即段表字符串表的下标为[12]
```

section table(段表)
```bash
[root@9d0875627904 read-book]# readelf -S test1.o 
There are 13 section headers, starting at offset 0x340:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       000000000000003c  0000000000000000  AX       0     0     4
  [ 2] .rela.text        RELA             0000000000000000  00000290
       0000000000000030  0000000000000018   I      10     1     8    # 是 .text 的重定位表， info 为1， 就是 .text 的下标[1]
  [ 3] .data             PROGBITS         0000000000000000  0000007c
       0000000000000004  0000000000000000  WA       0     0     4
  [ 4] .bss              NOBITS           0000000000000000  00000080
       0000000000000000  0000000000000000  WA       0     0     1
  [ 5] .rodata           PROGBITS         0000000000000000  00000080
       000000000000000c  0000000000000000   A       0     0     8
  [ 6] .comment          PROGBITS         0000000000000000  0000008c
       000000000000002d  0000000000000001  MS       0     0     1
  [ 7] .note.GNU-stack   PROGBITS         0000000000000000  000000b9
       0000000000000000  0000000000000000           0     0     1
  [ 8] .eh_frame         PROGBITS         0000000000000000  000000c0
       0000000000000030  0000000000000000   A       0     0     8
  [ 9] .rela.eh_frame    RELA             0000000000000000  000002c0
       0000000000000018  0000000000000018   I      10     8     8
  [10] .symtab           SYMTAB           0000000000000000  000000f0   #符号表
       0000000000000180  0000000000000018          11    13     8
  [11] .strtab           STRTAB           0000000000000000  00000270  # 字符串表， 变量名之类
       000000000000001a  0000000000000000           0     0     1
  [12] .shstrtab         STRTAB           0000000000000000  000002d8  # 段表字符串表 ， 常见的是段名
       0000000000000061  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  p (processor specific)
[root@9d0875627904 read-bo

  ```



source code 
test4.c
```c
#include <stdio.h>
extern int a;
extern int fun1();

int fun2()
{
    return 0;
}

int main()
{
    printf("%d\n", a);
    fun1();
    fun2();
    return 0;
}
```
test5.c
```c
int a = 10;

int fun1()
{
    return 0;
}
```
gcc -c test4.c test5.c
will get test4.o and test5.o


```bash
[root@9d0875627904 read-book]# readelf -s test4.o

Symbol table '.symtab' contains 17 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS test4.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 
     5: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    1 $x
     6: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 
     7: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    5 $d
     8: 0000000000000000     0 SECTION LOCAL  DEFAULT    7 
     9: 0000000000000014     0 NOTYPE  LOCAL  DEFAULT    8 $d
    10: 0000000000000000     0 SECTION LOCAL  DEFAULT    8 
    11: 0000000000000000     0 SECTION LOCAL  DEFAULT    6 
    12: 0000000000000000     8 FUNC    GLOBAL DEFAULT    1 fun2
    13: 0000000000000008    56 FUNC    GLOBAL DEFAULT    1 main    # main 这个符号所属的段的下标是[1], 通过readefl -S test4.o 查看后是一个.text 段
    14: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND a       # 在别的目标文件
    15: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf  # 在别的目标文件
    16: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND fun1    # 在别的目标文件

```