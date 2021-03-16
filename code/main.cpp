#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "detour.h"

int func(int a, int b)
{
    printf("calling to func\n");
    return a+b;
}
typedef int (*funcptr)(int a, int b); 

funcptr fptr;

int hook(int a, int b)
{
    printf("hook~~\n");
    if (fptr == 0)
    {
        printf("not write...?\n");
        return -1;
    }
    int r = fptr(a, b);
    return a*b+r;
}

int main(int ac,char**av)
{
    detour_func((uint64_t)func, 12, (uint64_t)hook, (uint64_t*)(&fptr));
    printf("res:%d\n", func(1,2));
}