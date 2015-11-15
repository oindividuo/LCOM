#ifndef __KBD_H
#define __KBD_H

#include "i8042.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#define KBD_IRQ		1
#define BIT(n) (0x01<<(n))
#define PAR_ERR BIT(7)
#define TO_ERR BIT(5)
#define OBF BIT(0)
#define IBF BIT(1)

int kbd_interrupt_handler_read();
int kbd_subscribe_int(void );
int kdb_unsubscribe_int();
int kbd_Toogle_Leds(unsigned long led_state);
int kbd_interrupt_handler_write(unsigned char command);

#endif /* _KBD_H */
