#include "inc/gpio.h"
#include "inc/led.h"


void led_init()
{
	struct s5pv210_gpio *gpio_base;
	gpio_base = (struct s5pv210_gpio *)GPIO_BASE;
	//base->gpio_a0.con = ...

	/*
	the address of gpio has already existed. We just need a pointer to
	point to it instead of creating new space. Thus, the method below is 
	incorrect.
	struct s5pv210_gpio base; 
	base.gpio_a0.con = ...
	*/
}



