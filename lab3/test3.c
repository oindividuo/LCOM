#include "test3.h"
#include <stdio.h>
#include <stdbool.h>

int kbd_test_scan(unsigned short ass) {

	 int ipc_status, r, irq_set = 0;
	 bool break_code_flag = false;
	 unsigned long key;
		 message msg;
	     irq_set = kbd_subscribe_int();
	     irq_set = BIT(irq_set);
	while(!break_code_flag) {  //Interrupt loop
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
	             	            	  if(ass == 0){ // IH written in C
	             	                      key = kbd_interrupt_handler_read();

	             	                     if ((key) & BIT(7)){
	             	                     			printf("breakcode: 0x%X \n", key);}
	             	                     else{
	             	                     			printf("makecode: 0x%X \n", key);}

	             	                     if (key == ESC_BREAK)
	             	                    	break_code_flag = true;
	             	            	  }

	             	            	  if(ass == 1){ //IH written in assembly
	             	           //completar
	             	            	  }
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

int kbd_test_leds(unsigned short n, unsigned short *leds){

}
int kbd_test_timed_scan(unsigned short n){

}
