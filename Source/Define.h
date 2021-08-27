#ifndef __DEFINE_H_
#define __DEFINE_H_

#define uchar unsigned char 			//unsigned 8 bit data
#define uint  unsigned int
#define ulong unsigned long

#define TRUE 1
#define FALSE 0

#define BELOW_ZERO  0x55
#define ABOVE_ZERO  0xAA

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

//按键端口定义
#define KeyPort P1

typedef union{
	unsigned char un08;
	struct{
		unsigned char 	b0:1;
		unsigned char 	b1:1;
		unsigned char 	b2:1;
		unsigned char 	b3:1;
		unsigned char 	b4:1;
		unsigned char 	b5:1;
		unsigned char 	b6:1;
		unsigned char 	b7:1;
	}b;
}TUNION08;

/*************************** 端口定义区 ********************************/
//1602显示
#define DAT P2
sbit RS = P3^5;    //寄存器选择位，将RS位定义为P2.0引脚
sbit RW = P3^6;    //读写选择位，将RW位定义为P2.1引脚
sbit E = P3^7;     //使能信号位，将E位定义为P2.2引脚
sbit BF = P2^7;    //忙碌标志位，，将BF位定义为P0.7引脚

//1302时钟芯片
sbit SCLK = P1^6;   //位定义1302芯片的接口，时钟输出端口定义在P1.0引脚
sbit DATA = P1^7;   //位定义1302芯片的接口，数据输出端定义在P1.1引脚
sbit RST = P1^5;    //位定义1302芯片的接口，复位端口定义在P1.2引脚

//18B20温度传感器
sbit DQ = P3^4;

#endif/*__DEFINE_H_*/