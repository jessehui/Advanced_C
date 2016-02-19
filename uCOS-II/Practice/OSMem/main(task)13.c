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

//定义消息队列所需要的资源
const int KeyNum = 10;
INT8U KeyData[KeyNum] = {0};
OS_EVENT *pqueue;

//定义联系人用户信息结构体
typedef struct{
	char Name[20];//姓名
	char TelNum[12];//电话号码
}USER_INFO;

//定义可用内存块
const int USER_NUM = 100;//定义联系人数量最多为100个
const int USER_ONE_SIZE = sizeof(USER_INFO);//定义每个联系人所占空间大小
USER_INFO User[USER_NUM] = {0};
OS_MEM *pTelUser;//块空间


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
	//创建消息队列
	pqueue = OSQCreate( (void **)&KeyData, KeyNum);//KeyData作为数组名本身就是地址了 对地址求地址用双指针
	
	//创建内存块供uCOS管理
	pTelUser = OSMemCreate(User,USER_NUM,USER_ONE_SIZE,&err);
	//之后就通过pTelUser来操作这个内存区域
	OSStart();
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
	INT8U *pkey;
	int i;
	static int num = 0;//静态局部变量
	USER_INFO *pTel[USER_NUM];//USER_INFO 类型的数组
	BEEP_Init();

	while(1)
	{
		//等待接收到消息
		pkey = (INT8U *)OSQPend(pqueue,0,&err);
		KeyData = *pkey;
		//接收到消息,蜂鸣器鸣响一声
		OPEN_BEEP();
		OSTimeDlyHMSM(0,0,0,50);
		CLOSE_BEEP();
		OSTimeDlyHMSM(0,0,0,50);
		
		//判断按下的按键
		if(KeyData == KEY0)//如果按下的是KEY0键
		{
			//增加一个联系人信息
			//首先申请一段内存空间 然后把联系人信息填上
			if(num < USER_NUM)
			{
				pTel(num) = (USER_INFO *)OSMemGet(pTelUser,&err);//申请块空间
				if( (err == OS_ERR_NONE) && (pTel != (void*)0) )
				{
					strcpy(pTel[num]->Name,"Zhangsan");
					strcpy(pTel[num]->TelNum,"1383838438")
				} 
				num++;

			}
		}

		else if(KeyData == KEY1)//如果按下K1
		{
			//删除一个用户联系人信息
			if(num>0)
			{
				num--;
				OSMemPut(pTelUser,pTel[num]);
			}

		}


	
	}	
}

void keyTask(void *pdata)
{
	Key_Init();
	INT8U keyvalue = 0;
	INT8U err;
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,50);
		if( (!KEY0) || (!KEY1) || (!KEY2))//按下变低电平
		{
			OSTimeDlyHMSM(0,0,0,10);
			if((!KEY0) || (!KEY1) || (!KEY2))//消抖
			{
				//发信号
				if(!KEY0)
				{
					keyvalue = 1;
				}
				else if(!KEY0)
				{
					keyvalue = 2;
				}
				else if(!KEY0)
				{
					keyvalue = 2;
				}
				//发送消息
				OSQPost(pqueue,&keyvalue)
				//等待按键抬起
				while((!KEY0) || (!KEY1) || (!KEY2))
					OSTimeDlyHMSM(0,0,0,10);
			}
		}
	}
}
