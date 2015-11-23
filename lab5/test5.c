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


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
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

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
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
						if (kdb_unsubscribe_int() != 0)
							return 1;
						if (vg_exit() != 0)
							return 1;
					}
				}
				if (msg.NOTIFY_ARG & irq_timer) {
					time_counter++;
					vg_clear();
					animate_sprite(sp); // Todo completar função animate
					draw_sprite(sp);
					if (time_counter > time * 60)
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

	destroy_sprite(sp);
	timer_unsubscribe_int();
	if (kdb_unsubscribe_int() != 0)
		return 1;
	if (vg_exit() != 0)
		return 1;

	return 0;
}					

int test_controller() {
	static uint16_t *video_modes;
	vbe_info_block_t info_block;
	unsigned num_video_modes;

	if (vbe_get_info_block(&info_block, &video_modes, &num_video_modes) == 1)
		return 1;


	printf("Controller capabilities: 0x%X\n", info_block.Capabilities);

	if (info_block.Capabilities & BIT(0))
		printf("DAC width is switchable to 8 bits per primary color\n");
	else
		printf("DAC is fixed width, with 6 bits per primary color\n");

	if (info_block.Capabilities & BIT(1))
		printf("Controller is not VGA compatible\n");
	else
		printf("Controller is VGA compatible\n");
	
	if (info_block.Capabilities & BIT(2))
		printf("When programming large blocks of information to the RAMDAC, the blank bit must be used in function 0x09\n");
	else
		printf("Normal RAMDAC operation\n");

	printf("\n\nVideo modes: ");
	unsigned i = 0;
	for (i; i < num_video_modes; ++i)
	{
		printf("0x%X", video_modes[i]);
		if (i < num_video_modes)
			printf(", ");
	}
	
	printf("\n\nSize of VRAM: %d KB\n", info_block.TotalMemory * 64);
	return 0;
}					
