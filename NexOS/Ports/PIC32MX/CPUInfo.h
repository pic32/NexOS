/*
    NexOS Kernel Version v1.01.04
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

#ifndef CPU_INFO_H
	#define CPU_INFO_H
/*
	Developer: brodie
	Date: January 19, 2022
	File Name: CPUInfo.h
	Version: 1.03
	IDE:  MPLAB X v3.26
	Compiler: XC32 v1.40

	Description:
	This file contains mostly just macros for getting various information about the 
	PIC32 CPU.
*/

/*! \mainpage CPUInfo Library
 *  \brief This is a Library written in C for getting information about the PIC32 CPU, RAM and Program Memory.
 *  \author brodie
 *  \version 1.03
 *  \date   January 19, 2022
 */

#include "GenericTypes.h"
#include "p32xxxx.h"
	
/*
	Macro: UINT32 GetRAMSizeInBytes(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of bytes of internal RAM the PIC32 has. 

	Description: This will return the number of bytes of internal RAM the PIC32 has.

	Notes: None
*/
/**
		* @brief Returns the number of bytes of internal RAM.
		* @param None
		* @return UINT32 - The number of bytes of internal RAM the PIC32 the.
		* @note None
		* @sa GetRAMSizeInWords()
		* @since v1.00
*/
#define GetRAMSizeInBytes()					(UINT32)BMXDRMSZ

/*
	Macro: UINT32 GetRAMSizeInWords(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of words of internal RAM the PIC32 has.  1 word equals 4 bytes. 

	Description: This will return the number of words of internal RAM the PIC32 has.
	1 word equals 4 bytes. 

	Notes: None
*/
/**
		* @brief Returns the number of words of internal RAM.
		* @param None
		* @return UINT32 - The number of words of internal RAM the PIC32.  1 word equals 4 bytes.
		* @note None
		* @sa GetRAMSizeInBytes()
		* @since v1.00
*/
#define GetRAMSizeInWords()					(UINT32)(GetRAMSizeInBytes() / (UINT32)sizeof(UINT32))	

/*
	Macro: UINT32 GetProgramMemorySizeInBytes(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of bytes of internal Program Memory the PIC32 has.

	Description: This will return the number of bytes of internal Program Memory the PIC32 has, 
	excluding the Boot Memory.

	Notes: None
*/
/**
		* @brief Returns the number of bytes of internal Program Memory.
		* @param None
		* @return UINT32 - The number of bytes of internal Program Memory the PIC32 has, excluding the Boot Memory.
		* @note None
		* @sa GetProgramMemorySizeInWords()
		* @since v1.00
*/
#define GetProgramMemorySizeInBytes()		(UINT32)BMXPFMSZ

/*
	Macro: UINT32 GetProgramMemorySizeInWords(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of words of internal Program Memory the PIC32 has, excluding the Boot Memory.
		1 word equals 4 bytes. 

	Description: This will return the number of words of internal Program Memory the PIC32 has 
	in it, excluding the Boot Memory.  1 word equals 4 bytes. 

	Notes: None
*/
/**
		* @brief Returns the number of words of internal Program Memory.
		* @param None
		* @return UINT32 - The number of words of internal Program Memory the PIC32 has, excluding the Boot Memory.
		1 word equals 4 bytes.
		* @note None
		* @sa GetProgramMemorySizeInBytes()
		* @since v1.00
*/
#define GetProgramMemorySizeInWords()		(UINT32)(GetProgramMemorySizeInBytes() / (UINT32)sizeof(UINT32))

/*
	Macro: UINT32 GetBootMemorySizeInBytes(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of bytes of internal Boot Memory the PIC32 has.

	Description: This will return the number of bytes of internal Boot Memory the PIC32 has.

	Notes: None
*/
/**
		* @brief Returns the number of bytes of internal Boot Memory.
		* @param None
		* @return UINT32 - The number of bytes of internal Boot Memory the PIC32 has.
		* @note None
		* @sa GetBootMemorySizeInWords()
		* @since v1.00
*/
#define GetBootMemorySizeInBytes()			(UINT32)BMXBOOTSZ

/*
	Macro: UINT32 GetBootMemorySizeInWords(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of words of internal Boot Memory the PIC32 has.
		1 word equals 4 bytes. 

	Description: This will return the number of words of internal Boot Memory the PIC32 has 
	in it.  1 word equals 4 bytes. 

	Notes: None
*/
/**
		* @brief Returns the number of words of internal Boot Memory.
		* @param None
		* @return UINT32 - The number of words of internal Boot Memory the PIC32 has.
		* @note None
		1 word equals 4 bytes.
		* @sa GetBootMemorySizeInBytes()
		* @since v1.00
*/
#define GetBootMemorySizeInWords()			(UINT32)(GetBootMemorySizeInBytes() / (UINT32)sizeof(UINT32))

