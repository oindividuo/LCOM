#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

void *vg_init(unsigned short mode){
	struct reg86u r;
	r.u.w.ax = SET_VBE_MODE; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1 << 14 | mode; // set bit 14: linear framebuffer with mode passed as arg
	r.u.b.intno = VIDEO_BIOS;
	if (sys_int86(&r) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return ;
	}

	vbe_mode_info_t video_mode_info;
	if (vbe_get_mode_info(mode, &video_mode_info) != 0) {
		printf("\nvbe_get_mode_info error\n");
		return;
	}

	h_res = video_mode_info.XResolution;
	v_res = video_mode_info.YResolution;
	bits_per_pixel = video_mode_info.BitsPerPixel;
}

char* get_video_mem(){

	return video_mem;
}

unsigned get_h_res(){

	return h_res;
}

unsigned get_v_res(){

	return v_res;
}

int vg_set_pixel(unsigned int x, unsigned int y, char color) {
	if (x > h_res || y > v_res)
		return 1;
	*(video_mem + x + (y * h_res)) = color;
	return 0;
}

//Para test_square, é suposto interromper com ESC, mesmo que o quadrado nao esteja acabado. Esta função acaba por ser inutil por essa razao
int vg_draw_rectangle(unsigned long x, unsigned long y, unsigned long width, unsigned long height, unsigned long color)
{
	unsigned long i, j, xf = x + width, yf = y + height;
	if (x > h_res || y > v_res || xf > h_res || yf > v_res)
		return 1;
	for (i = x; i < xf; ++i)
	{
		for (j = y; j < yf; ++j)
		{
			if (vg_set_pixel(i, j, color) != 0)
				return 1;
		}
	}
	return 0;
}
