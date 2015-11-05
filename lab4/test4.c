#include "test4.h"
#include "timer.h"

static char packet[3];

void print_packet(){
	printf("B1=0x%02x ", packet[0]);
	printf("B2=0x%02x ", packet[1]);
	printf("B3=0x%02x ", packet[2]);
}

int test_packet(unsigned short cnt){
	int ipc_status, r, irq_ms;
	message msg;
	irq_ms = ms_subscribe_int();
	char packet[3]; //to store the packet bytes
	int byte_counter = 0; //keep track of byte number
	int counter = 0;
	if (irq_ms == -1)
		return 1;

	irq_ms = BIT(irq_ms);
	MS_to_KBD_Commands(0xEA);
	MS_to_KBD_Commands(MS_DATA_PACKETS);

	while (counter < cnt) {  //Interrupt loop
		/* Get a request message. */
		r = driver_receive(ANY, &msg, &ipc_status);
		if (r != 0) {
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: { /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_ms) {
					ms_int_handler(&byte_counter, &packet[byte_counter]);

					if (byte_counter == 3) {
						byte_counter = 0;
						counter++;
						print_packet();
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
	MS_to_KBD_Commands(MS_DSB_STREAM_M);
	if (ms_unsubscribe_int() == -1)
		return 1;
	else
		return 0;
}

int test_async(unsigned short idle_time) {
	int ipc_status, r, irq_ms = 0, irq_timer = 0, time_counter = 0;
    bool time_flag = false;
	message msg;
	irq_ms = ms_subscribe_int();
	irq_timer = timer_subscribe_int();

	char packet[3]; //to store the packet bytes
	int byte_counter = 0; //keep track of byte number

	irq_ms = BIT(irq_ms);
	MS_to_KBD_Commands(0xEA);
	MS_to_KBD_Commands(MS_DATA_PACKETS);

	if (irq_timer == -1 || irq_ms== -1)
		return 1;

	irq_ms = BIT(irq_ms);
	irq_timer = BIT(irq_timer);
	while (!time_flag) {  //Interrupt loop
		/* Get a request message. */
		r = driver_receive(ANY, &msg, &ipc_status);
		if (r != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_ms) {
					time_counter = 0;
					ms_int_handler(&byte_counter, &packet[byte_counter]);

					if (byte_counter == 3) {
						byte_counter = 0;
						print_packet();
					}
				}
				if (msg.NOTIFY_ARG & irq_timer) {
					time_counter++;
					if (time_counter > idle_time * 60)
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
	MS_to_KBD_Commands(MS_DSB_STREAM_M);
	timer_unsubscribe_int();
	if (ms_unsubscribe_int() != 0)
		return 1;
	return 0;
}
	
int test_config(void) {
    /* To be completed ... */
}	
	
int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
