#include "System.h"

THEARTRATE 		HeartRate;
TTIMER 			Timer;
TSTEP 			Step;
TUNION08 		SysFlag;  //系统标志位

unsigned char KeyCounter;
unsigned char KeyTimer;
unsigned char KeyValue;
/****************************************************************
函数功能:延时1us
入口参数:n
****************************************************************/
void Delay1us(unsigned int n)
{
    unsigned int j;
    for(j=0;j<n;j++);
}

/*****************************************************
函数功能：延时1ms
入口参数：延时时间数
返回值：无
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
函数功能：初始化定时器1
入口参数：无
返回值：无
***************************************************/
void InitTimer0(void)
{
	ET0=1;                 //定时器T1中断允许         
	TMOD=0x01;             //TMOD=0001 0001使用定时器T1的模式1
	//定时器每隔10ms中断一次
	TH0=(65536-10000)/256;   //定时器T1的高8位赋初值
	TL0=(65536-10000)%256;  //定时器T1的高8位赋初值
	TR0=1;                 //启动定时器T1
}
/*****************************************************
函数功能：使能中断
入口参数：无
返回值：无
***************************************************/
void EnableInterrupt(void)
{
	EA = 1;                  //开总中断
	IT0 = 1;
	EX0 = 1;
	IT1 = 1;
	EX1 = 1;
}
/*****************************************************
函数功能：关闭中断
入口参数：无
返回值：无
***************************************************/
void DisnableInterrupt(void)
{
	EA = 0;                  //开总中断
}
/*****************************************************
函数功能：系统初始化
入口参数：无
返回值：无
***************************************************/
void InitSystem(void)
{
	Init1602();	  //初始化1602
	Display.DisNum = 0xFF;
	Display.DisTimer = 1;
	Step.Length = 60;
	ProcDisplay();    //显示开机欢迎界面
	
	InitTimer0();
	Init18B20();
	InitDS1302();
	EnableInterrupt();
}

/*****************************************************
函数功能：100ms任务
入口参数：无
返回值：无
***************************************************/
void Timer100msTask(void)
{
	if(Timer.T100msFlag == 0x55)
	{
		Timer.T100msFlag = 0;
		Timer.T100msCounter++;
		
		if(Timer.T100msCounter%2)
		{
			DisplayFlag = 1;    //100ms刷现一次显示
		}
		else
		{
			ReadDS1302Flag = 1;
			Read18B20Flag = 1;
		}
	}
}
/*****************************************************
函数功能：500ms任务
入口参数：无
返回值：无
***************************************************/
void Timer500msTask(void)
{
	if(Timer.T500msFlag == 0x55)
	{
		Timer.T500msFlag = 0;
		Timer.T500msCounter++;  //用于数据设置时闪烁显示
	}
}
/*****************************************************
函数功能：1秒任务
入口参数：无
返回值：无
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
函数功能：定时任务
入口参数：无
返回值：无
***************************************************/
void ProcTimer(void)
{
	Timer100msTask();
	Timer500msTask();
	TimerSecTask();
}
/*****************************************************
函数功能：按键扫描
入口参数：无
返回值：无
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
函数功能：设置步长
入口参数：键值
返回值：无
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
函数功能：系统设置
入口参数：无
返回值：无
***************************************************/
void SysSet(void)
{
	KeyScan();
	
	if(KeyValue == 0)
	{
		return;
	}
	
	if(KeyValue == 1) //显示切换键按下
	{
		if(!SysSetFlag) //非设置模式切换屏幕
		{
			Display.DisNum++;
			if(Display.DisNum > 2)
			{
				Display.DisNum = 0;
			}
		}
		else  //设置模式退出设置模式
		{
			SysSetFlag = 0;
			SetDS1302Pointer = 0;
		}
	}	
	else if((KeyValue == 2) && (Display.DisNum != 0)) //第一屏不允许设置数据
	{
		if(SysSetFlag == 0)
		{
			SysSetFlag = 1;
		}
	}
	else if(KeyValue == 5) //距离和步数清零
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
		else if(Display.DisNum == 2) //歩距设置
		{
			SetStepLength(KeyValue);
		}
	}
	KeyValue = 0;
}
/**************************************************************
函数功能:定时器T1的中断服务程序 每10ms产生一次中断
**************************************************************/
void Time0() interrupt 1 //“interrupt”声明函数为中断服务函数
{
	Timer.T10msCounter++; //655350
	
	//按键消抖
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
	
	TH0=(65536-10000)/256;   //定时器T1的高8位重新赋初值
	TL0=(65536-10000)%256;  //定时器T1的高8位重新赋初值
}
/**************************************************************
函数功能:外部中断0
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
函数功能:外部中断1 心率计算
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
		HeartRate.Num = 60*100/HeartRate.SumTimer; //单位ms 1分钟 = 60s*100
		
		HeartRate.Counter = 0;
		HeartRate.SumTimer = 0;
	}
}

