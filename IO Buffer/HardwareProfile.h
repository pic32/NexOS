/* 
 * File:   HardwareProfile.h
 * Author: NexSys Controls
 *
 * Created on April 13, 2013, 1:36 AM
 */

#ifndef HARDWAREPROFILE_H
    #define HARDWAREPROFILE_H

#include <plib.h>

#include <GenericTypeDefs.h>

UINT32 GetPeripheralClock(void);

void SetSystemClocks(UINT32 CPUFrequency);

// Various clock values
#define GetSystemClock()                                (80000000ul)
#define GetInstructionClock()                           (GetSystemClock())

//#define USE_SELF_POWER_SENSE_IO
//#define tris_self_power     TRISAbits.TRISA2    // Input
#define self_power          1

//#define USE_USB_BUS_SENSE_IO
//#define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
#define USB_BUS_SENSE       1

// Below are the UART's that the system will be using
#define USING_UART_1                                    1
//#define USING_UART_2                                    1
//#define USING_UART_3                                    1
//#define USING_UART_4                                    1
//#define USING_UART_5                                    1
//#define USING_UART_6                                    1

#define DEFAULT_BAUD_RATE                               9600

/*
	In order for this library to be fully utilized, the CS pin of
	the EEPROM CANNOT BE MANUALLY CONTROLLED BY THE SPI PORT!  This
	is because the CS must remain active between certain data transfers
	to get the optimal performance out of the EEPROM.
*/

#define EEPROM_SPI_CHANNEL                              SPI_CHANNEL4
#define EEPROM_CS_PIN                                   LATBbits.LATB0
#define EEPROM_CS_TRIS                                  TRISBbits.TRISB0
#define EEPROM_SPI_CONFIG_BITS                          (SPI_OPEN_MSTEN | SPI_OPEN_SMP_END | SPI_OPEN_MODE8 | SPI_OPEN_CKE_REV)
#define EEPROM_CS_ACTIVE                                0
#define EEPROM_CS_INACTIVE                              1


/*
	In order for this library to be fully utilized, the CS pin of
	the SRAM CANNOT BE MANUALLY CONTROLLED BY THE SPI PORT!  This
	is because the CS must remain active between certain data transfers
	to get the optimal performance out of the SRAM.
*/

#define SRAM_SPI_CHANNEL                                SPI_CHANNEL4
#define SRAM_SPI_CONFIG_BITS                            (SPI_OPEN_MSTEN | SPI_OPEN_SMP_END | SPI_OPEN_MODE8 | SPI_OPEN_CKE_REV)
#define SRAM_CS_PIN                                     LATBbits.LATB5
#define SRAM_CS_TRIS                                    TRISBbits.TRISB5
#define SRAM_CS_ACTIVE                                  0
#define SRAM_CS_INACTIVE                                1


// Select your interface type
// This library currently only supports a single physical interface layer at a time

// Description: Macro used to enable the SD-SPI physical layer (SD-SPI.c and .h)
//#define USE_SD_INTERFACE_WITH_SPI

// Description: Macro used to enable the USB Host physical layer (USB host MSD library)
#define USE_USB_INTERFACE

#define MEDIA_SECTOR_SIZE                               512
// All tristate outputs

#define HEARTBEAT_LED_TRIS_PIN                          TRISBbits.TRISB4
#define HEARTBEAT_LED_PIN                               LATBbits.LATB4

#define ERROR_LED_TRIS_PIN                              TRISBbits.TRISB3
#define ERROR_LED_PIN                                   LATBbits.LATB3

#define USER_LED_TRIS_PIN                               TRISAbits.TRISA0
#define USER_LED_PIN                                    LATAbits.LATA0

#define BOOT_BTN_TRIS_PIN                               TRISFbits.TRISF12
#define BOOT_BTN_PIN                                    PORTFbits.RF12

#define HeartbeatLEDOn()                                (HEARTBEAT_LED_PIN = 1)
#define HeartbeatLEDOff()                               (HEARTBEAT_LED_PIN = 0)
#define HeartbeatLEDToggle()                            (HEARTBEAT_LED_PIN ^= 1)

#define ErrorLEDOn()                                    (ERROR_LED_PIN = 1)
#define ErrorLEDOff()                                   (ERROR_LED_PIN = 0)
#define ErrorLEDToggle()                                (ERROR_LED_PIN ^= 1)

#define UserLEDOn()                                     (USER_LED_PIN = 1)
#define UserLEDOff()                                    (USER_LED_PIN = 0)
#define UserLEDToggle()                                 (USER_LED_PIN ^= 1)

#define GetBootBtn()                                    (BOOT_BTN_PIN == 0)


/*********************************************************************/
/******************* Pin and Register Definitions ********************/
/*********************************************************************/

/* SD Card definitions: Change these to fit your application when using
   an SD-card-based physical layer                                   */
