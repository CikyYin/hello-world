#include "18B20.h"

TTEMP Temp;

//��ʱ1΢��
void Delay18B20(unsigned int i)
{
 	while(i--);
}

//18b20 ��ʼ��
void Init18B20(void)
{
	unsigned char x=0;
	DQ = 1;          //DQ��λ
	Delay18B20(4);  //��ʱ
	DQ = 0;          //DQ����
	Delay18B20(100); //��ȷ��ʱ����480us
	DQ = 1;          //����
	Delay18B20(40);
}

//18B20������
uchar Read18B20Data(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; //�������ź�
		dat>>=1;
		DQ = 1; //�������ź�
		if(DQ)
		{
			dat|=0x80;
		}
		Delay18B20(10);
	}
	return(dat);
}

//18B20д����
void Write18B20Data(uchar wdata)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		DQ = wdata&0x01;
		Delay18B20(10);
		DQ = 1;
		wdata>>=1;
	}
}

//��ȡ�¶�ֵ��ת��
void ReadTemp(void)
{
	uchar TmpHigh, TmpLow;  //�洢�¶ȸ�λ�͵�λ
	//uint Diff;

	if(Read18B20Flag == 0)
	{
		return;
	}
	
	DisnableInterrupt(); //���ɶ�ȡ�¶�ʱ�ᱻ�жϴ�� ���¶�ȡ�¶�ֵ��׼ �������� ��˶�ȡ�¶�ʱ�ر��ж�
	Init18B20();
	Write18B20Data(0xcc);//*���������к�*/
	Write18B20Data(0x44);//*�����¶�ת��*/
	Init18B20();
	Write18B20Data(0xcc);//*���������к�*/
	Write18B20Data(0xbe);//*��ȡ�¶�*/
	TmpLow = Read18B20Data();
	TmpHigh = Read18B20Data();
	EnableInterrupt();

	Temp.Value = TmpHigh;
	Temp.Value <<= 8;
	Temp.Value |= TmpLow;

	//�¶�Ϊ��
	if( Temp.Value < 0x0fff )
	{
		Temp.Flag = ABOVE_ZERO;
	}
	//�¶�Ϊ��
	else
	{
		Temp.Value = ~Temp.Value+1;
		Temp.Flag = BELOW_ZERO;
	}
	Temp.Value = Temp.Value*(0.625);//�¶�ֵ����10��?��ȷ��1λС��

	Read18B20Flag = 0;
}



