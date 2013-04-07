#include <msp430f5438a.h>

unsigned char 	i = 0;
char stack[128];

void push(char c) {
	volatile int k = 0;
	if (i < 127) {
		stack[i++] = c;
		stack[i] = '\0'; // cap with null for string
	}
	else {
		return;
	}
}

char pop(void){
	return stack[--i];
}

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;					// Stop WDT
	P3SEL = 0x30;								// P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSWRST;						// **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_2;						// SMCLK
	UCA0BR0 = 109;								// 1MHz 9600 (see User's Guide)
	UCA0BR1 = 0; 								// 1MHz 9600
	UCA0MCTL |= UCBRS_2 + UCBRF_0;				// Modulation UCBRSx=2, UCBRFx=0
	UCA0CTL1 &= ~UCSWRST;						// **Initialize USCI state machine**
	UCA0IE |= UCRXIE;							// Enable USCI_A0 RX interrupt

	__bis_SR_register(GIE);
	//__bis_SR_register(LPM0_bits + GIE);			// Enter LPM0, interrupts enabled
	for(;;){
		__no_operation();							// For debugger
	}
}

void sendByteUART(char byte) {
	while(!(UCA0IFG & UCTXIFG)); // wait for buffer availability (TX)
	UCA0TXBUF = byte;
}

void sendStringUART(char *string) {
	for(; *string; string++)
		sendByteUART(*string);
}

void store_data(char c) {
	push(c);
}


// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch(__even_in_range(UCA0IV,4)) {
	case 0:
		break; // Vector 0 - no interrupt
	case 2:    // Vector 2 - RXIFG
		// received some data at RX buf
		store_data(UCA0RXBUF);
		/*
		if ((UCRXIFG & UCA0IFG) == 0) {
			if(strncmp(toupper(stack), "HOLA", 4)) {
				send_string("Hola. Mi nombre es Juan.");
			} else {
				send_string("No entiendo su idioma.");
			}
		}
		*/
		break;
	case 4:
		break; // Vector 4 - TXIFG
	default:
		break;
	}
}
