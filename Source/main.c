#include "System.h"

void main()
{
	InitSystem();   //ϵͳ��ʼ��
	
	while(1)
	{
		GetDateTime();
		ReadTemp();
		ProcTimer();		//ʱ���������
		ProcDisplay();      //��ʾ�������
		SysSet();
	}
}