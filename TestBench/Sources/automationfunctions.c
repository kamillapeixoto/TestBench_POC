/*
 * automationfunctions.c
 *
 *  Created on: Jun 28, 2023
 *      Author: kamil
 */

#include "Cpu.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include"automationdefinit.h"
#include"automationfunctions.h"

void Init_varAuto (void)
{
 onoff_state = BUTTON_STATE_IDLE;
  onoff_counter = 0;
 onoff_release_delay=200;

 bell_state = BUTTON_STATE_IDLE;
 bell_counter = 0;
 bell_release_delay = 300; /* Default press duration */

 plus_state = BUTTON_STATE_IDLE;
 plustime_state = TIME_STATE_IDLE;
 plus_counter = 0;
 plus_release_delay=15;

 minus_state = BUTTON_STATE_IDLE;
 minustime_state = TIME_STATE_IDLE;
 minus_counter = 0;
minus_release_delay=15;
/************************auto increment or decrement**********************/
 autotime_press_counter=900;
 autotime_counter = 0;

//*************** settings increment decrement ISR***************************//
 count_pm_init= COUNTER_STATE_IDLE; // initialization of counter for settings increment and decrement
 setting_counter = 1;
 const uint8_t MAX_SETTING_COUNTER = 20U;
 const uint8_t MIN_SETTING_COUNTER = 1U;
 snprintf(checkCommand, sizeof(checkCommand),CHECK_COMMAND);
		speed_value = 20;
		  uart_state = UART_STATE_IDLE;

		  consigne =0;
		   bolusPerMinute=0;
		   tauxOxygen=0;
		   temperature=0 ;
		   vitesseCompresseur=0;
}

/********************************************************
char P5S[8]; // this should be outside so it can be modified by the function
int current;
int pressure;
we should use extern function to call variables in safety feature
void Init_varAuto (void)
{
	P5S = STATUSOK; // this should be outside so it can be modified by the function
	current = 0;
	pressure = 0;
}


void safety_power() {
    if(current >= 5000) { //CHANGE WITH REQUIRED VALUES
        strcpy(P5S, CURRENT_HIGH); // use strcpy to change the value of the string
        onoff_state = BUTTON_STATE_PRESSED;
    }
    if(pressure <= 5000) {  //CHANGE WITH REQUIRED VALUES
        strcpy(P5S, PRESSURE_ERROR); // use strcpy to change the value of the string
        onoff_state = BUTTON_STATE_PRESSED;
    }
    else
    	strcpy(P5S, STATUSOK);
}
*/

/**
 Title: UART STATE MACHINE DEFINITION
 description: State Machine responsible for the UART communication for the P5
 in terms of sending and receiving data packets, and commands to the Device under test
 use: defines different states for the UART communication for P5
 components:
 1-IDLE: do nothing
 2- send: for Get_Status command using sendCheckCommand()
 3- receive: activated to receive the status packet from the P5 receiveResponse()
 4- process: activated after receiving the packet to set each packet index into a variable using processResponse()
 5- speed: for CC-%d command to change the compressor speed using sendspeedCommand()

 **/
// The function updatespeedvalue is used to generate a formatted speed value string.
void updatespeedvalue(){
    // snprintf formats and stores a series of characters and values in the array "speed_minus".
    // The format of the string is "CC -%d\n\r", where %d is replaced by the value of speed_value.
    snprintf(speed_minus, sizeof(speed_minus), "CC -%d\n\r", speed_value);
}

// sendCheckCommand function sends a pre-defined command to the UART interface
void sendCheckCommand(volatile uart_state_t* uartstate) {
    // LPUART_DRV_SendData sends data over the LPUART interface.
    // The data being sent is the string checkCommand.
    LPUART_DRV_SendData(INST_LPUART1,checkCommand, strlen((const char*)checkCommand));
}

// sendspeedCommand function sends the speed command string to the UART interface
void sendspeedCommand() {
    // The formatted speed command string stored in speed_minus is sent over the UART interface
    LPUART_DRV_SendData(INST_LPUART1, (unsigned char*)speed_minus, strlen((const char*)speed_minus));
}

