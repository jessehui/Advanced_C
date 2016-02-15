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
	OSTaskNameSet(BEEP_TASK_PRIO,(INT8U *)BEEP_NAME,&err);//BEEP_NAME必须为全局变量
	len = OSTaskNameGet(BEEP_TASK_PRIO,&pname,&err)//pname本身为一个指针变量，
	//对pname取地址变成二维指针 
	memcopy(buffer,pname,len);//把pname的内容拷贝到buffer中


	OSTaskSuspend(START_TASK_PRIO);//挂起起始任务
	OS_EXIT_CRITICLE();		//退出临界区 可以被中断打断

	OS_

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
	int i = 0;
	BEEP_Init();
	while(1)
	{
		OPEN_BEEP();
		//Delay(100000);//实际上是空语句 可以把这个时间让给优先级低的执行
		OSTimeDly(500);
		CLOSE_BEEP();
		//Delay(100000);
		OSTimeDly(500);//这个时候CPU可以去执行别的事情 可以执行多任务
		i++;
		if(i>=3)
		{
			OSTaskChangePrio(BEEP_TASK_PRIO,12);
			OSTaskDel(12);
			//先把蜂鸣器任务优先级改为12 然后删除第12优先级的任务 	
		}
	}	
}
