#ifndef TASK_SELECTION_H
#define TASK_SELECTION_H

#define TASK_DIMMING

#define TASK_TIME

#define TASK_IGROSENSORPUMP

#define TASK_LCD

#define TASK_KEYBOARD

#define TASK_EEPROM

#define TASK_SD

#define TASK_BT


#define TASK_DIMMING_DELAY			       1000	          
#define TASK_TIME_DELAY				    	100
#define TASK_IGROSENSORPUMP_DELAY	    	250	
#define TASK_LCD_DELAY				    	500
#define TASK_KEYBOARD_DELAY			    	 25
#define TASK_EEPROM_DELAY	               1000
#define TASK_SD_DELAY			           2500	
#define TASK_BT_DELAY				    	200


#define TASK_DIMMING_HEAP			    	 64	          
#define TASK_TIME_HEAP				    	 64
#define TASK_IGROSENSORPUMP_HEAP	    	 64	
#define TASK_LCD_HEAP				    	128
#define TASK_KEYBOARD_HEAP			    	 64
#define TASK_EEPROM_HEAP	                 64
#define TASK_SD_HEAP			             64	
#define TASK_BT_HEAP				    	 64


#define TASK_DIMMING_PRIORITY				  1	          
#define TASK_TIME_PRIORITY					  3
#define TASK_IGROSENSORPUMP_PRIORITY		  2	
#define TASK_LCD_PRIORITY					  1
#define TASK_KEYBOARD_PRIORITY				  3
#define TASK_EEPROM_PRIORITY	              0
#define TASK_SD_PRIORITY			          0
#define TASK_BT_PRIORITY					  3

#endif