// receiveResponse function receives a response from the UART interface
void receiveResponse(volatile uart_state_t* uartstate)
{
    // LPUART_DRV_ReceiveData receives data over the LPUART interface.
    // The received data is stored in the packet array.
    status = LPUART_DRV_ReceiveData(INST_LPUART1, packet, sizeof(packet)-1);

    // If data is successfully received, the UART state is set to PROCESS.
    if(status == STATUS_SUCCESS) {
    	    *uartstate = UART_STATE_PROCESS;
    }
}

// processResponse function processes the received response from the UART interface
void processResponse(volatile uart_state_t* uartstate) {

	int fieldIndex = 0;
	int byteIndex = 0;
	uint8_t fieldBuffer[PACKET_SIZE];
	int fieldBufferIndex = 0;
	int commaCount = 0;  // Counter for commas

    // This loop goes through the packet array byte by byte.
	for(byteIndex = 0; byteIndex < PACKET_SIZE; byteIndex++) {
        // If the current byte is a comma (0x2C) or null character ('\0'), a field has ended.
	    if(packet[byteIndex] == 0x2C || packet[byteIndex] == '\0') {
	        commaCount++;  // Increment comma count
	        fieldBuffer[fieldBufferIndex] = '\0';

	        // Depending on the index of the field, the data is interpreted differently.
	        switch (fieldIndex) {
	        case BOLUS_PER_MINUTE_INDEX:
	                bolusPerMinute = atoi((char*)fieldBuffer);
	                break;
	            case TAUX_OXYGENE_INDEX:
	                tauxOxygen = atof((char*)fieldBuffer);
	                break;
	            case TEMPERATURE_INDEX:
	            	temperature = atoi((char*)fieldBuffer);
	                break;
	            case  VITESSE_COMPRESSEUR_INDEX:
	                vitesseCompresseur= atoi((char*)fieldBuffer);
	                break;
	        }

	        fieldBufferIndex = 0;
	        fieldIndex++;

	        // If the end of the packet is reached or more than 11 fields have been processed, stop processing.
	        if(packet[byteIndex] == '\0' || commaCount > 11) {
	            break;
	        }
	    } else {
	        // If the byte is not a comma or null character, add it to the current field buffer.
	        fieldBuffer[fieldBufferIndex] = packet[byteIndex];
	        fieldBufferIndex++;
	    }
	}

	// Reset packet array for next use and set UART state to IDLE.
	memset(packet, 0, PACKET_SIZE);
	*uartstate = UART_STATE_IDLE;
}

// p5handshake function implements a finite state machine for UART communication.
void p5handshake(volatile uart_state_t* uartstate){
	switch (*uartstate) {
    case UART_STATE_IDLE:
        	break;
    case UART_STATE_SEND:
        // If the UART state is SEND, send the check command.
        sendCheckCommand(uartstate);
        // Then set the UART state to RECEIVE.
        *uartstate= UART_STATE_RECEIVE;
        break;
    case UART_STATE_RECEIVE:
        // If the UART state is RECEIVE, receive a response.
        receiveResponse(uartstate);
        break;
    case UART_STATE_PROCESS:
        // If the UART state is PROCESS, process the received response.
     processResponse(uartstate);
        break;
    case UART_STATE_SPEED:
        // If the UART state is SPEED, send the speed command.
    	sendspeedCommand(uartstate);
    	// Evaluate the setting_counter to set button states accordingly.
    	if (setting_counter<=10U)
		        {
		         plus_state = BUTTON_STATE_PRESSED;
		         minus_state= BUTTON_STATE_PRESSED;
		        }
    	  if(setting_counter>=11U) {
    		  minus_state= BUTTON_STATE_PRESSED;
    		  plus_state = BUTTON_STATE_PRESSED;
    	  }

    	// Set the UART state back to IDLE.
    	*uartstate= UART_STATE_IDLE;
            break;
    default:
    break;
    }
}


/**********************************************************************************/
/**
Title:switch for time counter for automated tests
decription: if the function is enabled, it presses either plus or minus button depending on user's required test
eg: auto increment of settings, or auto decrement of settings, time of decrement or increment decided by user for autotime_counter variable
use: automated tests for P5 oxygen concentrator
states:
1-timer state pressed: either plus or minus time state enabeled. enabling timed pressing of the enabled test
2-IDLE: do nothing, counter equal 0

 **/

