// gcc  test4.c test5.c
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