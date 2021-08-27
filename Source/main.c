#include "System.h"

void main()
{
	InitSystem();   //系统初始化
	
	while(1)
	{
		GetDateTime();
		ReadTemp();
		ProcTimer();		//时间任务进程
		ProcDisplay();      //显示任务进程
		SysSet();
	}
}