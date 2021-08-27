#ifndef __18B20_H_
#define __18B20_H_

#include <reg52.h>
#include "Define.h"
#include "System.h"

//温度数据结构体
typedef struct{
    unsigned int Value; //温度值
   // unsigned int LastValue;
    unsigned char Flag; //正负标志
}TTEMP;

/*************************** 变量声明区 ********************************/
extern TTEMP Temp;
/*************************** 函数声明区 ********************************/
extern void Init18B20(void);
extern void ReadTemp(void);

#endif
