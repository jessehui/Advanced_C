//*******************************************
//		USART 初始化
//*******************************************

#include "includes.h"

//加入以下代码 支持printf函数
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
}

__FILE _stdout;

//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
	x = x;
}

//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while( (USART1->SR & 0X40) == 0 );//循环发送直到发送完毕
	USART1->DR = (u8) ch;
	return ch;
}
#endif

void USART1_Init(void)
{
	//GPIO端口设置
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;

	//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//使能USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//pin9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//pin10复用为USART1

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//USART1对应pin9 和 10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHZ;//速度为50M
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化GPIOA


	//USART1初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流程控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式


	USART_Init(USART1, &USART_InitStructure);//初始化USART

	//使能串口1
	USART_Cmd(USART1, ENABLE);


}




















