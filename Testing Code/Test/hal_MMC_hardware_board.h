//----------------------------------------------------------------------------
//  This include file contains definitions specific to the hardware board.
//----------------------------------------------------------------------------
// ********************************************************
//
//   
//
//            MSP430F169                  MMC Card 
//         -----------------          -----------------
//     /|\|              XIN|-   /|\ |                 |
//      | |                 |     |  |                 |
//      --|RST          XOUT|-    |--|Pin4/Vcc         |
//        |                 |        |                 |
//        |                 |        |                 |
//        |            P5.6 |<-------|Pin6/CD          |
//        |            P5.0 |------->|Pin1/CS          |
//        |                 |        |                 |
//        |      P5.2/SOMI1 |------->|Pin7/DOUT        |
//        |      P5.1/SIMO1 |<-------|Pin2/DIN         |
//        |      P5.3/UCLK1 |------->|Pin5/CLK         |
//        |                 |        |                 |
//        |                 |     |--|Pin3/GND         |
//                                |
//                                =
//
//  Pin configuration at MSP430F169:
//  --------------------------------
//  MSP430F169      MSP Pin        MMC             MMC Pin
//  -------------------------------------------------------------
//  P5.0              48           ChipSelect       1
//  P5.2 / SOMI       46           DataOut          7
//                                 GND              3 (0 V)
//                                 VDD              4 (3.3 V)
//  P5.3 / UCLK1      47           Clock            5
//  P5.6              44           CardDetect       6
//  P5.1 / SIMO       45           DataIn           2
//  -------------------------------------------------------------



#include "msp430fr5969.h"              // Adjust this according to the
                                     // MSP430 device being used.
// SPI port definitions              // Adjust the values for the chosen
#define SPI_PxSEL         P2SEL1      // interfaces, according to the pin
#define SPI_PxDIR         P2DIR      // assignments indicated in the
#define SPI_PxIN          P2IN       // chosen MSP430 device datasheet.
#define SPI_PxOUT         P2OUT
#define SPI_SIMO          0x00 // P2.0
#define SPI_SOMI          0x02 // P2.1
#define SPI_UCLK          0x20 // P1.5
#define SPI_UCLKOUT		  P1OUT // P1 for clock wtf
#define SPI_UCLKDIR		  P1DIR
#define SPI_UCLKSEL		  P1SEL1

//----------------------------------------------------------------------------
// SPI/UART port selections.  Select which port will be used for the interface 
//----------------------------------------------------------------------------
//#define SPI_SER_INTF      SER_INTF_USART1  // Interface to MMC
#define SPI_SER_INTF      SER_INTF_BITBANG  // Interface to MMC


/*

#include "msp430x47x4.h"            // Adjust this according to the
                                     // MSP430 device being used.
// SPI port definitions              // Adjust the values for the chosen
#define SPI_PxSEL         P3SEL      // interfaces, according to the pin
#define SPI_PxDIR         P3DIR      // assignments indicated in the
#define SPI_PxIN          P3IN       // chosen MSP430 device datasheet.
#define SPI_PxOUT         P3OUT
#define SPI_SIMO          0x02
#define SPI_SOMI          0x04
#define SPI_UCLK          0x08

//----------------------------------------------------------------------------
// SPI/UART port selections.  Select which port will be used for the interface 
//----------------------------------------------------------------------------
#define SPI_SER_INTF      SER_INTF_USCIB0  // Interface to MMC
*/



// SPI port definitions                  // Adjust the values for the chosen
#define MMC_PxSEL         SPI_PxSEL      // interfaces, according to the pin
#define MMC_PxDIR         SPI_PxDIR      // assignments indicated in the
#define MMC_PxIN          SPI_PxIN       // chosen MSP430 device datasheet.
#define MMC_PxOUT         SPI_PxOUT      
#define MMC_SIMO          SPI_SIMO
#define MMC_SOMI          SPI_SOMI
#define MMC_UCLK          SPI_UCLK
#define MMC_UCLKOUT       SPI_UCLKOUT
#define MMC_UCLKDIR		  SPI_UCLKDIR
#define MMC_UCLKSEL		  SPI_UCLKSEL

// Chip Select
#define MMC_CS_PxOUT      P3OUT
#define MMC_CS_PxDIR      P3DIR
#define MMC_CS            0x04 // P3.2 CS

// Card Detect
#define MMC_CD_PxIN       P2IN
#define MMC_CD_PxDIR      P2DIR
#define MMC_CD            0x04 // P2.2 SD_CD (card detect)


#define CS_LOW()    MMC_CS_PxOUT &= ~MMC_CS               // Card Select
#define CS_HIGH()   while(!halSPITXDONE); MMC_CS_PxOUT |= MMC_CS  // Card Deselect

#define DUMMY_CHAR 0xFF
