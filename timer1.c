#include <CC2540.h>
typedef unsigned char uchar
typedef unsigned int uint
#define LED1 = P1_0

void ledinit()
{
	P1DIR |= 0x01;//0000 0001 设置P1_0为输出
	LED1 = 0;

}

void T1init()
{
	T1CTL = 0x0d;//0000 1101 11表示是128分频，01表示从0开始计数
	T1STAT = 0x21;
}

void main()
{
	uchar count = 0;
	ledinit();
	T1init();
	while(1)
	{
		if(IRCON > 0)
		{
			IRCON =0;
			if(count++ >= 1)
			{
				count = 0;
				LED1 = !LED1;
			}
		}
	}
}
