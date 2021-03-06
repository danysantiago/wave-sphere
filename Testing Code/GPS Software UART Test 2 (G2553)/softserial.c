/**
 * softserial.c - full-duplex software UART routines using TimerA interrupts
 *
 * Note: This code monopolizes the TIMER_A interrupts. If you are using a
 * small chips such as the msp430g2231 and your code needs to do timer
 * related things you are going to have to accomplish that without using
 * TIMER_A. Maybe the Watchdog timer or take turns with the CCR0 interrupt. On
 * larger chips such as the msp430g2553 that have multiple TimerA peripherals,
 * just use a different TimerA and leave TIMER0_A for softserial.
 *
 * License: Do with this code what you want. However, don't blame
 * me if you connect it to a heart pump and it stops.  This source
 * is provided as is with no warranties. It probably has bugs!!
 * You have been warned!
 *
 * Author: Rick Kimball
 * email: rick@kimballsoftware.com
 * Version: 1.00 04-20-2011 Initial version 
 * Version: 1.01 04-21-2011 cleanup
 * Version: 1.02 04-21-2011 modified ISR defines to make msp430g2553 happy
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "config.h"
#include "softserial.h"

/**
 * The clocks ticks per bit calculations below are accurate if your clock is accurate.
 * Some CPU frequencies and baud rates combinations will have builtin errors.  You
 * might want to checkout the information at: http://www.wormfood.net/avrbaudcalc.php
 * For best results, you might have to tweak and hardcode these calculations values.
 *
 */

#define TICKS_PER_BIT ( F_CPU / BAUD_RATE )                    // timer clock ticks per bit
#define TICKS_PER_BIT_DIV2 (( F_CPU / (BAUD_RATE*2) ) )        // timer clock ticks per half bit

/**
 * typedef ringbuffer_t - ring buffer structure
 */
typedef struct {
    uint8_t buffer[RX_BUFFER_SIZE];
    int head;
    int tail;
} ringbuffer_t;

//--------------------------------------------------------------------------------
// F I L E   G L O B A L S
//--------------------------------------------------------------------------------

volatile ringbuffer_t rx_buffer = {{0}, 0, 0};
volatile unsigned int USARTTXBUF; // Software UART TX data

//--------------------------------------------------------------------------------
// E X P O S E D   E X T E R N A L   F U N C T I O N S
//--------------------------------------------------------------------------------

/**
 * SoftSerial_init() - configure pins and timers.
 *
 */

void SoftSerial_init(void) {

    P2OUT |= TX_PIN | RX_PIN;           // Initialize all GPIO
    P2SEL |= TX_PIN | RX_PIN;           // Enabled Timer ISR function for TXD/RXD pins
    P2DIR |= TX_PIN;                    // Enable TX_PIN for output

    TA1CCTL0 = OUT;                      // Set TXD Idle state as Mark = '1', +3.3 volts normal
    TA1CCTL1 = SCS | CM1 | CAP | CCIE;   // Sync TA1CLK and MCLK, Detect Neg Edge, Enable Capture mode and RX Interrupt

    TA1CTL = TASSEL_2 | MC_2 | TACLR;    // Clock TIMERA from SMCLK, run in continuous mode counting from to 0-0xFFFF
}

/**
 * SoftSerial_empty() - returns true if rx_buffer is empty
 */

int8_t SoftSerial_empty(void) {
    return rx_buffer.head == rx_buffer.tail;    // this is a single asm statement we
                                                // don't have to worry about interrupts
}

/**
 * SoftSerial_read() - remove an RXD character from the ring buffer
 */
int SoftSerial_read(void) {
    int c=-1;

    _disable_interrupts();  // disable interrupts to protect head and tail values
                            // This prevents the RX_ISR from modifying them
                            // while we are trying to read and modify

    // if the head isn't ahead of the tail, we don't have any characters
    if (rx_buffer.head != rx_buffer.tail) {
        c = (uint8_t)rx_buffer.buffer[rx_buffer.tail];
        rx_buffer.tail = (unsigned int)(rx_buffer.tail + 1) % RX_BUFFER_SIZE;
    }

    _enable_interrupt();  // ok .. let everyone at them

    return c;
}

/**
 * SoftSerial_xmit() - send one byte of data
 *
 * Wait for any transmissions in progress to complete
 * then queue up the USARTTXBUF byte with new data.
 *
 */

