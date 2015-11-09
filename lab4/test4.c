#include "test4.h"
#include "timer.h"

static char packet[3]; //to store the packet bytes
int byte_counter; //keep track of byte number

int ms_int_handler() {

	long byte;
	byte = ms_read();
	if (byte == -1) {
		return -1;
	}

	packet[byte_counter] = (unsigned int) byte;
	byte_counter += 1;
	return 0;
}

void packet_order(void) {
	char packet_tmp[3];
	packet_tmp[0] = packet[0];
	packet_tmp[1] = packet[1];
	packet_tmp[2] = packet[2];
	if (((packet[0] & BIT(3)) >> 3) == 0) {
		if (((packet[1] & BIT(3)) >> 3) == 1) {
			packet[0] = packet_tmp[1];
			packet[1] = packet_tmp[2];
			byte_counter = 2;
		} else if (((packet[2] & BIT(3)) >> 3) == 1) {
			packet[0] = packet_tmp[2];
			byte_counter = 1;
		}
	}
}

void print_packet(void) {

	unsigned mb, lb, rb, xov, yov;
	short dx, dy;

	yov = (packet[0] & BIT(7)) >> 3;
	xov = (packet[0] & BIT(6)) >> 2;
	mb = (packet[0] & BIT(2)) >> 2;
	rb = (packet[0] & BIT(1)) >> 1;
	lb = (packet[0] & BIT(0));

	dy = abs(packet[2]);

	/*
	 if ((packet[0] & BIT(5)) != 0) {
	 //absolute value of a negative number in 2's complement
	 dy = ~(0xFF & packet[2]);
	 dy += 1;
	 dy = ~(0xFF & dy);
	 } else
	 dy = packet[2];
	 */

	dx = abs(packet[1]);

	/*
	 if ((packet[0] & BIT(4)) != 0) {
	 //absolute value of a negative number in 2's complement
	 dx = ~(0xFF & packet[1]);
	 dx += 1;
	 dx = ~(0xFF & dx);
	 } else
	 dx = packet[1];
	 */

	printf("B1=0x%X	B2=0x%X	B3=0x%X ", packet[0], packet[1], packet[2]);
	printf("LB=%u ", lb);
	printf("MB=%u ", mb);
	printf("RB=%u ", rb);
	printf("XOV=%u ", xov);
	printf("YOV=%u ", yov);
	printf("X=%u ", dx);
	printf("Y=%u\n", dy);
}

void print_config(int rb, int mb, int lb, int sca, int sta, int mod, int res,
		unsigned smp) {

	printf("\n\n---------------Configuration:---------------\n|\n|  ");

	if ((bool) sta)
		printf("En");
	else
		printf("Dis");
	printf("abled\n|  ");

	if ((bool) mod)
		printf("Remote");
	else
		printf("Stream");
	printf(" Mode\n|  ");

	printf("Left button:  ");
	if ((bool) lb)
		printf(" ");
	else
		printf(" not ");
	printf("pressed\n|  ");

	printf("Right button: ");
	if ((bool) rb)
		printf(" ");
	else
		printf(" not ");
	printf("pressed\n|  ");

	printf("Middle button:");
	if ((bool) mb)
		printf(" ");
	else
		printf(" not ");
	printf("pressed\n|  ");

	printf("Scaling set to ");
	if ((bool) sca)
		printf("1:1\n|  ");
	else
		printf("2:1\n|  ");

	printf("Resolution type: %u units per mm\n|  ", res);

	printf("Sample rate: %u Hz\n", smp);

	printf("|___________________________________________\n\n");
}

