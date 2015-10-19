#include "test3.h"
#include <stdio.h>

static int hook_id, scanned_key;

int kbd_subscribe_int(void ) {
	hook_id = 1;  //keyboard's IRQ number is 1.
	if(sys_irqsetpolicy(, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id) != OK){ //output the EOI command to the PIC
		printf("Irqsetpolicy failed \n");
		return -1;
	   	   }
	else if(sys_irqenable((&hook_id)) != OK){ //enables interrupts on the IRQ line associated with the hook_id
		printf("Irqenable failed \n");
		return -1;
		}
	else return 0;
}

int kbd_unsubscribe_int() {
	if ((sys_irqrmpolicy(&hook_id) != OK))
		return 1;
		else if (sys_irqdisable(&hook_id) != OK)
		return 1;

	else return 0;
}

int kbd_interrupt_handler(){ //  reads the bytes from the KBC’s OUT_BUF
	unsigned long stat;

	while( 1 ) {
	sys_inb(STAT_REG, &stat); /* assuming it returns OK */
	   if( stat & OBF ) {
	     sys_inb(OUT_BUF, &scanned_key); // assuming it returns OK
	   if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
	     return scanned_key;
	    else
	     return -1;
	}
	   tickdelay(micros_to_ticks(DELAY_US);
	}
}

int kbd_test_scan(unsigned short ass) {

	 int ipc_status, r, irq_set = 0;
		 message msg;
	     irq_set = timer_subscribe_int();
	     if(irq_set != 0)
	    	 return 1;
	     irq_set = BIT(irq_set);
	     int scanned_key = 0;
	while(scanned_key != ESC_BREAK)) {  //Interrupt loop
	             	     /* Get a request message. */
	             		r = driver_receive(ANY, &msg, &ipc_status);
	             	    if ( r != 0 ) {
	             	      printf("driver_receive failed with: %d", r);
	             	        continue;
	             	    }
	             	    if (is_ipc_notify(ipc_status)) { /* received notification */
	             	       switch (_ENDPOINT_P(msg.m_source)) {
	             	           case HARDWARE: /* hardware interrupt notification */
	             	              if (msg.NOTIFY_ARG & irq_set) {
                                       kbc_interrupt_handler();
                                       if (scanned_key & BIT(7) == 0x00)
                                    	   printf("Makecode: 0x%X", scanned_key);
                                       else if (scanned_key & BIT(7) == 0x80)
                                    	   printf("Breakcode: 0x%X", scanned_key);
	             	              }
	             	               break;
	             	         default:
	             	        break; /* no other notifications expected: do nothing */
	             	   }
	             	} else { /* received a standard message, not a notification */
	             	     /* no standard messages expected: do nothing */
	             	       }
	             	}

	if(kdb_unsubscribe_int()!= 0) //in order to use Minix 3 virtual terminals
		return 1;
}

int kbd_test_leds(unsigned short n, unsigned short *leds);

int kbd_test_timed_scan(unsigned short n);
