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

查看 动态链接的可执行文件的段， 其中包含了.got, global offset table ， 全局偏移表， 用来间接寻址
```bash
[root@9d0875627904 test_dynamic_link]# objdump -h build/bin/run_main 

build/bin/run_main:     file format elf64-littleaarch64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .interp       0000001b  0000000000400238  0000000000400238  00000238  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .note.ABI-tag 00000020  0000000000400254  0000000000400254  00000254  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .note.gnu.build-id 00000024  0000000000400274  0000000000400274  00000274  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .gnu.hash     00000040  0000000000400298  0000000000400298  00000298  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .dynsym       00000168  00000000004002d8  00000000004002d8  000002d8  2**3  # 动态符号表
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .dynstr       00000132  0000000000400440  0000000000400440  00000440  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .gnu.version  0000001e  0000000000400572  0000000000400572  00000572  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .gnu.version_r 00000020  0000000000400590  0000000000400590  00000590  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  8 .rela.dyn     00000048  00000000004005b0  00000000004005b0  000005b0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  9 .rela.plt     00000078  00000000004005f8  00000000004005f8  000005f8  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 10 .init         00000014  0000000000400670  0000000000400670  00000670  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 11 .plt          00000070  0000000000400690  0000000000400690  00000690  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 12 .text         000001ac  0000000000400700  0000000000400700  00000700  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 13 .fini         00000010  00000000004008ac  00000000004008ac  000008ac  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 14 .rodata       00000010  00000000004008c0  00000000004008c0  000008c0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 15 .eh_frame_hdr 00000044  00000000004008d0  00000000004008d0  000008d0  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 16 .eh_frame     000000e4  0000000000400918  0000000000400918  00000918  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 17 .init_array   00000008  000000000041fd98  000000000041fd98  0000fd98  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 18 .fini_array   00000008  000000000041fda0  000000000041fda0  0000fda0  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .dynamic      00000220  000000000041fda8  000000000041fda8  0000fda8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 20 .got          00000020  000000000041ffc8  000000000041ffc8  0000ffc8  2**3 #用来保存全局变量引用
                  CONTENTS, ALLOC, LOAD, DATA
 21 .got.plt      00000040  000000000041ffe8  000000000041ffe8  0000ffe8  2**3 #用来保存函数引用的地址
                  CONTENTS, ALLOC, LOAD, DATA
 22 .data         00000004  0000000000420028  0000000000420028  00010028  2**0
                  CONTENTS, ALLOC, LOAD, DATA
 23 .bss          00000004  000000000042002c  000000000042002c  0001002c  2**0
                  ALLOC
 24 .comment      00000058  0000000000000000  0000000000000000  0001002c  2**0
                  CONTENTS, READONLY
 25 .gnu.build.attributes 00001988  0000000000420030  0000000000420030  00010084  2**2
                  CONTENTS, READONLY
```


贴上一段chatgpt 对一些概念的解释

.got.plt和.plt是Unix和类Unix系统中的两种特殊的符号表，它们在动态链接的过程中起到了关键的作用。

1. .plt (Procedure Linkage Table):  # 这个跟代码段放在一起， 地址无关的

    过程链接表是一种特殊的跳转表，运行时链接器可以修改这个表使得程序跳转到正确的函数地址。当程序第一次尝试调用一个动态链接库中的函数时，它会首先跳转到一个在PLT中的"桩"函数。这个桩函数会调用运行时链接器，运行时链接器会查找正确的函数地址，然后更新.got.plt中对应的条目。后续的函数调用将直接跳转到正确的函数地址。

2. .got.plt (Global Offset Table for PLT):   #保存的是函数， 这个跟数据段放在一起， 可修改

    **全局偏移表用于存储动态链接的函数地址**。每个动态链接的函数在GOT中都有一个条目，这个条目原本指向PLT中的一个桩函数，但在函数第一次被调用之后，运行时链接器会将这个条目更新为正确的函数地址。GOT是必要的，因为程序的代码段通常是只读的，而GOT位于数据段，可以在运行时被修改。

总的来说，.plt和.got.plt是动态链接过程的关键部分，它们使得程序可以在运行时加载和链接到动态链接库中的函数。





查看动态链接的可执行文件的段的内容， 其中有一个叫做.interp  
```bash

[root@9d0875627904 test_dynamic_link]# objdump -s build/bin/run_main 

build/bin/run_main:     file format elf64-littleaarch64

Contents of section .interp: # 这个就是动态连接器的路径
 400238 2f6c6962 2f6c642d 6c696e75 782d6161  /lib/ld-linux-aa
 400248 72636836 342e736f 2e3100             rch64.so.1.     
Contents of section .note.ABI-tag:
 400254 04000000 10000000 01000000 474e5500  ............GNU.
 400264 00000000 03000000 07000000 00000000  ................
Contents of section .note.gnu.build-id:
 400274 04000000 14000000 03000000 474e5500  ............GNU.
 400284 16b33af7 785829e8 b1b2a81e 874a7d45  ..:.xX)....

```


