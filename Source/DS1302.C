#include "DS1302.h"
#include "System.h"
uchar ReadRtcFlag = 0;
TDATE DateTime;  //����ʱ������
uchar SetDS1302Pointer = 0;  //DS1302��������
code uchar GreatMon[7] = {1,3,5,7,8,10,12};  //����Ϊ31����·�
/****************************************************************
 �������ܣ���1302дһ���ֽ�����
 ��ڲ�����
****************************************************************/
void Write1302Byte(uchar dat)
{
    uchar i;
    Delay1us(2);
    for(i=0;i<8;i++)
    {
        DATA=dat&0x01;
        Delay1us(2);
        SCLK=1;
        Delay1us(2);
        SCLK=0;
        dat>>=1;
    } 
}
/****************************************************************
�������ܣ����������֣���1302дһ������
��ڲ�����cmd(commamd):���������� dat�������д������
****************************************************************/
void Write1302Date(uchar cmd,uchar dat)
{
    RST=0;		//�������ݴ���ͼ��ֻ��RST(CE)��0�䵽1��
    SCLK=0;	//������һ������SCLKΪ0���ܿ������ݴ��ͣ��൱���Ǹ���ʼ����
    RST=1;
    Delay1us(2);
    Write1302Byte(cmd);
    Write1302Byte(dat);
    SCLK=1;
    RST=0;
}
/***************************************************************
�������ܣ���1302��һ������
��ڲ�����
****************************************************************/
uchar Read1302Byte()
{
    uchar i,dat;
    Delay1us(2);
    for(i=0;i<8;i++)
    {
        dat>>=1;
        if(DATA==1)
            dat|=0x80;
        SCLK=1;
        Delay1us(2);
        SCLK=0;					
        Delay1us(2);
    }
    return dat;
}
/**************************************************************
�������ܣ����������֣���1302��ȡһ���ֽ�����
��ڲ�����
**************************************************************/
uchar Read1302Data(uchar cmd)
{
    uchar dat;
    RST=0;
    SCLK=0;
    RST=1;
    Write1302Byte(cmd);
    dat=Read1302Byte();
    SCLK=1;
    RST=0;
    return dat;
}
/*************************************************************
�������ܣ�1302��׼��ʼ�� �趨��ʼʱ��Ϊ23��59��56������Ϊ2011.12.29
*************************************************************/
void InitDS1302(void)
{
    uchar flag;
    flag=Read1302Data(0x81); //�ж�ʱ��оƬ�Ƿ�ر� 1Ϊ�ر�
    if(flag&0x80)
    {  
        Write1302Date(0x8e,0x00);                //����д״̬�Ĵ��������֣�д�벻����ָ��
        Write1302Date(0x80,((56/10)<<4|(56%10)));  //����д��Ĵ��������֣�д����ĳ�ʼֵ
        Write1302Date(0x82,((59/10)<<4|(59%10)));  //����д�ּĴ��������֣�д��ֵĳ�ʼֵ
        Write1302Date(0x84,((23/10)<<4|(23%10)));	 //����дʱ�Ĵ��������֣�д��ʱ�ĳ�ʼֵ
        Write1302Date(0x86,((29/10)<<4|(29%10)));  //����д�ռĴ��������֣�д���յĳ�ʼֵ
        Write1302Date(0x88,((12/10)<<4|(12%10)));	 //����д�¼Ĵ��������֣�д���µĳ�ʼֵ
        Write1302Date(0x8c,((11/10)<<4|(11%10)));	 //����д��Ĵ��������֣�д����ĳ�ʼֵ
        Write1302Date(0x90,0xa5);				 //�򿪳��ģʽ��ѡ��2K������ģʽ
        Write1302Date(0x8e,0x80);				 //����д״̬�Ĵ��������֣�д�뱣��ָ��
    }					
}
/***********************************************************************
�������ܣ���ȡʱ������ڲ���
************************************************************************/
void GetDateTime(void)
{
    unsigned char y;
    
    if(ReadDS1302Flag == 0)
    {
        return;
    }

    y = Read1302Data(0x81);  //����
    DateTime.Sec = ((y&0x70)>>4)*10+(y&0x0f);//����ת��
    
    y = Read1302Data(0x83); //����
    DateTime.Min = ((y&0x70)>>4)*10+(y&0x0f);
    
    y = Read1302Data(0x85);	 //��Сʱ
    DateTime.Hour = ((y&0x70)>>4)*10+(y&0x0f);
    
    y = Read1302Data(0x87);	 //����
    DateTime.Day=((y&0x70)>>4)*10+(y&0x0f);
    
    y = Read1302Data(0x89);	 //����
    DateTime.Mon=((y&0x70)>>4)*10+(y&0x0f);
    
    y = Read1302Data(0x8b);	 //����
    DateTime.Week = y&0x07;
    
    y = Read1302Data(0x8d);	  //����
    DateTime.Year = ((y&0xf0)>>4)*10+(y&0x0f);
    
    ReadDS1302Flag = 0;
}
/***********************************************************************
�������ܣ������жϺ���
************************************************************************/
unsigned char IsLeapYear(unsigned char Year)
{		
	if( Year & 0x0003 )
	{
		return FALSE;
	}

	return TRUE;
}
/***********************************************************************
�������ܣ����������¼�
************************************************************************/
void SetDateTime(uchar KeyValue)
{
    uchar i;
    uchar MaxDay;
    Write1302Date(0x8e,0x00); //�ر�д����
    
    if(KeyValue == 2)
    {
        SetDS1302Pointer++;
        if(SetDS1302Pointer > 6)
        {
            SetDS1302Pointer = 1;
        }
    }
    else if(KeyValue == 3)
    {
        switch(SetDS1302Pointer)
        {
            case 1: //������
                DateTime.Year++;
                if(DateTime.Year > 99)
                {
                    DateTime.Year = 0; 
                }
                Write1302Date(0x8c,((DateTime.Year/10)<<4|(DateTime.Year%10)));
                break;
            case 2:  //������
                DateTime.Mon++;
                if(DateTime.Mon > 12)
                {
                   DateTime.Mon = 1; 
                }
                Write1302Date(0x88,((DateTime.Mon/10)<<4|(DateTime.Mon%10)));
                break;
            case 3:  //������ 
                if(DateTime.Mon == 2)
                {
                    if(IsLeapYear(DateTime.Year) == TRUE)
                    {
                        MaxDay = 29;
                    }
                    else
                    {
                        MaxDay = 28; 
                    }
                }
                else
                {
                    for(i=0;i<7;i++)
                    {
                        if(DateTime.Mon == GreatMon[i])
                        {
                            MaxDay = 31;
                            break;
                        }
                    }
                    if(i == 7)
                    {
                       MaxDay = 30; 
                    }
                }
                DateTime.Day++;
                if(DateTime.Day > MaxDay)
                {
                    DateTime.Day = 1;
                }
                Write1302Date(0x86,((DateTime.Day/10)<<4|(DateTime.Day%10)));
                break;
            case 4:
                DateTime.Hour++; 
                if(DateTime.Hour > 24)
                {
                   DateTime.Hour = 0;
                }
                Write1302Date(0x84,((DateTime.Hour/10)<<4|(DateTime.Hour%10)));
                break;
            case 5:
                DateTime.Min++;
                if(DateTime.Min > 60)
                {
                   DateTime.Min = 0; 
                }
                Write1302Date(0x82,((DateTime.Min/10)<<4|(DateTime.Min%10)));
                break;
            case 6:
                DateTime.Sec = 0;
                Write1302Date(0x80,((DateTime.Sec/10)<<4|(DateTime.Sec%10)));  //����д��Ĵ��������֣�д����ĳ�ʼֵ
                break;           
            default:
                break;
        }
    }
    else if(KeyValue == 4)
    {
        switch(SetDS1302Pointer)
        {
            case 1: //������
                DateTime.Year--;
                if(DateTime.Year < 0)
                {
                    DateTime.Year = 99;
                }
                Write1302Date(0x8c,((DateTime.Year/10)<<4|(DateTime.Year%10)));
                break;
            case 2:  //������
                DateTime.Mon--;
                if(DateTime.Mon < 1)
                {
                    DateTime.Mon = 12;
                }
                Write1302Date(0x88,((DateTime.Mon/10)<<4|(DateTime.Mon%10)));
                break;
            case 3:  //������ 
                if(DateTime.Mon == 2)
                {
                    if(IsLeapYear(DateTime.Year) == TRUE)
                    {
                        MaxDay = 29;
                    }
                    else
                    {
                        MaxDay = 28; 
                    }
                }
                else
                {
                    for(i=0;i<7;i++)
                    {
                        if(DateTime.Mon == GreatMon[i])
                        {
                            MaxDay = 31;
                            break;
                        }
                    }
                    if(i == 7)
                    {
                       MaxDay = 30; 
                    }
                }
                DateTime.Day--;
                if(DateTime.Day < 1)
                {
                    DateTime.Day = MaxDay;
                }
                Write1302Date(0x86,((DateTime.Day/10)<<4|(DateTime.Day%10)));
                break;
            case 4:
                DateTime.Hour--;
                if(DateTime.Hour < 0)
                {
                   DateTime.Hour = 24; 
                }
                Write1302Date(0x84,((DateTime.Hour/10)<<4|(DateTime.Hour%10)));
                break;
            case 5:
                 DateTime.Min--;
                if(DateTime.Min < 0)
                {
                    DateTime.Min = 60;
                }
                Write1302Date(0x82,((DateTime.Min/10)<<4|(DateTime.Min%10)));
                break;
            default:
                break; 
        }         
    }
    Write1302Date(0x8e,0x80);				 //����д״̬�Ĵ��������֣�д�뱣��ָ��
}
