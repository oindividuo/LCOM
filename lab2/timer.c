#include <minix/syslib.h>
#include <minix/drivers.h>

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
	switch(timer){
	case 0:
		sys_inb(TIMER_0, *st);
		return 0;
	case 1:
		sys_inb(TIMER_1, *st);
		return 0;
	case 2:
		sys_inb(TIMER_2, *st);
		return 0;
	}
	return 1;
}

int timer_display_conf(unsigned char conf)
{
	//bit 0
	if ((conf & BIT(0)) == TIMER_BCD)
		printf("O contador esta em BCD");
	else
		printf("O contador esta em binario");

	//bits 1, 2, e 3
	if ((conf & BIT(1)) == 0)
		if ((conf & BIT(2)) == 0)
			if ((conf & BIT(2)) == 0)
				printf("Modo 0");
			else
				printf("Modo 1");
		else
			if ((conf & BIT(2)) == 0)
				printf("Modo 2");
			else
				printf("Modo 3");
	else
		if ((conf & BIT(2)) == 0)
			if ((conf & BIT(2)) == 0)
				printf("Modo 4");
			else
				printf("Modo 5");
		else
			if ((conf & BIT(2)) == 0)
				printf("Modo 2");
			else
				printf("Modo 3");

	//bits 4 e 5
	if ((conf & BIT(4)) == 0 && (conf & BIT(5)) == 1)
		printf("LSB");
	else if ((conf & BIT(4)) == 1 && (conf & BIT(5)) == 0)
		printf("MSB");
	else if ((conf & BIT(4)) == 1 && (conf & BIT(5)) == 1)
		printf("LSB seguido de MSB");
	else
		return 1;

	//bits 6 e 7
	if ((conf & BIT(6)) == 0)
		if ((conf & BIT(7)) == 0)
			printf("Timer 0");
		else
			printf("Timer 1");
	else if ((conf & BIT(7)) == 0)
		printf("Timer 2");
	else
		return 1;
	return 0;
}

int timer_test_square(unsigned long freq) {
	
	return 1;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	
	return 1;
}
