#include "number.h"
#include "lw_oopc.h"
#include "stdio.h"

// 函数声明
// static void add(int a,int b);
// static void sub(int a,int b);

// 构造器  分配空间给类
CTOR (Number)
    FUNCTION_SETTING(add,add);
    FUNCTION_SETTING(sub,sub);
END_CTOR

void add(int a,int b)
{
    printf("sum:%d\n",a+b);
}

void sub(int a,int b)
{
    printf("sub:%d\n",a-b);
}



