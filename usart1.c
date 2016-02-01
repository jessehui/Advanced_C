#include <cc2540.h>
void initUART()
{
	PERCFG = 0x00;//外设控制寄存器 设置USART0在位置0
	P0SEL = 0x0c;// 0000 1100;P0_2 P0_3作为串口（外设功能）
	P2DIR &= ~0xc0//0011 1111;00表示uart0优先级最高

	U0CSR |= 0x80;//1000 0000  USART模式选择为UART模式
	U0GCR |= 11;
	U0BAUD |= 216; //设置波特率为115200 数据手册有对应的表
	UTX0IF = 0;//中断标志初始置为0
	U0CSR |= 0x40;//0100 0000 接收器使能
	IEN0 |= 0x84;




}