/*
	Macro: UINT32 GetCacheHitCount(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of cache hits the CPU made while loading or storing information in a
		cacheable region.

	Description:  Returns the number of cache hits made by the CPU.  The cache hit is incremented
	each time the processor issues an instruction fetch or load that hits the prefetch cache from
	a cacheable region.  Non-cacheable accesses do not modify this value.

	Notes: None
*/
/**
		* @brief Returns the number of cache hits.
		* @param None
		* @return UINT32 - Returns the number of cache hits made by the CPU.  The cache hit is incremented
		each time the processor issues an instruction fetch or load that hits the prefetch cache from
		a cacheable region.  Non-cacheable accesses do not modify this value.
		* @note None
		* @sa ClearCacheHitCount(), GetCacheMissCount(), ClearCacheMissCount()
		* @since v1.00
*/
#define GetCacheHitCount()					(UINT32)CHEHIT

/*
	Macro: void ClearCacheHitCount(void)

	Parameters: 
		None

	Returns:
		None

	Description:  Clears the cache hit count.

	Notes: None
*/
/**
		* @brief Clears the cache hit count.
		* @param None
		* @return None
		* @note None
		* @sa GetCacheHitCount(), GetCacheMissCount(), ClearCacheMissCount()
		* @since v1.00
*/
#define ClearCacheHitCount()				CHEHIT = (UINT32)0

/*
	Macro: UINT32 GetCacheMissCount(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of cache misses exhibited by the processor.

	Description:  Returns the number of cache misses made by the CPU.  This is incremented each time
	the processor issues an instruction fetch from a cacheable region that misses the prefetch cache.
	Non-cacheable accesses do not modify this value.

	Notes: None
*/
/**
		* @brief Returns the number of cache misses.
		* @param None
		* @return UINT32 - Returns the number of cache misses made by the CPU.  This is incremented each time
		the processor issues an instruction fetch from a cacheable region that misses the prefetch cache.
		Non-cacheable accesses do not modify this value.
		* @note None
		* @sa ClearCacheMissCount(), ClearCacheHitCount(), GetCacheHitCount()
		* @since v1.00
*/
#define GetCacheMissCount()					(UINT32)CHEMIS

/*
	Macro: void ClearCacheMissCount(void)

	Parameters: 
		None

	Returns:
		UINT32 - Clears the number of cache misses exhibited by the processor.

	Description:  Clears the cache miss count.

	Notes: None
*/
/**
		* @brief Clears the cache miss count.
		* @param None
		* @return None
		* @note None
		* @sa GetCacheMissCount(), GetCacheHitCount(), ClearCacheHitCount()
		* @since v1.00
*/
#define ClearCacheMissCount()				CHEMIS = (UINT32)0

/*
	Macro: UINT32 GetPrefetchCacheAbortCount(void)

	Parameters: 
		None

	Returns:
		UINT32 - The number of cache aborts exhibited by the processor.

	Description:  Returns the number of cache aborts made by the CPU.  This is incremented each time
	an automatic prefetch cache is aborted due to a non-sequential instruction fetch, load or store.

	Notes: None
*/
/**
		* @brief Returns the number of cache aborts.
		* @param None
		* @return UINT32 - Returns the number of cache aborts made by the CPU.  This is incremented each time
		an automatic prefetch cache is aborted due to a non-sequential instruction fetch, load or store.
		Non-cacheable accesses do not modify this value.
		* @note None
		* @sa ClearPrefetchCacheAbortCount()
		* @since v1.00
*/
#define GetPrefetchCacheAbortCount()		(UINT32)CHEPFABT

/*
	Macro: void ClearPrefetchCacheAbortCount(void)

	Parameters: 
		None

	Returns:
		None

	Description:  Clears the number of cache aborts exhibited by the processor.

	Notes: None
*/
/**
		* @brief Clears the cache abort count.
		* @param None
		* @return None
		* @note None
		* @sa GetPrefetchCacheAbortCount()
		* @since v1.00
*/
#define ClearPrefetchCacheAbortCount()		CHEPFABT = (UINT32)0

