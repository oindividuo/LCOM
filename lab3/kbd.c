#include "kbd.h"

static int hooke_id;

int kbd_subscribe_int(void ) {
	hooke_id = KBD_IRQ;  //keyboard's IRQ number is 1.
	if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hooke_id) != OK){ //output the EOI command to the PIC
		printf("Irqsetpolicy failed \n");
		return -1;
	   	   }
	else if(sys_irqenable((&hooke_id)) != OK){ //enables interrupts on the IRQ line associated with the hook_id
		printf("Irqenable failed \n");
		return -1;
		}
	else return KBD_IRQ;
}

int kdb_unsubscribe_int() {
	if ((sys_irqrmpolicy(&hooke_id) != OK))
		return 1;
		else if (sys_irqdisable(&hooke_id) != OK)
		return 1;

	else return 0;
}

int kbd_interrupt_handler_read(){ //  reads the bytes from the KBC’s OUT_BUF
	unsigned long stat, key;

	while( 1 ) {
	if(sys_inb(STAT_REG, &stat) != OK)
		return -1;

	   if( stat & OBF ) {
	     sys_inb(OUT_BUF, &key); // assuming it returns OK
	   if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
	     return key;
	    else
	     return -1;
	}
	   tickdelay(micros_to_ticks(DELAY_US));
	}
}

int kbd_interrupt_handler_write(unsigned char command){
	unsigned long stat;
	while( 1 ) {
		/* loop while 8042 input buffer is not empty */
		if(sys_inb(STAT_REG, &stat) != OK)
			return -1;

			if( (stat & IBF) == 0 ) {
				sys_outb(OUT_BUF, command); /* no args command */
					return 0;
			}
			tickdelay(micros_to_ticks(DELAY_US));
	}
}

int kbd_Toogle_Leds(unsigned long led_state){
	unsigned long byte;

	kbd_interrupt_handler_write(KBD_LED);
	byte = kbd_interrupt_handler_read();
	kbd_interrupt_handler_write(led_state);
	byte = kbd_interrupt_handler_read();
}

