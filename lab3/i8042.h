#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <minix/sysutil.h>

#define DELAY_US    20000
#define BIT(n) (0x01<<(n))
//Registers
	//status
#define STAT_REG    0x64	//Read the KBC state

	//input buffer (write)
#define KBC_CMD_REG 0x64	//Commands to the KBC
#define KBC_ARG_REG 0x60	//Arguments of KBC commands
#define KBD_CMD_REG 0x60	//Commands to the keyboard
#define KBD_ARG_REG 0x60	//Arguments of keyboard commands

	//output buffer (read)
#define KBD_SCNCOD_REG 	0x60	//Scandcodes, both make and break, received from the keyboar
#define KBC_CMD_RET_REG	0x60	//Return values from KBC command
#define KBD_CMD_RET_REG	0x60	//Return values from keyboard command
#define KBD_PRTCL_MSG 	0x60	//Confirmation protocol messages (ACK, Resend and Error)



//Keys
#define ESC_MAKE	0x01
#define ESC_BREAK 	0x81



//Confirmation protocol messages
#define KBD_ACK		0xFA	//Acknowledgement
#define KBD_RESEND	0xFB	//Resend
#define KBD_ERROR	0xFC	//Error



//Keyboard commands
#define KBD_RESET   0xFF 	//Reset KBD
#define KBD_DEFAULT 0xF6 	//Set default values and enable KBD
#define KBD_DISABLE 0xF5 	//Disable KBD (set default values and stop scanning)
#define KBD_ENABLE  0xF4 	//Clear buffer and enable KBD
#define KBD_REP     0xF3	//Change KBD repetition rate/delay - bits 0-4 rate, bits 5-6 delay
#define KBD_LED		0xED	//Switch on/off KBD LEDs - bits 0-2



//KBC commands
#define KBC_READ		0x20	//Read Command Byte
#define KBC_WRITE		0x60	//Write Command Byte
#define KBC_CHECK		0xAA	//Check KBC (Self-test)
#define KBC_CHECK_ITF	0xAB	//Check Keyboard Interface
#define KBC_ENABLE		0xAE	//Enable KBD Interface
#define KBC_DISABLE		0xAD	//Disable KBD Interface





#endif /* _LCOM_I8042_H */
