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
[root@9d0875627904 read-book]# readelf -s test4.o   # 查看符号表

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




静态链接
```c
// a.c
extern int shared;
int main()
{
    int a = 100;
    swap(&a, &shared);
    return 0;
}
```
```c
// b.c
int shared = 1;
void swap(int *a, int * b)
{
    *a ^= *b ^= *a ^= *b;
}
```

`gcc -c a.c b.c`

vma 为 virtual memory address, 即虚拟地址
LMA 为 load memory address 即加载地址
```bash
[root@9d0875627904 read-book]# objdump -h a.o

a.o:     file format elf64-littleaarch64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000030  0000000000000000  0000000000000000  00000040  2**2    # 链接之前 ， 虚拟地址都是0， 未分配
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000000  0000000000000000  0000000000000000  00000070  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  00000070  2**0
                  ALLOC
  3 .comment      0000002d  0000000000000000  0000000000000000  00000070  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  0000009d  2**0
                  CONTENTS, READONLY
  5 .eh_frame     00000038  0000000000000000  0000000000000000  000000a0  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
[root@9d0875627904 read-book]# objdump -h b.o

b.o:     file format elf64-littleaarch64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         0000006c  0000000000000000  0000000000000000  00000040  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000004  0000000000000000  0000000000000000  000000ac  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  000000b0  2**0
                  ALLOC
  3 .comment      0000002d  0000000000000000  0000000000000000  000000b0  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000dd  2**0
                  CONTENTS, READONLY
  5 .eh_frame     00000030  0000000000000000  0000000000000000  000000e0  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA


```

`ld a.o b.o -e main -o ab`

```bash
[root@9d0875627904 read-book]# objdump -h ab

ab:     file format elf64-littleaarch64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         0000009c  0000000000400120  0000000000400120  00000120  2**2  #链接后 ， 虚拟地址分配好了
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .eh_frame     00000050  00000000004001c0  00000000004001c0  000001c0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .data         00000004  000000000041ffe8  000000000041ffe8  0000ffe8  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  3 .comment      0000002c  0000000000000000  0000000000000000  0000ffec  2**0
                  CONTENTS, READONLY
```



看一下 a.o 反汇编结果

```bash
[root@9d0875627904 read-book]# objdump -d a.o

a.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <main>:
   0:   a9be7bfd        stp     x29, x30, [sp, #-32]!
   4:   910003fd        mov     x29, sp
   8:   52800c80        mov     w0, #0x64                       // #100
   c:   b9001fe0        str     w0, [sp, #28]
  10:   910073e2        add     x2, sp, #0x1c
  14:   90000000        adrp    x0, 0 <shared>
  18:   91000001        add     x1, x0, #0x0
  1c:   aa0203e0        mov     x0, x2
  20:   94000000        bl      0 <swap>
  24:   52800000        mov     w0, #0x0                        // #0
  28:   a8c27bfd        ldp     x29, x30, [sp], #32
  2c:   d65f03c0        ret
```

让chatgpt来解释一下吧！

