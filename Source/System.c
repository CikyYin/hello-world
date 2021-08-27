#include "System.h"

THEARTRATE 		HeartRate;
TTIMER 			Timer;
TSTEP 			Step;
TUNION08 		SysFlag;  //ϵͳ��־λ

unsigned char KeyCounter;
unsigned char KeyTimer;
unsigned char KeyValue;
/****************************************************************
��������:��ʱ1us
��ڲ���:n
****************************************************************/
void Delay1us(unsigned int n)
{
    unsigned int j;
    for(j=0;j<n;j++);
}

/*****************************************************
�������ܣ���ʱ1ms
��ڲ�������ʱʱ����
����ֵ����
***************************************************/
void Delay1ms(uint i)
{
	unsigned char j;	
	while(i--)
	{
		for(j=0;j<125;j++);		
	}		
}

/*****************************************************
�������ܣ���ʼ����ʱ��1
��ڲ�������
����ֵ����
***************************************************/
void InitTimer0(void)
{
	ET0=1;                 //��ʱ��T1�ж�����         
	TMOD=0x01;             //TMOD=0001 0001ʹ�ö�ʱ��T1��ģʽ1
	//��ʱ��ÿ��10ms�ж�һ��
	TH0=(65536-10000)/256;   //��ʱ��T1�ĸ�8λ����ֵ
	TL0=(65536-10000)%256;  //��ʱ��T1�ĸ�8λ����ֵ
	TR0=1;                 //������ʱ��T1
}
/*****************************************************
�������ܣ�ʹ���ж�
��ڲ�������
����ֵ����
***************************************************/
void EnableInterrupt(void)
{
	EA = 1;                  //�����ж�
	IT0 = 1;
	EX0 = 1;
	IT1 = 1;
	EX1 = 1;
}
/*****************************************************
�������ܣ��ر��ж�
��ڲ�������
����ֵ����
***************************************************/
void DisnableInterrupt(void)
{
	EA = 0;                  //�����ж�
}
/*****************************************************
�������ܣ�ϵͳ��ʼ��
��ڲ�������
����ֵ����
***************************************************/
void InitSystem(void)
{
	Init1602();	  //��ʼ��1602
	Display.DisNum = 0xFF;
	Display.DisTimer = 1;
	Step.Length = 60;
	ProcDisplay();    //��ʾ������ӭ����
	
	InitTimer0();
	Init18B20();
	InitDS1302();
	EnableInterrupt();
}

/*****************************************************
�������ܣ�100ms����
��ڲ�������
����ֵ����
***************************************************/
void Timer100msTask(void)
{
	if(Timer.T100msFlag == 0x55)
	{
		Timer.T100msFlag = 0;
		Timer.T100msCounter++;
		
		if(Timer.T100msCounter%2)
		{
			DisplayFlag = 1;    //100msˢ��һ����ʾ
		}
		else
		{
			ReadDS1302Flag = 1;
			Read18B20Flag = 1;
		}
	}
}
/*****************************************************
�������ܣ�500ms����
��ڲ�������
����ֵ����
***************************************************/
void Timer500msTask(void)
{
	if(Timer.T500msFlag == 0x55)
	{
		Timer.T500msFlag = 0;
		Timer.T500msCounter++;  //������������ʱ��˸��ʾ
	}
}
/*****************************************************
�������ܣ�1������
��ڲ�������
����ֵ����
***************************************************/
void TimerSecTask(void)
{
	if(Timer.TSecFlag == 0x55)
	{
		if(Display.DisTimer != 0)
		{
			Display.DisTimer--;
		}
		Timer.TSecFlag = 0;
	}
}

