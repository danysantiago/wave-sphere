/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//   MSP430F543xA Demo - USCI_A0, SPI 3-Wire Master Incremented Data
//
//   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//   data is sent by the master starting at 0x01. Received data is expected to
//   be same as the previous transmission.  USCI RX ISR is used to handle
//   communication with the CPU, normally in LPM0. If high, P1.0 indicates
//   valid data reception.  Because all execution after LPM0 is in ISRs,
//   initialization waits for DCO to stabilize against ACLK.
//   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz.  BRCLK = SMCLK/2
//
//   Use with SPI Slave Data Echo code example.  If slave is in debug mode, P1.1
//   slave reset signal conflicts with slave's JTAG; to work around, use IAR's
//   "Release JTAG on Go" on slave device.  If breakpoints are set in
//   slave RX ISR, master must stopped also to avoid overrunning slave
//   RXBUF.
//
//                   MSP430F5438A
//                 -----------------
//             /|\|                 |
//              | |             P1.4|<- RF wakeup interrupt signal
//              --|RST          P1.0|-> LED
//                |                 |
//                |             P3.1|-> Data Out (UCB0SIMO)
//                |                 |
//                |             P3.2|<- Data In (UCB0SOMI)
//                |                 |
//                |             P3.3|-> Serial Clock Out (UCB0CLK)

//                              P3.0|-> Chip Select for Wakeup
//
//
//   M. Morales
//   Texas Instruments Inc.
//   June 2009
//   Built with CCE Version: 3.2.2 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************

#include <msp430.h>
#include <stdio.h>
#include <string.h>

#define SELECT()	P3OUT &= ~BIT0				// CS = L
#define DESELECT()	P3OUT |= BIT0				//CS = H

void sendByteSPI(const unsigned char address, const unsigned char data);
unsigned char readByteSPI(const unsigned char address);
void readMultipleBytesSPI(const unsigned char address, const unsigned char n, unsigned char *arr);
void sendByteUART(unsigned char byte);
void sendStringUART(char *arr);
void sendGyroDataUART(unsigned char *arr);

//volatile unsigned char datax, datay, dataz; // to prevent compiler optimizations
volatile unsigned char data;
unsigned char gyroArr[6];

int main(void)
{
	data = 0;
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  P1OUT |= 0x01;                            // Set P1.0 for LED
                                            // P1.4 is RF input and should be interruptable
  P1DIR |= 0x01;                            // Set P1.0-2 to output direction
  P3SEL |= BIT1 + BIT2 + BIT3 + BIT4 + BIT5; // P3.1,2,3,4,5 option select
  P3DIR |= BIT0;
  DESELECT();

  //UCB0 is SPI
  UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCB0CTL0 |= UCMST+UCSYNC+UCMSB;    		// 3-pin, 8-bit SPI master
  UCB0CTL0 |= UCCKPL;
                                            // Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 0x04;                           // /4 ? ANTES ERA 0x02 Bitch, cuz we want slow clock to be cool
  UCB0BR1 = 0;                              //
//  UCB0MCTL = 0;                             // No modulation
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  
  //UCA0 is UART
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 109;                            // 1MHz 9600 (see User's Guide)
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL |= UCBRS_2 + UCBRF_0;            // Modulation UCBRSx=2, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt


  // with SPI and UART initialized and everything in order, wait a bit.
  __delay_cycles(1000);
  // now with SPI initialized, select slave...
  SELECT();
  sendByteSPI(0x20,0x0F);
  DESELECT();

  __delay_cycles(5000);

  SELECT();
  data = readByteSPI(0x8F); // should return 0xD4
  DESELECT();

  __delay_cycles(5000);
  while(1) {
	  SELECT();
	  readMultipleBytesSPI(0xE8, 6, gyroArr);
	  DESELECT();

	  sendGyroDataUART(gyroArr);
	  __delay_cycles(1500000);
  }


  __no_operation(); // for debugging
  __no_operation();
  __no_operation();
  __no_operation();

  __bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
  __no_operation();
  __no_operation();

}

void sendByteSPI(const unsigned char address, const unsigned char data) {
	while (!(UCB0IFG&UCTXIFG)); // UCB0 tx buffer ready?
	UCB0TXBUF = address;
	while (!(UCB0IFG&UCTXIFG)); // UCB0 tx buffer ready?
	UCB0TXBUF = data;
	while (!(UCB0IFG&UCTXIFG));
	__delay_cycles(25);
}

unsigned char readByteSPI(const unsigned char address) {
	sendByteSPI(address, 0x00); // dummy byte

	// wait for receive byte
	while(!(UCB0IFG & UCRXIFG)); // wait for rx buffer
	return UCB0RXBUF;
}

void readMultipleBytesSPI(const unsigned char address, const unsigned char n, unsigned char *arr) {
	int i;
	// reads the amount of bytes given by n at the address
	while (!(UCB0IFG&UCTXIFG)); // UCB0 tx buffer ready?
		UCB0TXBUF = address; // send first address byte

	// now send n dummy bits
	for(i = 0; i < n; i++) {
		while(!(UCB0IFG&UCTXIFG));
			UCB0TXBUF = 0x00;
		while(!(UCB0IFG & UCRXIFG)); // wait for rx buffer
			arr[i] = UCB0RXBUF;
	}
	__delay_cycles(35);
}

void sendByteUART(unsigned char byte) {
	while(!(UCA0IFG & UCTXIFG)); // wait for buffer availability (TX)
	UCA0TXBUF = byte;
}

void sendGyroDataUART(unsigned char *arr) {
	unsigned int i;
	char axis[3] = {'x', 'y', 'z'};
	char string[30];

	for (i = 0; i < 3; i++) {
		sprintf(string, "%c: %d%c\n",axis[i], arr[i*2] + (arr[i*2+1] << 8), (i==2) ? '\n' : ' ');
		sendStringUART(string);
	}
}

void sendStringUART(char *string) {
	while(*string) {
		sendByteUART(*string);
		string++;
	}
}
