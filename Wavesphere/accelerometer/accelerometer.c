/*
 * Accelerometer.c
 *
 *  Created on: Mar 23, 2013
 *      Author: Adrian
 */

#include <msp430fr5969.h>

void initAcc() {
	P4DIR |= BIT3 | BIT4;							 // set p4.3 and p4.4 as output
	// Turn on Accelerometer
	P4OUT |= BIT4;

	// Configure ADC
	P4SEL1 |= BIT1 | BIT0 | BIT2;
	P4SEL0 |= BIT1 | BIT0 | BIT2;

	REFCTL0 |= REFON; // Turn on voltage reference to 1.2 V
	REFCTL0 &= ~REFTCOFF; // Disable Temperature Sensor

	ADC12CTL0 |= ADC12SHT0_2 | ADC12SHT1_2 | ADC12ON; // Sampling time, S&H=16, ADC12 on
	ADC12CTL1 |= ADC12SHP; // | ADC12CONSEQ_1;                     // Use sampling timer, Select ACLK,
	ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
	//ADC12CTL2 |= ADC12PWRMD; //ADC LPM.  Samples cannot exceed 50ksps

	//Select ADC memory registers and input
	ADC12MCTL0 |= ADC12INCH_8;// | ADC12VRSEL_9;  // A8 ADC input select; Vref=AVCC; Vref-=1.2
	ADC12MCTL1 |= ADC12INCH_9;// | ADC12VRSEL_9;  // A9 ADC input select; Vref=AVCC
	ADC12MCTL2 |= ADC12INCH_10;	// | ADC12VRSEL_9; // A10 ADC input select; Vref=AVCC
}

void getAccData(int *dataArr) {
	volatile unsigned long xSample = 0, ySample = 0, zSample = 0;
	volatile unsigned int i = 0;
	//unsigned int dataArr[3];

	while (i < 8) {

		ADC12CTL3 &= ~0x1F; // Set starting measurement to 0
		ADC12CTL0 |= ADC12ENC | ADC12SC;        // Start sampling/conversion
		while (!(ADC12IFGR0 & BIT0));

		xSample += ADC12MEM0;
		ADC12IFGR0 &= ~BIT0;
		ADC12CTL0 &= ~ADC12ENC;

		ADC12CTL3 = 0x1;
		ADC12CTL0 |= ADC12ENC | ADC12SC;         // Start sampling/conversion
		while (!(ADC12IFGR0 & BIT1));

		ySample += ADC12MEM1;
		ADC12IFGR0 &= ~BIT1;
		ADC12CTL0 &= ~ADC12ENC;

		ADC12CTL3 = 0x2;
		ADC12CTL0 |= ADC12ENC | ADC12SC;         // Start sampling/conversion
		while (!(ADC12IFGR0 & BIT2));

		zSample += ADC12MEM2;
		ADC12IFGR0 &= ~BIT2;
		ADC12CTL0 &= ~ADC12ENC;

		i = i + 1;
	}

	dataArr[0] = xSample >> 3;
	dataArr[1] = ySample >> 3;
	dataArr[2] = zSample >> 3;
}

void accSleep() {
	P4OUT &= ~BIT4; // Turn off Accelerometer Switch
	ADC12CTL0 &= ~ADC12ON; // Turn off ADC
}

void accWakeup() {
	P4OUT |= BIT4; // Turn on Accelerometer Switch
	ADC12CTL0 |= ADC12ON; // Turn off ADC
}

void accStartST() {
	P4OUT |= BIT3; // Turn on Self Test Bit
}

void accEndST() {
	P4OUT &= ~BIT3; // Turn off Self Test Bit
}

