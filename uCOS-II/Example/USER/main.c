
#include "includes.h"

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
 			   
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO       			7 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		64
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);

//BEEP任务
//设置任务优先级
#define BEEP_TASK_PRIO       			8 
//设置任务堆栈大小
#define BEEP_STK_SIZE  		    		64
//任务堆栈	
OS_STK BEEP_TASK_STK[BEEP_STK_SIZE];
//任务函数
void BEEP_task(void *pdata);

void Delay(unsigned int t)
{
    while(t--);
}

int main(void)
{ 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	
}

 //开始任务
void start_task(void *pdata)
{
    INT8U err=0;
    INT8U **pname;
    
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
    
    delay_init(168);
    
  	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
// 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
//    OSTaskCreate(BEEP_task,(void *)0,(OS_STK*)&BEEP_TASK_STK[BEEP_STK_SIZE-1],BEEP_TASK_PRIO);
    OSTaskCreateExt(BEEP_task,(void *)0,(OS_STK*)&BEEP_TASK_STK[BEEP_STK_SIZE-1],BEEP_TASK_PRIO,BEEP_TASK_PRIO,
                    (OS_STK*)&BEEP_TASK_STK[0],BEEP_STK_SIZE,(void *)0,OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    OSTaskNameSet(BEEP_TASK_PRIO,"beep",&err);
    if(err == OS_ERR_NONE)
    {
        ;
    }
    
    OSTaskNameGet(BEEP_TASK_PRIO,pname,&err);
    
    OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
} 

//LED0任务
void led0_task(void *pdata)
{	
    LED_Init();		        //初始化LED端口  	
	while(1)
	{
		OPEN_LED0();
		Delay(10000000);
		CLOSE_LED0();
		Delay(10000000);
	};
}

//BEEP任务
void BEEP_task(void *pdata)
{	
    int i=0;
    
    BEEP_Init();		        //初始化LED端口  	
	while(1)
	{
		OPEN_BEEP();
		Delay(10000000);
		CLOSE_BEEP();
		Delay(10000000);
        i++;
        if(i==3)
        {
            OSTaskChangePrio(START_TASK_PRIO,11);
        }
	};
}



