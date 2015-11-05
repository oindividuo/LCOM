#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include "i8042.h"

#define PAR_ERR BIT(7)
#define TO_ERR BIT(5)
#define OBF BIT(0)
#define IBF BIT(1)

int ms_subscribe_int(void );
int ms_unsubscribe_int();
int ms_read();
int ms_write(unsigned char port, unsigned char command);
int MS_to_KBD_Commands(unsigned char command);
int ms_int_handler(int *b_counter, char *pack);

#endif
