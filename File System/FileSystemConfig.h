/*
    Copyright (c) 2022 brodie

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#ifndef FILE_SYSTEM_CONFIG_H
	#define FILE_SYSTEM_CONFIG_H

/*
 * This file configures various media and hardware for access by the FatFs.
 */

#ifdef SD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT
    #define USING_FILE_SYSTEM_RTCC                                      0
    #define USING_SD_CARD_FILE_SYSTEM                                   1
    #define USING_USB_HOST_FILE_SYSTEM                                  0
    #define USING_FLASH_FILE_SYSTEM                                     0
#endif // end of #ifdef SD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT

#ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT
    #define USING_FILE_SYSTEM_RTCC                                      0
    #define USING_SD_CARD_FILE_SYSTEM                                   0
    #define USING_USB_HOST_FILE_SYSTEM                                  0
    #define USING_FLASH_FILE_SYSTEM                                     1
#endif // end of #ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT

#ifdef USB_FILE_SYSTEM_EXAMPLE_PROJECT
    #define USING_FILE_SYSTEM_RTCC                                      0
    #define USING_SD_CARD_FILE_SYSTEM                                   0
    #define USING_USB_HOST_FILE_SYSTEM                                  1
    #define USING_FLASH_FILE_SYSTEM                                     0
#endif // end of #ifdef USB_FILE_SYSTEM_EXAMPLE_PROJECT

#define SD_CARD_MEDIA_SECTOR_SIZE_IN_BYTES                              512
#define USB_MEDIA_SECTOR_SIZE_IN_BYTES                                  512
#define SD_CARD_BLOCK_SIZE_IN_BYTES                                     4096

#ifdef SD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT
    /* Definitions of physical drive number for each drive */
    #define SD_MMC				0	/* Example: Map MMC/SD card to physical drive 0 */
    #define USB					1	/* Example: Map USB MSD to physical drive 1 */
    #define SERIAL_FLASH		2	/* Example: Map Serial Flash to physical drive 2 */
#else
    #ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT
        /* Definitions of physical drive number for each drive */
        #define SD_MMC				2	/* Example: Map MMC/SD card to physical drive 0 */
        #define USB					1	/* Example: Map USB MSD to physical drive 1 */
        #define SERIAL_FLASH		0	/* Example: Map Serial Flash to physical drive 2 */
    #else
        #ifdef USB_FILE_SYSTEM_EXAMPLE_PROJECT
            /* Definitions of physical drive number for each drive */
            #define SD_MMC				2	/* Example: Map MMC/SD card to physical drive 0 */
            #define USB					0	/* Example: Map USB MSD to physical drive 1 */
            #define SERIAL_FLASH		1	/* Example: Map Serial Flash to physical drive 2 */
        #else
            /* Definitions of physical drive number for each drive */
            #define SD_MMC				0	/* Example: Map MMC/SD card to physical drive 0 */
            #define USB					1	/* Example: Map USB MSD to physical drive 1 */
            #define SERIAL_FLASH		2	/* Example: Map Serial Flash to physical drive 2 */
        #endif // end of #ifdef USB_FILE_SYSTEM_EXAMPLE_PROJECT
    #endif // end of #ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT
#endif // end of #ifdef SD_CARD_FILE_SYSTEM

#endif // end of #ifndef FILE_SYSTEM_CONFIG_H
