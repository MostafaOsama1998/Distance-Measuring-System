/******************************************************************************
 *
 * Module: Ultra Sonic
 *
 * File Name: ultra_sonic.c
 *
 * Description: Source file for the Ultra Sonic HC-SR04 driver
 *
 * Author: Mostafa Osama
 *
 *******************************************************************************/

#include "ultra_sonic.h"
#include "icu.h"
#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include<avr/delay.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

static uint8 edgeCounter=0; /* Variable to calculate the ICU edges */
static uint16 timeHigh=0; /* variable to count the high time */
static uint16 timeLow=0; /* variable to calculate the low time */
static uint16 timePeriod=0; /* variable to calculate the period */

/*******************************************************************************
 *                               Functions                                     *
 *******************************************************************************/

/*
 * Description: Function to initialize the Ultra Sonic sensor
 * set sensor's pin directios and initialize the ICU
 */

void Ultrasonic_init(void)
{
	/*the ICU setup structure*/

	ICU_ConfigType ICU_setup={F_CPU_8,RAISING};

	/*passing the structure by address to set ICU settings */

	ICU_init(&ICU_setup);

	/*passing the address of the callback function */

	ICU_setCallBack(Ultrasonic_edgeProcessing);

	/*setting the Trigger pin as output pin*/

	GPIO_setupPinDirection(US_TRIGGER_PORT,US_TRIGGER_PIN,PIN_OUTPUT);

	/*setting the Echo pin as input pin*/

	GPIO_setupPinDirection(US_ECHO_PORT,US_ECHO_PIN,PIN_INPUT);
}

/*
 * Description: Function to send the Trigger 10us signal
 */

void Ultrasonic_Trigger(void)
{
	/*send the Trigger signal by giving logic high on the pin*/

	GPIO_writePin(US_TRIGGER_PORT,US_TRIGGER_PIN,LOGIC_HIGH);

	/*clear the TCNT1 to start calculation */

	ICU_clearTimerValue();

	/*a 10us delay from the data sheet of the module*/

	_delay_us(10);

	/*stop the trigger signal to start calculation*/

	GPIO_writePin(US_TRIGGER_PORT,US_TRIGGER_PIN,LOGIC_LOW);
}

/*
 * Description: Function to calculate the distance by the time that get
 * from the ICU and return the distance
 */

uint16 Ultrasonic_readDistance(void)
{
	uint16 distance=0 ;

	Ultrasonic_Trigger();/*call the trigger function to start the trigger pulse*/

	/*
	 * this equation is to calculate the distance by the ultra sonic sensor
	 * the distance = (high time * velocity(340m/s))/2
	 */

	distance= timeHigh/58;

	return distance; /*return the distance*/
}

/*
 * Description: the call back function that is passed to the ICU ISR
 * it calculate the time that the Ultra sound wave takes to be sent and received
 * by the Echo
 */
void Ultrasonic_edgeProcessing(void)
{
	/*increase the edgeCounter before we start calculation*/
	edgeCounter++;

	if(edgeCounter==1)
	{
		/*capture the time low of the signal*/

		timeLow=ICU_getInputCaptureValue();

		/*setting the ICU interrupt by falling edge*/

		ICU_setEdgeDetectionType(FALLING);
	}
	else if(edgeCounter==2)
	{
		/*capture the time of the period */

		timePeriod=ICU_getInputCaptureValue();

		/*setting the ICU interrupt by raising edge*/

		ICU_setEdgeDetectionType(RAISING);

		/*set the edge counter to zero to start again*/

		edgeCounter=0;

		/*time high is the time that the Echo takes to send and receive the signal */

		timeHigh= timePeriod-timeLow;

		/*clear the TCNT1 to start calculation again*/

		ICU_clearTimerValue();
	}
}
