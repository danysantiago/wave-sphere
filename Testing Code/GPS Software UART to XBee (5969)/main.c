#include <msp430.h>
#include <stdint.h>
#include "config.h"
#include "softserial.h"


/*
 * main.c
 */

// Xbee in UCA1 (normal UART)

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

	// Startup clock system with max DCO setting ~8MHz
	CSCTL0 = CSKEY;                           // Unlock clock registers
	CSCTL1 = DCOFSEL_3 | DCORSEL;             // DCO - 8Mhz
	CSCTL2 = SELA__VLOCLK + SELS__DCOCLK + SELM__DCOCLK;
	CSCTL3 = DIVA__1 + DIVS__1 + DIVM__1;     // set all dividers

	// Setup LED Pin
	P1OUT |= BIT3;
	P1DIR |= BIT3;

	// Configure UART pins
	P2REN &= ~(BIT5 + BIT6);
	P2SEL1 |= BIT5 + BIT6;
	P2SEL0 &= ~(BIT5 + BIT6);
	P2DIR |= BIT6; // TX as output

	// Configure UART
	UCA1CTL1 |= UCSWRST;                      // Put eUSCI in reset
	UCA1CTL1 |= UCSSEL__SMCLK;                // CLK = SMCLK

	// Baud Rate calculation
	// 8000000/(16*9600) = 52.083
	// Fractional portion = 0.083
	// User's Guide Table 21-4: UCBRSx = 0x04
	// UCBRFx = int ( (52.083-52)*16) = 1
	UCA1BR0 = 52;                             // 8000000/16/9600
	UCA1BR1 = 0x00;
	UCA1MCTLW |= UCOS16 | UCBRF_1;

	UCA1CTL1 &= ~UCSWRST;                     // Initialize eUSCI
	UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

	_enable_interrupts();
	_nop();                         // For debugger
	// get data from GPS
	SoftSerial_init();

	for (;;) {
		if (!SoftSerial_empty()) {
			while (!(UCA1IFG&UCTXIFG));                // USCI_A0 TX buffer ready?
			UCA1TXBUF = SoftSerial_read();
		}
	}

}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
	unsigned char c;
  switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      c = UCA1RXBUF;
      P1OUT = (c << 3);
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}