int test_packet(unsigned short cnt) {
	int ipc_status, r, irq_ms;
	message msg;
	irq_ms = ms_subscribe_int();
	byte_counter = 0; //keep track of byte number
	int counter = 0;
	if (irq_ms == -1)
		return 1;

	irq_ms = BIT(irq_ms);
	MS_to_KBD_Commands(MS_SET_STR_MODE);
	MS_to_KBD_Commands(MS_DATA_PACKETS);

	while (counter < cnt) {  //Interrupt loop
		/* Get a request message. */
		r = driver_receive(ANY, &msg, &ipc_status);
		if (r != 0) {
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_ms) {
					if (ms_int_handler() == -1) {
						printf("\nint_handler() erro\n");
						break;
					}

					if (byte_counter == 3) {
						packet_order();
						byte_counter = 0;
						counter++;
						print_packet();
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

	byte_counter = 0; //keep track of byte number

	MS_to_KBD_Commands(MS_SET_STR_MODE);
	MS_to_KBD_Commands(MS_DATA_PACKETS);

	if (irq_timer == -1 || irq_ms == -1)
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
				if (msg.NOTIFY_ARG & irq_timer) {
					time_counter++;
					if (time_counter > idle_time * 60)
						time_flag = true;
				}
				if (msg.NOTIFY_ARG & irq_ms) {
					time_counter = 0;
					if (ms_int_handler() == -1) {
						printf("\nMouse int handler erro\n");
						break;
					}

					if (byte_counter == 3) {
						packet_order();
						byte_counter = 0;
						print_packet();
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

	timer_unsubscribe_int();
	MS_to_KBD_Commands(MS_DSB_STREAM_M);
	if (ms_unsubscribe_int() != 0)
		return 1;
	return 0;
}

int test_config(void) {
	int irq_ms = 0;
	irq_ms = ms_subscribe_int();
	ms_int_handler();
	MS_to_KBD_Commands(0xEA);
	MS_to_KBD_Commands(MS_DATA_PACKETS);

	int error = 0;
	int rb, mb, lb, sca, sta, mod, res;
	unsigned smp;
	char c1[3], c2[3];
	c1[0] = ms_read();
	c1[1] = ms_read();
	c1[2] = ms_read();

	c2[0] = c1[0];
	c2[1] = c1[1];
	c2[2] = c1[2];
	if (((c1[0] & BIT(3)) >> 3) == 0) {
		if (((c1[1] & BIT(3)) >> 3) == 1) {
			c1[0] = c2[1];
			c1[1] = c2[2];
			c1[2] = c2[0];
		} else if (((c1[2] & BIT(3)) >> 3) == 1) {
			c1[0] = c2[2];
			c1[1] = c2[0];
			c1[2] = c2[1];
		}
	}

	if (c1[0] == -1) {
		error = 1;
		printf("Can't read from mouse (1st try)\n");
	}

	rb = BIT(0) & c1[0];
	mb = (BIT(1) & c1[0]) >> 1;
	lb = (BIT(2) & c1[0]) >> 2;
	sca = (BIT(4) & c1[0]) >> 4;
	sta = (BIT(5) & c1[0]) >> 5;
	mod = (BIT(6) & c1[0]) >> 6;

	if (c1[1] == -1) {
		error = 1;
		printf("Can't read from mouse (2nd try)\n");
	}

	if ((int) c1[1] >= 8)
		res = 8;
	else if ((int) c1[1] >= 4)
		res = 4;
	else if ((int) c1[1] >= 2)
		res = 2;
	else if ((int) c1[1] >= 1)
		res = 1;

	if (c1[2] == -1) {
		error = 1;
		printf("Can't read from mouse (3rd try)\n");
	}
	smp = c1[2];

	if (error != 0)
		return error;

	/*//set values for testing
	 rb = 1;
	 mb = 0;
	 lb = 1;
	 sca = 1;
	 sta = 1;
	 mod = 0;
	 res = 4;
	 smp = 60;
	 */
	printf("%u, %u, %u, %u, %u, %u, %u, %u\n", rb, mb, lb, sca, sta, mod, res,
			smp);
	print_config(rb, mb, lb, sca, sta, mod, res, smp);

	return 0;
}

int test_gesture(short length, unsigned short tolerance) {
	int ipc_status, r, irq_ms = 0;
	bool vertical_line_flag = false;
	message msg;
	irq_ms = ms_subscribe_int();

	int vertical_length = 0;
	int horizontal_length = 0;
	byte_counter = 0; //keep track of byte number

	MS_to_KBD_Commands(MS_SET_STR_MODE);
	MS_to_KBD_Commands(MS_DATA_PACKETS);
	irq_ms = BIT(irq_ms);
	while (!vertical_line_flag) {
		r = driver_receive(ANY, &msg, &ipc_status);

		if (r != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_ms) {
					if (ms_int_handler() == -1) {
						printf("\nMouse int handler erro\n");
						break;
					}

					if (byte_counter == 3) {
						packet_order();
						byte_counter = 0;
						print_packet();

						if (packet[0] & BIT(1)) { //Right button has been pressed
							horizontal_length += (int)packet[1];
							vertical_length += (int)packet[2];
						} else { //Right button has been released
							horizontal_length = 0;
							vertical_length = 0;
						}
						if (abs(vertical_length) >= length) { //Vertical line with desired length drawn
							vertical_line_flag = true;
							printf(
									"\nYou have done the required vertical line with the right button pressed.\n");
						}

						else if (abs(horizontal_length) >= tolerance) { //if true, the movement isn't considered vertical
							vertical_length = 0;
							horizontal_length = 0;
						}
					}
				}

				break;
			default:
				break;
			}
		} else {
		}
	}

	MS_to_KBD_Commands(MS_DSB_STREAM_M);
	if (ms_unsubscribe_int() != 0)
		return 1;
}
