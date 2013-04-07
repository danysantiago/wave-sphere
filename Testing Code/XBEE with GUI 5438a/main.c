//******************************************************************************
//   MSP430F543xA Demo - USCI_A0, 9600 UART Echo ISR, DCO SMCLK
//
//   Description: Echo a received character, RX ISR used. Normal mode is LPM0.
//   USCI_A0 RX interrupt triggers TX Echo.
//   Baud rate divider with 1048576hz = 1048576/9600 = ~109.2 (06Dh|02h)
//   ACLK = REFO = ~32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//   See User Guide for baud rate divider table
//
//                 MSP430F5438A
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P3.4/UCA0TXD|------------>
//            |                 | 115200 - 8N1
//            |     P3.5/UCA0RXD|<------------
//
//   M. Morales
//   Texas Instruments Inc.
//   June 2009
//   Built with CCE Version: 3.2.2 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************

#include <msp430f5438a.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
  P1DIR |= BIT1 + BIT0;						// LEDs Output
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 109;                            // 1MHz 9600 (see User's Guide)
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL |= UCBRS_2 + UCBRF_0;            // Modulation UCBRSx=2, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  __no_operation();                         // For debugger
}

void sendByteUART(char byte) {
	while(!(UCA0IFG & UCTXIFG)); // wait for buffer availability (TX)
	UCA0TXBUF = byte;
}

void sendStringUART(char *string) {
	for(; *string; string++)
		sendByteUART(*string);
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:
	  break;                             // Vector 0 - no interrupt
  case 2:                                // Vector 2 - RXIFG
	  __no_operation();
	  volatile unsigned char *c = &UCA0RXBUF;

	  if(*c == 'A'){
		  P1OUT ^= BIT0;
		  sendStringUART("OK");
	  } else if (*c == 'B'){
		  P1OUT ^= BIT1;
		  sendStringUART("OK");
	  }
	  break;
  case 4:
	  break;                             // Vector 4 - TXIFG
  default:
	  break;
  }
}


