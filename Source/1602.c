#include "1602.h"
#include <intrins.h>  //包含_nop_()函数定义的头文件

TDISPLAY Display;

//显示常量
code uchar DisNum[] = {"0123456789."};
code uchar WelScreen[2][16] = {{"Welcome To Use"},{"Please Wait..."}};
code uchar Screen0[2][16]  =  {{"SN:      HR:   "},{"DS:    m T:    C"}};	//第一屏显示
code uchar Screen1[2][16]  =  {{"Date:20  -  -  "},{"Time:  :  :     "}};	//第二屏显示
code uchar Screen2[2][16]  =  {{"Length:      cm"},{"               "}};	//第三屏显示
/*****************************************************
函数功能：判断液晶模块的忙碌状态
入口参数：无
返回值：result。result=1，忙碌;result=0，不忙
***************************************************/
bit BusyTest(void)
{
	bit result;
	RS=0;       //根据规定，RS为低电平，RW为高电平时，可以读状态
	RW=1;
	E=1;        //E=1，才允许读写
	_nop_();   //空操作
	_nop_();
	_nop_(); 
	_nop_();   //空操作四个机器周期，给硬件反应时间	
	result=BF;  //将忙碌标志电平赋给result
	E=0;
	return result;
}
/*****************************************************
函数功能：将模式设置指令或显示地址写入液晶模块
入口参数：dictate
***************************************************/
void WriteCmd (unsigned char dictate)
{   
	while(BusyTest()==1); //如果忙就等待
	RS=0;                  //根据规定，RS和R/W同时为低电平时，可以写入指令
	RW=0;   
	E=0;                   //E置低电平(写指令时，
						 // 就是让E从0到1发生正跳变，所以应先置"0"
	_nop_();             //空操作两个机器周期，给硬件反应时间
	DAT= dictate;            //将数据送入P0口，即写入指令或地址
	_nop_();               //空操作四个机器周期，给硬件反应时间
	E=1;                   //E置高电平
	_nop_();               //空操作四个机器周期，给硬件反应时间
	E=0;                  //当E由高电平跳变成低电平时，液晶模块开始执行命令
}
/*****************************************************
函数功能：指定字符显示的实际地址
入口参数：x-行,y-列
返回值：
***************************************************/
void WriteAddress(uchar x,uchar y)
{
	if(x == 0)  
	{		
		WriteCmd(y|0x80); //显示位置的确定方法规定为"80H+地址码x"	
	}
	else if(x == 1)
	{
		WriteCmd(y|0xC0); //显示位置的确定方法规定为"80H+地址码x"			1100
	}
}
/*****************************************************
函数功能：将数据(字符的标准ASCII码)写入液晶模块
入口参数：y(为字符常量)
返回值：无
***************************************************/
void WriteData(unsigned char y)
{
	while(BusyTest()==1);  
	RS=1;           //RS为高电平，RW为低电平时，可以写入数据
	RW=0;
	E=0;            //E置低电平(写指令时，，
				   // 就是让E从0到1发生正跳变，所以应先置"0"
	DAT=y;           //将数据送入P0口，即将数据写入液晶模块
	_nop_();       //空操作四个机器周期，给硬件反应时间
	E=1;          //E置高电平
	_nop_();        //空操作四个机器周期，给硬件反应时间
	E=0;            //当E由高电平跳变成低电平时，液晶模块开始执行命令
}
/**************************************************************
函数功能：显示字符串
入口参数：字符串首地址
返回值：无
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
函数功能：对LCD的显示模式进行初始化设置
***************************************************/
void Init1602(void)
{
	Delay1ms(15);             //延时15ms，首次写指令时应给LCD一段较长的反应时间
	WriteCmd(0x38);  //显示模式设置：16×2显示，5×7点阵，8位数据接口
	Delay1ms(5);   //延时5ms　
	WriteCmd(0x38);
	Delay1ms(5);
	WriteCmd(0x38); //3次写 设置模式
	Delay1ms(5);
	WriteCmd(0x0C);  //显示模式设置：显示开，有光标，光标闪烁
	Delay1ms(5);
	WriteCmd(0x06);  //显示模式设置：光标右移，字符不移
	Delay1ms(5);											  
	WriteCmd(0x01);  //清屏幕指令，将以前的显示内容清除
	Delay1ms(5);
}
/**************************************************************
函数功能：显示数字
入口参数：Row:地址行 Column:地址列 Num:待显示的数字 最多显示99
返回值：无
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
函数功能：显示字母
入口参数：Row:地址行 Column:地址列 str:待显示的字符首地址
返回值：无
**************************************************************/
void ShowChar(uchar Row, uchar Column, uchar *str)
{
	WriteAddress(Row, Column);
	
	Print(str);
}
/**************************************************************
函数功能:显示处理函数 
入口参数:无 
返回值:无
**************************************************************/
void ProcDisplay(void)
{
	if(DisplayFlag == 0)  //没到显示刷新时间 返回
	{
		return;
	}
	
	if(Display.DisNum == 0xFF)
	{
		if(Display.DisTimer != 0)
		{
			if(Display.LastDisNum != 0xFF)
			{
				ShowChar(0, 0, WelScreen[0]);  //不需要动态刷新的显示 只有切换显示屏时才刷新
				ShowChar(1, 0, WelScreen[1]);
				Display.LastDisNum = 0xFF;
			}
		}
		else
		{
			Display.DisNum = 0;   //切换到第一屏
		}
	}
	else if(Display.DisNum == 0)
	{
		if(Display.LastDisNum != 0)
		{
			ShowChar(0, 0, Screen0[0]);  //不需要动态刷新的显示 只有切换显示屏时才刷新
			ShowChar(1, 0, Screen0[1]);
			Display.LastDisNum = 0;
		}
		//显示步数 0~9999
		ShowNum(0, 3 , Step.Num, 4);
		//显示心率 0~99
		ShowNum(0, 12 , HeartRate.Num, 3);
		//显示距离 0~9999
		ShowNum(1, 3 , Step.Distance/100, 4);
		//显示温度
		if(Temp.Flag == BELOW_ZERO) //零下显示符号
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
			ShowChar(0, 0, Screen1[0]);  //不需要动态刷新的显示 只有切换显示屏时才刷新
			ShowChar(1, 0, Screen1[1]);
			Display.LastDisNum = 1;
		}
		//日期刷新
		if((SysSetFlag == 0) || ((SysSetFlag == 1) && (Timer.T500msCounter%2)))
		{
			ShowNum(0, 7, DateTime.Year, 2);
			ShowNum(0, 10, DateTime.Mon, 2);
			ShowNum(0, 13, DateTime.Day, 2);
			
			//时间刷新
			ShowNum(1, 5, DateTime.Hour, 2);
			ShowNum(1, 8, DateTime.Min, 2);
			ShowNum(1, 11, DateTime.Sec, 2);	
		}
		else
		{
			switch(SetDS1302Pointer)
			{
				case 1:
					ShowChar(0, 7, "  "); //显示空
					break;
				case 2:
					ShowChar(0, 10, "  "); //显示空
					break;
				case 3:
					ShowChar(0, 13, "  "); //显示空
					break;
				case 4:
					ShowChar(1, 5, "  "); //显示空
					break;
				case 5:
					ShowChar(1, 8, "  "); //显示空
					break;
				case 6:
					ShowChar(1, 11, "  "); //显示空
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
			ShowChar(0, 0, Screen2[0]);  //不需要动态刷新的显示 只有切换显示屏时才刷新
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
				ShowChar(0, 8, "    "); //显示空
			}
		}
	}
	
	DisplayFlag = 0;   //清零刷新标志
}

