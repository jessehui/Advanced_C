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
	//系统在不配置工作频率时默认为 2分频，即 32M/2=16M，所以定时
	//器每次溢出时 T=1/(16M/128)*65536≈0.5s, 所以总时间
	//Ta=T*count=0.5*1=0.5S切换 1次状态
	T3CTL |= 0x08;//0000 1000 开溢出中断
	T3IE = 1;//开启T3
	T3CTL |= 0xE0; //1110 0000 128分频 128/1600 0000 *N = 0.5s, N = 62500
	T3CTL &= ~0x03;//0000 0011 计数模式是从0x00-0xff 共255个数 625000/255 = 245
	T3CTL |= 0x10;//0001 0000 定时器开始启动
	EA = 1;//开启总中断

}

//定时器T3中断服务函数
#pragma vector = T3_VECTOR
__interrupt void T3_ISR(void)
{
	IRCON = 0x00;//先清零
	if(count++ > 245)
		{
			count = 0;
			LED1 = ~LED1;
		}
}

void main(void)
{
	ledinit();
	T3init();
	While(1);
}


