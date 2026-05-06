/*
 * Ports.c
 *
 *  Created on: Feb 18, 2025
 *      Author: jacoby
 */
#include "hardware.h"
#include "ports.h"
#include "config.h"



void Init_Ports(void)
{

	 	 	 //Enable clocking for port A B C,E

 	 	 	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
			SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
			SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
			SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

			//****** Define RGBled pins attributes.*******//

            // Set Attributes for pins 21(Blue) , 22(Red) on port B
            // Then Set Attributes pin 26(Green)


			// Set Attributes for pin 21(Blue) on PORT B

			PORTB->PCR[PIN_BLUE_LED]=0x0; //Clear all bits
			PORTB->PCR[PIN_BLUE_LED]|=PORT_PCR_MUX(PORT_mGPIO); //Set MUX to GPIO
			PORTB->PCR[PIN_BLUE_LED]|=PORT_PCR_DSE(1);          //Drive strength enable=Slow rate

			// Set Attributes for pin 22(Red) on PORT B

			PORTB->PCR[PIN_RED_LED]=0x0; //Clear all bits
			PORTB->PCR[PIN_RED_LED]|=PORT_PCR_MUX(PORT_mGPIO); 	//Set MUX to GPIO
			PORTB->PCR[PIN_RED_LED]|=PORT_PCR_DSE(1);          	//Drive strength enable=Slow rate

			// Set Attributes for pin 26(Green) on PORT E

			PORTE->PCR[PIN_GREEN_LED]=0x0; //Clear all bits
			PORTE->PCR[PIN_GREEN_LED]|=PORT_PCR_MUX(PORT_mGPIO); //Set MUX to GPIO
			PORTE->PCR[PIN_GREEN_LED]|=PORT_PCR_DSE(1);          //Drive strength enable=Slow rate


			// Set Attributes for Test points PTC11 ,PTC10 on PORT C

			PORTC->PCR[TP0]=0x0; //Clear all bits
			PORTC->PCR[TP0]|=PORT_PCR_MUX(PORT_mGPIO); //Set MUX to GPIO
			PORTC->PCR[TP0]|=PORT_PCR_DSE(1);          //Drive strength enable=Slow rate

			PORTC->PCR[TP1]=0x0; //Clear all bits
			PORTC->PCR[TP1]|=PORT_PCR_MUX(PORT_mGPIO); //Set MUX to GPIO
			PORTC->PCR[TP1]|=PORT_PCR_DSE(1);          //Drive strength enable=Slow rate

			// Set Attributes for Switches PTA4 and PTC6

			PORTC->PCR[SW2]=0x0; //Clear all bits
			PORTC->PCR[SW2]|=PORT_PCR_MUX(PORT_mGPIO); //Set MUX to GPIO
			PORTC->PCR[SW2]|=PORT_PCR_DSE(1);          //Drive strength enable=Slow rate
			PORTC->PCR[SW2]|=PORT_PCR_PE(1);          //Drive Pull up enable
			PORTC->PCR[SW2]|=PORT_PCR_PS(1);          //Drive strength enable=Slow rate

			PORTA->PCR[SW3]=0x0; //Clear all bits
			PORTA->PCR[SW3]|=PORT_PCR_MUX(PORT_mGPIO); //Set MUX to GPIO
			PORTA->PCR[SW3]|=PORT_PCR_DSE(1);          //Drive strength enable=Slow rate
			PORTA->PCR[SW3]|=PORT_PCR_PE(1);
			PORTA->PCR[SW3]|=PORT_PCR_PS(1);

			PORTA->PCR[DO]=0x0; //Clear all bits
			PORTA->PCR[DO]|=PORT_PCR_MUX(PORT_mGPIO); //Set MUX to GPIO
			PORTA->PCR[DO]|=PORT_PCR_DSE(1);          //Drive strength enable=Slow rate
			PORTA->PCR[DO]|=PORT_PCR_PE(1);
			PORTA->PCR[DO]|=PORT_PCR_PS(1);


			// Turn off pin 22 y pin 21 PTB (active low) do this first to avoid glitches
			PTB->PSOR = (1<<PIN_BLUE_LED)|(1<<PIN_RED_LED);

			// Set pins 21 and 22 on PTB as output
			PTB->PDDR |= (1<<PIN_BLUE_LED)|(1<<PIN_RED_LED);


			// Turn off pin 26 on PTE (active low) do this first to avoid glitches
			PTE->PSOR = (1<<PIN_GREEN_LED);

			// Set pin 26 as output
			PTE->PDDR |= (1<<PIN_GREEN_LED);


			//****** Test Pins PTC0 (TP0) , PTC1 (TP1) Setup *********

			// Turn off Test Points (do this first to avoid glitches)
			PTC->PCOR =  (1<<TP0)|(1<<TP1);
			// Set pins 10 and 11 on PTC as output
			PTC->PDDR |= (1<<TP0)|(1<<TP1);

			//****** Switches Pins PTC6 (SW2 on board) , PTA4 (SW3 on board) Setup *********

			// Setting as input, so set value is not required
			//PTC->PSOR |=  (1<<SW2);
			//PTA->PSOR |=  (1<<SW3);
			// Clear pins PTC6 and PTA4 to set as input
			PTC->PDDR &= ~(1<<SW2);
			PTA->PDDR &= ~(1<<SW3);
			PTA->PDDR &= ~(1<<DO);


}

// Test points Services

//************************
// Turns on (High) a pin
// E.g. TurnON(TP0);
//************************
void TurnON(uint32_t pin)
{
	#if TEST_POINT==ENABLED
	PTC->PSOR = (1<<pin);
	#endif

}
//************************
// Turns off (Low) a pin
// E.g. TurnOFF(TP0);
//************************
void TurnOFF(uint32_t pin)
{
	#if TEST_POINT==ENABLED
	PTC->PCOR = (1<<pin);
	#endif

}
//************************
// Toggles a pin
// E.g. Toggle(TP0);
//************************
void Toggle(uint32_t pin)
{

	#if TEST_POINT==ENABLED
	PTC->PTOR = (1<<pin);
	#endif

}

//************************
// Read SW2 status
//************************
bool SW2_read(void){
	return (bool)!(PTC->PDIR & (1<<SW2));
}


//************************
// Read SW3 status
//************************
bool SW3_read(void){
	return (bool)!(PTA->PDIR & (1<<SW3));
}


//************************
// Read SW2 status
//************************
bool DO_read(void){
	return (bool)!(PTA->PDIR & (1<<DO));
}

