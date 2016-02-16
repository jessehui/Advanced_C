#ifndef __BEEP_H
#define __BEEP_H

//beep端口定义 高电平工作 低电平关闭
#define OPEN_BEEP() 	GPIO_SetBits(GPIOF,GPIO_Pin_8)
#define CLOSE_BEEP()	GPIO_ResetBits(GPIOF,GPIO_Pin_8)

void BEEP_Init(void);
#endif
