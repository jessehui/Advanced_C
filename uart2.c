#include <cc2540.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int uint;


#define UART0_RX 1
#define UART0_TX 2  //定义发送接收状态
#define SIZE 	 51

#define LED1 = P1_0
#define LED2 = P1_1
#define LED3 = P1_4
#define LED4 = P0_1

#define Beep = P0_0 //定义蜂鸣器

char RxBuf;
char uartState;
uchar count;
char RxData[SIZE];
uint gManageMode;//管理LED灯模式
uint beepMode;//管理蜂鸣器模式

void delay(int sec)
{
	int i,j;
	for (i=0;i<sec;i++)
		for (j=0;j<1050;j++);
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
	P0DIR |= 0x03; //0000 0011 P0.1定义为输出  P0.0为蜂鸣器
	//1为输出 0为输入
	LEDmode(0);//先全部关闭
}


void initUart(void)
{
	//配置端口
	PERCFG |= 0x00;//设置uart0的io位置用备用位置1
	P0SEL |= 0x0c;//0000 1100 端口0功能选择
	//表示P0_2，P0_3用作外设功能
	P2DIR &= 0x3f; //0011 1111 usart0最高优先级

	//配置uart0
	U0CSR |= 0x80;//1000 0000选择为uart模式
	U0GCR |= 11;
	U0BAUD |= 216;//查表可知 115200波特率对应的寄存器数值
	UTX0IF = 0;//中断标志初始置为0 表示无中断未决 
	//UTX0IF是是IRCON2中断标志寄存器的一位 表示UART0 TX中断标志
	U0CSR |= 0x40;//0100 0000 接收器使能
	IEN0 |= 0x84;//1000 0100 打开usart0中断使能

}

void uartSendString(char *data, int length)
{
	uint i;
	for (i=0;i<length;i++)
	{
		U0DBUF = *data++;// = * (data++)
		while(UTX0IF == 0);//等待 直到出现中断未决 说明处于发送状态 
		//同时把该位清零 读下个数据
		UTX0IF = 0;
	}
}

void lightMode1()
{
	LEDmode(1);
	delay(500);
	LEDmode(0);
	delay(500);
}

void lightMode2()
{
	LEDmode(0);
	LED1 = 1;
	delay(300);
	LED1 = 0;
	delay(300);

	LED2 = 1;
	delay(300);
	LED2 = 0;
	delay(300);
	LED3 = 1;
	delay(300);
	LED3 = 0;
	delay(300);
	LED4 = 1;
	delay(300);
	LED4 = 0;
	delay(300);

	LEDmode(1);
	delay(500);
}

void lightMode3()
{
	LEDmode(0);
	LED1 = 1;
	delay(300);
	LED2 = 1;
	delay(300);
	LED3 = 1;
	delay(300);
	LED4 = 1;
	delay(300);

	LEDmode(1);
	delay(500);

}

void beepMode1()
{
	Beep = 1;
	delay(500);
	Beep = 0;
	delay(500);
}

void beepMode2()
{
	Beep = 1;
	delay(1000);
	Beep = 0;
	delay(1000);
	Beep = 1;
	delay(500);
	Beep = 0;
	delay(500);
	Beep = 1;
	delay(1000);
	Beep = 0;
	delay(1000);
	Beep = 1;
	delay(500);
	Beep = 0;
	delay(500);
}

void lightMode_sel(uchar num)
{
	switch(num)
	{
		case 1:lightMode1();break;
		case 2:lightMode2();break;
		case 3:lightMode3();break;
	}
}

void beepMode_sel(uchar num)
{
	switch(num)
	{
		case 1:beepMode1();break;
		case 2:beepMode2();break;
		case 3:Beep = 1;delay(2000);break;
	}
}





