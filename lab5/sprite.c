#include <stdlib.h>
#include "sprite.h"
#include "video_gr.h"
#include "read_xpm.h"

Sprite *create_sprite(char * pic[], unsigned short xi, unsigned short yi) {
	Sprite * sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;
	sp->map = read_xpm(pic, &(sp->width), &(sp->height), get_h_res(),
			get_v_res());
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
	sp->x = xi;
	sp->y = yi;
	sp->xspeed = 0;
	sp->yspeed = 0;
	return sp;
}

int clear_sprite(Sprite * sp) {
	unsigned short xf = sp->x + sp->width;
	unsigned short yf = sp->y + sp->height;

	if (sp->x > get_h_res() || sp->y > get_v_res() || xf > get_h_res()
			|| yf > get_v_res())
		return 1;

	unsigned int i, j, x = 0;
	for (i = sp->y; i < yf; i++) {
		for (j = sp->x; j < xf; j++, x++) {
			if (vg_set_pixel(j, i, 0) != 0)
				return 1;
		}
	}
	return 0;
}

void destroy_sprite(Sprite * sp) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL;
}

int draw_sprite(Sprite * sp){
	unsigned short xf = sp->x + sp->width;
	unsigned short yf = sp->y + sp->height;

	if (sp->x > get_h_res() || sp->y > get_v_res() || xf > get_h_res() || yf > get_v_res())
		return 1;

	unsigned int i, j, x = 0;
	for (i = sp->y; i < yf; i++) {
		for (j = sp->x; j < xf; j++, x++) {
			if (vg_set_pixel(j, i, sp->map[x]) != 0)
				return 1;
		}
	}
	return 0;
}

int check_collision(Sprite * sp) {
}

int animate_sprite(Sprite *fig, float velx, float vely){
	clear_sprite(fig);
	fig->xspeed = velx;
	fig->yspeed = vely;
	fig->x += fig->xspeed;
	fig->y += fig->yspeed;
	return draw_sprite(fig);
}
