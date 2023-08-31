/*
 * automationdefinit.h
 *
 *  Created on: Jun 27, 2023
 *      Author: amgad
 */

#ifndef AUTOMATIONDEFINIT_H_
#define AUTOMATIONDEFINIT_H_

#define EVB

#define GPIO_PORT1	PTD
#define MINUS		9U
#define BELL		8U
#define ONOFF		2U
#define PLUS		0U
#define Tx			7U
#define Rx			6U
#define LPIT0_CHANNEL	    0UL
#define LPIT_Channel_IRQn   LPIT0_Ch0_IRQn


/********************buttons state machine declarations************************/


//*************** settings increment decrement ISR***************************//


/*****************************handshake parameters**************************/

#define UTILISATION_TOTAL_INDEX 1
#define JOUR_MOIS_ANNEE_INDEX 2
#define HEURE_MINUTE_SECONDE_INDEX 3
#define SESSION_TIMER_INDEX 4
#define CONSIGNE_INDEX 5
#define BOLUS_PER_MINUTE_INDEX 6
#define TAUX_OXYGENE_INDEX 7
#define TEMPERATURE_INDEX 8
#define VITESSE_COMPRESSEUR_INDEX 9
#define PRESSION_ATMOSPHERIQUE_INDEX 10
#define PRESSION_COLONNE_ZEOLITE_1_INDEX 11
#define PRESSION_COLONNE_ZEOLITE_2_INDEX 12
#define DETECTION_CHUTE_INDEX 13
#define PRESENCE_CHARGEUR_INDEX 14
#define ETAT_BATTERIE_INDEX 15
#define TENSION_BATTERIE_INDEX 16
#define CHARGE_BATTERIE_RESTANTE_INDEX 17
#define CODE_ALARME_INDEX 18
#define ALARM_SUBCLASS_INDEX 19
#define ETAT_COLONNE_ZEOLITE_INDEX 20
#define VERSION_SOFTWARE_INDEX 21


/**********************************handshake parameters end******************/


/**********************error safety feature***************/
#define STATUSOK "STATUS_OK"
#define PRESSURE_ERROR "pressure_low_turning_off"
#define CURRENT_HIGH "current_error_turning_off"



#endif /* AUTOMATIONDEFINIT_H_ */
