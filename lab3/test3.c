#include "test3.h"
#include <stdio.h>

static int hook_id;

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

void kbd_interrupt_handler(){ //  reads the bytes from the KBC’s OUT_BUF

}

int kbd_test_scan(unsigned short ass) {

	 int ipc_status, r, irq_set = 0;
		 message msg;
	     irq_set = timer_subscribe_int();
	     if(irq_set != 0)
	    	 return 1;
	     irq_set = BIT(irq_set);
	while(!= ESC_BREAK)) {  //Interrupt loop
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
Chat conversation end
