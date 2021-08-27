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

//�����˿ڶ���
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

/*************************** �˿ڶ����� ********************************/
//1602��ʾ
#define DAT P2
sbit RS = P3^5;    //�Ĵ���ѡ��λ����RSλ����ΪP2.0����
sbit RW = P3^6;    //��дѡ��λ����RWλ����ΪP2.1����
sbit E = P3^7;     //ʹ���ź�λ����Eλ����ΪP2.2����
sbit BF = P2^7;    //æµ��־λ������BFλ����ΪP0.7����

//1302ʱ��оƬ
sbit SCLK = P1^6;   //λ����1302оƬ�Ľӿڣ�ʱ������˿ڶ�����P1.0����
sbit DATA = P1^7;   //λ����1302оƬ�Ľӿڣ���������˶�����P1.1����
sbit RST = P1^5;    //λ����1302оƬ�Ľӿڣ���λ�˿ڶ�����P1.2����

//18B20�¶ȴ�����
sbit DQ = P3^4;

#endif/*__DEFINE_H_*/