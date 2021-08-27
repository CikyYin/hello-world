#include "1602.h"
#include <intrins.h>  //����_nop_()���������ͷ�ļ�

TDISPLAY Display;

//��ʾ����
code uchar DisNum[] = {"0123456789."};
code uchar WelScreen[2][16] = {{"Welcome To Use"},{"Please Wait..."}};
code uchar Screen0[2][16]  =  {{"SN:      HR:   "},{"DS:    m T:    C"}};	//��һ����ʾ
code uchar Screen1[2][16]  =  {{"Date:20  -  -  "},{"Time:  :  :     "}};	//�ڶ�����ʾ
code uchar Screen2[2][16]  =  {{"Length:      cm"},{"               "}};	//��������ʾ
/*****************************************************
�������ܣ��ж�Һ��ģ���æµ״̬
��ڲ�������
����ֵ��result��result=1��æµ;result=0����æ
***************************************************/
bit BusyTest(void)
{
	bit result;
	RS=0;       //���ݹ涨��RSΪ�͵�ƽ��RWΪ�ߵ�ƽʱ�����Զ�״̬
	RW=1;
	E=1;        //E=1���������д
	_nop_();   //�ղ���
	_nop_();
	_nop_(); 
	_nop_();   //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��	
	result=BF;  //��æµ��־��ƽ����result
	E=0;
	return result;
}
/*****************************************************
�������ܣ���ģʽ����ָ�����ʾ��ַд��Һ��ģ��
��ڲ�����dictate
***************************************************/
void WriteCmd (unsigned char dictate)
{   
	while(BusyTest()==1); //���æ�͵ȴ�
	RS=0;                  //���ݹ涨��RS��R/WͬʱΪ�͵�ƽʱ������д��ָ��
	RW=0;   
	E=0;                   //E�õ͵�ƽ(дָ��ʱ��
						 // ������E��0��1���������䣬����Ӧ����"0"
	_nop_();             //�ղ��������������ڣ���Ӳ����Ӧʱ��
	DAT= dictate;            //����������P0�ڣ���д��ָ����ַ
	_nop_();               //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	E=1;                   //E�øߵ�ƽ
	_nop_();               //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	E=0;                  //��E�ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ�鿪ʼִ������
}
/*****************************************************
�������ܣ�ָ���ַ���ʾ��ʵ�ʵ�ַ
��ڲ�����x-��,y-��
����ֵ��
***************************************************/
void WriteAddress(uchar x,uchar y)
{
	if(x == 0)  
	{		
		WriteCmd(y|0x80); //��ʾλ�õ�ȷ�������涨Ϊ"80H+��ַ��x"	
	}
	else if(x == 1)
	{
		WriteCmd(y|0xC0); //��ʾλ�õ�ȷ�������涨Ϊ"80H+��ַ��x"			1100
	}
}
/*****************************************************
�������ܣ�������(�ַ��ı�׼ASCII��)д��Һ��ģ��
��ڲ�����y(Ϊ�ַ�����)
����ֵ����
***************************************************/
void WriteData(unsigned char y)
{
	while(BusyTest()==1);  
	RS=1;           //RSΪ�ߵ�ƽ��RWΪ�͵�ƽʱ������д������
	RW=0;
	E=0;            //E�õ͵�ƽ(дָ��ʱ����
				   // ������E��0��1���������䣬����Ӧ����"0"
	DAT=y;           //����������P0�ڣ���������д��Һ��ģ��
	_nop_();       //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	E=1;          //E�øߵ�ƽ
	_nop_();        //�ղ����ĸ��������ڣ���Ӳ����Ӧʱ��
	E=0;            //��E�ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ�鿪ʼִ������
}
/**************************************************************
�������ܣ���ʾ�ַ���
��ڲ������ַ����׵�ַ
����ֵ����
**************************************************************/
void Print(uchar *str)
{
	while(*str!='\0')
	{
		WriteData(*str);
		str++;
	}
}
 /*****************************************************
�������ܣ���LCD����ʾģʽ���г�ʼ������
***************************************************/
void Init1602(void)
{
	Delay1ms(15);             //��ʱ15ms���״�дָ��ʱӦ��LCDһ�νϳ��ķ�Ӧʱ��
	WriteCmd(0x38);  //��ʾģʽ���ã�16��2��ʾ��5��7����8λ���ݽӿ�
	Delay1ms(5);   //��ʱ5ms��
	WriteCmd(0x38);
	Delay1ms(5);
	WriteCmd(0x38); //3��д ����ģʽ
	Delay1ms(5);
	WriteCmd(0x0C);  //��ʾģʽ���ã���ʾ�����й�꣬�����˸
	Delay1ms(5);
	WriteCmd(0x06);  //��ʾģʽ���ã�������ƣ��ַ�����
	Delay1ms(5);											  
	WriteCmd(0x01);  //����Ļָ�����ǰ����ʾ�������
	Delay1ms(5);
}
/**************************************************************
�������ܣ���ʾ����
��ڲ�����Row:��ַ�� Column:��ַ�� Num:����ʾ������ �����ʾ99
����ֵ����
**************************************************************/
void ShowNum(uchar Row, uchar Column, uint Num, uchar Len)
{
	WriteAddress(Row, Column);
	
	switch(Len)
	{
		case 4:
			WriteData(DisNum[Num/1000]);
		case 3:
			WriteData(DisNum[Num%1000/100]);
		case 2:
			WriteData(DisNum[Num%100/10]);
		case 1:
			WriteData(DisNum[Num%10]);
			break;
		default:
			break;
	}
}
/**************************************************************
�������ܣ���ʾ��ĸ
��ڲ�����Row:��ַ�� Column:��ַ�� str:����ʾ���ַ��׵�ַ
����ֵ����
**************************************************************/
void ShowChar(uchar Row, uchar Column, uchar *str)
{
	WriteAddress(Row, Column);
	
	Print(str);
}
/**************************************************************
��������:��ʾ������ 
��ڲ���:�� 
����ֵ:��
**************************************************************/
void ProcDisplay(void)
{
	if(DisplayFlag == 0)  //û����ʾˢ��ʱ�� ����
	{
		return;
	}
	
	if(Display.DisNum == 0xFF)
	{
		if(Display.DisTimer != 0)
		{
			if(Display.LastDisNum != 0xFF)
			{
				ShowChar(0, 0, WelScreen[0]);  //����Ҫ��̬ˢ�µ���ʾ ֻ���л���ʾ��ʱ��ˢ��
				ShowChar(1, 0, WelScreen[1]);
				Display.LastDisNum = 0xFF;
			}
		}
		else
		{
			Display.DisNum = 0;   //�л�����һ��
		}
	}
	else if(Display.DisNum == 0)
	{
		if(Display.LastDisNum != 0)
		{
			ShowChar(0, 0, Screen0[0]);  //����Ҫ��̬ˢ�µ���ʾ ֻ���л���ʾ��ʱ��ˢ��
			ShowChar(1, 0, Screen0[1]);
			Display.LastDisNum = 0;
		}
		//��ʾ���� 0~9999
		ShowNum(0, 3 , Step.Num, 4);
		//��ʾ���� 0~99
		ShowNum(0, 12 , HeartRate.Num, 3);
		//��ʾ���� 0~9999
		ShowNum(1, 3 , Step.Distance/100, 4);
		//��ʾ�¶�
		if(Temp.Flag == BELOW_ZERO) //������ʾ����
		{
			ShowChar(1, 11, "-");
		}
		else
		{
			ShowChar(1, 11, " ");
		}
		ShowNum(1, 12, Temp.Value/10, 2);
	}
	else if(Display.DisNum == 1)
	{
		if(Display.LastDisNum != 1)
		{
			ShowChar(0, 0, Screen1[0]);  //����Ҫ��̬ˢ�µ���ʾ ֻ���л���ʾ��ʱ��ˢ��
			ShowChar(1, 0, Screen1[1]);
			Display.LastDisNum = 1;
		}
		//����ˢ��
		if((SysSetFlag == 0) || ((SysSetFlag == 1) && (Timer.T500msCounter%2)))
		{
			ShowNum(0, 7, DateTime.Year, 2);
			ShowNum(0, 10, DateTime.Mon, 2);
			ShowNum(0, 13, DateTime.Day, 2);
			
			//ʱ��ˢ��
			ShowNum(1, 5, DateTime.Hour, 2);
			ShowNum(1, 8, DateTime.Min, 2);
			ShowNum(1, 11, DateTime.Sec, 2);	
		}
		else
		{
			switch(SetDS1302Pointer)
			{
				case 1:
					ShowChar(0, 7, "  "); //��ʾ��
					break;
				case 2:
					ShowChar(0, 10, "  "); //��ʾ��
					break;
				case 3:
					ShowChar(0, 13, "  "); //��ʾ��
					break;
				case 4:
					ShowChar(1, 5, "  "); //��ʾ��
					break;
				case 5:
					ShowChar(1, 8, "  "); //��ʾ��
					break;
				case 6:
					ShowChar(1, 11, "  "); //��ʾ��
					break;
				default:
					break;
			}
		}
	}
	else if(Display.DisNum == 2)
	{
		if(Display.LastDisNum != 2)
		{
			ShowChar(0, 0, Screen2[0]);  //����Ҫ��̬ˢ�µ���ʾ ֻ���л���ʾ��ʱ��ˢ��
			ShowChar(1, 0, Screen2[1]);
			Display.LastDisNum = 2;
		}
		if(SysSetFlag == 0)
		{
			ShowNum(0, 8, Step.Length, 4);
		}
		else
		{
			if(Timer.T500msCounter%2)
			{
				ShowNum(0, 8, Step.Length, 4);
			}
			else
			{
				ShowChar(0, 8, "    "); //��ʾ��
			}
		}
	}
	
	DisplayFlag = 0;   //����ˢ�±�־
}

