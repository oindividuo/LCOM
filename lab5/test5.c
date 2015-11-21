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
	vg_init(mode);
    timer_delay(delay);
	/**
	 * TODO PRINTF PHYSICAL ADDRESS
	 */
	if(vg_exit() != 0)
		return;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	vg_init(GRAPHICS_MODE);
	
	if (vg_draw_rectangle(x, y, size, color) != 0) {
		if (vg_exit() != 0)
			return 1;
		return 1;
	}

    kbd_scan(ESC_BREAK); // this function only finish when the key passed as argument is pressed

	if (vg_exit() != 0)
		return 1;

	return 0;
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	
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
	
