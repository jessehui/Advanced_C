#include "includes.h"
//同时创建两个任务 lED和BEEP 并不能同时执行 因为一个优先级高 会一直执行下去
//****************START 任务
#define START_TASK_PRIO		10//开始任务的优先级设计为最低
//任务堆栈
#define START_STK_SIZE 		64
OS_STK START_TASK_STK[START_STK_SIZE];//OS_STK类型数组

//任务函数声明
void start_task(void *pdata);


//****************LED0任务
#define LED0_TASK_PRIO		7
//led0Task任务堆栈
#define LED0_STK_SIZE 		64
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0Task(void *pdata);

//****************Beep任务
#define BEEP_TASK_PRIO		6
#define BEEP_STK_SIZE		64
OS_STK BEEP_TASK_STK[BEEP_STK_SIZE];

const INT8U BEEP_NAME[] = "beep";//必须为全局变量

//****************KEY任务
#define KEY_TASK_PRIO		5
#define KEY_STK_SIZE		64
OS_STK KEY_TASK_STK[KEY_STK_SIZE];

void keyTask(void *pdata);

OS_EVENT *pkey;//事件类型的变量 必须为全局变量

void Delay(unsigned int t)
{
	while(t--);
}

int main(void)
{
	OSInit();
	OSTaskCreate(	start_task, 
			(void *)0, //空指针 即NULL
			(OS_STK *)START_TASK_STK[START_STK_SIZE - 1],//最后一个元素作为栈顶
			START_TASK_PRIO);//创建起始任务
	OSTimeSet(0);
	//创建信号量
	pkey = OSSemCreate(0);//参数为cnt，表示计数值
	OSStart();
}

void start_task(void *pdata)
{	
	INT8U err;//错误码
	INT8U *pname;
	INT8U len;//名字的长度
	char buffer[16];

	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;

	delay_init(168);

	OS_ENTER_CRITICLE();	//进入临界区 无法被中断打断
	OSTaskCreate(	led0Task, 
			(void *)0, 
			(OS_STK *)LED0_TASK_STK[LED0_STK_SIZE-1],
			LED0_TASK_PRIO);//创建LED任务
	OSTaskCreate(	beepTask, (void *)0, 
			(OS_STK *)BEEP_TASK_STK[BEEP_STK_SIZE-1],
			BEEP_TASK_PRIO);//创建BEEP任务
	OSTaskCreate(	keyTask, (void *)0,
			(OS_STK *)KEY_TASK_STK[KEY_STK_SIZE-1],
			KEY_TASK_PRIO);//创建keytask
	OSTaskSuspend(START_TASK_PRIO);//挂起起始任务
	
	OS_EXIT_CRITICLE();		//退出临界区 可以被中断打断

	

}

void led0Task(void *pdata)
{
	LED_Init();
	while(1)
	{
		OPEN_LED0();
		//Delay(100000);
		//OSTimeDly(500);
		OSTimeDlyHMSM(0,0,0,500);
		CLOSE_LED0();
		//Delay(100000);
		//OSTimeDly(500);
		OSTimeDlyHMSM(0,0,0,500);//好用但是效率低 其实还是调用OSTimeDly()
	}
}

void beepTask(void *pdata)
{
	//int i = 0;
	INT8U err;
	BEEP_Init();
	while(1)
	{
		//等待信号量
		OSSemPend(pkey,0,&err);// 等待时间设置为0 表示死等待 
		//蜂鸣器动作
		OPEN_BEEP();
		OSTimeDlyHMSM(0,0,0,50);
		CLOSE_BEEP();
		
	}	
}

void keyTask(void *pdata)
{
	Key_Init();
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,50);
		if(!KEY0)//按下变低电平
		{
			OSTimeDlyHMSM(0,0,0,10);
			if(!KEY0)//消抖
			{
				//发信号
				OSSemPost(pkey);
				//等待按键抬起
				while(!KEY0)
					OSTimeDlyHMSM(0,0,0,10);
			}
		}
	}
}
