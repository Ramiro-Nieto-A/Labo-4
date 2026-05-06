/*
 * ports.h
 *
 *  Created on: Feb 18, 2025
 *      Author: jacoby
 */

#ifndef PORTS_H_
#define PORTS_H_

void Init_Ports(void);
void TurnON(uint32_t pin);
void TurnOFF(uint32_t pin);
void Toggle(uint32_t pin);
bool SW2_read(void);
bool SW3_read(void);



#define PIN_RED_LED 		22     	//PTB22
#define PIN_BLUE_LED 		21     	//PTB21
#define PIN_GREEN_LED 		26 	   	//PTE26


// Switches
#define SW2 		6     	//PTC6
#define SW3 		4    	//PTA4
#define DO			2		//PTA2


// Test Points
#define TP0 		10     	//PTC10
#define TP1 		11    	//PTC11

typedef enum
{
	PORT_mAnalog,
	PORT_mGPIO,
	PORT_mAlt2,
	PORT_mAlt3,
	PORT_mAlt4,
	PORT_mAlt5,
	PORT_mAlt6,
	PORT_mAlt7,

} PORTMux_t;

typedef enum
{
	PORT_eDisabled				= 0x00,
	PORT_eDMARising				= 0x01,
	PORT_eDMAFalling			= 0x02,
	PORT_eDMAEither				= 0x03,
	PORT_eInterruptDisasserted	= 0x08,
	PORT_eInterruptRising		= 0x09,
	PORT_eInterruptFalling		= 0x0A,
	PORT_eInterruptEither		= 0x0B,
	PORT_eInterruptAsserted		= 0x0C,
} PORTEvent_t;


#endif /* PORTS_H_ */
