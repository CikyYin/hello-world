#ifndef __1602_H_
#define __1602_H_

#include <reg52.h>
#include "Define.h"
#include "System.h"

typedef struct{
	unsigned char DisNum;			//显示屏数
	unsigned char DisTimer;			//显示时间
	unsigned char LastDisNum;			//上一屏显示
}TDISPLAY;

/*************************** 变量声明区 ********************************/
extern TDISPLAY Display;

/*************************** 函数定义区 *********************************/
extern void Init1602(void);
extern void ProcDisplay(void);
extern void ShowNum(uchar Row, uchar Column, uint Num, uchar Len);

#endif/*__1602_H_*/
