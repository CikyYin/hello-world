#ifndef __DS1302_H_
#define __DS1302_H_

#include <reg52.h>
#include "Define.h"
#include "System.h"

//日期时间数据结构体
typedef struct{
	char Sec;
	char Min;
	char Hour;
	char Day;
	char Week;
	char Mon;
	char Year;
}TDATE;

/*************************** 变量声明区 ********************************/
extern TDATE DateTime;  //日期时间数据
extern uchar SetDS1302Pointer;  //DS1302设置索引
/*************************** 函数声明区 ********************************/
extern void InitDS1302(void);						//1302标准初始化 设定初始时间为23：59：56，日期为2011.12.29
extern void GetDateTime(void);
extern void SetDateTime(uchar KeyValue);
#endif /*__DS1302_H_*/