#include "includes.h"

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
void beepTask(void *pdata);

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
}

void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;

	delay_init(168);

	OS_ENTER_CRITICLE();	//进入临界区 无法被中断打断
	OSTaskCreate(	led0Task, 
			(void *)0, 
			(OS_STK *)LED0_TASK_STK[LED0_STK_SIZE-1],
			LED0_TASK_PRIO);//创建LED任务
	// OSTaskCreate(	beepTask, (void *)0, 
	// 		(OS_STK *)BEEP_TASK_STK[BEEP_STK_SIZE-1],
	// 		BEEP_TASK_PRIO);//创建BEEP任务
	//用OSTaskCreateExt改写
	OSTaskCreateExt(	beepTask, (void *)0,
				(OS_STK *)BEEP_TASK_STK[BEEP_STK_SIZE-1],
				BEEP_TASK_PRIO,
				BEEP_TASK_PRIO,//ID设置为跟优先级一样
				(OS_STK *)BEEP_TASK_STK[0],//栈底指针
				BEEP_STK_SIZE,//堆栈大小
				(void *) 0,//额外参数为空
				OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR//使用任务堆栈检查和清零功能

		)
	OSTaskSuspend(START_TASK_PRIO);//挂起起始任务
	OS_EXIT_CRITICLE();		//退出临界区 可以被中断打断

	

}

void led0Task(void *pdata)
{
	LED_Init();
	while(1)
	{
		OPEN_LED0();
		Delay(100000);
		CLOSE_LED0();
		Delay(100000);
	}
}

void beepTask(void *pdata)
{
	int i = 0;
	BEEP_Init();
	while(1)
	{
		OPEN_BEEP();
		Delay(100000);
		CLOSE_BEEP();
		Delay(100000);
		i++;
		if(i>=3)
		{
			//OSTaskDel(BEEP_TASK_PRIO);//i计数大于等于3 任务被删除 但同样占据ROM
			//OSTaskDel(OS_PRIO_SELF);//不需要知道优先级 删除自己即可
			//请求删除
			if( OSTaskDelReq(OS_PRIO_SELF) == OS_ERR_TASK_DEL_REQ )
			{
				OSTaskDel(OS_PRIO_SELF);//有请求 且返回值合适 再删除任务
			}
			//经测试 这种方法并不能达到目的。因为该任务一直在执行。所以
			//使用OSTaskDelReq应该在别的任务中删除本任务
		}
	}	
}
