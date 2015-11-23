#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	mmap_t map;
	struct reg86u reg86;

	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL)
		return 1;

	reg86.u.w.ax = VBE_MODE_INFO; /* VBE get mode info */
	/* translate the buffer linear address to a far pointer */
	reg86.u.w.es = PB2BASE(map.phys); /* set a segment base */
	reg86.u.w.di = PB2OFF(map.phys); /* set the offset accordingly */
	reg86.u.w.cx = mode;
	reg86.u.b.intno = VIDEO_BIOS;

	if (sys_int86(&reg86) != OK) {/* call BIOS */
		printf("\n vg_get_mode_info(): sys_int86() failed \n");
		lm_free(&map);
		return 1;
	}

	else {
		*vmi_p = *(vbe_mode_info_t*) map.virtual;
		lm_free(&map);
		return 0;
	}
}

int vbe_get_info_block(vbe_info_block_t *vib_p, uint16_t **video_modes, unsigned *num_video_modes) {
	mmap_t map;
	struct reg86u reg86;

	if (lm_alloc(sizeof(vbe_info_block_t), &map) == NULL)
		return 1;

	memcpy(vib_p->VbeSignature, "VBE2", 4);

	reg86.u.b.intno = 0x10;
	reg86.u.b.ah = 0x4F;
	reg86.u.b.al = 0x0;
	reg86.u.w.es = PB2BASE(map.phys);
	reg86.u.w.di = PB2OFF(map.phys);

	if (sys_int86(&reg86) != OK)
		return 1;

	if (reg86.u.w.ax != 0x4F | 0x0)
		return 1;

	*vib_p = *((vbe_info_block_t *)map.virtual);

	if (memcmp(vib_p->VbeSignature, "VESA", sizeof(vib_p->VbeSignature)) != 0)
		return 1;

	//linear to physic
	void *farptr = (void *)((vib_p->VideoModePtr & 0xffff0000) >> 12);
	farptr += PB2OFF(vib_p->VideoModePtr);
	farptr += (uint32_t)map.virtual & 0xF0000000;

	lm_free(&map);

	int16_t *modes = farptr;
	*num_video_modes = 0;
	size_t i;
	for (i = 0; *modes != -1; ++modes, *num_video_modes += 1)
	{
		//Find the total number of video modes
	}

	if ((*video_modes = malloc(*num_video_modes * sizeof(uint16_t))) == NULL)
		return 1;

	for (i = 0, modes = farptr; i < *num_video_modes; ++i, ++modes)
	{
		(*video_modes)[i] = *modes;
	}
	return 0;
}
