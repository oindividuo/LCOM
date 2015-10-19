#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

//Keys
#define ESC_MAKE 0x01
#define ESC_BREAK 0x81


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
