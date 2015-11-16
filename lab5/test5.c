#include "test5.h"

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
    timer_delay(delay);
	/**
	 * TODO PRINTF PHYSICAL ADDRESS
	 */
	if(vg_exit() != 0)
		return;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	int ipc_status, r, irq_set = 0;
	bool break_code_flag = false;
	unsigned long key;
	message msg;
	
	unsigned short i = x, j = y, xf = x + size, yf = y + size;
	unsigned h_res = get_h_res(), v_res = get_v_res();
	if (x > h_res || y > v_res || xf > h_res || yf > v_res)
		return 1;

	irq_set = kbd_subscribe_int();
	irq_set = BIT(irq_set);
	while (!break_code_flag) {  //Interrupt loop
		/* Get a request message. */
		if (i <= xf)
		{
			if (j <= yf)
			{
				if (vg_set_pixel(i, j, color) != 0)
					return 1;
				if (i == xf)
					j++;
			}
			if (i == xf)
				i = 0;
			else
				i++;
		}
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
	