void autotimecounter(volatile timecount_state_t* sstate,volatile button_state_t* timerstate, volatile timecount_state_t* other_sstate)
{
    // If the other state is active, we should not update this state
    if (*other_sstate == TIME_STATE_PRESSED) {
        return;
    }

    switch (*sstate) {
        case TIME_STATE_PRESSED:
            (autotime_counter)++;
            if (autotime_counter >= autotime_press_counter)
            {
                *timerstate = BUTTON_STATE_PRESSED;
                autotime_counter = 0;
            }
            break;
        case TIME_STATE_IDLE: autotime_counter = 0;
        break;
        default:
            break;
    }
}
/**************************************************************************************/
/**
Title: buttons pressing state machine
description: state machine to govern the pressing of buttons and their durations of pressing
use: checks if a button is pressed from the dashboard and act accordingly in terms of duration of press and assigned pin
states:
1- IDLE: ensure that the pin is high
2-BUTTON_STATE_PRESSED: write the pin as low, and assign the desired press duration to a counter then start decrementing the counter
when counter equals 0 return to IDLE
 **/
void update_button_state(volatile button_state_t* state, volatile uint32_t* counter, uint32_t pin, uint32_t* release_delay)
{
    /* State machine */
    switch (*state) {
        case BUTTON_STATE_IDLE:
        	PINS_DRV_WritePin(GPIO_PORT1, pin, 1);
            *counter = *release_delay;
            break;
        case BUTTON_STATE_PRESSED:
        	PINS_DRV_WritePin(GPIO_PORT1, pin, 0);
        	 *counter = *release_delay;
            if (*counter > 0) {
                (*counter)--;
            } else {
                 /* End button press */
                *state = BUTTON_STATE_IDLE;
            }
            break;
        default:
            break;
    }
}
/*********************************************************************************************/
/**
Title: P5 settings indicator and manager
description: switch that ensures that tests are performed at the desired setting
use: when enabled, the user states in which setting the DUT is. afterwards, for each press of plus or minus buttons the setting either increments or decrements
states:
1:counter state init: checks either plus or minus pressed, and checks the previous state of of the buttons to eliminate floating change of setting number
if the counter is more than 20 or less than 1, don't exceed these parameters
2: IDLE: Do Nothing
 **/
void updatecounter(count_state_t* count_state)
{
	static bool plus_previous_state = false;  // Added variables to store the previous state of the buttons
	static bool minus_previous_state = false; // Added variables to store the previous state of the buttons

	switch(*count_state)
	{
	case COUNTER_STATE_INIT:
		    // Check if plus button was just pressed
		    if (plus_state == BUTTON_STATE_PRESSED && plus_previous_state == false)
		    {
		    	setting_counter++;
		    	plus_previous_state = true; // Store the current state of the plus button
		        if (setting_counter>=20U)
		        {
		            setting_counter = 20U;
		        }
		    }
            // If the button is not pressed, reset the previous state
		    else if (plus_state == BUTTON_STATE_IDLE)
		    {
		    	plus_previous_state = false;
		    }

		    // Check if minus button was just pressed
		    if (minus_state == BUTTON_STATE_PRESSED && minus_previous_state == false)
		    {
		        setting_counter--;
		        minus_previous_state = true; // Store the current state of the minus button
		        if (setting_counter<=0U)
		        {
		            setting_counter = 1U;
		        }
		    }
		    // If the button is not pressed, reset the previous state
		    else if (minus_state == BUTTON_STATE_IDLE)
		    {
		    	minus_previous_state = false;
		    }
		    break;
	case COUNTER_STATE_IDLE:
            break;
	default:
        	break;
    }
}



void timing_pal_AUTO_callBack(void * userData)
{
	(void)userData;


	update_button_state(&onoff_state, &onoff_counter, ONOFF, &onoff_release_delay);
	    update_button_state(&bell_state, &bell_counter, BELL, &bell_release_delay);
	    update_button_state(&plus_state, &plus_counter, PLUS, &plus_release_delay );
	    update_button_state(&minus_state, &minus_counter, MINUS, &minus_release_delay);
	    autotimecounter(&minustime_state, &minus_state, &plustime_state);
	    autotimecounter(&plustime_state, &plus_state, &minustime_state);
	    updatecounter(&count_pm_init);
	    p5handshake(&uart_state);
	    updatespeedvalue();
	    /*if(){   //essential condition that system doesn't shutdown before the start of the test
	    	safety_power();
	    }
	*/

}
