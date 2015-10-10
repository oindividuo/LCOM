#include "timer.h"
#include<stdio.h>

int timer_set_square(unsigned long timer, unsigned long freq)
{

	return 1;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st)
{
	unsigned long int timer_config;
	switch(timer){
	case 0:
		sys_inb(TIMER_0, &timer_config);
		st = (char*)&timer_config;
		return 0;
	case 1:
		sys_inb(TIMER_1, &timer_config);
		st = (char*)&timer_config;
		return 0;
	case 2:
		sys_inb(TIMER_2, &timer_config);
		st = (char*)&timer_config;
		return 0;
	}
	return 1;
}

int timer_display_conf(unsigned char conf)
{
	//bit 0 - Counter in BCD or Binary
	if ((conf & BIT(0)) == TIMER_BCD)
		printf("BCD Counter \n");
	else
		printf("Binary Counter \n");

	//bits 1, 2, e 3 - Operation mode
		if(conf & ((BIT(1) | BIT(2)) | BIT(2)) == TIMER_RATE_GEN)
	printf("Mode 2: Rate generator \n");

		else if(conf & ((BIT(1) | BIT(2)) | BIT(2)) == TIMER_SQR_WAVE)
			printf("Mode 3: Square wave generator \n");

	else printf("Operation mode unnecessary for this class \n");

	//bits 4 e 5
	if (conf & (BIT(4) | BIT(5)) == TIMER_LSB_MSB)
			printf("LSB followed by MSB \n");
	else if (conf & (BIT(4) | BIT(5)) == TIMER_LSB)
		printf("LSB \n");
	else if (conf & (BIT(4) | BIT(5)) == TIMER_MSB)
		printf("MSB \n");

	return 0;
}

int timer_test_square(unsigned long freq) {

	return 1;
}

int timer_test_int(unsigned long time) {

	return 1;
}

int timer_test_config(unsigned long timer){
	unsigned char st;
	if (timer_get_conf(timer, &st) != 0)
		return 1;
	if (timer_display_conf(st) != 0)
		return 1;
	return 0;
}