软连接 指向../lib64/ld-2.28.so， 真正的动态链接器
```
[root@9d0875627904 test_dynamic_link]# ls /lib/ld-linux-aarch64.so.1 -l
lrwxrwxrwx 1 root root 19 Aug 24  2021 /lib/ld-linux-aarch64.so.1 -> ../lib64/ld-2.28.so
```


查看动态链接的文件的重定位表
```bash
[root@9d0875627904 build]# readelf -r lib/libhellolib.so 

Relocation section '.rela.dyn' at offset 0x670 contains 11 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
00000001fd88  000000000403 R_AARCH64_RELATIV                    968
00000001fd90  000000000403 R_AARCH64_RELATIV                    a08
00000001fd98  000000000403 R_AARCH64_RELATIV                    920
00000001fda0  000000000403 R_AARCH64_RELATIV                    1fda0
00000001ffb0  000300000401 R_AARCH64_GLOB_DA 0000000000000000 _ZSt4endlIcSt11char_tr@GLIBCXX_3.4 + 0
00000001ffb8  000400000401 R_AARCH64_GLOB_DA 0000000000000000 __cxa_finalize@GLIBC_2.17 + 0
00000001ffc0  000800000401 R_AARCH64_GLOB_DA 0000000000000000 _ZSt4cout@GLIBCXX_3.4 + 0
00000001ffc8  000a00000401 R_AARCH64_GLOB_DA 0000000000000000 _ITM_deregisterTMClone + 0
00000001ffd0  000b00000401 R_AARCH64_GLOB_DA 0000000000000000 __gmon_start__ + 0
00000001ffd8  000c00000401 R_AARCH64_GLOB_DA 0000000000000000 _ITM_registerTMCloneTa + 0
00000001ffe0  000d00000401 R_AARCH64_GLOB_DA 0000000000000000 _ZNSt8ios_base4InitD1E@GLIBCXX_3.4 + 0

Relocation section '.rela.plt' at offset 0x778 contains 6 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000020000  000400000402 R_AARCH64_JUMP_SL 0000000000000000 __cxa_finalize@GLIBC_2.17 + 0
000000020008  000500000402 R_AARCH64_JUMP_SL 0000000000000000 _ZStlsISt11char_traits@GLIBCXX_3.4 + 0
000000020010  000600000402 R_AARCH64_JUMP_SL 0000000000000000 _ZNSolsEPFRSoS_E@GLIBCXX_3.4 + 0
000000020018  000700000402 R_AARCH64_JUMP_SL 0000000000000000 __cxa_atexit@GLIBC_2.17 + 0
000000020020  000900000402 R_AARCH64_JUMP_SL 0000000000000000 _ZNSt8ios_base4InitC1E@GLIBCXX_3.4 + 0
000000020028  000b00000402 R_AARCH64_JUMP_SL 0000000000000000 __gmon_start__ + 0
```

从上面可以看到三种type
R_AARCH64_RELATIV   
R_AARCH64_GLOB_DA
R_AARCH64_JUMP_SL
下面两种被修正的位置直接填入符号的地址即可



查看dynamic section 
```bash
[root@9d0875627904 test_dynamic_link]# readelf -d build/lib/libhellolib.so 

Dynamic section at offset 0xfda8 contains 28 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [libstdc++.so.6]
 0x0000000000000001 (NEEDED)             Shared library: [libm.so.6]
 0x0000000000000001 (NEEDED)             Shared library: [libgcc_s.so.1]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000000e (SONAME)             Library soname: [libhellolib.so]

 ```



 在x86 架构的服务器上
 test.c
 ```c
 int main()
 {
      return 123;
 }
```

`gcc -S test.c`
```asm
main:
.LFB0:
        .cfi_startproc
        pushq   %rbp               // 压入 帧指针
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp         // 将 帧指针寄存器更新栈顶
        .cfi_def_cfa_register 6
        movl    $123, %eax         // 返回值存放在 eax 寄存器
        popq    %rbp               // 恢复 帧指针寄存器
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
```

例子2
```c
int fun()
{
	int a = 1;
	return a;
}

int main()
{
	fun();
	return 123;
}
```


汇编代码
```asm
        .file   "test.c"
        .text
        .globl  fun
        .type   fun, @function
fun:
.LFB0:
        .cfi_startproc
        pushq   %rbp // 保存在栈上
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp 
        .cfi_def_cfa_register 6
        movl    $1, -4(%rbp)    // 局部变量赋值， 往下生长
        movl    -4(%rbp), %eax
        popq    %rbp
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
.LFE0:
        .size   fun, .-fun
        .globl  main
        .type   main, @function
main:
.LFB1:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        movl    $0, %eax
        call    fun
        movl    $123, %eax
        popq    %rbp
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
.LFE1:

```
上面的汇编代码中看不到rsp 寄存器的恢复， 猜测应该时自动的