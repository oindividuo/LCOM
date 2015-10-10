#include "timer.h"
#include <stdio.h>

static unsigned int time_counter = 0;
static int hook_id;

int timer_set_square(unsigned long timer, unsigned long freq)
{
	if(freq < 0)
		return 1;

	unsigned long frequency = TIMER_FREQ/freq; // Timer_Freq is the frequency of the Clock input and freq is the value loaded initially in the timer
	unsigned char out_port = ' ', timer_control = ' ';
	if(timer == 0){
		timer_control = TIMER_SEL0;
		out_port = TIMER_0;
	}
	else if(timer == 1){
		timer_control = TIMER_SEL1;
		out_port = TIMER_1;
	}
	else if(timer == 2){
		timer_control = TIMER_SEL2;
		out_port = TIMER_2;
	}

	else return 1;

	unsigned char st;
		if (timer_get_conf(timer, &st) != 0)
			return 1;

		if ((st & BIT(0)) == TIMER_BCD)
			sys_outb(TIMER_CTRL, timer_control | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BCD);

		else sys_outb(TIMER_CTRL, timer_control | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN );

		return 0;
}

int timer_subscribe_int(void ) {
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK) //output the EOI command to the PIC
			return 1;
	else if(sys_irqenable((&hook_id)) != OK) //enables interrupts on the IRQ line associated with the hook_id
		return 1;
	else return 0;
}

int timer_unsubscribe_int() {
	if ((sys_irqrmpolicy(&hook_id) == OK) && (sys_irqdisable(&hook_id) == OK))
		return 0;

	else return 1;

}

void timer_int_handler() {
   time_counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st)
{
	sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_| TIMER_RB_SEL(timer) | TIMER_RB_STATUS_) ; //Direct I/O port access
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
	 return timer_set_square(0, freq);
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
