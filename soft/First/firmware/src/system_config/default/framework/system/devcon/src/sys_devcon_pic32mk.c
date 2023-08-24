/*******************************************************************************
  Device Control System Service Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    sys_devcon_pic32mk.c

  Summary:
    Device Control System Service implementation.

  Description:
    The DEVCON system service provides a simple interface to manage the Device 
    Control module on Microchip microcontrollers. This file Implements the core
    interface routines for the Device Control system service. While building 
    the system service from source, ALWAYS include this file in the build.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include <cp0defs.h>
#include "sys_devcon_local.h"
#include "peripheral/int/plib_int.h"
#include "peripheral/pcache/plib_pcache.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Variable Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: SYS DEVCON Client Setup Routines
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Function:
    void SYS_DEVCON_PerformanceConfig( unsigned int sysclk )

  Summary:
    Configures the PFM wait states and prefetch (cache) module for maximum 
    performance.

  Description:
    This function configures the PFM wait states and prefetch (cache) module 
    for maximum performance.

  Remarks:
    None.
*/

void __attribute__((nomips16)) SYS_DEVCON_PerformanceConfig( unsigned int sysclk )
{
    bool int_flag = false;

	/* Configure CP0.K0 for optimal performance (cached instruction pre-fetch)*/
	__builtin_mtc0(16, 0,(__builtin_mfc0(16, 0) | 0x3));
	
    /* Set the PFM wait states based on the system clock and ECC setting */
    #if defined(PLIB_PCACHE_ExistsWaitState)    
    if (PLIB_PCACHE_ExistsWaitState(PCACHE_ID_0))
    {
        int ws; /* number of wait states */

        if (sysclk <= 60000000)
            ws = 1;
        else if (sysclk <= 80000000)
            ws = 2;
        else
            ws = 3;

        /* Interrupts must be disabled when changing wait states */
        int_flag = (bool)(PLIB_INT_GetStateAndDisable( INT_ID_0 ) & 0x01);

        PLIB_PCACHE_WaitStateSet(PCACHE_ID_0, ws);

        if (int_flag)
        {
            PLIB_INT_Enable(INT_ID_0);
            int_flag = false;
        }
    }
    #endif // defined(PLIB_PCACHE_ExistsWaitState)
    /* Interrupts must be disabled when enabling the Prefetch Cache Module */
    int_flag = (bool)(PLIB_INT_GetStateAndDisable( INT_ID_0 ) & 0x01);

    /* Enable Prefetch Cache Module */
    /*
	#if defined(PLIB_PCACHE_ExistsPrefetchEnable)
    if (PLIB_PCACHE_ExistsPrefetchEnable(PCACHE_ID_0))
    {
        PLIB_PCACHE_PrefetchEnableSet(PCACHE_ID_0, PLIB_PCACHE_PREFETCH_ENABLE_ALL);
    }
    #endif
	*/
    
    if (int_flag)
    {
        PLIB_INT_Enable(INT_ID_0);
    }
}

/*******************************************************************************
 End of File
*/

