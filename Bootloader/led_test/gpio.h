#ifndef ASM_ARCH_GPIO_H__
#define ASM_ARCH_GPIO_H__

struct s5pc1xx_gpio_bank{
	unsigned int	con;
	unsigned int 	dat;
	unsigned int 	pull;	//pull up/down register
	unsigned int 	drv;
	unsigned int 	pdn_con;//power down configuration register
	unsigned int 	pdn_pull;//power down Mode Pull up/down register
	unsigned char 	resl[8];
};

struct s5pv210_gpio 
{
	struct s5pc1xx_gpio_bank gpio_a0;
	struct s5pc1xx_gpio_bank gpio_a1;
	struct s5pc1xx_gpio_bank gpio_b;
	struct s5pc1xx_gpio_bank gpio_c0;
	struct s5pc1xx_gpio_bank gpio_c1;
	//....
	
};

#endif