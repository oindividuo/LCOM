#include "mouse.h"
static int hooke_id;

int ms_subscribe_int(void ) {
	int hooke_id = MS_IRQ;  //Mouse IRQ number is 12.
	if(sys_irqsetpolicy(MS_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hooke_id) != OK){ //output the EOI command to the PIC
		printf("Irqsetpolicy failed \n");
		return -1;
	   	   }
	else if(sys_irqenable((&hooke_id)) != OK){ //enables interrupts on the IRQ line associated with the hook_id
		printf("Irqenable failed \n");
		return -1;
		}
	else return MS_IRQ;
}

int ms_unsubscribe_int() {
	if ((sys_irqrmpolicy(&hooke_id) != OK))
		return 1;
		else if (sys_irqdisable(&hooke_id) != OK)
		return 1;

	else return 0;
}

int ms_read() {
	unsigned long stat, key;
	unsigned int i = 0;
		while(i < 10) {

			if (sys_inb(STAT_REG, &stat) != OK )
				return 1;
			if( stat & OBF ) {
				sys_inb(OUT_BUF, &key);
				if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
				return key;

				else
				return -1;
			}
			tickdelay(micros_to_ticks(DELAY_US));
			i++;
		}
		return 1;
}

int ms_write(unsigned char port, unsigned char command){
	unsigned long stat;
	while( 1 ) {
		/* loop while 8042 input buffer is not empty */
		if(sys_inb(STAT_REG, &stat) != OK)
			return -1;

			if( (stat & IBF) == 0 ) {
				sys_outb(port, command); /* no args command */
					return 0;
			}
			tickdelay(micros_to_ticks(DELAY_US));
	}
}

int MS_to_KBD_Commands(unsigned char command){
	unsigned long byte;

	do {
		ms_write(STAT_REG, MS_WRITE_BYTE); //first write command 0xD4 to the KBC, i.e. using port 0x64

		do{
			ms_write(OUT_BUF, command);// mouse command to port 0x60
			byte = ms_read();//acknowledgment (message) in the output buffer
		}while(byte == MS_NACK);//the latest byte should be written again

	}while (byte == MS_ERROR);
}

int ms_int_handler(int *b_counter, char *pack) {

	unsigned long byte, aux;
	byte = ms_read();
	if (byte == -1) {
		return 1;
	} else {
		do{
			aux = ms_read();
		}while(aux != 1);
		printf("532 ");
		*pack = (unsigned int) byte;
		*b_counter = *b_counter + 1;
		return 0;
	}
}
