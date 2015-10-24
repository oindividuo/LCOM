#include "kbd.h"

static int hook_id;

int kbd_subscribe_int(void ) {
	hook_id = KBD_IRQ;  //keyboard's IRQ number is 1.
	if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id) != OK){ //output the EOI command to the PIC
		printf("Irqsetpolicy failed \n");
		return -1;
	   	   }
	else if(sys_irqenable((&hook_id)) != OK){ //enables interrupts on the IRQ line associated with the hook_id
		printf("Irqenable failed \n");
		return -1;
		}
	else return KBD_IRQ;
}

int kdb_unsubscribe_int() {
	if ((sys_irqrmpolicy(&hook_id) != OK))
		return 1;
		else if (sys_irqdisable(&hook_id) != OK)
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
