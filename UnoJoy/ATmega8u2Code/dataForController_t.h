/*  dataForController_t.h
 *
 *	  Alan Chatham - 2011
 *
 *  This is simply a typedef for a struct
 *   that holds information about controller
 *   button presses.  It is used by the controller
 *   libraries to pass information from an application
 *   to a library function that formats and sends
 *   appropriate controller data
 */

#ifndef DATA_FOR_CONTROLLER_T
#define DATA_FOR_CONTROLLER_T

	typedef struct dataForController_t
	{
		uint8_t button1 : 1;  // variables to abstractly tell us which buttons are pressed		
		uint8_t button2 : 1;
		uint8_t button3 : 1;
		uint8_t button4 : 1;
		uint8_t button5 : 1;
		uint8_t button6 : 1;
		uint8_t button7 : 1;
		uint8_t button8 : 1;
		
		uint8_t button9 : 1;
		uint8_t button10 : 1;
		uint8_t button11 : 1;
		uint8_t button12 : 1;
		uint8_t button13 : 1;
		uint8_t button14 : 1;
		uint8_t button15 : 1;
		uint8_t button16 : 1;

		uint8_t dpadLeftOn : 1;
		uint8_t dpadUpOn : 1;
		uint8_t dpadRightOn : 1;
		
		uint8_t dpadDownOn : 1;
		uint8_t leftStickX : 8; 
		uint8_t leftStickY : 8;
		uint8_t rightStickX : 8;
		uint8_t rightStickY : 8;
	} dataForController_t;


#endif