/*
	Macro: UINT32 GetProcessorID(void)

	Parameters: 
		None

	Returns:
		UINT32 - Returns the Processor ID.

	Description:  Returns the Processor ID.
					Bits 31 - 24: Reserved
					Bits 23 - 16: Company ID.  Identifies the company that designed or manufactured the processor.
					In the PIC32MX this field contains a value of 1 to indicate MIPS Technologies, Inc.
					Bits 15 - 8: Identifies the type of processor.  this field allow software to distinguish
					between the various types of MIPS Technologies processors.
					Bits 7 - 0: Specifies the revision number of the processor.  This field allows software to
					distinguish between one revision and another of the same processor type.  This field is broken
					up into the following three subfields.
					Major Revision<7:5> The number is increased on major revisions of the processor core.
					Minor Revision<4:2> This number is increased on each incremental revision of the processor and 
					reset on each new major revision.
					Patch Level<1:0> If a patch is made to modify an older revision of the processor, this field
					will be incremented.

	Notes: None.
*/
/**
		* @brief Returns the Processor ID.
		* @param None
		* @return UINT32 - The Processor ID.
				Bits 31 - 24: Reserved
				Bits 23 - 16: Company ID.  Identifies the company that designed or manufactured the processor.
				In the PIC32MX this field contains a value of 1 to indicate MIPS Technologies, Inc.
				Bits 15 - 8: Identifies the type of processor.  this field allow software to distinguish
				between the various types of MIPS Technologies processors.
				Bits 7 - 0: Specifies the revision number of the processor.  This field allows software to
				distinguish between one revision and another of the same processor type.  This field is broken
				up into the following three subfields.
				Major Revision<7:5> The number is increased on major revisions of the processor core.
				Minor Revision<4:2> This number is increased on each incremental revision of the processor and 
				reset on each new major revision.
				Patch Level<1:0> If a patch is made to modify an older revision of the processor, this field
				will be incremented.
		* @note None
		* @sa None
		* @since v1.00
*/
#define GetProcessorID()					(UINT32)(_CP0_GET_PRID())

/*
	Macro: UINT32 GetLastBadVirtualAddress(void)

	Parameters: 
		None

	Returns:
		UINT32 - Returns the last address error related instruction.

	Description:  Returns the most recent virtual address that caused an address error exception.
	Address errors are caused by executing load, store, or fetch operations from unaligned
	addresses, and also by trying to access Kernel mode addresses from User mode.

	Notes: None
*/
/**
		* @brief Returns the last address error related instruction.
		* @param None
		* @return UINT32 - Returns the most recent virtual address that caused an address error exception.
		Address errors are caused by executing load, store, or fetch operations from unaligned
		addresses, and also by trying to access Kernel mode addresses from User mode.
		* @note None
		* @sa None
		* @since v1.00
*/
#define GetLastBadVirtualAddress()			(UINT32)(_CP0_GET_BADVADDR())

/*
	Macro: UINT32 GetDeviceID(void)

	Parameters: 
		None

	Returns:
		UINT32 - Returns the device ID assigned by Microchip.

	Description:  Each PIC32 has a unique device ID to identify it.  Refer to 
    this link to get the device ID of each device: (DS61145 Rev L).
    http://ww1.microchip.com/downloads/en/DeviceDoc/61145L.pdf

	Notes: None
*/
/**
		* @brief Each PIC32 has a unique device ID to identify it.  Refer to 
        * this link to get the device ID of each device: (DS61145 Rev L).
        * http://ww1.microchip.com/downloads/en/DeviceDoc/61145L.pdf
		* @param None
		* @return UINT32 - Returns the device ID assigned by Microchip.
		* @note None
		* @sa None
		* @since v1.01
*/
#define GetDeviceID()                       ((UINT32)DEVIDbits.DEVID)


/*
	Macro: UINT32 GetDeviceSiliconRev(void)

	Parameters: 
		None

	Returns:
		UINT32 - Returns the silicon Rev ID assigned by Microchip.

	Description:  This method can be used to get the rev silicon of the uC.

	Notes: None
*/
/**
		* @brief This method can be used to get the rev silicon of the uC.
		* @param None
		* @return UINT32 - Returns the silicon Rev ID assigned by Microchip.
		* @note None
		* @sa None
		* @since v1.01
*/
#define GetDeviceSiliconRev()               ((UINT32)DEVIDbits.VER)


/*
	Macro: UINT64 GetDeviceMACID(void)

	Parameters: 
		None

	Returns:
		UINT64 - Returns the unique 48-bit MAC ID assigned by the factory.

	Description:  This method gets the unique 48-bit MAC ID.  It can be useful
    for using as a seed, or a unique identifier of some sort.

	Notes: None
*/
/**
		* @brief This method gets the unique 48-bit MAC ID.  IT can be useful
        * for using as a seed, or a unique identifier of some sort.
		* @param None
		* @return UINT64 - Returns the unique 48-bit MAC ID assigned by the factory.
		* @note None
		* @sa None
		* @since v1.01
*/
#define GetDeviceMACID()                    (UINT64)((UINT64)EMAC1SA2 << 32 | (UINT64)EMAC1SA1 << 16 | (UINT64)EMAC1SA0)

/*
	Macro: UINT16 GetUserID(void)

	Parameters: 
		None

	Returns:
		UINT16 - The user ID which is programmed at bootloader program time.

	Description: This macro will get the value which was programmed into the uC
        when it was programmed with the bootloader at the factory.

	Notes: None
*/
/**
		* @brief Gets user ID which is programmed at bootloader program time.
		* @param None
		* @return UINT16 - The user ID which is programmed at bootloader program time.
		* @note None
		* @sa None
		* @since v1.03
*/
#define GetUserID()                         (UINT16)(DEVCFG3bits.USERID)

#endif // end of CPU_INFO_H
