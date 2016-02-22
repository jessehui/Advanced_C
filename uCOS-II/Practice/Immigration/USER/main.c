
#include "includes.h"

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO       			7 
//���������ջ��С
#define LED0_STK_SIZE  		    		64
//�����ջ	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *pdata);

//BEEP����
//�����������ȼ�
#define BEEP_TASK_PRIO       			8 
//���������ջ��С
#define BEEP_STK_SIZE  		    		64
//�����ջ	
OS_STK BEEP_TASK_STK[BEEP_STK_SIZE];
//������
void BEEP_task(void *pdata);

void Delay(unsigned int t)
{
    while(t--);
}

int main(void)
{ 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
}

 //��ʼ����
void start_task(void *pdata)
{
    INT8U err=0;
    INT8U **pname;
    
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
    
    delay_init(168);
    
  	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
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
    
    OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
} 

//LED0����
void led0_task(void *pdata)
{	
    LED_Init();		        //��ʼ��LED�˿�  	
	while(1)
	{
		OPEN_LED0();
		Delay(10000000);
		CLOSE_LED0();
		Delay(10000000);
	};
}

//BEEP����
void BEEP_task(void *pdata)
{	
    int i=0;
    
    BEEP_Init();		        //��ʼ��LED�˿�  	
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



