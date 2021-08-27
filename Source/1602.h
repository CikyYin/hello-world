#ifndef __1602_H_
#define __1602_H_

#include <reg52.h>
#include "Define.h"
#include "System.h"

typedef struct{
	unsigned char DisNum;			//��ʾ����
	unsigned char DisTimer;			//��ʾʱ��
	unsigned char LastDisNum;			//��һ����ʾ
}TDISPLAY;

/*************************** ���������� ********************************/
extern TDISPLAY Display;

/*************************** ���������� *********************************/
extern void Init1602(void);
extern void ProcDisplay(void);
extern void ShowNum(uchar Row, uchar Column, uint Num, uchar Len);

#endif/*__1602_H_*/
