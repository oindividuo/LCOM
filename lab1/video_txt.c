#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {
	int i;
	char *vptr;
				vptr = video_mem;
	for(i = 0; i< scr_width*scr_lines; i++, vptr++) {
		*vptr = ch;
		  vptr++;
		  *vptr = attr;
}
}


void vt_blank() {
	int i;
	char *vptr;
				vptr = video_mem;
		for(i = 0; i< scr_width*scr_lines; i++, vptr++) {
			*vptr = 0x00;
			  vptr++;
			  *vptr = 0x00;
	}
}

int vt_print_char(char ch, char attr, int r, int c) {
	char *vptr;
				vptr = video_mem;

	vptr = vptr + 2 * r * scr_width + 2 * c;

	if(r >= scr_lines || c >= scr_width || r < 0 || c < 0)
		return 1;

	*vptr = ch;
	vptr++;
	*vptr = attr;

    return 0;
}

int vt_print_string(char *str, char attr, int r, int c) {

	char *str_ptr = str;
		char *vptr;
			vptr = video_mem;
			vptr = vptr + 2 * r * scr_width + 2 * c;

			if(r >= scr_lines || c >= scr_width || r < 0 || c < 0 )
				return 1;


		for (; *str_ptr !=0; ++str_ptr, ++vptr)
		{
			*vptr = *str_ptr;
			++vptr;
			*vptr = attr;
		}

		return 0;
}

int vt_print_int(int num, char attr, int r, int c) {

	if(r >= scr_lines || c >= scr_width || r < 0 || c < 0 )
					return 1;
    int aux = num, count = 0;
		char *vptr;
			vptr = video_mem;
			vptr = vptr + 2 * r * scr_width + 2 * c;

	while(aux % 10 != 0){
		count++;
		aux = aux /10;
	}

	if(num < 0)
	count = count+1;

    char int_str[count];

	sprintf(int_str, "%d", num);
	vt_print_string(int_str, attr, r, c);
	return 0;
}


int vt_draw_frame(int width, int height, char attr, int r, int c) {

	if(r+height >= scr_lines || c+width >= scr_width || r < 0 || c < 0 )
						return 1;

	else{
	 int aux_width = width, aux_height = height;
     vt_print_char(UL_CORNER, attr, r, c);
     vt_print_char(LL_CORNER, attr, r+height-1, c);
     vt_print_char(UR_CORNER, attr, r, c+width-1);
     vt_print_char(LR_CORNER, attr, r+height-1, c+width-1);

     int i = aux_width - 2;
     for(i; i!=0; i--){
    	 vt_print_char(HOR_BAR, attr, r, c+i);
    	 vt_print_char(HOR_BAR, attr, r+height-1, c+i);
    	 }

     i = aux_height - 2;
     for(i; i!=0; i--){
         	 vt_print_char(VERT_BAR, attr, r+i, c);
         	 vt_print_char(VERT_BAR, attr, r+i, c + width-1);
         	 }


     return 0;
	}
}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
