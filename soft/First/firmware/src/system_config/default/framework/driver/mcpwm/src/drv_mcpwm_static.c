/*******************************************************************************
  MCPWM Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_mcpwm_static.c

  Summary:
   MCPWM driver implementation for the static single instance driver.

  Description:
    The MCPWM device driver provides a simple interface to manage the MCPWM
    modules on Microchip microcontrollers.
    
  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.
    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

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
// Header Includes
// *****************************************************************************
// *****************************************************************************
#include "framework/driver/mcpwm/drv_mcpwm_static.h"


// *****************************************************************************
// *****************************************************************************
// Section: MCPWM static driver functions
// *****************************************************************************
// *****************************************************************************

void DRV_MCPWM_Enable (void)
{
/*Enable MCPWM*/

PLIB_MCPWM_Enable (MCPWM_ID_0);

}

void DRV_MCPWM_Disable (void)
{
/*Disable MCPWM*/

PLIB_MCPWM_Disable (MCPWM_ID_0);

}

void DRV_MCPWM_Initialize(void)
{

PLIB_MCPWM_Disable (MCPWM_ID_0);


/* Configure Primary MCPWM Master Timer */
PLIB_MCPWM_PrimaryTimerSetup (MCPWM_ID_0 , MCPWM_CLOCK_DIVIDE_BY_1 , 8);

/* Configure Secondary MCPWM Master Timer */
PLIB_MCPWM_SecondaryTimerSetup (MCPWM_ID_0 , MCPWM_CLOCK_DIVIDE_BY_1 , 8);

/* Configure Primary Special Event Trigger */
PLIB_MCPWM_PrimarySpecialEventTriggerSetup (MCPWM_ID_0 , 0 , MCPWM_TRIGGER_DIVIDE_BY_1);


/* Configure Secondary Special Event Trigger */
PLIB_MCPWM_SecondarySpecialEventTriggerSetup (MCPWM_ID_0 , 0 , MCPWM_TRIGGER_DIVIDE_BY_1);


/* Configure Chop Clock */
PLIB_MCPWM_ChopClockSetup (MCPWM_ID_0 , 2 ,MCPWM_CHOP_CLOCK_DISABLE);
/*Configure PWM Channel */


















}

/*******************************************************************************
 End of File
*/
