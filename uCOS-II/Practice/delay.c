
#include "includes.h"					//ucos 使用	  

//systick中断服务函数,使用ucos时用到
void SysTick_Handler(void)
{	
    OS_CPU_SR cpu_sr=0;
    
    OS_ENTER_CRITICAL();
	OSIntEnter();		//进入中断
    OS_EXIT_CRITICAL();
    OSTimeTick();       //调用ucos的时钟服务程序               
    OSIntExit();        //触发任务切换软中断
}
			   
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(u8 SYSCLK)
{
	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	    
	reload=SYSCLK/8;		//每秒钟的计数次数 单位为K	   
	reload*=1000000/OS_TICKS_PER_SEC;//根据OS_TICKS_PER_SEC设定溢出时间
							//reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右		   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK
}								    
			 
