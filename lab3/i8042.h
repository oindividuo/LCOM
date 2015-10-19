#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

//Keys
#define ESC_MAKE 0x01
#define ESC_BREAK 0x81

//Commands
#define KBD_RESET 0xFF   //Reset KBD
#define KBD_DEFAULT 0xF6 //Set default values and enable KBD
#define KBD_DISABLE 0xF5 //Disable KBD (set default values and stop scanning)
#define KBD_ENABLE  0xF4 //Clear buffer and enable KBD



#endif /* _LCOM_I8042_H */
