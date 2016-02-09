#include <CC2540.h>
#define ADC_REF_125 0x00;//ADCCON3 选择参考电压为内部参考电压（1.25V）最高两位为0
#define ADC_DEC_64  0x00;//ADCCON3 选择抽取率为64 Decimation Ratio 次高两位为0
#define ADC_CHN_VDD3 0x0f;//选择VDD3通道 对应低四位全为1

typedef unsigned char uchar;
typedef unsigned int uint;

uint read_VDD_value(void)
{
	uint value;
	uchar temp_adccon3 = ADCCON3;
	ADCIF = 0;//TCON 寄存器中， 中断清零
	ADCCON3 = (ADC_REF_125 | ADC_DEC_64 | ADC_CHN_VDD3);//有1变成1 没一保持0
	while(!ADCIF);//一旦出现中断 就继续进行 否则继续等待
	value = ADCL >> 2;//低2位没用 
	value |= ( ((uint)ADCH) << 6 );//ADCH左移6位 然后与ADCL一起合成一个数
	ADCCON3 = temp_adccon3;
	return(value);

}


#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{
	URX0IF = 0;//TCON寄存器中 中断清零
	cmd = U0DBUF;
}

void uartSendString(char *data, uint length)
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

void initUart(void)
{
	CLKCONCMD &= ~0x40;
	while(CLKCONSTA & 0x40);
	CLKCONCMD &= ~0x47;
	//配置端口
	PERCFG |= 0x00;//设置uart0的io位置用备用位置1
	P0SEL |= 0x0c;//0000 1100 端口0功能选择
	//表示P0_2，P0_3用作外设功能
	P2DIR &= 0x3f; //0011 1111 usart0最高优先级

	U0CSR |= 0x80;//1000 0000选择为uart模式
	U0GCR |= 11;
	U0BAUD |= 216;//查表可知 115200波特率对应的寄存器数值
	UTX0IF = 0;//中断标志初始置为0 表示无中断未决 
	//UTX0IF是是IRCON2中断标志寄存器的一位 表示UART0 TX中断标志
	U0CSR |= 0x40;//0100 0000 接收器使能
	IEN0 |= 0x84;//1000 0100 打开usart0中断使能
}

void main(void)
{
	uint VDD_value;
	uchar buf[5] = "0.0V";
	initUart();
	while(1)
	{
		if(cmd == "start")
		{
			cmd = 0;
			VDD_value = read_VDD_value();
			VDD_value = (VDD_value * 25) >> 8;
			//基于参考电压 vdd真实值应为 读数*1.25/(2^7) => 读数*2.5/(2^8)
			VDD_value = VDD_value * 3;//vdd/3 作为一个ADC输入通道 该通道获得的值应该乘以3 来得到VDD的值
			buf[0]=VDD_value/10+'0';
    		buf[1]='.';
    		buf[2]=VDD_value%10+'0';

		}
	}
}