```
这是 ARM64（也被称为 AArch64）的汇编代码。让我们一起分析一下：

- `stp x29, x30, [sp, #-32]!`：这将64位寄存器`x29`和`x30`的值存入堆栈。`!`表示它是一个预索引存储，也就是说在存储之前会更新堆栈指针`sp`。

- `mov x29, sp`：这将堆栈指针移动到`x29`（在函数调用中经常用作帧指针）。

- `mov w0, #0x64`：将立即值`0x64`（十进制为100）移动到32位寄存器`w0`。

- `str w0, [sp, #28]`：将`w0`的值存储到由`sp+28`指定的内存位置。

- `add x2, sp, #0x1c`：将`0x1c`加到堆栈指针`sp`上，并将结果存储在`x2`。

- `adrp x0, 0 <shared>`：这个指令用于将一页的地址加载到寄存器中，在这里是`x0`。

- `add x1, x0, #0x0`：将0加到`x0`并将结果存储在`x1`。

- `mov x0, x2`：将`x2`的值移动到`x0`。

- `bl 0 <swap>`：这是一个带链接的分支。它调用地址为0的函数，这里标记为`swap`。链接意味着它还将返回地址存储在`x30`中。

- `mov w0, #0x0`：将立即值0移动到`w0`。

- `ldp x29, x30, [sp], #32`：从堆栈中加载寄存器`x29`和`x30`对的值，然后将堆栈指针加32。

- `ret`：从函数返回。

这似乎是一个简化的函数，它在堆栈上准备一些值，然后调用另一个名为`swap`的函数。`swap`函数返回后，它执行一些清理工作，然后返回。具体的目的会取决于`swap`函数的功能。
```



装载相关

```bash
[root@9d0875627904 read-book]# readelf -l  ab

Elf file type is EXEC (Executable file)
Entry point 0x400120
There are 4 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr   # 这部分对应了一个 文件头结构体
                 FileSiz            MemSiz              Flags  Align
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x0000000000000210 0x0000000000000210  R E    0x10000
  LOAD           0x000000000000ffe8 0x000000000041ffe8 0x000000000041ffe8
                 0x0000000000000004 0x0000000000000004  RW     0x10000
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x000000000000ffe8 0x000000000041ffe8 0x000000000041ffe8
                 0x0000000000000004 0x0000000000000018  R      0x1

 Section to Segment mapping:  # 具有相同属性的section 被映射到一个segment
  Segment Sections...
   00     .text .eh_frame 
   01     .data 
   02     
   03     .data 

```
`VirtAddr`为segment 的第一个字节在进程虚拟地址空间的起始位置
`PhysAddr`为 物理装载地址， 即LMA（load memory address）
这两个值一般一样

注意： 
```bash
[root@9d0875627904 read-book]# readelf -l  a.o     // 目标文件没有程序头

There are no program headers in this file.
```




动态链接
terminal 1
```bash
[root@9d0875627904 build]# ./bin/run_main &
[1] 236
[root@9d0875627904 build]# hello world, new
```

terminal 2
```bash
[root@9d0875627904 local]# cat /proc/236/maps 
00400000-00401000 r-xp 00000000 00:a1 11792410                           /usr/local/wujiayang/linux/read-book/test_dynamic_link/build/bin/run_main
0041f000-00420000 r--p 0000f000 00:a1 11792410                           /usr/local/wujiayang/linux/read-book/test_dynamic_link/build/bin/run_main
00420000-00421000 rw-p 00010000 00:a1 11792410                           /usr/local/wujiayang/linux/read-book/test_dynamic_link/build/bin/run_main
31e71000-31e92000 rw-p 00000000 00:00 0                                  [heap]
ffffbd1eb000-ffffbd345000 r-xp 00000000 fe:01 3152546                    /usr/lib64/libc-2.28.so
ffffbd345000-ffffbd357000 ---p 0015a000 fe:01 3152546                    /usr/lib64/libc-2.28.so
ffffbd357000-ffffbd35b000 r--p 0015c000 fe:01 3152546                    /usr/lib64/libc-2.28.so
ffffbd35b000-ffffbd35d000 rw-p 00160000 fe:01 3152546                    /usr/lib64/libc-2.28.so
ffffbd35d000-ffffbd360000 rw-p 00000000 00:00 0 
ffffbd360000-ffffbd373000 r-xp 00000000 fe:01 3152400                    /usr/lib64/libgcc_s-8-20210514.so.1
ffffbd373000-ffffbd38f000 ---p 00013000 fe:01 3152400                    /usr/lib64/libgcc_s-8-20210514.so.1
ffffbd38f000-ffffbd390000 r--p 0001f000 fe:01 3152400                    /usr/lib64/libgcc_s-8-20210514.so.1
ffffbd390000-ffffbd391000 rw-p 00020000 fe:01 3152400                    /usr/lib64/libgcc_s-8-20210514.so.1
ffffbd391000-ffffbd43e000 r-xp 00000000 fe:01 3152550                    /usr/lib64/libm-2.28.so
ffffbd43e000-ffffbd450000 ---p 000ad000 fe:01 3152550                    /usr/lib64/libm-2.28.so
ffffbd450000-ffffbd451000 r--p 000af000 fe:01 3152550                    /usr/lib64/libm-2.28.so
ffffbd451000-ffffbd452000 rw-p 000b0000 fe:01 3152550                    /usr/lib64/libm-2.28.so
ffffbd452000-ffffbd5cb000 r-xp 00000000 fe:01 3152735                    /usr/lib64/libstdc++.so.6.0.25
ffffbd5cb000-ffffbd5e6000 ---p 00179000 fe:01 3152735                    /usr/lib64/libstdc++.so.6.0.25
ffffbd5e6000-ffffbd5f2000 r--p 00184000 fe:01 3152735                    /usr/lib64/libstdc++.so.6.0.25
ffffbd5f2000-ffffbd5f3000 rw-p 00190000 fe:01 3152735                    /usr/lib64/libstdc++.so.6.0.25
ffffbd5f3000-ffffbd5f6000 rw-p 00000000 00:00 0 
ffffbd5f6000-ffffbd5f7000 r-xp 00000000 00:a1 11780443                   /usr/local/wujiayang/linux/read-book/test_dynamic_link/build/lib/libhellolib.so  #w为什么分成了4个segment
ffffbd5f7000-ffffbd615000 ---p 00001000 00:a1 11780443                   /usr/local/wujiayang/linux/read-book/test_dynamic_link/build/lib/libhellolib.so
ffffbd615000-ffffbd616000 r--p 0000f000 00:a1 11780443                   /usr/local/wujiayang/linux/read-book/test_dynamic_link/build/lib/libhellolib.so
ffffbd616000-ffffbd617000 rw-p 00010000 00:a1 11780443                   /usr/local/wujiayang/linux/read-book/test_dynamic_link/build/lib/libhellolib.so
ffffbd617000-ffffbd63a000 r-xp 00000000 fe:01 3152539                    /usr/lib64/ld-2.28.so
ffffbd64a000-ffffbd64e000 rw-p 00000000 00:00 0 
ffffbd651000-ffffbd653000 rw-p 00000000 00:00 0 
ffffbd653000-ffffbd655000 r--p 00000000 00:00 0                          [vvar]
ffffbd655000-ffffbd656000 r-xp 00000000 00:00 0                          [vdso]
ffffbd656000-ffffbd657000 r--p 0002f000 fe:01 3152539                    /usr/lib64/ld-2.28.so   # 动态连接器
ffffbd657000-ffffbd659000 rw-p 00030000 fe:01 3152539                    /usr/lib64/ld-2.28.so
fffff4981000-fffff49a2000 rw-p 00000000 00:00 0                          [stack]

```


来自chatgpt的解释
```
这四行代表了动态链接库`libhellolib.so`在进程内存中的不同区段。每个区段有其自己的权限并服务于不同的目的。具体来说：

- 第一行：`r-xp`这个内存段是可读和可执行的。动态链接库的代码部分通常在这个区段。

- 第二行：`---p`这个内存段没有任何访问权限（既不可读，也不可写，也不可执行）。这通常是两个不同权限的区段之间的一个“保护页”，用于阻止内存访问溢出到不应访问的区段。

- 第三行：`r--p`这个内存段是只读的。这里通常存储了库的只读数据，例如常量或者字符串字面量。

- 第四行：`rw-p`这个内存段是可读写的。这里通常存储了库的读写数据，例如全局变量。

这些区段的存在使得操作系统能够更好地管理内存，例如防止代码被意外修改（代码区段不可写），防止数据被执行（数据区段不可执行），等等。同时，这些信息也非常有利于开发者进行调试和性能优化。
```


查看动态链接库的装载属性
```bash
[root@9d0875627904 test_dynamic_link]# readelf build/lib/libhellolib.so  -l

Elf file type is DYN (Shared object file)  #s是一个共享对象
Entry point 0x8a0
There are 7 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000b74 0x0000000000000b74  R E    0x10000
  LOAD           0x000000000000fd88 0x000000000001fd88 0x000000000001fd88
                 0x00000000000002a8 0x00000000000002b8  RW     0x10000
  DYNAMIC        0x000000000000fda8 0x000000000001fda8 0x000000000001fda8
                 0x0000000000000200 0x0000000000000200  RW     0x8
  NOTE           0x00000000000001c8 0x00000000000001c8 0x00000000000001c8
                 0x0000000000000024 0x0000000000000024  R      0x4
  GNU_EH_FRAME   0x0000000000000a54 0x0000000000000a54 0x0000000000000a54
                 0x0000000000000044 0x0000000000000044  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x000000000000fd88 0x000000000001fd88 0x000000000001fd88
                 0x0000000000000278 0x0000000000000278  R      0x1

 Section to Segment mapping:
  Segment Sections...
   00     .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .plt .text .fini .rodata .eh_frame_hdr .eh_frame 
   01     .init_array .fini_array .data.rel.ro .dynamic .got .got.plt .bss 
   02     .dynamic 
   03     .note.gnu.build-id 
   04     .eh_frame_hdr 
   05     
   06     .init_array .fini_array .data.rel.ro .dynamic .got 
```