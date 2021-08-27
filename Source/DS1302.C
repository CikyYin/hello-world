#include "DS1302.h"
#include "System.h"
uchar ReadRtcFlag = 0;
TDATE DateTime;  //日期时间数据
uchar SetDS1302Pointer = 0;  //DS1302设置索引
code uchar GreatMon[7] = {1,3,5,7,8,10,12};  //月数为31天的月份
/****************************************************************
 函数功能：向1302写一个字节数据
 入口参数：
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
函数功能：根据命令字，向1302写一个数据
入口参数：cmd(commamd):储存命令字 dat：储存待写的数据
****************************************************************/
void Write1302Date(uchar cmd,uchar dat)
{
    RST=0;		//根据数据传输图，只有RST(CE)由0变到1，
    SCLK=0;	//且在这一过程中SCLK为0才能开启数据传送，相当于是个起始命令
    RST=1;
    Delay1us(2);
    Write1302Byte(cmd);
    Write1302Byte(dat);
    SCLK=1;
    RST=0;
}
/***************************************************************
函数功能：从1302读一个数据
入口参数：
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
函数功能：根据命令字，从1302读取一个字节数据
入口参数：
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
函数功能：1302标准初始化 设定初始时间为23：59：56，日期为2011.12.29
*************************************************************/
void InitDS1302(void)
{
    uchar flag;
    flag=Read1302Data(0x81); //判断时钟芯片是否关闭 1为关闭
    if(flag&0x80)
    {  
        Write1302Date(0x8e,0x00);                //根据写状态寄存器命令字，写入不保护指令
        Write1302Date(0x80,((56/10)<<4|(56%10)));  //根据写秒寄存器命令字，写入秒的初始值
        Write1302Date(0x82,((59/10)<<4|(59%10)));  //根据写分寄存器命令字，写入分的初始值
        Write1302Date(0x84,((23/10)<<4|(23%10)));	 //根据写时寄存器命令字，写入时的初始值
        Write1302Date(0x86,((29/10)<<4|(29%10)));  //根据写日寄存器命令字，写入日的初始值
        Write1302Date(0x88,((12/10)<<4|(12%10)));	 //根据写月寄存器命令字，写入月的初始值
        Write1302Date(0x8c,((11/10)<<4|(11%10)));	 //根据写年寄存器命令字，写入年的初始值
        Write1302Date(0x90,0xa5);				 //打开充电模式，选择2K电阻充电模式
        Write1302Date(0x8e,0x80);				 //根据写状态寄存器命令字，写入保护指令
    }					
}
/***********************************************************************
函数功能：读取时间和日期参数
************************************************************************/
void GetDateTime(void)
{
    unsigned char y;
    
    if(ReadDS1302Flag == 0)
    {
        return;
    }

    y = Read1302Data(0x81);  //读秒
    DateTime.Sec = ((y&0x70)>>4)*10+(y&0x0f);//数据转换
    
    y = Read1302Data(0x83); //读分
    DateTime.Min = ((y&0x70)>>4)*10+(y&0x0f);
    
    y = Read1302Data(0x85);	 //读小时
    DateTime.Hour = ((y&0x70)>>4)*10+(y&0x0f);
    
    y = Read1302Data(0x87);	 //读天
    DateTime.Day=((y&0x70)>>4)*10+(y&0x0f);
    
    y = Read1302Data(0x89);	 //读月
    DateTime.Mon=((y&0x70)>>4)*10+(y&0x0f);
    
    y = Read1302Data(0x8b);	 //读月
    DateTime.Week = y&0x07;
    
    y = Read1302Data(0x8d);	  //读年
    DateTime.Year = ((y&0xf0)>>4)*10+(y&0x0f);
    
    ReadDS1302Flag = 0;
}
/***********************************************************************
函数功能：闰年判断函数
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
函数功能：设置日期事件
************************************************************************/
void SetDateTime(uchar KeyValue)
{
    uchar i;
    uchar MaxDay;
    Write1302Date(0x8e,0x00); //关闭写保护
    
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
            case 1: //年设置
                DateTime.Year++;
                if(DateTime.Year > 99)
                {
                    DateTime.Year = 0; 
                }
                Write1302Date(0x8c,((DateTime.Year/10)<<4|(DateTime.Year%10)));
                break;
            case 2:  //月设置
                DateTime.Mon++;
                if(DateTime.Mon > 12)
                {
                   DateTime.Mon = 1; 
                }
                Write1302Date(0x88,((DateTime.Mon/10)<<4|(DateTime.Mon%10)));
                break;
            case 3:  //日设置 
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
                Write1302Date(0x80,((DateTime.Sec/10)<<4|(DateTime.Sec%10)));  //根据写秒寄存器命令字，写入秒的初始值
                break;           
            default:
                break;
        }
    }
    else if(KeyValue == 4)
    {
        switch(SetDS1302Pointer)
        {
            case 1: //年设置
                DateTime.Year--;
                if(DateTime.Year < 0)
                {
                    DateTime.Year = 99;
                }
                Write1302Date(0x8c,((DateTime.Year/10)<<4|(DateTime.Year%10)));
                break;
            case 2:  //月设置
                DateTime.Mon--;
                if(DateTime.Mon < 1)
                {
                    DateTime.Mon = 12;
                }
                Write1302Date(0x88,((DateTime.Mon/10)<<4|(DateTime.Mon%10)));
                break;
            case 3:  //日设置 
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
    Write1302Date(0x8e,0x80);				 //根据写状态寄存器命令字，写入保护指令
}

