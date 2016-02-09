#include <CC2540.h>
typedef unsigned char uchar;
typedef unsigned int  uint;

#define LED1 = P1_0
#define LED2 = P1_1
#define LED3 = P1_4
#define LED4 = P0_1

void delay(uint msec)
{
	int i,j;
	for(i=0;i<msec;i++)
		for(j=0;j<535;j++)
}

void LEDmode(uchar mode)
{
	LED1 = mode;
	LED2 = mode;
	LED3 = mode;
	LED4 = mode;
}

void initLED(void)
{
	P1DIR |= 0x13;//0001 0011 P1.0 P1.1 P1.4定义为输出
	P0DIR |= 0x02; //0000 0010 P0.1定义为输出
	LEDmode(0);//先全部关闭
}

void main(void)
{
	initLED();
	while(1)
	{
		LED1 = !LED1;
		delay(200);
		LED2 = !LED2;
		delay(200);
		LED3 = !LED3;
		delay(200);
		LED4 = !LED4;
		delay(200);

		for(i=0;i<2;i++)
			{
				LEDmode(0);
				delay(200);
				LEDmode(1);
				delay(200);
			}
		LEDmode(0);
		delay(500);
	}

}
