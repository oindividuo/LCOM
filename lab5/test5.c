#include "test5.h"

/*
 unsigned short hex_to_dec(unsigned short n) {
 unsigned short sum = 0;
 int i;
 for (i = 1; i < 8; i++)
 {
 sum += (n & BIT(i)) * 16;
 }
 return sum;
 }
 */

void *test_init(unsigned short mode, unsigned short delay) {
	char *video_mem;
	video_mem = vg_init(mode);
	timer_delay(delay);
	if (vg_exit() != 0)
		return;
	printf("\nPhysical VRAM adress: 0x%02x\n", video_mem);
}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	vg_init(GRAPHICS_MODE);

	if (vg_draw_rectangle(x, y, size, color) != 0) {
		if (vg_exit() != 0)
			return 1;
		return 1;
	}

	kbd_scan(ESC_BREAK); // this function only ends when the key passed as argument is pressed

	if (vg_exit() != 0)
		return 1;

	return 0;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {
	vg_init(GRAPHICS_MODE);

	if (vg_draw_line(xi, yi, xf, yf, color) != 0) {
		if (vg_exit() != 0)
			return 1;
		return 1;
	}

	kbd_scan(ESC_BREAK);

	if (vg_exit() != 0)
		return 1;

	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	char *video_mem;
	video_mem = vg_init(GRAPHICS_MODE);

	int width, height;
	char * map;
	map = read_xpm(xpm, &width, &height, get_h_res(), get_v_res());
	if (vg_draw_xpm(xi, yi, width, height, map) != 0) {
		if (vg_exit() != 0)
			return 1;
		return 1;
	}

	kbd_scan(ESC_BREAK);

	if (vg_exit() != 0)
		return 1;

	return 0;
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {
	char *video_mem;
	video_mem = vg_init(GRAPHICS_MODE);

	Sprite *sp = create_sprite(xpm, xi, yi);
	float velx = 0, vely = 0;
	if (hor) {
		if (delta + xi > H_RES || delta + xi < 0 || xi > H_RES) {
			if (vg_exit() != 0)
				return 1;
			return 1;
		}
		velx = delta;
		velx /= (time * 60);
	} else {
		if (delta + yi > V_RES || delta + xi < 0 || yi > V_RES) {
			if (vg_exit() != 0)
				return 1;
			return 1;
		}
		vely = delta;
		vely /= (time * 60);
	}
	int ipc_status, r, irq_kbd = 0, irq_timer = 0, time_counter = 0;
	bool time_flag = false;
	unsigned long key;
	message msg;
	irq_kbd = kbd_subscribe_int();
	irq_timer = timer_subscribe_int();

	if (irq_timer == -1 || irq_kbd == -1)
		return 1;

	irq_kbd = BIT(irq_kbd);
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
				if (msg.NOTIFY_ARG & irq_kbd) {
					key = kbd_interrupt_handler_read();
					if (key == ESC_BREAK) {
						destroy_sprite(sp);
						timer_unsubscribe_int();
						if (vg_exit() != 0)
							return 1;
						if (kdb_unsubscribe_int() != 0)
							return 1;

						return 0;
					}
				}
				if (msg.NOTIFY_ARG & irq_timer) {
					time_counter++;
					animate_sprite(sp, velx, vely);
					if (time_counter > time * 60) {
						time_flag = true;
						destroy_sprite(sp);
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
	if (vg_exit() != 0)
		return 1;
	if (kdb_unsubscribe_int() != 0)
		return 1;

	return 0;
}

int test_controller() {
}
