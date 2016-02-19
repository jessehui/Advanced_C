#include "includes.h"
//****************START 任务
#define START_TASK_PRIO		10//开始任务的优先级设计为最低
//任务堆栈
#define START_STK_SIZE 		64
OS_STK 	START_TASK_STK[START_STK_SIZE];//OS_STK类型数组

//任务函数声明
void start_task(void *pdata);


OS_TMR  *pTmrLed;
OS_TMR 	*pTmrBeep;




void Delay(unsigned int t)
{
	while(t--);
}

int main(void)
{
	INT8U err;
	OSInit();
	OSTaskCreate(	start_task, 
			(void *)0, //空指针 即NULL
			(OS_STK *)START_TASK_STK[START_STK_SIZE - 1],//最后一个元素作为栈顶
			START_TASK_PRIO);//创建起始任务
	OSTimeSet(0);
	OSStart();
}

//led回调函数
void LedTmrCallback(OS_TMR *ptmr,  void *parg)
{
	//获取LED灯的状态
	if( GetLEDStatus() == LEDStatusOpen )
	{
		CLOSE_LED0();
	}
	else
		OPEN_LED0();
}

void BeepTmrCallback(OS_TMR *ptmr,  void *parg)
{
	//获取LED灯的状态
	if( GetBEEPStatus() == BEEPStatusOpen )
	{
		CLOSE_BEEP();
	}
	else
		OPEN_BEEP();
}


void start_task(void *pdata)
{	
	
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	u8 err;
	delay_init(168);

	USART1_Init();//初始化
	LED_Init();
	BEEP_Init();

	pringf("Hello world\n");

	OS_ENTER_CRITICLE();	//进入临界区 无法被中断打断

	//创建系统定时器
	pTmrLed = OSTmrCreate(	0,//dly 计时前的延迟
				OS_TMR_CFG_TICKS_PER_SEC/2,//周期 宏本身表示1s
				OS_TMR_OPT_PERIODIC,//周期模式
				(OS_TMR_CALLBACK)LedTmrCallback,
				NULL,
				NULL,
				&err
				);//定时器时间到了 自动调用回调函数
	pTmrBeep = OSTmrCreate(	0,
				OS_TMR_CFG_TICKS_PER_SEC/2,
				OS_TMR_OPT_PERIODIC,
				(OS_TMR_CALLBACK)BeepTmrCallback,
				NULL,
				NULL,
				&err
				);

	//启动定时器
	if( OSTmrStart(pTmrLED,&err) == OS_TRUE )
	{
		printf("LED TMR Start.\n");
	}
	else
		printf("LED TMR Fail.\n");

	if( OSTmrStart(pTmrBeep,&err) == OS_TRUE )
	{
		printf("BEEP TMR Start.\n");
	}
	else
		printf("BEEP TMR Fail.\n");
	

	OS_EXIT_CRITICLE();		//退出临界区 可以被中断打断

	while(1)//一直延时 等待程序运行
		OSTimeDlyHMSM(0,0,0,500);
	

}