/*****************************************************
�������ܣ���ʱ����
��ڲ�������
����ֵ����
***************************************************/
void ProcTimer(void)
{
	Timer100msTask();
	Timer500msTask();
	TimerSecTask();
}
/*****************************************************
�������ܣ�����ɨ��
��ڲ�������
����ֵ����
***************************************************/
void KeyScan(void)
{
	uchar KeyTmp;
	
	if((KeyDownFlag == 0)&&(KeyCounter == 1))
	{
		KeyDownFlag = 1;
		KeyTmp = ~(KeyPort | 0xE0); //0001 1111 0001 1101
		switch(KeyTmp)
		{
			case BIT0:
				KeyValue = 1;
				//KeyTimer = 3;
				break;
			case BIT1:
				KeyValue = 2;
				break;
			case BIT2:
				KeyValue = 3;
				break;
			case BIT3:
				KeyValue = 4;
				break;
			case BIT4:
				KeyValue = 5;
				break;
			default:
				KeyValue = 0;
				break;
		}
	}
	else if((KeyDownFlag  == 1) && (KeyCounter == 0))
	{
		KeyDownFlag = 0;
	}
}
/*****************************************************
�������ܣ����ò���
��ڲ�������ֵ
����ֵ����
***************************************************/
void SetStepLength(uchar KeyValue)
{
	if(KeyValue == 3)
	{
		Step.Length++;
		if(Step.Length > 150)
		{
			Step.Length = 0;
		}
	}
	else if(KeyValue == 4)
	{
		Step.Length--;
		if(Step.Length == 0)
		{
			Step.Length = 150;
		}
	}
}
/*****************************************************
�������ܣ�ϵͳ����
��ڲ�������
����ֵ����
***************************************************/
void SysSet(void)
{
	KeyScan();
	
	if(KeyValue == 0)
	{
		return;
	}
	
	if(KeyValue == 1) //��ʾ�л�������
	{
		if(!SysSetFlag) //������ģʽ�л���Ļ
		{
			Display.DisNum++;
			if(Display.DisNum > 2)
			{
				Display.DisNum = 0;
			}
		}
		else  //����ģʽ�˳�����ģʽ
		{
			SysSetFlag = 0;
			SetDS1302Pointer = 0;
		}
	}	
	else if((KeyValue == 2) && (Display.DisNum != 0)) //��һ����������������
	{
		if(SysSetFlag == 0)
		{
			SysSetFlag = 1;
		}
	}
	else if(KeyValue == 5) //����Ͳ�������
	{
		Step.Distance = 0;
		Step.Num = 0;
	}
	
	if(SysSetFlag == 1)
	{
		if(Display.DisNum == 1)
		{
			SetDateTime(KeyValue);
		}
		else if(Display.DisNum == 2) //�i������
		{
			SetStepLength(KeyValue);
		}
	}
	KeyValue = 0;
}
/**************************************************************
��������:��ʱ��T1���жϷ������ ÿ10ms����һ���ж�
**************************************************************/
void Time0() interrupt 1 //��interrupt����������Ϊ�жϷ�����
{
	Timer.T10msCounter++; //655350
	
	//��������
	if( (KeyPort & 0x1F) != 0x1F )
	{
		if(KeyCounter < 1)
		{
			KeyCounter++;
		}
	}
	else
	{
		KeyCounter = 0;
	}

	if((Timer.T10msCounter%10) == 0)
	{
		Timer.T100msFlag = 0x55;
	}
	if((Timer.T10msCounter%50) == 0)
	{
		Timer.T500msFlag = 0x55;
	}
	if((Timer.T10msCounter%100) == 0)
	{
		Timer.TSecFlag = 0x55;
	}
	
	TH0=(65536-10000)/256;   //��ʱ��T1�ĸ�8λ���¸���ֵ
	TL0=(65536-10000)%256;  //��ʱ��T1�ĸ�8λ���¸���ֵ
}
/**************************************************************
��������:�ⲿ�ж�0
**************************************************************/
void Int0() interrupt 0 
{
	if(Step.Num < 9999)
	{
		Step.Num++;
	}
	if(Step.Distance < 999999)
	{
		Step.Distance = Step.Num * Step.Length;
		if( Step.Distance > 999999)
		{
			Step.Distance = 999999;
		}
	}
}
/**************************************************************
��������:�ⲿ�ж�1 ���ʼ���
**************************************************************/
void Int1() interrupt 2 
{
	HeartRate.LastTimer = HeartRate.CurrentTimer;
	HeartRate.CurrentTimer = Timer.T10msCounter;
	
	if(HeartRate.LastTimer != 0)
	{
		if(HeartRate.CurrentTimer > HeartRate.LastTimer)
		{
			HeartRate.SumTimer += (HeartRate.CurrentTimer - HeartRate.LastTimer);
		}
		else
		{
			HeartRate.SumTimer += (HeartRate.CurrentTimer+0x80000 - HeartRate.LastTimer);
		}
		HeartRate.Counter++;
	}
	else
	{
		HeartRate.Counter = 0;
	}
	
	if(HeartRate.Counter == 5)
	{
		HeartRate.SumTimer = HeartRate.SumTimer/HeartRate.Counter;
		HeartRate.Num = 60*100/HeartRate.SumTimer; //��λms 1���� = 60s*100
		
		HeartRate.Counter = 0;
		HeartRate.SumTimer = 0;
	}
}

