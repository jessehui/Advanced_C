#ifndef __LED_H
#define __LED_H

//LED端口定义
#define OPEN_LED0()     GPIO_ResetBits(GPIOF,GPIO_Pin_9)  
#define CLOSE_LED0()    GPIO_SetBits(GPIOF,GPIO_Pin_9) 

#define OPEN_BEEP()     GPIO_SetBits(GPIOF,GPIO_Pin_8)  
#define CLOSE_BEEP()    GPIO_ResetBits(GPIOF,GPIO_Pin_8) 

void LED_Init(void);//初始化	
void BEEP_Init(void);
#endif
