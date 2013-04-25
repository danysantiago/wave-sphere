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

	while (i < 32) {

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

	dataArr[0] = xSample >> 5;
	dataArr[1] = ySample >> 5;
	dataArr[2] = zSample >> 5;

	//return dataArr;

	/*if (xSample > 2270 || ySample > 2270 || zSample > 2270) {
		P1OUT |= BIT0;
		__delay_cycles(50000);
	} else {
		P1OUT &= ~BIT0;
		__delay_cycles(50000);
	}*/
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

//void acc_ISR() {
//	volatile unsigned int xSample, ySample, zSample;
//	switch (__even_in_range(ADC12IV, 34)) {
//	case 0:
//		break;                         // Vector  0:  No interrupt
//	case 2:
//		break;                         // Vector  2:  ADC12MEMx Overflow
//	case 4:
//		break;                         // Vector  4:  Conversion time overflow
//	case 6:
//		break;                         // Vector  6:  ADC12HI
//	case 8:
//		break;                         // Vector  8:  ADC12LO
//	case 10:
//		break;                         // Vector 10:  ADC12IN
//	case 12:                                // Vector 12:  ADC12MEM0 Interrupt
//		break;
//	case 14:
//		break;                         // Vector 14:  ADC12MEM1
//	case 16:
//		break;                         // Vector 16:  ADC12MEM2
//	case 18:
//		break;                         // Vector 18:  ADC12MEM3
//	case 20:
//		break;                         // Vector 20:  ADC12MEM4
//	case 22:
//		break;                         // Vector 22:  ADC12MEM5
//	case 24:
//		break;                         // Vector 24:  ADC12MEM6
//	case 26:
//		break;                         // Vector 26:  ADC12MEM7
//	case 28:
//		break;                         // Vector 28:  ADC12MEM8
//	case 30:
//		break;                         // Vector 30:  ADC12MEM9
//	case 32:
//		break;                         // Vector 32:  ADC12MEM10
//	case 34: 								// Vector 34:  ADC12MEM11
//		xSample = ADC12MEM0 + ADC12MEM3 + ADC12MEM6 + ADC12MEM9;
//		xSample = xSample >> 2;
//
//		ySample = ADC12MEM1 + ADC12MEM4 + ADC12MEM7 + ADC12MEM10;
//		ySample = ySample >> 2;
//
//		zSample = ADC12MEM2 + ADC12MEM5 + ADC12MEM8 + ADC12MEM11;
//		zSample = zSample >> 2;
//
//		ADC12IFGR0 &= ~ADC12IFG11;
//		break;
//	case 36:
//		break;								// Vector 36:  ADC12MEM12
//	case 38:
//		break;                         // Vector 38:  ADC12MEM13
//	case 40:
//		break;                         // Vector 40:  ADC12MEM14
//	case 42:
//		break;                         // Vector 42:  ADC12MEM15
//	case 44:
//		break;                         // Vector 44:  ADC12MEM16
//	case 46:
//		break;                         // Vector 46:  ADC12MEM17
//	case 48:
//		break;                         // Vector 48:  ADC12MEM18
//	case 50:
//		break;                         // Vector 50:  ADC12MEM19
//	case 52:
//		break;                         // Vector 52:  ADC12MEM20
//	case 54:
//		break;                         // Vector 54:  ADC12MEM21
//	case 56:
//		break;                         // Vector 56:  ADC12MEM22
//	case 58:
//		break;                         // Vector 58:  ADC12MEM23
//	case 60:
//		break;                         // Vector 60:  ADC12MEM24
//	case 62:
//		break;                         // Vector 62:  ADC12MEM25
//	case 64:
//		break;                         // Vector 64:  ADC12MEM26
//	case 66:
//		break;                         // Vector 66:  ADC12MEM27
//	case 68:
//		break;                         // Vector 68:  ADC12MEM28
//	case 70:
//		break;                         // Vector 70:  ADC12MEM29
//	case 72:
//		break;                         // Vector 72:  ADC12MEM30
//	case 74:
//		break;                         // Vector 74:  ADC12MEM31
//	case 76:
//		break;                         // Vector 76:  ADC12RDY
//	default:
//		break;
//	}
//}
