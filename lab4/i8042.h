#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <minix/sysutil.h>

#define DELAY_US    20000

#define BIT(n) (0x01<<(n))
#define KBD_IRQ	 1
#define MS_IRQ   12

//Registers
//status
#define STAT_REG    0x64	//Read the KBC state

//input buffer (write)
#define KBC_CMD_REG 0x64	//Commands to the KBC
#define KBC_ARG_REG 0x60	//Arguments of KBC commands
#define KBD_CMD_REG 0x60	//Commands to the keyboard
#define KBD_ARG_REG 0x60	//Arguments of keyboard commands

//output buffer (read)
#define OUT_BUF			0x60	//General use
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

//Mouse-Related KBC Commands
#define MS_READ		    0x20	//Read Command Byte
#define MS_WRITE		0x60	//Write Command Byte
#define MS_CHECK_ITF    0xA9	//Check Mouse Interface
#define MS_WRITE_BYTE	0xD4	//Write Byte to Mouse
#define MS_ENABLE		0xA8	//Disable Mouse
#define MS_DISABLE		0xA7	//Enable Mouse
#define MS_ACK          0xFA    //Reset
#define MS_NACK         0xFE    //Resend
#define MS_ERROR        0xFC    //second consecutive invalid byte

//MS_WRITE_BYTE arguments
#define MS_RESET		0xFF	//Mouse reset
#define MS_RESEND		0xFE	//For serial communications errors
#define MS_SET_DEFAULTS 0xF6	//Set default values
#define MS_DSB_STREAM_M	0xF5	//Disable Stream Mode - In stream mode, should be sent before any other command
#define MS_DATA_PACKETS	0xF4	//Enable Sending Data Packets - In stream mode only
#define MS_SET_SMP_RATE	0xF3	//Set Sample Rate - Sets state sampling rate
#define MS_SET_REM_MODE	0xF0	//Set Remote mode - Send data on request only
#define MS_READ_DATA	0xEB	//Send data packet request
#define MS_SET_STR_MODE	0xEA	//Set Stream Mode - Send data on events
#define MS_STATUS_REQ	0xE9	//Status Request - Get mouse configuration (3 bytes)
#define MS_SET_RES		0xE8	//Set Resolution
#define MS_SET_SCAL_2	0xE7	//Set Scaling 2:1 - Acceleration mode
#define MS_SET_SCAL_1	0xE6	//Set Scaling 1:1 - Linear mode

#endif /* _LCOM_I8042_H */
