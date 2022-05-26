#include <plib.h>

#include "p32xxxx.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"

UINT32 gPBFrequency = 0;

void __attribute__((weak))OSCFailCallback(void)
{
	// TODO: To override the default handler define the same function.
}

UINT32 GetPeripheralClock(void)
{
    return gPBFrequency;
}

void SetSystemClocks(UINT32 CPUFrequency)
{
    gPBFrequency = SYSTEMConfigPerformance(CPUFrequency);
}