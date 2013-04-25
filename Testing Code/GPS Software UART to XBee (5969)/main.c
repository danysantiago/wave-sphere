#include <msp430.h>
#include <stdint.h>
#include "config.h"
#include "softserial.h"


/*
 * main.c
 */

// Xbee in UCA1 (normal UART)

void shutdown_gps(void);

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

	// Startup clock system with max DCO setting ~8MHz
	CSCTL0 = CSKEY;                           // Unlock clock registers
	CSCTL1 = DCOFSEL_6 | DCORSEL;             // DCO - 24MHz
	CSCTL2 = SELA__VLOCLK + SELS__DCOCLK + SELM__DCOCLK;
	CSCTL3 = DIVA__1 + DIVS__2 + DIVM__2;     // set all dividers

	// Setup LED Pin
	P2OUT |= BIT7;
	P2DIR |= BIT7;

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
	// NOPE, 12000000/(16*9600) = 78.125
	// Fractional portion = 0.125
	// User's Guide Table 21-4: UCBRSx = 0x04
	// for 12mhz, UCBRSx = 0x10
	// UCBRFx = int ( (52.083-52)*16) = 1
	UCA1BR0 = 78;                             // 8000000/16/9600
	UCA1BR1 = 0x00;
	UCA1MCTLW |= UCOS16 | UCBRF_2;


	UCA1CTL1 &= ~UCSWRST;                     // Initialize eUSCI
	UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

	_enable_interrupts();
	_nop();                         // For debugger
	// get data from GPS
	SoftSerial_init();

	// ESTE DELAY CYCLES TIENE QUE IR AQUI. SI NO SE PONE AQUI, POR ALGUNA RAZON EL SOFTWARE UART SE JODE Y NO LE HACE
	// SHUTDOWN AL GPS. SI PONES ESTE DELAY CYCLES DENTRO DE LA FUNCION DE SOFTSERIAL_INIT, SE JODE TAMBIEN. EL DELAY
	// CYCLES TIENE QUE IR AQUI, Y TIENE QUE SER ESTE NUMERO EXACTO. NO SE DEBE CAMBIAR. SON MISTERIOS DE LA VIDA.
	__delay_cycles(10001);
	shutdown_gps();

	for (;;) {
		if (!SoftSerial_empty()) {
			while (!(UCA1IFG&UCTXIFG));                // USCI_A0 TX buffer ready?
			UCA1TXBUF = SoftSerial_read();
		}
	}

}

void send_string_gps(char *string) {
	while(*string) {
		SoftSerial_xmit(*string);
		string++;
	}
}

/**
 * Shutdown the GPS with the command.
 */
void shutdown_gps(void) {
	send_string_gps("$PMTK161,0*28\r\n");
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
      P2OUT = (c << 7);
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}

