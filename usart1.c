#include <cc2540.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int uint;

#define UART0_RX 1
#define UART0_TX 2
#define SIZE 	 51

#define LED1 = P1_0
#define LED2 = P1_1
#define LED3 = P1_4
#define LED4 = P0_1

uchar RxBUF;
uchar UartState;
uchar count;
char RxData[SIZE];//存储发送字符串

void delay(int sec)
{
	uint i,j;
	for (i=0;i<sec;i++)
		for (j=0;j<1070;j++);
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


void initUART()
{
	PERCFG = 0x00;//外设控制寄存器 设置USART0在位置0
	P0SEL = 0x0c;// 0000 1100;P0_2 P0_3作为串口（外设功能）
	P2DIR &= ~0xc0//0011 1111;00表示uart0优先级最高

	U0CSR |= 0x80;//1000 0000  USART模式选择为UART模式
	U0GCR |= 11;
	U0BAUD |= 216; //设置波特率为115200 数据手册有对应的表
	UTX0IF = 0;//中断标志初始置为0 表示无中断未决 
	//UTX0IF是是IRCON2中断标志寄存器的一位 表示UART0 TX中断标志
	U0CSR |= 0x40;//0100 0000 接收器使能
	IEN0 |= 0x84;

}

void UartSendString(char *data, int len)
{
	uint i;
	for(i=0;i<len;i++)
	{
		U0DBUF = *data++;
		while(UTX0IF == 0);
		UTX0IF = 0;
	}
}

#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{
	URX0IF = 0;//USART RX中断标志，当USART0中断发生时设为1 
	//Tcon寄存器中
	RxBUF = U0DBUF;
}

void main()
{
	CLKCONCMD &= ~0x40;//0100 0000 设置系统为32MHz XOSC
	while(CLKCONSTA & 0x40);//CLKCONSTA.OSC(6 bit)从0变1表示晶振稳定
	CLKCONCMD &= ~0x47;//~0100 0111 = 1011 1000 时钟速度设置为32MHZ

	initUART();
	initLED();
	UartState = UART0_RX; //串口0默认处于接收模式
	memset(RxData, 0, SIZE);//前SIZE个字节全部置0 初始化清空操作

	while(1)
	{
		if(UartState = UART0_RX)//处于接收状态
		{
			if(RxBUF != 0)
			{
				if((RxBUF != "#") && (count < 50))
					RxData[count++] = RxBUF;
				else
				{
					if(count >= 50)//判断数据非法性
					{
						count = 0;//计数清零
						memset(RxData,0,SIZE);//清空接收缓冲区
					}
					else
						UartState = UART0_TX;//进入发送状态
				}
				RxBUF = 0; //清零 等待下一次接收
			}
		}


		if(UartState = UART0_TX)
		{
			U0CSR &= ~0x40;//1011 1111接收器禁止
			UartSendString(RxData,count);//发送已记录的字符串
			U0CSR |= 0x40;//0100 0000 恢复接收
			UartState = UART0_RX;
			count = 0;//计数清零
			memset(RxData,0,SIZE);//清空缓冲区

		}

		//串口发送数据控制LED开关
		if(strstr(RxData,"ledon") != NULL)
		{
			LEDmode(1);
		}

		if(strstr(RxData,"ledoff") != NULL)
		{
			LEDmode(0);
		}
	}

}





