#include <msp430.h> 

/*
 * PLEASE NOTICE THAT IN THIS CODE, THE DCO IS RUNNING AT 8MHZ, AND THE UART
 * BAUDRATE IS 9600. LOOKUP CALCULATIONS FOR DIVIDER REGISTERS IN CASE THAT THE
 * UART CLOCK WILL NOT BE RUNNING AT 8MHZ
 */
/*
 * main.c
 */
int main(void) {
	  WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

	  // Startup clock system with max DCO setting ~8MHz
	  CSCTL0 = CSKEY;                           // Unlock clock registers
	  CSCTL1 = DCOFSEL_3 | DCORSEL;             // DCO - 8Mhz
	  CSCTL2 = SELA__VLOCLK + SELS__DCOCLK + SELM__DCOCLK;
	  CSCTL3 = DIVA__1 + DIVS__1 + DIVM__1;     // set all dividers

	  // Configure UART pins
	  P2REN &= ~(BIT5 + BIT6);
	  P2SEL1 |= BIT5 + BIT6;
	  P2SEL0 &= ~(BIT5 + BIT6);

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
	  UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt


	  __no_operation();                         // For debugger
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA1IFG&UCTXIFG)); // echo instead to xbee when you receive data from the GPS
      UCA1TXBUF = UCA1RXBUF;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
