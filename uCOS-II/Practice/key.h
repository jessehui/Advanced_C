#ifndef _key_h
#define _key_h

#define KEY0		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//E4
#define KEY1		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//E3
#define KEY2		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//E2
#define WK_UP		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//A0


void Key_Init(void);
#endif
