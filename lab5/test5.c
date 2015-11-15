#include "test5.h"

void *test_init(unsigned short mode, unsigned short delay) {
	if(vg_init(mode) != 0)
		return;

	int ipc_status, r, time_counter = 0;
	message msg;
	int irq_set = 0;
	irq_set = timer_subscribe_int();
	if (irq_set == -1)
		return 1;
	irq_set = BIT(irq_set);
	
	while (time_counter < delay * 60) {  //Interrupt loop
		/* Get a request message. */
		r = driver_receive(ANY, &msg, &ipc_status);
		if (r != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set)
					time_counter++;
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	/**
	 * TODO PRINTF PHYSICAL ADDRESS
	 */

	if(timer_unsubscribe_int() == -1)
		printf("\nError while unsubscribing timer\n");
	if(vg_exit() != 0)
		return;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	int ipc_status, r, irq_set = 0;
	bool break_code_flag = false;
	unsigned long key;
	message msg;
	
	

	irq_set = kbd_subscribe_int();
	irq_set = BIT(irq_set);
	while (!break_code_flag) {  //Interrupt loop
		/* Get a request message. */
		r = driver_receive(ANY, &msg, &ipc_status);
		if (r != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					key = kbd_interrupt_handler_read();
					if (key == ESC_BREAK)
						break_code_flag = true;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	if (kdb_unsubscribe_int() != 0)
		return 1;
	if (vg_exit() != 0)
		return 1;
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	/* To be completed */
	
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	/* To be completed */
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	/* To be completed */
	
}					

int test_controller() {
	
	/* To be completed */
	
}					
	
