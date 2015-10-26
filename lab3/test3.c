#include "test3.h"
#include <stdio.h>
#include <stdbool.h>

int kbd_test_scan(unsigned short ass) {

	 int ipc_status, r, irq_set = 0, two_byte = false;
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
	             	            		  if (key == 0xE0)
	             	                      {
	             	                    	  two_byte = true;
	             	                      }
	             	            		  else if (two_byte == true)
	             	                      {
	             	            			  key |= 0xE0 << 8;
	             	            			  two_byte = false;
	             	            			  if ((key) & BIT(7)){
	             	            				  printf("breakcode: 0x%X \n", key);}
	             	            			  else{
	             	            				  printf("makecode:  0x%X \n", key);}

	             	            			  if (key == ESC_BREAK)
	             	            				  break_code_flag = true;
	             	                      }
	             	            		  else {
	             	            			  if ((key) & BIT(7)){
	             	            				  printf("breakcode: 0x%X \n", key);}
	             	            			  else{
	             	            				  printf("makecode:  0x%X \n", key);}
	             	            			  if (key == ESC_BREAK)
	             	            				  break_code_flag = true;
	             	            		  }}
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

	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *leds){
		int r, ipc_status, time_counter = 0;
		int l0 = 0, l1 = 0, l2 = 0; //remember previous led status
		unsigned long led_state = l0 | l1 | l2;
		message msg;
		int irq_set = 0;
		irq_set = kbd_subscribe_int();

		     if(irq_set == -1)
		    	 return 1;


		irq_set = BIT(irq_set);
		unsigned int i = 0;
		 while(time_counter < n*60) {  //Interrupt loop
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
		             	            	  	 time_counter++;
		             	            	 	 if(time_counter % 60 == 0)
		             	            	 	 {
												 switch(leds[i]){
												 case 0: {
													 if(l0 == 0) l0 = 1;
													 else l0 = 0;}
												 case 1:{
													 if(l1 = 0) l1 = 2;
													 else l1 = 0;}
												 case 2:{
													 if(l2 = 0) l2 = 4;
													 else l2 = 0;
												 }
												 led_state = l0|l1|l2;
												 if(kbd_Toogle_Leds(led_state) == -1)
													 i = 0; //in order to start the sequence
												 else i++;
												 }
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
		 return kdb_unsubscribe_int();

}

int kbd_test_timed_scan(unsigned short n){
	int ipc_status, r, irq_kbd = 0, irq_timer = 0, time_counter = 0;
		 bool break_code_flag = false, time_flag = false;
		 unsigned long key;
			 message msg;
		     irq_kbd = kbd_subscribe_int();
             irq_timer = timer_subscribe_int();

		     if (irq_timer == -1 || irq_kbd == -1)
		         return 1;

		     irq_kbd = BIT(irq_kbd);
		     irq_timer = BIT(irq_timer);
		while(!break_code_flag && !time_flag) {  //Interrupt loop
		             	     /* Get a request message. */
		             		r = driver_receive(ANY, &msg, &ipc_status);
		             	    if ( r != 0 ) {
		             	      printf("driver_receive failed with: %d", r);
		             	        continue;
		             	    }
		             	    if (is_ipc_notify(ipc_status)) { /* received notification */
		             	       switch (_ENDPOINT_P(msg.m_source)) {
		             	           case HARDWARE: /* hardware interrupt notification */
		             	              if (msg.NOTIFY_ARG & irq_kbd) {
		             	                      key = kbd_interrupt_handler_read();
                                              time_counter=0;
		             	                      if ((key) & BIT(7)){
		             	                     			printf("breakcode: 0x%X \n", key);}
		             	                     else{
		             	                     			printf("makecode: 0x%X \n", key);}

		             	                     if (key == ESC_BREAK)
		             	                    	break_code_flag = true;
		             	            	  }
		             	              if(msg.NOTIFY_ARG & irq_timer){
                                          time_counter++;
                                          if(time_counter > n*60)
                                         	time_flag = true;
		             	             }
		             	               break;
		             	         default:
		             	        break; /* no other notifications expected: do nothing */
		             	   }
		             	} else { /* received a standard message, not a notification */
		             	     /* no standard messages expected: do nothing */
		             	}
		}
        timer_unsubscribe_int();
		if(kdb_unsubscribe_int()!= 0) //in order to use Minix 3 virtual terminals
			return 1;
		return 0;
}
