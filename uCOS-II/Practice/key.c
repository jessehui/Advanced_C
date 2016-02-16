//定义按键
#include "includes.h"

void Key_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE,ENABLE);
	//使能GPIOA,GPIOE时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;//key0,1,2对应的IO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHZ;//速度为100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉 正常为高电平 按下变为低
	GPIO_Init(GPIOE,&GPIO_InitStructure);//初始化GPIOE

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//wake_up对应的IO
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化GPIOA
}
