#include "test4.h"

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
	ms_write(0x64, 0xD4);
		ms_write(0x60, 0xF4);
	//MS_to_KBD_Commands(MS_DATA_PACKETS);
	while (counter < cnt) {  //Interrupt loop
		printf("byte counter: %d \n", byte_counter);
		/* Get a request message. */
		r = driver_receive(ANY, &msg, &ipc_status);
		printf("byte counter: %d \n", byte_counter);
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
    /* To be completed ... */
}
	
int test_config(void) {
    /* To be completed ... */
}	
	
int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
