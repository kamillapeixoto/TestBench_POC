/*
 * automationfunctions.h
 *
 *  Created on: Jun 27, 2023
 *      Author: amgad
 */

#ifndef AUTOMATIONFUNCTIONS_H_
#define AUTOMATIONFUNCTIONS_H_


typedef enum { // UART STATE MACHINE
	 UART_STATE_IDLE,
	 UART_STATE_SEND,
	 UART_STATE_RECEIVE,
	 UART_STATE_PROCESS,
	 UART_STATE_SPEED,
} uart_state_t;
typedef enum { // SETTING COUNTER MACHINE
    COUNTER_STATE_IDLE,
    COUNTER_STATE_INIT,
} count_state_t;
typedef enum { // BUTTON STATE MACHINE
    BUTTON_STATE_IDLE,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_WAIT,
} button_state_t;
typedef enum { // TIMER STATE MACHINE
    TIME_STATE_IDLE,
    TIME_STATE_PRESSED,
} timecount_state_t;

/********************buttons state machine declarations************************/
volatile button_state_t onoff_state;
volatile uint32_t onoff_counter;
 uint32_t onoff_release_delay;

volatile button_state_t bell_state ;
volatile uint32_t bell_counter ;
uint32_t bell_release_delay ; /* Default press duration */

volatile button_state_t plus_state ;
volatile timecount_state_t plustime_state ;
volatile uint32_t plus_counter ;
 uint32_t plus_release_delay;

volatile button_state_t minus_state;
volatile timecount_state_t minustime_state;
volatile uint32_t minus_counter;
uint32_t minus_release_delay;
/************************auto increment or decrement**********************/
volatile uint32_t autotime_press_counter;
volatile uint32_t autotime_counter;

//*************** settings increment decrement ISR***************************//
 count_state_t  count_pm_init; // initialization of counter for settings increment and decrement
volatile uint8_t setting_counter;
volatile int consigne;
volatile int bolusPerMinute;
volatile double tauxOxygen;
volatile int temperature ;
volatile int vitesseCompresseur;
/*volatile int PRESSION_ATMOSPHERIQUE =0;
volatile int PRESSION_COLONNE_ZEOLITE_1 =0;
volatile int PRESSION_COLONNE_ZEOLITE_2 =0;
volatile int DETECTION_CHUTE =0;
volatile int PRESENCE_CHARGEUR =0;
volatile int ETAT_BATTERIE =0;
volatile int TENSION_BATTERIE =0;
volatile int CHARGE_BATTERIE =0;
volatile int CODE_ALARME =0;
volatile int ALARM_SUBCLASS =0;
volatile int ETAT_COLONNE =0;
volatile int VERSION_SOFTWARE =0;
volatile int UTILISATION_TOTAL =0;
volatile int JOUR_MOIS_ANNEE =0;
volatile int HEURE_MINUTE_SECONDE =0;
volatile int SESSION_TIMER =0;
*/

#define CHECK_COMMAND "GET_STATUS\n\r" // The command to send to the device for checking UART connection


#define PACKET_SIZE 69 // don't go less than 68


uint8_t packet[PACKET_SIZE];
char  checkCommand[20];
status_t status;
 int speed_value ;
char speed_minus[20];
volatile uart_state_t uart_state ;
/**********************************handshake parameters end******************/


/**********************error safety feature***************/


void Init_varAuto (void);

void safety_power();

void updatespeedvalue();

void sendCheckCommand(volatile uart_state_t* uartstate);

void sendspeedCommand();

void receiveResponse(volatile uart_state_t* uartstate);

void processResponse(volatile uart_state_t* uartstate);

void p5handshake(volatile uart_state_t* uartstate);

void autotimecounter(volatile timecount_state_t* sstate,volatile button_state_t* timerstate, volatile timecount_state_t* other_sstate);

void update_button_state(volatile button_state_t* state, volatile uint32_t* counter, uint32_t pin, uint32_t* release_delay);

void updatecounter(count_state_t* count_state);


void timing_pal_AUTO_callBack(void * userData);



#endif /* AUTOMATIONFUNCTIONS_H_ */
