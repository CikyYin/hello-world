#ifndef __DS1302_H_
#define __DS1302_H_

#include <reg52.h>
#include "Define.h"
#include "System.h"

//����ʱ�����ݽṹ��
typedef struct{
	char Sec;
	char Min;
	char Hour;
	char Day;
	char Week;
	char Mon;
	char Year;
}TDATE;

/*************************** ���������� ********************************/
extern TDATE DateTime;  //����ʱ������
extern uchar SetDS1302Pointer;  //DS1302��������
/*************************** ���������� ********************************/
extern void InitDS1302(void);						//1302��׼��ʼ�� �趨��ʼʱ��Ϊ23��59��56������Ϊ2011.12.29
extern void GetDateTime(void);
extern void SetDateTime(uchar KeyValue);
#endif /*__DS1302_H_*/