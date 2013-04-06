#include <msp430f5438a.h>
#include "main.h"
#include "spi.h"

/* Initialize and enable the SPI module */
void spi_initialize()
{
	P10SEL = 0x00E; // Setup P10 for SPI mode
	P10OUT |= 0x020; // Setup P10.6 as the SS signal, active low. So, initialize it high.
	P10DIR |= 0x020; // Set up P10.6 as an output

	UCB3CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
	UCB3CTL1 |= UCSSEL_2; // SMCLK
	UCB3BR0 = 0x02; // SPICLK = SMCLK/2
	UCB3BR1 = 0;
	// UCB3MCTL - always 0, no modulation

	UCB3CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**, SPI enable
	UCB3IE |= UCRXIE;                         // Enable USCI_B3 RX interrupt
}

/* Set the baud-rate divisor. The correct value is computed by dividing
the clock rate by the desired baud rate. The minimum divisor allowed
is 2. */
void spi_set_divisor(unsigned int divisor)
{
	UCB3CTL1 |= UCSWRST; // Temporarily disable the SPI module
	UCB3BR1 = divisor >> 8;
	UCB3BR0 = divisor;
	UCB3CTL1 &= ~UCSWRST;; // Re-enable SPI
}

/* Assert the CS signal, active low (CS=0) */
void spi_cs_assert()
{
	// Pin 10.6
	P10OUT &= ~BIT6;
}

/* Deassert the CS signal (CS=1) */
void spi_cs_deassert()
{
	// Pin 10.6
	P10OUT |= BIT6;
}

/* Send a single byte over the SPI port */
void spi_send_byte(unsigned char input)
{
	UCB3IFG &= ~UCRXIFG; // clear receive flag (why?) - SARM

	/* Send the byte */
	UCB3TXBUF = input;

	/* Wait for the byte to be sent */
	// apparently it waits until it receives something? - SARM - SAME AS CODE BELOW? WEIRD
	while ((UCB3IFG & UCRXIFG) == 0) { }
}

/* Receive a byte. Output an 0xFF (the bus idles high) to receive the byte */
unsigned char spi_rcv_byte()
{
	unsigned char tmp;
	UCB3IFG &= ~UCRXIFG; // clear receive flag

	/* Send the byte */
	UCB3TXBUF = 0xFF;

	/* Wait for the byte to be received */
	while (!(UCB3IFG & UCRXIFG)) { }
	tmp = UCB3RXBUF;
	return (tmp);
}

/* Disable the SPI module. This function assumes the module had
 * already been initialized. */
void spi_disable()
{
	/* Put the SPI module in reset mode */
	UCB3CTL1 |= UCSWRST;

	/* Disable the USART module */
	UCB3IE &= ~UCRXIE;
	//ME1 &= ~USPIE0;
}

void spi_enable()
{
	/* Enable the USART module */
	//ME1 |= USPIE0;
	/* Take the SPI module out of reset mode */
	//U0CTL &= ~SWRST;

	UCB3CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**, SPI enable
	UCB3IE |= UCRXIE;                         // Enable USCI_B3 RX interrupt
}
