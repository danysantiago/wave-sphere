#include <msp430fr5969.h>
#include "accelerometer.h"

/*
 * main.c
 */

void main() {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	//To Know that program has started running
	P1DIR |= BIT0;                            // Set P1.0/LED to output
	P1OUT &= ~BIT0;                           // Clear LED to start
	P4DIR |= BIT3;							 // set p4.3 as output
	P4OUT |= BIT3;

	initAcc();

	//Setup Timer A0
	//TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
	//TA1CCR0 = 50000;
	//TA1CTL = TASSEL_2 + MC_2 + TACLR;         // SMCLK, contmode, clear TAR
	__bis_SR_register(GIE);
	while (1) {
		//__bis_SR_register(LPM0_bits + GIE);  	// LPM0 + GIE
		//__no_operation();                       // For debugger
		__delay_cycles(50000);
		P1OUT ^= BIT0;
		__delay_cycles(50000);
		getSamples();

	}
}

//#pragma vector = ADC12_VECTOR
//__interrupt void ADC12_ISR(void){
//	acc_ISR();
//}

//#pragma vector=TIMER1_A0_VECTOR
//__interrupt void TIMER1_A0_ISR(void) {
//	getSamples();
//}

