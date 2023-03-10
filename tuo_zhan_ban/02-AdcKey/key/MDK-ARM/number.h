#ifndef  _NUMBER_H_
#define _NUMBER_H_

#include "lw_oopc.h"

// 构造一个类
CLASS (Number)
{
    void (*add) (int,int);
    void (*sub) (int,int);
};

void add(int a,int b);
void sub(int a,int b);

#endif
