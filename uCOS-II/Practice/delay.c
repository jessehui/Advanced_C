
#include "includes.h"					//ucos ʹ��	  

//systick�жϷ�����,ʹ��ucosʱ�õ�
void SysTick_Handler(void)
{	
    OS_CPU_SR cpu_sr=0;
    
    OS_ENTER_CRITICAL();
	OSIntEnter();		//�����ж�
    OS_EXIT_CRITICAL();
    OSTimeTick();       //����ucos��ʱ�ӷ������               
    OSIntExit();        //���������л����ж�
}
			   
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK)
{
	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	    
	reload=SYSCLK/8;		//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;//����OS_TICKS_PER_SEC�趨���ʱ��
							//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.7989s����		   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK
}								    
			 
