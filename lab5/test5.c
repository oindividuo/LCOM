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

	int width, height;
	char * map;
	map = read_xpm(xpm, &width, &height, get_h_res(), get_v_res());
	if (1) { //TODO Chamar a funçao (e faze-la) vg_move_xpm
		if (vg_exit() != 0)
			return 1;
		return 1;
	}


	kbd_scan(ESC_BREAK);

	if (vg_exit() != 0)
		return 1;

	return 0;
}					

int test_controller() {
	
	/* To be completed */
	
}					
	
