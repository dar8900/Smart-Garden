#ifndef TASK_SELECTION_H
#define TASK_SELECTION_H

#define TASK_DIMMING

#define TASK_TIME

#define TASK_IGROSENSORPUMP

#define TASK_LCD

#define TASK_KEYBOARD

#define TASK_EEPROM

#define TASK_ETH_SD

#define TASK_BT

// #define TASK_ETH

#define TASK_DIMMING_DELAY			       1000	          
#define TASK_TIME_DELAY				    	100
#define TASK_IGROSENSORPUMP_DELAY	    	250	
#define TASK_LCD_DELAY				    	500
#define TASK_KEYBOARD_DELAY			    	 25
#define TASK_EEPROM_DELAY	               1000
#define TASK_ETH_SD_DELAY			         50	
#define TASK_BT_DELAY				    	200
// #define TASK_ETH_DELAY						250


/* STACK IN UINT16_T = 2 bytes */

#define TASK_DIMMING_STACK			    	192	          
#define TASK_TIME_STACK				    	192
#define TASK_IGROSENSORPUMP_STACK	    	192	
#define TASK_LCD_STACK				    	192
#define TASK_KEYBOARD_STACK			    	192
#define TASK_EEPROM_STACK	                192
#define TASK_ETH_SD_STACK			        192	
#define TASK_BT_STACK				    	192
// #define TASK_ETH_STACK						192
										// 3072 bytes = 37% della RAM


#define TASK_DIMMING_PRIORITY				  1	          
#define TASK_TIME_PRIORITY					  3
#define TASK_IGROSENSORPUMP_PRIORITY		  2	
#define TASK_LCD_PRIORITY					  1
#define TASK_KEYBOARD_PRIORITY				  3
#define TASK_EEPROM_PRIORITY	              0
#define TASK_ETH_SD_PRIORITY			      1
#define TASK_BT_PRIORITY					  3
// #define TASK_ETH_PRIORITY					  2

#endif