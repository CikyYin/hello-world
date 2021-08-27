#ifndef __18B20_H_
#define __18B20_H_

#include <reg52.h>
#include "Define.h"
#include "System.h"

//�¶����ݽṹ��
typedef struct{
    unsigned int Value; //�¶�ֵ
   // unsigned int LastValue;
    unsigned char Flag; //������־
}TTEMP;

/*************************** ���������� ********************************/
extern TTEMP Temp;
/*************************** ���������� ********************************/
extern void Init18B20(void);
extern void ReadTemp(void);

#endif
