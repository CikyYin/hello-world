#include "18B20.h"

TTEMP Temp;

//延时1微秒
void Delay18B20(unsigned int i)
{
 	while(i--);
}

//18b20 初始化
void Init18B20(void)
{
	unsigned char x=0;
	DQ = 1;          //DQ复位
	Delay18B20(4);  //延时
	DQ = 0;          //DQ拉低
	Delay18B20(100); //精确延时大于480us
	DQ = 1;          //拉高
	Delay18B20(40);
}

//18B20读数据
uchar Read18B20Data(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; //给脉冲信号
		dat>>=1;
		DQ = 1; //给脉冲信号
		if(DQ)
		{
			dat|=0x80;
		}
		Delay18B20(10);
	}
	return(dat);
}

//18B20写数据
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

//读取温度值并转换
void ReadTemp(void)
{
	uchar TmpHigh, TmpLow;  //存储温度高位和低位
	//uint Diff;

	if(Read18B20Flag == 0)
	{
		return;
	}
	
	DisnableInterrupt(); //怀疑读取温度时会被中断打断 导致读取温度值不准 产生跳动 因此读取温度时关闭中断
	Init18B20();
	Write18B20Data(0xcc);//*跳过读序列号*/
	Write18B20Data(0x44);//*启动温度转换*/
	Init18B20();
	Write18B20Data(0xcc);//*跳过读序列号*/
	Write18B20Data(0xbe);//*读取温度*/
	TmpLow = Read18B20Data();
	TmpHigh = Read18B20Data();
	EnableInterrupt();

	Temp.Value = TmpHigh;
	Temp.Value <<= 8;
	Temp.Value |= TmpLow;

	//温度为正
	if( Temp.Value < 0x0fff )
	{
		Temp.Flag = ABOVE_ZERO;
	}
	//温度为负
	else
	{
		Temp.Value = ~Temp.Value+1;
		Temp.Flag = BELOW_ZERO;
	}
	Temp.Value = Temp.Value*(0.625);//温度值扩大10倍?精确到1位小数

	Read18B20Flag = 0;
}



