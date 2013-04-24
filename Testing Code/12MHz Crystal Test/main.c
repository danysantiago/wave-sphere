#include <msp430.h>				

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	// LED setup
//	P1OUT = 0;
//	P1DIR |= BIT0;

	// output SMCLK on P3.4
	P3SEL1 |= BIT4;
	P3DIR |= BIT4;

	// Set PJ for HF Crystal use
	PJSEL0 |= BIT6;
	PJSEL1 &= ~BIT6;

	// Configure CS module
	CSCTL0_H = 0xA5; 							// Write password to unlock clock registers
	CSCTL1 = DCOFSEL_0; 						// Set DCO to 1MHz
	CSCTL1 &= ~DCORSEL;
	CSCTL2 = SELS__HFXTCLK; 	// set SMCLK and MCLK to HFXTCLK 12MHz crystal oscillator
	CSCTL3 = DIVS__1; 				// set SMCLK divisor to 1, SMCLK = 12MHz
	CSCTL4 |= HFFREQ_2 + HFXTDRIVE_2;			// set HFX frequency to 8-16MHz, HFXTOFF = 0, turn it on
	CSCTL4 &= ~HFXTOFF;
	CSCTL5 |= ENSTFCNT2;						// enable HF counter

	do {
		CSCTL5 &= ~HFXTOFFG;   					// Clear XT1 fault flag
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1&OFIFG);					// Test oscillator fault flag

	CSCTL0_H = 0;
	SFRIE1 |= OFIE; // enable oscillator fault interrupt. If there is a fault.... then what?

//	P1OUT |= BIT0;
	while(1);
	_nop();

	return 0;
}


#pragma vector=UNMI_VECTOR
__interrupt void UNMI_ISR(void)
{
	do
	{
		// set a breakpoint on the line below to observe XT1 operating from LFMODOSC
		// when the breakpoint is hit during a crystal fault
		CSCTL5 &= ~HFXTOFFG;                    // Clear XT1 fault flag
		SFRIFG1 &= ~OFIFG;
		P1OUT &= ~BIT0;
		__delay_cycles(25000);                  // time for flag to get set again
	} while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	P1OUT |= BIT0;
}
