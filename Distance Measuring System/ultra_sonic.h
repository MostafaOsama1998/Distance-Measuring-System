/******************************************************************************
 *
 * Module: Ultra Sonic
 *
 * File Name: ultra_sonic.h
 *
 * Description: Source file for the Ultra Sonic HC-SR04 driver
 *
 * Author: Mostafa Osama
 *
 *******************************************************************************/

#ifndef ULTRA_SONIC_H_
#define ULTRA_SONIC_H_
#include "std_types.h"

#define US_ECHO_PORT PORTD_ID /*the ultra sonic echo port*/
#define US_ECHO_PIN PIN6_ID /*the ultra sonic echo pin*/
#define US_TRIGGER_PORT PORTB_ID /*the ultra sonic trigger port*/
#define US_TRIGGER_PIN PIN5_ID /*the ultra sonic trigger pin*/

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 * Description: Function to initialize the Ultra Sonic sensor
 * set sensor's pin directions and initialize the ICU
 */

void Ultrasonic_init(void);

/*
 * Description: Function to send the Trigger 10us signal
 */

void Ultrasonic_Trigger(void);

/*
 * Description: Function to calculate the distance by the time that get
 * from the ICU and return the distance
 */

uint16 Ultrasonic_readDistance(void);

/*
 * Description: the call back function that is passed to the ICU ISR
 * it calculate the time that the Ultra sound wave takes to be sent and received
 * by the Echo
 */

void Ultrasonic_edgeProcessing(void);

#endif /* ULTRA_SONIC_H_ */