void SoftSerial_xmit(uint8_t byte) {

    // TIMERA0 disables the interrupt flag when it has sent
    // the final stop bit. While a transmit is in progress the
    // interrupt is enabled

    while (TA1CCTL0 & CCIE) {
        ; // wait for previous xmit to finish
    }

    // make the next output at least TICKS_PER_BIT in the future
    // so we don't stomp on the the stop bit from our previous xmt

    TA1CCR0 = TA1R;               // resync with current TimerA clock
    TA1CCR0 += TICKS_PER_BIT;    // setup the next timer tick
    TA1CCTL0 = OUTMOD0 + CCIE;   // set TX_PIN HIGH and reenable interrupts

    // now that we have set the next interrupt in motion
    // we quickly need to set the TX data. Hopefully the
    // next 2 lines happens before the next timer tick.

    // Note: This code makes great use of multiple peripherals
    //
    // In the code above, we start with a busy wait on the CCIE
    // interrupt flag. As soon as it is available, we setup the next
    // send time and then enable the interrupt. Until that time happens,
    // we have a few free cycles available to stuff the start and stop bits
    // into the data buffer before the timer ISR kicks in and handles
    // the event.  Note: if you are using a really slow clock or a really
    // fast baud rate you could run into problems if the interrupt is
    // triggered before you have finished with the USARTTXBUF

    USARTTXBUF = byte;      // load our psuedo register used by the TIMERA0_VECTOR
    USARTTXBUF |= 0x100;    // Add the stop bit '1'
    USARTTXBUF <<= 1;       // Add the start bit '0'
}

//--------------------------------------------------------------------------------
// I N T E R N A L   F U N C T I O N S
//--------------------------------------------------------------------------------

/**
 * _SoftSerial_append() - add a character to the ring buffer
 *
 * Note: we don't have to disable interrupts in this macro
 * as we expect to be called from inside of the RX_ISR routine
 */

#define SoftSerial_append(c) { \
    int i = (unsigned int)(rx_buffer.head + 1) % RX_BUFFER_SIZE; \
    if ( i != rx_buffer.tail ) { \
        rx_buffer.buffer[rx_buffer.head] = c; \
        rx_buffer.head = i; \
    } \
}

/**
 * SoftSerial_TX_ISR - TX Interrupt Handler
 *
 * Handle the sending of a data byte with one
 * start bit + 8 data bits + stop bit.
 *
 */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void SoftSerial_TX_ISR(void) {
    static uint8_t txBitCnt = 10;       // 1 Start bit + 8 data bits + 1 stop bit

    if (txBitCnt == 0) {                // All bits TXed?
        TA1CCTL0 &= ~CCIE;               // disable interrupt, used as a flag to SoftSerial_xmit
        txBitCnt = 10;                  // Re-load bit counter
    } else {
        TA1CCR0 += TICKS_PER_BIT;        // setup next time to send a bit

        if (USARTTXBUF & 0x01) {        // look at LSB and decide what to send
            TA1CCTL0 &= ~OUTMOD2;        // TX Mark '1'
        } else {
            TA1CCTL0 |= OUTMOD2;         // TX Space '0'
        }
        USARTTXBUF >>= 1;               // pull in the next bit to send
        txBitCnt--;
    }
}

/**
 * SoftSerial_RX_ISR - Receive Interrupt Handler
 *
 * This ISR works in two modes. It starts out in capture mode
 * waiting for the RX line to go from HI to LO indicating a
 * start bit from the sender.  It then switches to
 * compare mode. Each tick, it sets a future time to wake up and
 * sample the RX line.  The sample is done by calculating
 * the time for the center of the data bit for the given
 * baud rate and waking up and taking a sample at that time.
 * Once the stop bit is received it goes back into
 * capture mode waiting for the next start bit.
 *
 * Note: serial data is LSB first
 *
 */

#pragma vector = TIMER1_A1_VECTOR       // this mcu has multiple TIMERA peripherals
__interrupt void SoftSerial_RX_ISR(void) {
    static unsigned char rxBitCnt = 8;
    static unsigned char rxData = 0;

    if ( TA1IV == TA1IV_TACCR1 ) {     // this mcu has multiple TIMERA peripherals
        TA1CCR1 += TICKS_PER_BIT;             // Setup next time to sample
        if (TA1CCTL1 & CAP) {                 // Is this the start bit?
            TA1CCTL1 &= ~CAP;                 // Switch capture to compare mode
            TA1CCR1 += TICKS_PER_BIT_DIV2;    // Sample from the middle of D0
        }
        else {
            rxData >>= 1;
            if (TA1CCTL1 & SCCI) {            // Get bit waiting in receive latch
                rxData |= 0x80;
            }
            rxBitCnt--;
            if (rxBitCnt == 0) {             // All bits RXed?
                SoftSerial_append(rxData);   // Store in ring_buffer
                rxBitCnt = 8;                // Re-load bit counter
                TA1CCTL1 |= CAP;              // Switch compare to capture mode
                TA1CCR1 += TICKS_PER_BIT;     // account for the stop bit
            }
        }
    }
}
