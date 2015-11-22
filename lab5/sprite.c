#include <stdlib.h>
#include "sprite.h"
#include "video_gr.h"
#include "read_xpm.h"

static int check_collision(Sprite * sp, char * base);
static int draw_sprite(Sprite * sp, char * base);

Sprite *create_sprite(char * pic[], char * bas) {
	Sprite * sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;
	sp->map = read_xpm(pic, &(sp->width), &(sp->height), get_h_res(), get_v_res());
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
return sp;
}

void destroy_sprite(Sprite * sp, char * base) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL;
}

int draw_sprite(Sprite * sp, char * base){
}

int check_collision(Sprite * sp, char * base) {
}

int animate_sprite(Sprite *fig, char *base){
}
