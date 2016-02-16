#ifndef __LED_H
#define __LED_H

//LED0端口定义 低电平打开 高电平关闭
#define OPEN_LED0()   GPIO_ResetBits(GPIOF,GPIO_Pin_9);
#define CLOSE_LED0()  GPIO_SetBits(GPIOF,GPIO_Pin_9);

void LED_Init(void);

#endif
