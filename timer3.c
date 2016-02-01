#include <CC2540.h>
typedef unsigned char uchar
typedef unsigned int uint

#define LED1 = P1_0

uint count;

void ledinit()
{
	P1DIR |= 0x01;//0000 0001 设置P1_0为输出
	LED1 = 0;
}

void T3init()
{
	T3CTL |= 0x08;//1111 1000
}