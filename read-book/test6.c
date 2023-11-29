#include <stdio.h>
// void test_weak_ref()
// {
//     printf("this is weak ref\n");
// }

static void foo() __attribute__((weakref("test_weak_ref")));
// __attribute__((weak)) void foo();
int main()
{
    if (foo)
        foo();
    else {
        printf("no weak ref\n");
    }
    return 0;
}