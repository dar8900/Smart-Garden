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

#define PERCENT_RAM_ALLOCATED(Perc)			  ((Perc * 8192)/100)


#define TASK_DIMMING_DELAY			       1000	          
#define TASK_TIME_DELAY				    	250
#define TASK_IGROSENSORPUMP_DELAY	    	250	
#define TASK_LCD_DELAY				    	500
#define TASK_KEYBOARD_DELAY			    	 25
#define TASK_EEPROM_DELAY	               1000
#define TASK_ETH_SD_DELAY			         50	
#define TASK_BT_DELAY				    	 50
// #define TASK_ETH_DELAY						250


/* STACK IN UINT16_T = 2 bytes - minima dimensione per task = 128 (FreeRTOSConfig.h) */

#define TASK_DIMMING_STACK			    	128	          
#define TASK_TIME_STACK				    	128
#define TASK_IGROSENSORPUMP_STACK	    	128	
#define TASK_LCD_STACK				    	192
#define TASK_KEYBOARD_STACK			    	128
#define TASK_EEPROM_STACK	                128
#define TASK_ETH_SD_STACK			        192	
#define TASK_BT_STACK				    	192
// #define TASK_ETH_STACK						192
										
#define TASKS_STACK_SIZE				   (TASK_DIMMING_STACK+TASK_TIME_STACK+TASK_IGROSENSORPUMP_STACK+TASK_LCD_STACK+TASK_KEYBOARD_STACK+TASK_EEPROM_STACK+TASK_ETH_SD_STACK+TASK_BT_STACK)*2//2944
#define PROGRAM_RAM_OCCUPIED			   3689	
#define TOTAL_RAM_USED					   TASKS_STACK_SIZE+PROGRAM_RAM_OCCUPIED
										

#if (TOTAL_RAM_USED>PERCENT_RAM_ALLOCATED(80))
	#error OVER_80_PERCENT_RAM_USED
#endif


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