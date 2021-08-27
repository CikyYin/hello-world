#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include <reg52.h>
#include "Define.h"
#include "1602.h"
#include "DS1302.h"
#include "18B20.h"

typedef struct{
	uchar 	Counter;
	uchar 	Num;
	uint 	CurrentTimer;
	uint 	LastTimer;
	ulong 	SumTimer;	
}THEARTRATE;

typedef struct{
	uchar 	T100msFlag;
	uchar 	T500msFlag;
	uchar 	TSecFlag;
	uint 	T10msCounter;
	uint 	T100msCounter;
	uint 	T500msCounter;
}TTIMER;

typedef struct{
	uint 	Length;
	uint 	Num;
	ulong 	Distance;
}TSTEP;
/*************************** 标志位声明区 ********************************/
#define DisplayFlag			SysFlag.b.b0
#define ReadDS1302Flag		SysFlag.b.b1
#define Read18B20Flag		SysFlag.b.b2
#define KeyDownFlag			SysFlag.b.b3
#define SysSetFlag			SysFlag.b.b4
/*************************** 变量声明区 ********************************/
extern TUNION08 		SysFlag;  //系统标志位
extern THEARTRATE 		HeartRate;
extern TTIMER 			Timer;
extern TSTEP 			Step;
extern unsigned char KeyValue;
/*************************** 函数声明区 ********************************/
extern void Delay1us(unsigned int n);
extern void Delay1ms(uint i);
extern void InitSystem(void);
extern void EnableInterrupt(void);
extern void DisnableInterrupt(void);
extern void ProcTimer(void);
extern void KeyScan(void);
extern void SysSet(void);
#endif /*__SYSTEM_H_*/

