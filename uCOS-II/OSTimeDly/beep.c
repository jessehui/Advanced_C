#include "includes.h"
//初始化PF8为输出口

void BEEP_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//使能GPIO时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//BEEP对应的IO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽模式 push & pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHZ;//速度为100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);//初始化GPIO
}