//#define USE_SD_INTERFACE_WITH_SPI
//#ifdef USE_SD_INTERFACE_WITH_SPI

    // Registers for the SPI module you want to use
    #define MDD_USE_SPI_4

    //SPI Configuration
    #define SPI_START_CFG_1     (PRI_PRESCAL_64_1 | SEC_PRESCAL_8_1 | MASTER_ENABLE_ON | SPI_CKE_ON | SPI_SMP_ON)
    #define SPI_START_CFG_2     (SPI_ENABLE)

    // Define the SPI frequency
    #define SPI_FREQUENCY       (20000000)

    // Description: SD-SPI Chip Select Output bit
    #define SD_CS               LATBbits.LATB1
    // Description: SD-SPI Chip Select TRIS bit
    #define SD_CS_TRIS          TRISBbits.TRISB1

    // Description: SD-SPI Card Detect Input bit
    #define SD_CD               PORTBbits.RB2
    // Description: SD-SPI Card Detect TRIS bit
    #define SD_CD_TRIS          TRISBbits.TRISB2

    // define if the SD Card Detect is active high or low
    #define SD_CD_ACTIVE        0

    #if defined MDD_USE_SPI_1
        // Description: The main SPI control register
        #define SPICON1             SPI1CON
        // Description: The SPI status register
        #define SPISTAT             SPI1STAT
        // Description: The SPI Buffer
        #define SPIBUF              SPI1BUF
        // Description: The receive buffer full bit in the SPI status register
        #define SPISTAT_RBF         SPI1STATbits.SPIRBF
        // Description: The bitwise define for the SPI control register (i.e. _____bits)
        #define SPICON1bits         SPI1CONbits
        // Description: The bitwise define for the SPI status register (i.e. _____bits)
        #define SPISTATbits         SPI1STATbits
        // Description: The enable bit for the SPI module
        #define SPIENABLE           SPICON1bits.ON
        // Description: The definition for the SPI baud rate generator register (PIC32)
        #define SPIBRG			    SPI1BRG

        // Tris pins for SCK/SDI/SDO lines
        #define SPICLOCK            TRISFbits.TRISF13
        // Description: The TRIS bit for the SDI pin
        #define SPIIN               TRISFbits.TRISF4
        // Description: The TRIS bit for the SDO pin
        #define SPIOUT              TRISFbits.TRISF5

        //SPI library functions
        #define putcSPI             putcSPI1
        #define getcSPI             getcSPI1
        #define OpenSPI(config1, config2)   OpenSPI1(config1, config2)

        #elif defined MDD_USE_SPI_2
            // Description: The main SPI control register
            #define SPICON1             SPI2CON
            // Description: The SPI status register
            #define SPISTAT             SPI2STAT
            // Description: The SPI Buffer
            #define SPIBUF              SPI2BUF
            // Description: The receive buffer full bit in the SPI status register
            #define SPISTAT_RBF         SPI2STATbits.SPIRBF
            // Description: The bitwise define for the SPI control register (i.e. _____bits)
            #define SPICON1bits         SPI2CONbits
            // Description: The bitwise define for the SPI status register (i.e. _____bits)
            #define SPISTATbits         SPI2STATbits
            // Description: The enable bit for the SPI module
            #define SPIENABLE           SPI2CONbits.ON
            // Description: The definition for the SPI baud rate generator register (PIC32)
            #define SPIBRG              SPI2BRG

            // Tris pins for SCK/SDI/SDO lines

            // Description: The TRIS bit for the SCK pin
            #define SPICLOCK            TRISGbits.TRISG6
            // Description: The TRIS bit for the SDI pin
            #define SPIIN               TRISGbits.TRISG7
            // Description: The TRIS bit for the SDO pin
            #define SPIOUT              TRISGbits.TRISG8
            //SPI library functions
            #define putcSPI             putcSPI2
            #define getcSPI             getcSPI2
            #define OpenSPI(config1, config2)   OpenSPI2(config1, config2)
        #elif defined MDD_USE_SPI_4
             // Description: The main SPI control register
            #define SPICON1             SPI4CON
            // Description: The SPI status register
            #define SPISTAT             SPI4STAT
            // Description: The SPI Buffer
            #define SPIBUF              SPI4BUF
            // Description: The receive buffer full bit in the SPI status register
            #define SPISTAT_RBF         SPI4STATbits.SPIRBF
            // Description: The bitwise define for the SPI control register (i.e. _____bits)
            #define SPICON1bits         SPI4CONbits
            // Description: The bitwise define for the SPI status register (i.e. _____bits)
            #define SPISTATbits         SPI4STATbits
            // Description: The enable bit for the SPI module
            #define SPIENABLE           SPI4CONbits.ON
            // Description: The definition for the SPI baud rate generator register (PIC32)
            #define SPIBRG			    SPI4BRG

            // Tris pins for SCK/SDI/SDO lines
            #define SPICLOCK            TRISFbits.TRISF13
            // Description: The TRIS bit for the SDI pin
            #define SPIIN               TRISFbits.TRISF4
            // Description: The TRIS bit for the SDO pin
            #define SPIOUT              TRISFbits.TRISF5

            #include "plib.h"

            //SPI library functions
            #define putcSPI(data)       SpiChnPutC(SPI_CHANNEL4, data)
            #define getcSPI()           SpiChnGetC(SPI_CHANNEL4)
            #define OpenSPI(config1, config2)   SpiChnOpenEx(SPI_CHANNEL4, config1, config2, GetPeripheralClock() / SPI_FREQUENCY)
        #endif

        // Will generate an error if the clock speed is too low to interface to the card
        #if (GetSystemClock() < 100000)
            #error Clock speed must exceed 100 kHz
        #endif

    //#endif

#endif /* HARDWAREPROFILE_H */


